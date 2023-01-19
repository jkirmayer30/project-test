# Can't You C I Am C-ing

# Members
## Jacob Kirmayer, Ariel Fuchs, Sam Cowan

# Chat Server
# A statement of the problem you are solving and/or a high level description of the project.
    Do you constantly feel lonely? Do you have no friends and requoire the compliments of strangers on the internet to boost your self esteem? Well then you are in luck because now with Cinder you can connect to strngers to communicate about all of your problems.
# A description as to how the project will be used (describe the user interface).
  
# A description of your technical design. This should include:
   
### How you will be using the topics covered in class in the project.

- Processes: Will involve forking in a forking server
- Sockets: To enable communication between different users
- Pipes: Once matched, there will be a pipe to send and recieve messages from someone
- Working with Files: chat logs and profile information will be stored in files
### How you are breaking down the project and who is responsible for which parts.
  
### What data structures you will be using and how.
     Structs- Will store all relevant infomoration about a user.
### What algorithms and /or data structures you will be using, and how.


  Client:
  - Has profile file (store profile struct)
  - Has a local file storing message history

  Server:
  - Has a list of profile structs stored in a binary file
  - Contiains pipes for communication that can be read/written to 
  
  Routes:  
    - swipe_right: Client sends a user id 
    - send_message: Sends a string message to its assigned pipe
    - initialize_account: Sends the profile struct
    - look_for_friends: Writes a randomly generated list of users' profile structs

# A timeline with expected completion dates of parts of the project.
### MVP
1. Single Use server - 1/10 (DONE)
2. Forking server - 1/11 (DONE)
3. Chat communication - 1/12  (DONE) 
### Expectation
4. Clients input info send profile struct to server - 1/13 (DONE)
5. Client can request profiles from server and swipe right to connect to chat - 1/17 NOPE
### Stretch Goals
6. More security (exec ssh keygen?) - NOPE

### Instructions:
##### Server:
1. `make` or `make compile` in root of repo to compile
2. `make s` to run server on localhost, `make s ARGS="<server_ip_address>"` in order for non-local clients to connect. Keep in mind this only works between linux machines
##### Clients:
1. To initialize a user on a local server, run `make init`, and enter username and password. Once you've done this, you can `make chat` and wait for another user to match with you. To have two clients running on the same machine, you'll need them in separate directories. For this, you can `cd` one into `tester_1` and one into `tester_2`, and run the same make commands
2. For a non-local server, run `make init ARGS="<server_ip_address>"`, and then `make chat ARGS="<server_ip_address>"`
3.  Once you've matched with someone, you can type and hit enter to send them a message. You will receive any messages they send you

Note: If at any point the client and server fail to connect, and there is nothing wrong with you IP addresses, you may have to edit the port number inside of the three files `server/server.c`, `client/client_init_test.c`, `client/client_chat_test.c`.


