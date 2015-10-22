#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/stat.h>


int main(int argc, char *argv[])
{
    int sockfd = 0;
    int bytesReceived = 0;
    char recvBuff[256];
    int portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int n;
    
       
    
    if (argc < 4) 
    {
       fprintf(stderr,"\nUsage %s <Filename> <Server hostname or IP> <port> \n", argv[0]);
       exit(0);
    }
    
    /* Create a socket first */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }
    
    
     server = gethostbyname(argv[2]);
     if (server == NULL) 
     {
        fprintf(stderr,"ERROR, no  host %s found!\n",argv[2]);
        exit(0);
     }
        
         /* Initialize sockaddr_in data structure */
        
        bzero((char*)&serv_addr,sizeof(serv_addr));
        
        portno = atoi(argv[3]);
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
        serv_addr.sin_port = htons(portno); // port
        
        
    /* Attempt a connection */
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }
    
     //printf("\nEnter a file name:");
     //bzero(recvBuff,256);
     //fgets(recvBuff,255,stdin);
     
     n=write(sockfd,argv[1],strlen(argv[1]));
     
     //recvBuff[strlen(recvBuff)-1]='\0'; 
    
     if (n<0)
     {
     printf("error writing to the socket");
     return(1);
     }
    /* Create file where data will be stored */
    FILE *fp;
    fp = fopen(argv[1], "wb"); 
    if(NULL == fp)
    {
        printf("Error opening file");
        return 1;
    }

     printf("Lets Read\n");
    /* Receive data in chunks of 5 bytes */
     bzero(recvBuff,256);
    while((bytesReceived = read(sockfd,recvBuff,10)) > 0)
    {
        printf("Bytes received %d\n",bytesReceived);    
        // recvBuff[n] = 0;
        fwrite(recvBuff,1,5,fp);
        fwrite(recvBuff+5,1,5,fp);
        // printf("%s \n", recvBuff);
    }

    if(bytesReceived < 0)
    {
        printf("\n Read Error \n");
    }


    return 0;
} 