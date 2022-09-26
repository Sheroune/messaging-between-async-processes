# Messaging between async processes

 ### *Server*:  
- Creates a message queue
- Writes as the 1st message all subdirectories of the current directory
- As the 2nd message – the number of files in them
- Finally, as the 3rd message – the host name of the computer

> Messages received from clients are printed, and then the message queue is deleted

### *1st Client*:  
- Selects the 2nd message from the message queue
- Determines the names of the owners of these files

### *2nd Client*:  
- Selects the 1st message from the queue
- Determines the size in bytes of each subdirectory
- Writes this information to the message queue

# Compiling and Usage

`make` to compile  
`./server.o` to run server  
`./client1.o` to run 1st client  
`./client2.o` to run 2nd client  
