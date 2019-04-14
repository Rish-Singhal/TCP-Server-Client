#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
using namespace std;

int main(){
     int PORT = 5060;
     int sockfd = socket(AF_INET,SOCK_STREAM,0);
     

     struct sockaddr_in server;
     memset(&server, '0', sizeof(server));
     
     server.sin_family = AF_INET;
     server.sin_port = htons(PORT);
     server.sin_addr.s_addr = inet_addr("127.0.0.1");

     if(connect(sockfd, (struct sockaddr*)&server, sizeof(server)) != 0){
	     cout<<"connection failed"<<endl;
     }
     while(1){
	     string t;
	     cin>>t;
	   //  cout<<t<<endl;
	     if(t=="exit") {
	     	char x[] = "exit";
		    write(sockfd,x,strlen(x));
	     	close(sockfd);
	     	return 0;
	     }

	     if(t=="listall"){
		     char x[] = "listall";
		     write(sockfd,x,strlen(x));
		     char buff[2048]={0};
		     int re = read(sockfd,buff,2048);
		     for(auto &c:buff){
		      if(c=='$') c='\n';
		     }
		     cout<<buff;
	     }
	     else if(t=="send"){
	     	 char x[] = "send";
		     write(sockfd,x,strlen(x));
		     char buff[2048]={0};
		     int re = read(sockfd,buff,2048);
		     if(buff[0]=='$'){
		     string ff; cin>>ff;
		     char x[ff.size()+1];
		     strcpy(x,ff.c_str());
		     write(sockfd,x,strlen(x));
		     char buff2[2048]={0};
		     re = read(sockfd,buff2,2048);
		     if(buff2[0]=='$' && buff2[1]=='$'){
		     	cout<<"File not Found.!"<<endl;
		     }
		     else{  	
			  FILE *fr = fopen(x, "a");
			  if(fr == NULL)
				printf("File %s Cannot be opened.\n", x);
			  else
				{
			  char rbuf[512];   	
		      bzero(rbuf, 512); 
		      int fr_b = 0;
	        	while(fr_b = read(sockfd, rbuf, 512))
	        	{
	            if(fr_b < 0)
	            {
	                cout<<"Error in recieving"<<endl;
	            }
	            int write_sz = fwrite(rbuf, sizeof(char), fr_b, fr);
	            if(write_sz < fr_b)
	            {
	                cout<<"Error in writing to file."<<endl;
	            }
				else if(fr_b)
				{
					break;
				}
	            bzero(rbuf,512);
	       		 }
	        fclose(fr);
		     }
		 }
	     }
	 }
     }
     return 0;
}
			

