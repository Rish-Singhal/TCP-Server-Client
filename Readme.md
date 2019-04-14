## A basic server and client using sockets
            The server and client will be two different codes which will be run in different directories.  
            They will both can communicate by establishing socket connections. The client will ask  
            for information like - “list of files in server’s directory”. The server will return the name of . 
            all the files in its directory. Then client will ask for a specific file.  
            The client will create a connection to the server and send requests to download files to . 
            the client directory. It will write the files into its own directory. For example, let the server . 
            directory contain: file1.txt, file2.txt, file3.txt . 

## Working
            Client
            >> listall // client asked for the list of files in server’s directory . 
            file1.txt // response [list of files] sent by server . 
            file2.txt . 
            file3.txt . 
            >> send file2.txt // client downloaded file2.txt in its own directory . 
            >> send file3.txt // client downloaded file2.txt in its own directory . 
            Error handling such as requesting missing files must be handled.  
            Multiple clients are possible.  

## Implementation
     1. Using Sockets
     2. Using Fdset
    
