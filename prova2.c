#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <locale.h>
#include <math.h>
#include <string.h>
#include <errno.h>

static void
usage(char p[]) {
    fprintf(stderr, "usage: %s [-dR] [-l mod] [files]\n", p);
    exit(20);
}

static 
int cmpstring(const void *p1, const void *p2) { 
    return strcmp(* (char * const *) p1, * (char * const *) p2);
}

int dflag = 0;
int lflag = 0;
int Rflag = 0;
int lvalue = 0;
int opterr = 0;

void
resolve_link(struct stat mystat, char *path) {
    if(lflag) {
        if(S_ISLNK(mystat.st_mode)) {
            char linkname[1024];
            ssize_t r;

            if ((r = readlink(path, linkname, sizeof(linkname)-1)) != -1);
            linkname[r] = '\0';
            printf(" -> %s", linkname);
        }
    }
}

void 
permission(struct stat mystat) {
    if(lflag) {
        if(S_ISREG(mystat.st_mode)) printf( "-" );
        else if (S_ISDIR(mystat.st_mode)) printf( "d" );
        else if (S_ISFIFO(mystat.st_mode)) printf( "|" );
        else if (S_ISSOCK(mystat.st_mode)) printf( "s" );
        else if (S_ISCHR(mystat.st_mode)) printf( "c" );
        else if (S_ISBLK(mystat.st_mode)) printf( "b" );
        else printf( "l" );
        printf( (mystat.st_mode & S_IRUSR) ? "r" : "-");
        printf( (mystat.st_mode & S_IWUSR) ? "w" : "-");
        switch (mystat.st_mode & (S_IXUSR | S_ISUID)) {
            case 0:
                printf("-");
                break;
            case S_IXUSR:
                printf("x");
                break;
            case S_ISUID:
                printf("S");
                break;
            case S_IXUSR | S_ISUID:
                printf("s");
                break;
        }
        printf( (mystat.st_mode & S_IRGRP) ? "r" : "-");
        printf( (mystat.st_mode & S_IWGRP) ? "w" : "-");
        switch (mystat.st_mode & (S_IXGRP | S_ISGID)) {
            case 0:
                printf("-");
                break;
            case S_IXGRP:
                printf("x");
                break;
            case S_ISGID:
                printf("S");
                break;
            case S_IXGRP | S_ISGID:
                printf("s");
                break;
        }
        printf( (mystat.st_mode & S_IROTH) ? "r" : "-");
        printf( (mystat.st_mode & S_IWOTH) ? "w" : "-");
        switch (mystat.st_mode & (S_IXOTH | S_ISVTX)) {
            case 0:
                printf("-");
                break;
            case S_IXOTH:
                printf("x");
                break;
            case S_ISVTX:
                printf("T");
                break;
            case S_IXOTH | S_ISVTX:
                printf("t");
                break;
        }

        if(lvalue == 0) {
            printf("\t%ld", mystat.st_nlink);
            printf("\t%zu", mystat.st_size);
        }
        printf("\t");
    }
}

void RecDir(char * path, int flag) {
    int numOfFiles = 0;
    struct dirent **mydirectory = NULL;
    struct stat mystat;
    int total=0;
    numOfFiles = scandir(path, &mydirectory, 0, alphasort);

    if (numOfFiles < 0) {
        //fprintf(stderr, "System call scandir failed because of %e\n", error);
        exit(100);
    }

    lstat(path, &mystat);

    char newdir[512];
    printf("%s:\n", path);

    if(lflag) {
        for(int i=0; i<numOfFiles; i++) {
            if (mydirectory[i]->d_name[0] != '.' && !(mydirectory[i]->d_type == 10)) {
                //lstat(mydirectory[i]->d_name, &myblock);
                total += mystat.st_blocks;
            }
        }
        printf("total %d\n", total/2);
    }

    for(int j=0; j < numOfFiles; j++) {
        char buf[PATH_MAX + 1]="";
        struct stat myblock;
        if (mydirectory[j]->d_name[0] != '.') {
                //sprintf(buf, "%s/%s", argv[argc-1], mydirectory->d_name);
            strcat(buf, path);
            strcat(buf, "/");
            strcat(buf, mydirectory[j]->d_name);
            //if (
            lstat(buf, &myblock);  // < 0)
                //printf("stat error\n");

            permission(myblock);

            printf("%s", mydirectory[j]->d_name);
            
            resolve_link(myblock, buf);

            printf("\n");

        }
    }

    for(int j=0; j < numOfFiles; j++) {
        if (mydirectory[j]->d_name[0] != '.') {
            if(flag && mydirectory[j]->d_type == 4) {
                sprintf(newdir, "%s/%s", path, mydirectory[j]->d_name);
                printf("\n");
                RecDir(newdir, 1);
            }
        }
    }

    for(int k=0; k < numOfFiles; k++){
        free(mydirectory[k]);
    }

    free(mydirectory);
}


void 
ls(int optind, int argc, char* argv[], int file) {
    for(int i = optind; i < argc; i++) {
        struct dirent **mydirectory = NULL;
        struct stat mystat;

        if (lstat(argv[i], &mystat) == 0 && S_ISDIR(mystat.st_mode)) {
            if (file > 0 && !dflag) {
                printf("\n");
            }

            if(dflag && file == 0){
                if(lflag){
                    permission(mystat);
                }
                printf("%s\n", argv[i]);
            }

            if (!Rflag){
                if ((file > 0 || Rflag) && !dflag) {
                    printf("%s:\n", argv[i]);
                }

                int total = 0;
                int blocksize = 0;
                //struct dirent **directory;
                int numOfFiles = 0;

                if(getenv("BLOCKSIZE") == NULL) {
                    blocksize = 1024;
                }
                else {
                    blocksize = atoi(getenv("BLOCKSIZE"));
                }

                if (!dflag){
                    numOfFiles = scandir(argv[i], &mydirectory, 0, alphasort);
                    //printf("%d", numOfFiles);
                    if (numOfFiles < 0){
                        //fprintf(stderr, "System call scandir failed because of %e\n", error);
                        exit(100);
                    }

                    if(lflag) {
                        for(int i=0; i<numOfFiles; i++) {
                            if (mydirectory[i]->d_name[0] != '.') {
                                //lstat(mydirectory[i]->d_name, &myblock);
                                total += mystat.st_blocks;
                            }
                        }
                        printf("total %d\n", total/2);
                    }

                }

            //rewinddir(mydir);

                for(int j=0; j < numOfFiles; j++) {
                    char buf[PATH_MAX + 1]="";
                    struct stat myblock;
                    if (mydirectory[j]->d_name[0] != '.') {
                            //sprintf(buf, "%s/%s", argv[argc-1], mydirectory->d_name);
                        strcat(buf, argv[i]);
                        strcat(buf, "/");
                        strcat(buf, mydirectory[j]->d_name);
                        //if (
                        lstat(buf, &myblock);  //< 0)
                            //printf("stat error\n");

                        permission(myblock);

                        printf("%s", mydirectory[j]->d_name);
                        
                        resolve_link(myblock, mydirectory[j]->d_name);

                        printf("\n");

                    }
                    //free(mydirectory[j]);
                }

                for(int k=0; k < numOfFiles; k++) {
                    free(mydirectory[k]);
                }
            }

            if (Rflag && S_ISDIR(mystat.st_mode) && !dflag) {
                RecDir(argv[i], 1);
            }
            

        }
        //free(mydirectory);
        //closedir(mydir);
        
        free(mydirectory);
    }
}


int
main(int argc, char* argv[])
{
    //DIR *mydir;
    int exit_status = 0;
    //struct dirent *mydirectory;
    //struct stat mystat;

    //char buf[512];
    int c;

    while ((c = getopt (argc, argv, "dRl:")) != -1) {
        switch (c) {
            case 'R':
                Rflag = 1;
                break;
            case 'd':
                dflag = 1;
                Rflag = 0;
                break;
            case 'l':
                lflag = 1;
                lvalue = atoi(optarg);
                //printf("lflag: %d \nmod: %d\n", lflag, lvalue);
                break;
            case '?':
                usage(argv[0]);
                return 20;
            default:
                usage(argv[0]);
                return 20;
        }
    }

    //optind per parsare le cartelle e files

    //mydir = opendir(argv[argc-1]);
    if((argc - optind) == 0) {
        argv[optind] = ".";
        ++argc;
    }

    setlocale(LC_ALL, "C");

    //printf("sorting...\n");
    for(int i = optind; i < argc; i++){
        //printf("%s\n", argv[i]);
        if( access(argv[i], F_OK) != -1 ) {
            // file exists
            continue;
        }
        else {
            // file doesn't exist
            fprintf(stderr, "%s: cannot access \'%s\': No such file or directory\n", argv[0], argv[i]);
            exit_status++;
        }
    }
    //printf("file not existent...\n");

    if((argc - optind) >= 2)
        qsort(&argv[optind], argc - optind, sizeof(char *) , cmpstring);

    int file = 0;

    for(int i = optind; i < argc; i++){
        //printf("%s\n", argv[i]);
        struct stat sb;
        if( (access(argv[i], F_OK) != -1 || dflag  ) && (lstat(argv[i], &sb) == 0 && (S_ISREG(sb.st_mode) || S_ISLNK(sb.st_mode) || dflag ))) {
            // file exists and is a file
            file++;
            
            permission(sb);

            printf("%s", argv[i]);
            
            resolve_link(sb, argv[i]);

            printf("\n");
        }
    }
    //printf("files in input...\n");

    ls(optind, argc, argv, file);
    //printf("other...\n");

    return exit_status;
}

