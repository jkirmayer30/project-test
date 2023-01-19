### Instructions:
##### Server:
1. `make` or `make compile` in root of repo to compile
2. `make s` to run server on localhost, `make s ARGS="<server_ip_address>"` in order for non-local clients to connect. Keep in mind this doesn't work between a lab machine and our home computers, but across the lab network it works.
##### Clients:
1. To initialize a user on a local server, run `make init`, and enter username and password. Once you've done this, you can `make chat` and wait for another user to match with you. To have two clients running on the same machine, you'll need them in separate directories. For this, you can `cd` one into `tester_1` and one into `tester_2`, and run the same make commands. Note that make init creates a new account, overwriting your profile locally but creating a new profile on the server.
2. For a non-local server, run `make init ARGS="<server_ip_address>"`, and then `make chat ARGS="<server_ip_address>"`
3.  Once you've matched with someone, you can type and hit enter to send them a message. You will receive any messages they send you, signed by their username

Note: If at any point the client and server fail to connect, and there is nothing wrong with your IP addresses, you may have to edit the port number inside of the three files `server/server.c`, `client/client_init_test.c`, `client/client_chat_test.c`.



### All working features:
- Create forking server and connect via command line arguments
- initialize account, which locally stores your profile in the directory of your client, and in userfile on the server
- chat feature, which matches you with the first available person connected to chat.
- Each message you receive displays the username of the sender


### Known Bugs:
- In certain situations when two users get matched, one is not notified by the server and therefore not able to communicate
- A user sometimes get matched to a profile who is either overwritten or not online, which again means only one side is able to communicate
- Running a server several times or closing a server before all clients are all offline often messes up the socket, and to fix this you have to edit the port number inside the code for `server/server.c`, `client/client_init_test.c`, `client/client_chat_test.c` (yes, every client must be fixed). Waiting several minutes also seems to fix this issue
