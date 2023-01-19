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
    //printf("got here line 28\n");
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
    int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol); //socket descriptor using results
    if(sd < 0){
        printf("%s\n",strerror(errno));
        exit(1);
    }
    if ( connect(sd, results->ai_addr, results->ai_addrlen) < 0) { //connects or exits
        printf("Failed to connect\n");
        exit(1);
    }
	//filling profile with input information, and -1 as placeholder for userid, which will be edited by server and sent back
	//struct profile prof;
	struct request req;
	printf("pick a username\n");	
	fgets(&(req.prof.username),128,stdin); //sets the user's username, which should be a unique identifier (as well as user_id)
	printf("pick a password\n"); //sets the user's password
    fgets(&(req.prof.password),128,stdin);
	req.prof.user_id = -1; // placeholder since the server assigns user_id
	req.prof.byte_total = 0;
	//putting together the request
	req.type = 0; // sign in request
	//write?
    write(sd, &req, sizeof(struct request)); //send socket request
	int user_id;
	read(sd, &user_id , sizeof(int)); //take back in user_id
	printf("%d\n" , user_id);
    req.prof.user_id = user_id; //add user_id to req
    int profd = open("./myprofile", O_WRONLY | O_CREAT, 0777); //write profile locally
    write(profd, &(req.prof), sizeof(struct profile));
    close(profd);
    //profd = open("./myprofile", O_RDONLY | O_CREAT, 0777);
    // struct profile test;
    // read(profd, &test, 0777);
    // printf("%s, %s, %d\n", test.username, test.password, test.user_id);
	freeaddrinfo(results);

	return 0;
}
