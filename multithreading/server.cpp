#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/time.h>
#include <sys/select.h> 
#include<pthread.h>
#include <sys/types.h> 

using namespace std;
void *connection_handler(void *socket_desc);

int main(){
	 int PORT = 5060;
     int sockfd = socket(AF_INET,SOCK_STREAM,0) , nsockfd;
     int *new_sock;
     if(sockfd < 0){
	     cout<<"socket failure!"<<endl;
	     return -1;
     }
     
     struct sockaddr_in add, client;
     int xx = sizeof(client);
     int opt = 1;
     if( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,  
          sizeof(opt)) < 0 )   
    {   
        cout<<"setsocket failed!";
       return -1;  
    }   
     add.sin_family = AF_INET;
     add.sin_port = htons(PORT);
     add.sin_addr.s_addr = htonl(INADDR_ANY);

     if(bind(sockfd, (struct sockaddr *)&add,sizeof(add))<0){
     	cout<<"binding error!!"<<endl;
     	return -1;
     }

     if(listen(sockfd,3)!=0){
     	cout<<"error in listen!!"<<endl;
     	return -1;
     }

    while( nsockfd = accept(sockfd, (struct sockaddr *)&client, (socklen_t*)&xx))
    {
        cout<<"Connection created!!"<<endl;

        pthread_t nthread;
        new_sock =(int*) malloc(1);
        *new_sock = nsockfd;

        if( pthread_create( &nthread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            cout<<"Error in creating thread!!"<<endl;
            return 1;
        }
    }
    return 0;

}

void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int nsockfd = *(int*)socket_desc;
    while(1){     
      char buff[2048]={0};
     //bzero(buff,2048);
     int re = read(nsockfd, buff, 2048);
     cout<<buff<<endl;
     if(strncmp("listall",buff,7)==0){
          //char pp[]="gotchaaa!!!!";
          int i = 0;
          char pp[2048]={0};
          struct dirent *de;
          DIR *dir = opendir("."); 
           while ((de = readdir(dir)) != NULL) 
            {
               int j = 0;
               while(de->d_name[j]!='\0'){
                    pp[i]=de->d_name[j];
                    i++;
                    j++;
               }
               pp[i]='$';
               i++;     
            }
          closedir(dir);
          send(nsockfd,pp,strlen(pp),0);   
          cout<<"send!!"<<endl;
     }
     else if(strncmp("send",buff,4)==0){
          char pp[]="$";
          send(nsockfd,pp,strlen(pp),0); 
          char buff2[2048]={0};
          re = read(nsockfd, buff2, 2048);
          cout<<"file name: "<<buff2<<endl;
          FILE *fs = fopen(buff2, "r");
          if(fs == NULL)
          {
             char zz[]="$$";
              send(nsockfd,zz,strlen(zz),0); 
          }else{
               char zz[]="Sending.";
               send(nsockfd,zz,strlen(zz),0); 
               char fbuf[512];
               bzero(fbuf,512);
               int fs_b;
               while((fs_b = fread(fbuf, sizeof(char), 512, fs))>0){
                  if(send(nsockfd, fbuf, fs_b, 0) < 0)
                    {
                  cout<<"Error sending !! :(\n";
                 break;
                    }
                bzero(fbuf,512);
               }
               cout<<"File Sent!!\n";
          }
       }
     else if(strncmp("exit",buff,4)==0){
          break;
     }
  }
    free(socket_desc);
    close(nsockfd);
    pthread_exit(NULL); 
}
