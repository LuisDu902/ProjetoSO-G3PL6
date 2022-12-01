#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    
    /* check if exactly four arguments are present */
    if (argc != 4){
        printf("usage: tokenring n p t\n");
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);
    double p = atof(argv[2]);
    int t = atoi(argv[3]);
    
    int fd_write;
    int fd_read;

    char read_pipe[64] = "\0";
    char write_pipe[64] = "\0";
    char pipe[64] = "\0";
    pid_t pid;
    int nr_process = 1;
    
    /* create pipes */
    for (int i = 1; i <= n; i++){
        
        /* create last pipe (process n to process 1) */
        if (i == n){

            /* name pipe */
            sprintf(pipe,"pipe%dto1",n);

            /* create pipe */
            if (mkfifo(pipe, 0666) == -1){
                fprintf(stderr, "Unable to create pipe%dto1: %s\n",n, strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
        
        else{
            
            /* name pipe */
            sprintf(pipe,"pipe%dto%d",i,i+1);      

            /* create pipe */
            if (mkfifo(pipe, 0666) == -1){
                fprintf(stderr, "Unable to create pipe%dto%d: %s\n", i,i+1, strerror(errno));
                exit(EXIT_FAILURE);
            }  
        }
    }
   
    /*process 1: reads from pipe(n)to(1)*/
    sprintf(read_pipe,"pipe%dto1",n); 

    /*process 1: writes to pipe(1)to(2)*/
    sprintf(write_pipe,"pipe1to2"); 
    
    for (int i = 2; i <= n; i++){ 
        
        /* create child process */
        if ((pid = fork()) < 0) {
            perror("fork error");
            exit(EXIT_FAILURE);
            }
        
        /* child process */
        else if (pid == 0){  

            nr_process = i;
            
            /* process i: reads from pipe(i-1)to(i) */
            sprintf(read_pipe,"pipe%dto%d",i-1,i); 
            
            /* last process (process n) */ 
            if (i == n){
                
                /* process n: writes to pipe(n)to(1) */
                sprintf(write_pipe,"pipe%dto1",n);

                /* open pipe(n)to(1) to write */
                if ((fd_write = open(write_pipe,O_WRONLY)) < 0){
                    fprintf(stderr, "Unable to open write_pipe: %s\n", strerror(errno));
                }
                
                /* initialize token */
                int token = 0;

                /* write token */
                if (write(fd_write,&token,sizeof(int)) < 0){
                    fprintf(stderr, "Unable to write to pipe: %s\n", strerror(errno));
                }

                close(fd_write);
            }

            else {
                /* process i: writes to pipe(i)to(i+1) */
                sprintf(write_pipe,"pipe%dto%d",i,i+1);
            }
            
            break;
        }
    }
    
    srand(getpid());
   
    for(;;){
        
        /* open read_pipe to read */
        if ((fd_read = open(read_pipe,O_RDONLY)) < 0){
            fprintf(stderr, "Unable to open read_pipe: %s\n", strerror(errno));
        }

        int token;
        
        /* read token */
        if (read(fd_read,&token,sizeof(int)) < 0){
            fprintf(stderr, "Unable to read from pipe: %s\n", strerror(errno));
        }
    
        close(fd_read);

        /* increment token */
        token++;
        
        /* generate a random number between 0 and 1 */
        double random = (double) rand() / (double) RAND_MAX;
        
        if (random <= p) { 
            
            printf("[p%d] lock on token (val = %d)\n",nr_process,token);
            
            /* block submission during t seconds */
            sleep(t);

            printf("[p%d] unlock on token\n",nr_process);
        }

        /* open write_pipe to write */
        if ((fd_write = open(write_pipe,O_WRONLY)) < 0){
            fprintf(stderr, "Unable to open write_pipe: %s\n", strerror(errno));
        }
        
        /* write token */
        if (write(fd_write,&token,sizeof(int)) < 0){
            fprintf(stderr, "Unable to write to pipe: %s\n", strerror(errno));
        }

        close(fd_write);
    }
}
