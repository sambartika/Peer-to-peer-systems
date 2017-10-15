#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>

//the thread function
void *server_side(void *);
void *client_side(void *);

int main()
{
  printf("==================================\n");
  printf("============WINDOW 2==============\n");
  printf("==================================\n");  
  
 
     
  pthread_t thread_id,thread_id1;


  if( pthread_create( &thread_id , NULL , server_side ,NULL) < 0)
  {
	  perror("could not create thread");
	  return 1;
  }

  if( pthread_create( &thread_id1 , NULL , client_side ,NULL) < 0)
  {
	  perror("could not create thread");
	  return 1;
  }

  pthread_join( thread_id , NULL);
  pthread_join( thread_id1 , NULL);

  return 0;
}

void *server_side(void *socket_desc)
{
  int listenfd = 0,connfd = 0,n=0,i=0,no=0,flag=0,len=0;   //declaring variables
  int arr[200],in=0,flag1=0;

  
  struct sockaddr_in serv_addr;
 
  char sendBuff[1025],recvBuff[1025];  
  int numrv;  

  listenfd = socket(AF_INET, SOCK_STREAM, 0);    //creating server socket
  
  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(sendBuff, '0', sizeof(sendBuff));
      
  serv_addr.sin_family = AF_INET;    
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
  serv_addr.sin_port = htons(5001);    //assigning 5000 as the port of the server
 
  bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
  
  if(listen(listenfd, 10) == -1){
      printf("Failed to listen\n");
      return;
  }
     
  
  while(1)
  {
      flag=0;
      connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL); // accept awaiting request

      if(flag1==0)   //condition to check if the server needs to read or write
      {
        while((n = read(connfd, recvBuff, sizeof(recvBuff)-1)) > 0)   //reading from client
        {
          recvBuff[n] = 0;
          printf("\n");
        }

        len=strlen(recvBuff);
        for(i=0;i<(len-1);i++)   //loop to extract the number as a string
        sendBuff[i]=recvBuff[i];
        sendBuff[i]='\0';
  
        no=atoi(sendBuff);    //converting the number into integer
      

        for(i=0;recvBuff[i]!='\0';i++)
        {
	  if(recvBuff[i]=='p')    //condition to check if put operation is required or get
	  {
		for(i=0;i<in;i++)
		{
		    if(arr[i]==no)   //condition to check if the number already exists in the array or not
		    {
			flag=1;
			break;
		    }
		}
		if(flag==0)
		{
			arr[in++]=no;
		}


		break;
	  }
	  else if(recvBuff[i]=='g')
	  {
		for(i=0;i<in;i++)   // loop to chech if the number is present in the array or not
		{
		    if(arr[i]==no)
		    {
			strcpy(sendBuff,"THE NUMBER IS PRESENT IN 2ND SOCKET");
			flag=1;
			break;
		    }
		}
		if(flag==0)
		{
		    strcpy(sendBuff,"THE NUMBER IS NOT PRESENT IN 2ND SOCKET");
		}
		flag1=1;
		break;
	  }

        }

        if( n < 0)
        {
          printf("\n Read Error \n");
        }
   
        close(connfd);
      }
      else
      {    
	write(connfd, sendBuff, strlen(sendBuff));   //writin in the client
        flag1=0;
	close(connfd);
      }
      sleep(1);
  }
 
}

void *client_side(void *socket_desc)
{
  int sockfd = 0,n = 0,ch=0,no=0,i=0,len;    //declaring variables
  char recvBuff[1024],sendBuff[1024];
  struct sockaddr_in serv_addr;
  do{
      printf("DO YOU WANT TO CONTINUE ? ENTER 1 FOR YES 0 FOR NO .\n");
      scanf("%d",&ch);

      if(ch!=0)    //condition to check if the user wants to exit or not 
      {
        printf("ENTER 1 TO PUT 2 TO GET\n");
        scanf("%d",&ch);
      	printf("ENTER THE NO : \n");
      	scanf("%s",sendBuff);

      	n=atoi(sendBuff);
      	n=n%4;    //performing mod 4 operation
      	n=n+5000;

      	memset(recvBuff, '0' ,sizeof(recvBuff));    //opening client socket
      	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
      	{
      		printf("\n Error : Could not create socket \n");
      		return;
      	}
 
      	serv_addr.sin_family = AF_INET;
      	serv_addr.sin_port = htons(n);
      	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
 
      	if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
      	{
      		printf("\n Error : Connect Failed \n");
      		return;
      	}

      }

      switch(ch)   //switch statement to put and get numbers
      {
	case 1:
		len=strlen(sendBuff);
		sendBuff[len]='p';   //inserting p at the end to notify server that it is a put request
		sendBuff[len+1]='\0';
 
  		write(sockfd, sendBuff, strlen(sendBuff));    //write the string at the server
                close(sockfd);

		break;
	case 2:
		len=strlen(sendBuff);
		sendBuff[len]='g';    //inserting g at the end to notify server that it is a get request
		sendBuff[len+1]='\0';
 
  		write(sockfd, sendBuff, strlen(sendBuff));    //write the string at the server
                close(sockfd);

      		memset(recvBuff, '0' ,sizeof(recvBuff));
      		if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
      		{
      			printf("\n Error : Could not create socket \n");
      			return;
      		}
 
      		serv_addr.sin_family = AF_INET;
      		serv_addr.sin_port = htons(n);
      		serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
 
      		if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
      		{
      			printf("\n Error : Connect Failed \n");
      			return;
      		}

      		while((n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)    //read from server that if the numbet is present or not
      		{
			recvBuff[n] = 0;
      			if(fputs(recvBuff, stdout) == EOF)
    			{
      				printf("\n Error : Fputs error");
    			}
      			printf("\n");
      		}
                close(sockfd);

		break;
		
      }
    }while(ch!=0);

}
