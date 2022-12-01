#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    
    /* check if more than one argument is present */
    if (argc < 2){
        printf("usage: txt2epub filename_1.txt ... filename_n.txt\n");
        return EXIT_FAILURE;
    }
    
    pid_t pid;

    for(int i = 1; i < argc; i++){
        
        if ((pid = fork()) == -1){
            perror("fork");
            return EXIT_FAILURE;
            }
        
        /* child process */
        if(pid == 0){
            printf("[pid%d] converting %s\n",getpid(), argv[i]);
                
            /* convert filename.txt to filename.epub */
            char* file = malloc(strlen(argv[i])+1);
            strcpy(file,argv[i]);
            memmove(&file[strlen(argv[i])-4], &file[strlen(argv[i])], strlen(file));
            file = strcat(file,".epub");
            
            /* pandoc */
            char* pan[5] = {"pandoc", argv[i], "-o", file, NULL};
            if (execvp(pan[0], pan) == -1) {
                perror("execvp");
                return EXIT_FAILURE;
                }
            exit(0);
            }
        }
    /* parent process waits until all child processes finish */
    for(int i = 1; i < argc; i++)
        wait(NULL);
    
    /* parent process */
    char* ebooks[argc+2];
    ebooks[0] = "zip";
    ebooks[1] = "ebooks.zip";
    int index = 2;
        
    for(int i = 1; i < argc; i++){
            
        /* convert filename.txt to filename.epub */
        char* file = malloc(strlen(argv[i])+1);
        strcpy(file,argv[i]);
        memmove(&file[strlen(argv[i])-4], &file[strlen(argv[i])], strlen(file));
        file = strcat(file,".epub");
            
        ebooks[index++] = file;
        }
    ebooks[index] = NULL;
        
    /* zip */
    if (execvp(ebooks[0], ebooks) == -1) {
        perror("execvp");
        return EXIT_FAILURE;
        }
    return EXIT_SUCCESS;
}
