#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/time.h>
#include <sys/select.h> 
#include <sys/time.h> 
#include <sys/types.h> 

using namespace std;

int main(){
	 int PORT = 5060;
      int clientsock[50],sd,activity;
      int msd;
     for(int i=0;i<50;i++) clientsock[i] = 0;
     fd_set fdse;
     int sockfd = socket(AF_INET,SOCK_STREAM,0) , nsockfd;
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
     while(1){
          FD_ZERO(&fdse);
          FD_SET(sockfd,&fdse);
          msd = sockfd;
          for(int i=0;i<50;i++){
               sd = clientsock[i];
               if(sd>0)
                    FD_SET(sd,&fdse);
               if(sd>msd) msd = sd;
          }
          activity = select(msd+1,&fdse,  NULL , NULL , NULL);
          if(activity<0) cout<<"select error!!"<<endl;
     if(FD_ISSET(sockfd,&fdse)){ 
     nsockfd = accept(sockfd, (struct sockaddr *)&client, (socklen_t*)&xx);
     if( nsockfd < 0){
     	cout<<"error in accept!"<<endl;
     return -1;
     }
     else 
     	cout<<"client connected!!"<<endl;
     cout<<msd<<endl;
     for(int i = 0;i<50;i++){
          if(clientsock[i] == 0){
               clientsock[i] = nsockfd;
               break;
          }
     }
}
 for(int i=0;i<50;i++){
          sd = clientsock[i];
     if(FD_ISSET(sd,&fdse)){
     char buff[2048]={0};
     //bzero(buff,2048);
     int re = read(sd, buff, 2048);
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
          send(sd,pp,strlen(pp),0);   
       	cout<<"send!!"<<endl;
     }
     else if(strncmp("send",buff,4)==0){
          char pp[]="$";
          send(sd,pp,strlen(pp),0); 
          char buff2[2048]={0};
          re = read(sd, buff2, 2048);
          cout<<"file name: "<<buff2<<endl;
          FILE *fs = fopen(buff2, "r");
          if(fs == NULL)
          {
             char zz[]="$$";
              send(sd,zz,strlen(zz),0); 
          }else{
               char zz[]="Sending.";
               send(sd,zz,strlen(zz),0); 
               char fbuf[512];
               bzero(fbuf,512);
               int fs_b;
               while((fs_b = fread(fbuf, sizeof(char), 512, fs))>0){
                  if(send(sd, fbuf, fs_b, 0) < 0)
                    {
                  cout<<"Error sending !! :(\n";
                 break;
                    }
                bzero(fbuf,512);
               }
               cout<<"File Sent!!\n";
          }
       }
     }
}
}
     return 0;
 }