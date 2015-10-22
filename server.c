#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

int main(int argc,char *argv[])
{
    int sockfd = 0;
    int connfd = 0;
    struct sockaddr_in serv_addr,cli_addr;
    char sendBuff[1025];
    int portno;
    int clilen;
    int num;
    
    
    if( argc < 2)
    {
    fprintf(stderr,"\n Please enter port! \n");
    exit(1);
    }

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
    printf("\n Socket error \n");
    exit(1);
    }
    
    //printf("Socket retrieve success\n");
    
    bzero((char *)&serv_addr,sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(portno);

    /* Server binds to a socket with IP:Port */
    if((bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))) < 0 )
    {
    printf("\nSocket bind error\n");
    exit(1);
    }


    if(listen(sockfd, 10) < 0)
    {
        printf("Failed to listen on port %d\n",portno);
        exit(1);
    }

    /* Infinite loop to listen to client and send file to client */
    while(1)
    {
    
       clilen = sizeof(cli_addr);
       if((connfd = accept(sockfd, (struct sockaddr*)&cli_addr,&cli_addr)) < 0)
        {
        printf("Unable to connect with client - error in connection");
        exit(1);
        }
        
        bzero(sendBuff,1025);
        
        num = read(connfd,sendBuff,1024);
        
        //printf("%s",sendBuff);
        //sendBuff[strlen(sendBuff)-1]='\0';
    
        
        if ( num < 0 )
        {
        printf(" error reading socket");
        }
        
        
         

        FILE *fp = fopen(sendBuff,"rb");
        if(fp==NULL)
        {
            printf("\nFile %s not found\n",sendBuff);
            
            return 1;   
        }   

       
        while(1)
        {
            /* First read file in chunks of 10 bytes */
            unsigned char buff[10]={0};
            int nread = fread(buff,1,10,fp);
            printf("Bytes read %d \n", nread);     
              

            /* If read was success, send data. */
            if(nread > 0)
            {
                printf("Sending \n");
                write(connfd, buff, nread);
            }

           
            if (nread < 10)
            {
                if (feof(fp))
                    printf("End of file\n");
                if (ferror(fp))
                    printf("Error reading\n");
                break;
            }

}
        close(connfd);
        sleep(1);
    }


    return 0;
}