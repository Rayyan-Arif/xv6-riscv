#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

void find(char *target, char *path, int isExecCalled, char *execArgs[], int argc){
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0)
            continue;

         // Skip current and parent directory
        if (strcmp(de.name, ".") == 0 ||
            strcmp(de.name, "..") == 0)
            continue;

        char newpath[512];
        strcpy(newpath, path);

        int len = strlen(de.name);
        if (strlen(newpath) + 1 + DIRSIZ + 1 >= sizeof(newpath))
            continue;

        char *p = newpath + strlen(newpath);
        *p++ = '/';

        memmove(p, de.name, len);
        p[len] = '\0';

        int entryfd = open(newpath, O_RDONLY);

        if (entryfd < 0) {
            fprintf(2, "find: cannot open %s\n", newpath);
            continue;
        }

        if (fstat(entryfd, &st) < 0) {
            fprintf(2, "find: cannot stat %s\n", newpath);
            close(entryfd);
            continue;
        }

        if(!strcmp(de.name, target)){
            if(isExecCalled){
                int pid = fork();

                if(pid < 0){
                    fprintf(2, "fork failed\n");
                } else if(pid == 0){
                    char *args[MAXARG];
                    args[0] = execArgs[4];
                    int argLen = 1;
                    for(int i=1; i<argc-4 ; i++){
                        args[i] = execArgs[i+4];
                        argLen++;
                    }
                    // printf("%d\n", argLen);
                    args[argLen++] = newpath;
                    args[argLen] = 0;
                    exec(args[0], args);
                    fprintf(2, "exec failed\n");
                    exit(1);
                } else {
                    wait(0);
                }
            } else {
                printf("%s\n", newpath);
            }
        }
            
        if(st.type == T_DIR){
            find(target, newpath, isExecCalled, execArgs, argc);
        }

        close(entryfd);
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    if(argc < 3){
        fprintf(2, "Usage: find <starting-point> <filename>\n");
        return 1;
    }

    char startingPath[512];
    strcpy(startingPath, argv[1]);

    if(argc > 3 && !strcmp(argv[3], "-exec")){
        if(argc < 5){
            fprintf(2, "Incomplete options for -exec\n");
            return 1;
        }

        find(argv[2], startingPath, 1, argv, argc);
    } else {
        find(argv[2], startingPath, 0, argv, argc);
    }

    return 0;
}