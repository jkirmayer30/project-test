  #include <sys/socket.h>
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netdb.h>
  #include <time.h>
  #include <stdio.h>
  #include <string.h>
  #include <errno.h>
  #include <unistd.h>
  #include <stdlib.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include "../structs.h"
  #include <signal.h>

static struct addrinfo * hints, * results;
void change_struct(int user_id , struct profile * new_struct){
  printf("Change_struct printing out username: %s", new_struct -> username);
	int userfd = open("./userfile", O_WRONLY, 0777);
	lseek(userfd,user_id*sizeof(struct profile),SEEK_SET);
	write(userfd,new_struct,sizeof(struct profile));
	close(userfd);
	return;
}

int read_struct(int user_id, struct profile * dst){
	struct profile reader;
  int userfd = open("./userfile", O_RDONLY, 0777);
  lseek(userfd,user_id*sizeof(struct profile),SEEK_SET);
  int bytes = read(userfd,dst,sizeof(struct profile));
  close(userfd);
	return bytes;
}


static void sighandler(){
	free(hints);
  	freeaddrinfo(results);
	exit(0);
}

int main(int argc, char *ip[]){
  signal(SIGINT,sighandler);
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; //TCP socket
  hints->ai_flags = AI_PASSIVE; //only needed on server
  getaddrinfo(ip[1], "9999" , hints, &results);  //NULL is localhost or 127.0.0.
  //create socket
  int listen_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  bind(listen_socket, results->ai_addr, results->ai_addrlen);
  listen(listen_socket, 10);//10 clients can wait to be processe
  socklen_t sock_size;
  struct sockaddr_storage client_address;
  sock_size = sizeof(client_address);
  while(1){
    //wait for next client
	  struct request req;
    struct profile newProfile;
    int client_socket = accept(listen_socket,(struct sockaddr *)&client_address, &sock_size);
    fcntl(client_socket, F_SETFL, O_NONBLOCK);
	  printf("Received Connection\n");
	  int is_main_process = fork();
    if (!is_main_process){
      int user_complete = 0;
      int matched = 0;
      int printed = 0;
	    int user_id = -1;
	    int byte_total = 0;
      while(1){
        if (user_id>=0){
          char result_to_send[512];
		      char file_name[10];
          sprintf(file_name,"%d",user_id);
          int inbound_fd = open(file_name,O_CREAT,0777);
    
		      lseek(inbound_fd,byte_total,SEEK_SET);
		      int read_from_inbound = read(inbound_fd,result_to_send,512);
		      close(inbound_fd);
		      byte_total+=read_from_inbound;
		      //printf("%d byte total for user %d\n",byte_total,user_id);
		      if (read_from_inbound){
		        printf("%d bytes from inbound%s\n byte total:%d\n", read_from_inbound,result_to_send,byte_total);
            write(client_socket,result_to_send,512);
          }
        }
	      int bytes_read = read(client_socket,&req,sizeof(struct request));
	      //printf("bytes readL %d", bytes_read);
        //printf("diagnostic thing:\nreq type: %d\nuser_id: %d\nusername: %s\n", req.type, req.prof.user_id, req.prof.username);
        //printf("req type: %d\n", req.type);
        //printf("reached the if\n");
        if (bytes_read > 0){
		      printf("got request type %d from user %d\n",req.type,req.prof.user_id);
		      user_id = req.prof.user_id;
		      if (byte_total==0){
			      byte_total = req.prof.byte_total;
		        printf("set byte_total to %d", byte_total);
          }
          if (req.type==0 && !user_complete){
            printf("username requested\n");
            struct stat filedata;
            stat("./userfile", &filedata);
            int result_userid = filedata.st_size / sizeof(struct profile);
            printf("user id: %d\n", result_userid);
            int userfd = open("./userfile", O_WRONLY | O_APPEND | O_CREAT, 0777);
            if (userfd == -1){
              printf("err 2 %s\n", strerror(errno));
              exit(1);
            }
            write(userfd, &(req.prof), sizeof(struct profile));    
            //printf("what it wrote: %s\n", req.prof.username); 
            close(userfd);  
            userfd = open("./userfile", O_RDONLY, 0777);
            if (userfd == -1){
              printf("err 3%s\n", strerror(errno));
              exit(1);
            }
            struct profile test;
            // lseek(userfd,result_userid*sizeof(struct profile),SEEK_SET);
            // read(userfd, &test, sizeof(struct profile));   
            // printf("read username: %s\n", test.username);                   
            write(client_socket, &result_userid, sizeof(int));
		        char file_name[10];
		        sprintf(file_name,"%d",result_userid);
            int inbound_fd = open(file_name,O_CREAT,0777);
		        close(inbound_fd);
		        printf("opened inbound fikle on fd %d\n",inbound_fd);
		        //close that client
            close(client_socket);
            user_complete = 1;
	    exit(0);
          }
          else if (req.type==1 && !matched){ //requesting a match
            int sender_id = req.prof.user_id;
            printf("match requested\n");
            req.prof.match_id = -2;
            change_struct(req.prof.user_id,&(req.prof));
            int match_id = -1;
            while(match_id < 0){
              struct stat filedata;
              int num_users = filedata.st_size / sizeof(struct profile);
              //printf("%d users found\n",num_users);
	      stat("./userfile", &filedata);
              int userfd = open("./userfile", O_RDONLY, 0777);
              if (userfd == -1){
                printf("err 4 %s\n", strerror(errno));
                exit(1);
              }
              struct profile current_prof;
              //printf("up to for loop\n");
              for (int i = 0; i < num_users; i++){
                read(userfd, &current_prof, sizeof(struct profile));
		//printf("user %d in for loop at userid %d\n",req.prof.user_id,i);
                //printf("currently reading username %s, match_id %d\n", current_prof.username, current_prof.match_id);
                if (current_prof.user_id == req.prof.user_id){ //this is you, do not match
                  if (current_prof.match_id >= 0){
                    match_id = current_prof.match_id;
                    printf("user %d brok the match loop\n",req.prof.user_id);
		    break;
                  }
                }
                else{ //this is not you, try to match
                  if (current_prof.match_id == -2){ // the user is also waiting to match
                    current_prof.match_id = req.prof.user_id;
                    change_struct(i,&current_prof);
                    req.prof.match_id = i;
                    change_struct(req.prof.user_id,&(req.prof));
                    printf("matched %s and %s\n", current_prof.username, req.prof.username);
	            match_id = i;

		  }
                }
              }
	      printf("user %d in loop\n",req.prof.user_id);
              close(userfd);
            }
		        write(client_socket,"You've Been Matched",512);
            matched = 1;
          } 
          else if (req.type==2){
            char usrn[512] = "[";
            unsigned long len = strlen(req.prof.username);
            printf("len %ld\n", len);
            printf("username: %s",req.prof.username);
            for (int i = 0; i < len; i++){
              if (req.prof.username[i] != '\n'){
                printf("%c", req.prof.username[i]);
                usrn[i] = req.prof.username[i];
              }
            }
            strcat(usrn,": ");
            // strcat(usrn,req.prof.username);
            // int len = strlen(usrn);
            // usrn[len-1] = 0;
            //strcat(usrn, "): ");
			      int sender_id = req.prof.user_id;
			      int bytes_read = read_struct(sender_id,&(req.prof));
			      printf("read profile of size %d\n",bytes_read);
			      int receiver_id = req.prof.match_id;
			      char file_name[10];
            sprintf(file_name,"%d",receiver_id);
			      printf("got here\n");
			      int outbound_fd = open(file_name,O_WRONLY|O_APPEND,0777);
			      printf("Writing message %s to file %s on fd %d\n",req.payload,file_name,outbound_fd);
			      int retval = write(outbound_fd,strcat(usrn, req.payload),512);
			      printf("writing to receiver's file returned %d\n",retval);
			      close(outbound_fd);
			      printf("closed the pipe for writing, errno:%s\n",strerror(errno));
			      printf("waiting to read our own messages\n");
		      }
		      printf("user %d made it to bottom\n",user_id);
          //return 0;
        }
      }
      close(client_socket);
    }
  }
  //free(hints);
  //freeaddrinfo(results);
  return 0;
}
