#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <pthread.h>


int fd=-1;
pthread_t RX_pthread=NULL; //puntatore al task di ricezione
pthread_t TX_pthread=NULL; //puntatore al task di trasmissione
char exit_flag=0;



void help()
{
        printf("Usage: ./usart [/dev/ttyUSB0] [9600/19200/57600/115200] \n\n");
}

//apre la porta seriale
void openUart(char *file)
{
        fd =open(file,O_RDWR|O_NOCTTY|O_NDELAY);

        if(fd==-1){
            printf("open %s fail\n",file);
            return; 
        }       
        if(fcntl(fd,F_SETFL,0)<0){
            printf("fcntl faild!\n"); 
        }
        isatty(STDIN_FILENO);
}

//inizializza la baudrate
void initUart(int baudrate)
{
        struct termios opt;

        tcgetattr(fd,&opt);
        switch(baudrate)
        {
           
            case 9600:
                cfsetispeed(&opt,B9600);
                cfsetospeed(&opt,B9600);
                printf("baudrate = 9600 \n");
                break;
            case 19200:
                cfsetispeed(&opt,B19200);
                cfsetospeed(&opt,B19200);
                printf("baudrate = 19200 \n");
                break; 
            case 57600:
                cfsetispeed(&opt,B57600);
                cfsetospeed(&opt,B57600);
                printf("baudrate = 57600 \n");
                break; 
            case 115200:
                cfsetispeed(&opt,B115200);
                cfsetospeed(&opt,B115200);
                printf("baudrate = 115200 \n");
                break;
                
        }
	opt.c_lflag &= ~ECHO;
	opt.c_lflag &= ~ECHOE; // Disable erasure
	opt.c_lflag &= ~ECHONL; // Disable new-line echo
        opt.c_cflag &= ~CSIZE;
        opt.c_cflag |= CS8;
        opt.c_cflag |= CLOCAL|CREAD;
        opt.c_cflag &= ~PARENB;
        opt.c_iflag &= ~INPCK;
        opt.c_cflag &= ~CSTOPB;
        opt.c_cc[VTIME]=0;
        opt.c_cc[VMIN]=0;
        tcflush(fd,TCIOFLUSH);
        tcsetattr(fd,TCSANOW,&opt);
}

//chiude la porta seriale
void closeUart(){
    if(fd!=-1)
        close(fd);  
}

//thread di ricezionne
void * rx_func(void * argv)
{
    char rec_buf[1024];
    char *end;
    int step=1;
    FILE* f =NULL;
    while(exit_flag!=1)
    {
    	int count=0;
        memset(rec_buf,0x0,sizeof(rec_buf));
        while(exit_flag!=1){
        	int nread=read(fd,rec_buf+count,sizeof(rec_buf));
        	if(nread>0){
	        	count+=nread;
	        	if((end=strstr(rec_buf,"\n"))!=NULL)
			{
				printf("RX: %s",rec_buf);    
				
				if (strcmp(rec_buf,"START\n")==0) {
					if(f==NULL){
						//creo nuovo file
						printf("CREO FILE\n");
						f = fopen("data.dat", "w");
						if(f==NULL){
							printf("APERTURA FILE ERRATA");  
							exit_flag=1;
						}else{
							fprintf(f, "0,0,0,0,0.00\n");
						}
					}
				} else if(strcmp(rec_buf,"STOP\n")==0) {
				 
					//chiudo  file	
					printf("CHIUDO FILE\n");
					if(f!=NULL){
                        			printf("CLOSE\n");
						fclose(f);	
                        			f=NULL;
                    			}
					step=1;			
				} else {
					//aggiungo riga al file	
					if(f!=NULL){
						if((end=strstr(rec_buf,"#"))!=NULL){
							fprintf(f, "%d,%s",step,&rec_buf[1]);										
							step++;				
						}
					}
				}

			
			
			             
			        count=0;
			        memset(rec_buf,0x0,sizeof(rec_buf));			
			}

	        }
        	
        }
    }
    return;
}

//thread di trasmissione
void * tx_func(void * argv)
{
    char ch;
    char tx_buf[256];
    while(exit_flag!=1) {
    	int count=0;
    	memset(tx_buf,0,sizeof(tx_buf));
    	printf("TXBUFF: %s",tx_buf);
	while((ch = getchar()) != '\n')
	{
		if(ch=='Q') {
			exit_flag=1;
			break;
		}
		else {
			tx_buf[count]=ch;
			count++;
		}

	}
	if(ch!='Q') {
		tx_buf[count]=ch;
		count++;
	    	write(fd,tx_buf,strlen(tx_buf));
	}
	
    }
    exit_flag=1;
    return;
}

int main(int argc , char *argv[])
{

        int result;
        int baudrate=-1;
        char file_name[20];

        int i;
        printf("argc=%d\n",argc);
        for(i=0;i < argc;++i)
            printf("argv[%d]:=%s\n",i,argv[i]);

        if(argc<3) {
            help();
            return 0;
        }
        memset(file_name,0,sizeof(file_name));
        sprintf(file_name,"%s",argv[1]);
        printf("Seriale is %s\n",file_name);
        

        baudrate=atoi(argv[2]);
        
        openUart(file_name);
        initUart(baudrate);
        
        //write(fd,"open:uart0 send ......\n",25);


        pthread_create(&RX_pthread,NULL,rx_func,NULL);
        pthread_create(&TX_pthread,NULL,tx_func,NULL);

        pthread_join(RX_pthread,NULL);
        pthread_join(TX_pthread,NULL);
        closeUart();
        return 0;
}

