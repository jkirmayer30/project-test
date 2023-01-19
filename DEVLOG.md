
##### 1-06-2023 10:14 - SamC, ArielF, JacobK:  
- wrote project proposal for Cinder
- wrote profile struct
----------------------
##### 1-09-2023 10:05 - ArielF:  
- Changed project proposal to reflect change to our proposal
----------------------
##### 1-09-2023 10:12 - JacobK:  
- Changed profile struct to reflect new changes - username, password, status instead of name, rating, bio, etc.
- Added request struct to be sent between client/server
- Began work on server
----------------------
##### 1-09-2023 10:13 - SamC:  
- Began work on client
----------------------
##### 1-10-2023 10:14 - ArielF, SamC:
- Completed client code
- Established and tested client and server connection between multiple machines
- Client takes in data w/ fgets and sends a struct with another struct inside to server
----------------------
##### 1-10-2023 10:14 - JacobK:
- Completed server code
- Established and tested client and server connection between multiple machines
- Server receives request struct and confirms to client
----------------------
##### 1-11-2023 10:12 - SamC, JacobK, ArielF:
- Fixed merge conflicts
- Made and tested forking server
- Fixed formatting issues
----------------------
##### 1-11-2023 22:08 - SamC, JacobK
- server stores received profiles in userfile file
- server sends client user id identifier based on userfile size
- client stores profile in local myprofile file
----------------------
##### 1-12-2023 10:08 - ArielF
- Cleaned makefile for simpler server initialization, also added ARGS to select ip from command line
----------------------
##### 1-12-2023 10:08 - SamC and JacobK
- Planned chat communication and edited structs to match the plan
----------------------
##### 1-13-2023 10:14 - JacobK, SamC
- Updated struct.h further
- defined functions using lseek to write to a struct
- began work on request type 1 (request of match)
----------------------
##### 1-14-2023 18:53 - SamC
- extra documentation, formatting
- todo list
----------------------
##### 1-14-2023 21:59 - SamC, JacobK
- made the server match up two users and write it to file
----------------------
##### 1-15-2023 22:50 - JacobK, SamC
- Spent hours in pipe hell. Files probably better, but we have one way pipes working
----------------------
##### 1-16-2023 17:36 - JacobK
- Switched out pipes for files
- Working communication between two clients
---------------------- 
##### 1-16-2023 21:16 - SamC
- Displayed sender's username to client
