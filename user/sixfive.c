#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void sixfive(char* filename){
    int file = open(filename, O_RDONLY);

    if(file < 0){
        fprintf(2, "sixfive: cannot open %s\n", filename);
        return;
    }

    char buffer[512];

    while(read(file, buffer, 512) > 0){
        int i = 0;

        // printf("%s\n", buffer);
        while(buffer[i] != '\0'){
            char c = buffer[i];
            char num[512];
            int j=0;

            int convert = 1;
            if(c == ' ' || c == '/' || c == ',' || c == '\t'
            || c == '\r' || c == '\n' || c == '-' || c == '.'){
                i++;
                continue;
            }
            else {
                convert = 1;
                while(buffer[i] != '\0' && buffer[i] != ' ' && buffer[i] != '/' && buffer[i] != ',' && buffer[i] != '\t'
                    && buffer[i] != '\r' && buffer[i] != '\n' && buffer[i] != '-' && buffer[i] != '.'){
                        if(buffer[i] < '0' ||  buffer[i] > '9'){
                            convert = 0;
                        }
                        
                        num[j++] = buffer[i];
                        num[j] = '\0';
                        i++;
                        // printf("%s\n", num);
                    }
                }
                    
            int number = atoi(num);

            if(convert == 1 && number != 0 && (number%6 == 0 || number%5 == 0)){
                printf("%d \n", number);
            }
        }
    }

    close(file);
}

int main(int argc, char *argv[]){
    // if(argc < 2){
    //     fprintf(2, "usage: sixfive filename\n");
    //     exit(1);
    // }

    if(argc == 1){
        char buf[100];
        int n = read(0, buf, sizeof(buf) - 1);
    
        if (n > 0) {
            // printf("%d\n", n);
            // printf("%c\n", buf[n-2]);
            if (n >= 2 && buf[0] == '"' && buf[n - 2] == '"') {
                memmove(buf, buf + 1, n - 2);
                n -= 2;
            }

            buf[n] = '\0';
    
            int fd = open("test.txt", O_CREATE | O_WRONLY);
    
            if (fd < 0) {
                printf("Error creating file\n");
                exit(1);
            }
    
            write(fd, buf, n);
    
            close(fd);
    
            sixfive("test.txt");
    
            exit(0);
        }
    }

    for(int i=1 ; i<argc ; i++) sixfive(argv[i]);
    
    return 0;
}