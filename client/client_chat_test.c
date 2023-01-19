#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "../structs.h"
#include <signal.h>
static int sd;

static void sighandler(){
	struct request req;
	req.type = 3;
	write(sd,&req,sizeof(struct request));
	exit(0);
}

int main(int argc, char *ip[]){
	char port[128] = "9999";
    char server_address[128];
    if(!(ip[1])){
	printf("hit this condition\n");
        char temp[128] = "localhost";
        strcpy(server_address, temp);
    }

    else{
        strcpy(server_address, ip[1]);
    }
	printf("connecting to %s at port %s\n", server_address,port);

    struct addrinfo hints, *results; //hints is settings for the lookup, results is the linked list
    memset(&hints, 0, sizeof(hints)); //set memory

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int err;
    //         getaddrinfo(address,port,...)
    if ((err = getaddrinfo(server_address, port, &hints, &results)) != 0) {
        printf("error %d : %s\n", err, strerror(err));
        return 1;
    }

    sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol); //socket descriptor using results
    if(sd < 0){
        printf("%s\n",strerror(errno));
        exit(1);
    }

    if ( connect(sd, results->ai_addr, results->ai_addrlen) < 0) { //connects or exits
        printf("Failed to connect\n");
        exit(1);
    }
	struct request req;
    int profd = open("./myprofile", O_RDONLY, 0777);
    struct profile my_prof;
    read(profd, &my_prof, 0777);
    //printf("%s, %s, %d\n", test.username, test.password, test.user_id);
    req.type = 1;
    strcpy(req.prof.username,my_prof.username);
    strcpy(req.prof.password,my_prof.password);
    req.prof.user_id = my_prof.user_id;
    req.prof.byte_total = my_prof.byte_total;
    signal(SIGINT,sighandler);
    //req.prof.byte_total = 0;
    int written = write(sd, &req, sizeof(struct request)); //writes the type 1 request to socket
    printf("wrote %d bytes", written);
    printf("searching for match...\n");
    int b = 0;
    while(b==0){
	char match_message[512];
    	b = read(sd,match_message,512);
	printf("%s\n",match_message);
    }


    req.type = 2;
    	while (1){
		fd_set read_fds;

    		FD_ZERO(&read_fds);

    		//add listen_socket and stdin to the set
    		FD_SET(sd, &read_fds);
    		//add stdin's file desciptor
    		FD_SET(STDIN_FILENO, &read_fds);
		char message[512];
		int i = select(sd+1, &read_fds, NULL, NULL, NULL);

  		//if standard in, use fgets
  		if (FD_ISSET(STDIN_FILENO, &read_fds)) {
  			//printf("Send a message: \n");
        		fgets(req.payload,512,stdin);
       			int written2 = write(sd, &req, sizeof(struct request)); //writes the type 2 request to socket
        		//printf("written2 %d\n",written2);
		}
  		//if socket, accept the connection
  		//assume this function works correctly
  		if (FD_ISSET(sd, &read_fds)) {
  			int bytes = read(sd,message,512);
            if (bytes>0){
                req.prof.byte_total+=bytes;
                int profd = open("./myprofile", O_WRONLY, 0777); //write profile locally
    	        write(profd, &(req.prof), sizeof(struct profile));
    	        close(profd);
                printf("%s",message);
       		}

		}
    	}

    freeaddrinfo(results);
    return 0;
}
