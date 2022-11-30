#include <stdio.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    
    /* check if exactly four arguments are present */
    if (argc != 4) {
        printf("usage: samples file numberfrags maxfragsize\n");
        return EXIT_FAILURE;
    }
    
    struct stat info;
    int numberfrags = atoi(argv[2]);
    int maxfragsize = atoi(argv[3]);

    /* check if file can be opened and is readable */
    FILE* file = fopen(argv[1], "r");
    if (stat(argv[1], &info) == -1) {
        fprintf(stderr, "error: cannot open %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    /* calculate size of file */
    fseek(file, 0, SEEK_END);
    off_t size = ftello(file);
    fseek(file, 0, SEEK_SET);
    
    srand(0);

    /* print fragments */
    for (int i = 0; i < numberfrags; i++) {
        fseek(file, rand() % (size), SEEK_SET);
        printf(">");
        for (int j = 0; j < maxfragsize; j++) {
            char c = fgetc(file);
            if (!feof(file) && c != '\n') printf("%c", c);
        }
        printf("<\n");
    }

    /* close file */
    fclose(file);
    return EXIT_SUCCESS;
}