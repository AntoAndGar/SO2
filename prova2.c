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
ls(int optind, int argc, char* argv[], int file) {
    for(int i = optind; i < argc; i++) {
        struct dirent **mydirectory;
        struct stat mystat;

        if (lstat(argv[i], &mystat) == 0 && S_ISDIR(mystat.st_mode)) {
            if (file > 0) {
                printf("\n");
            }
            if (file > 0 || Rflag) {
                printf("%s:\n", argv[i]);
            }

            int total=0;
            int blocksize = 0;
            //struct dirent **directory;
            int numOfFiles;

            //setlocale(LC_ALL, "C");

            if(getenv("BLOCKSIZE") == NULL) {
                blocksize = 1024;
            }
            else {
                blocksize = atoi(getenv("BLOCKSIZE"));
            }

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

            //rewinddir(mydir);

            for(int j=0; j<numOfFiles; j++) {
                char buf[PATH_MAX + 1]="";
                struct stat myblock;
                if (mydirectory[j]->d_name[0] != '.') {
                        //sprintf(buf, "%s/%s", argv[argc-1], mydirectory->d_name);
                    strcat(buf, argv[i]);
                    strcat(buf, "/");
                    strcat(buf, mydirectory[j]->d_name);
                    if (lstat(buf, &myblock) < 0)
                        printf("stat error1\n");

                    if(lflag) {
                        if(S_ISREG(myblock.st_mode)) printf( "-" );
                        else if (S_ISDIR(myblock.st_mode)) printf( "d" );
                        else if (S_ISFIFO(myblock.st_mode)) printf( "|" );
                        else if (S_ISSOCK(myblock.st_mode)) printf( "s" );
                        else if (S_ISCHR(myblock.st_mode)) printf( "c" );
                        else if (S_ISBLK(myblock.st_mode)) printf( "b" );
                        else printf( "l" );
                        printf( (myblock.st_mode & S_IRUSR) ? "r" : "-");
                        printf( (myblock.st_mode & S_IWUSR) ? "w" : "-");
                        switch (myblock.st_mode & (S_IXUSR | S_ISUID)) {
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
                        printf( (myblock.st_mode & S_IRGRP) ? "r" : "-");
                        printf( (myblock.st_mode & S_IWGRP) ? "w" : "-");
                        switch (myblock.st_mode & (S_IXGRP | S_ISGID)) {
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
                        printf( (myblock.st_mode & S_IROTH) ? "r" : "-");
                        printf( (myblock.st_mode & S_IWOTH) ? "w" : "-");
                        switch (myblock.st_mode & (S_IXOTH | S_ISVTX)) {
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
                            printf("\t%ld", myblock.st_nlink);
                            printf("\t%zu", myblock.st_size);
                        }
                        printf("\t");
                        }
                        printf("%s", mydirectory[j]->d_name);
                        if(lflag) {
                            if(S_ISLNK(myblock.st_mode)) {
                                char linkname[1024];
                                ssize_t r;

                                if ((r = readlink(mydirectory[j]->d_name, linkname, sizeof(linkname)-1)) != -1);
                                linkname[r] = '\0';
                                printf(" -> %s", linkname);
                            }
                        }
                        printf("\n");

                    if (Rflag && S_ISDIR(myblock.st_mode) && !dflag) {
                        
                    }
                }
            }
            free(mydirectory);
        }
        //free(mydirectory);
        //closedir(mydir);
    }
}


int
main(int argc, char* argv[])
{
    //DIR *mydir;
    int exit_status = 0;
    //struct dirent *mydirectory;
    //struct stat mystat;
    char buf[512];
    int c;

    while ((c = getopt (argc, argv, "dRl:")) != -1) {
        switch (c) {
            case 'd':
                dflag = 1;
                break;
            case 'R':
                Rflag = 1;
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

    if((argc - optind) >= 2)
        qsort(&argv[optind], argc - optind, sizeof(char *) , cmpstring);

    printf("sorting...\n");
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
    printf("file not existent...\n");

    int file = 0;

    //TODO: FAR STAMPARE ANCHE I LINK
    for(int i = optind; i < argc; i++){
        //printf("%s\n", argv[i]);
        struct stat sb;
        if( (access(argv[i], F_OK) != -1) && (lstat(argv[i], &sb) == 0 && (S_ISREG(sb.st_mode) || S_ISLNK(sb.st_mode))) ) {
            // file exists and is a file
            file++;
            if(lflag) {
                if(S_ISREG(sb.st_mode)) printf( "-" );
                else if (S_ISDIR(sb.st_mode)) printf( "d" );
                else if (S_ISFIFO(sb.st_mode)) printf( "|" );
                else if (S_ISSOCK(sb.st_mode)) printf( "s" );
                else if (S_ISCHR(sb.st_mode)) printf( "c" );
                else if (S_ISBLK(sb.st_mode)) printf( "b" );
                else printf( "l" );
                printf( (sb.st_mode & S_IRUSR) ? "r" : "-");
                printf( (sb.st_mode & S_IWUSR) ? "w" : "-");
                //if (sb.st_mode & S_IXUSR){
                //    if (S_ISUID)
                //        printf("s");
                //    else
                //        printf( "x" );
                //}
                //else {
                //    if (S_ISUID)
                //        printf("S");
                //    else
                //        printf( "-" );
                //}
                switch (sb.st_mode & (S_IXUSR | S_ISUID)) {
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
                //printf( (sb.st_mode & S_IXUSR) ? "x" : "-");
                printf( (sb.st_mode & S_IRGRP) ? "r" : "-");
                printf( (sb.st_mode & S_IWGRP) ? "w" : "-");
                //if (sb.st_mode & S_ISGID & S_IXUSR)
                //    printf( "s" );
                //else if (sb.st_mode & S_ISGID & !S_IXUSR)
                //    printf( "S" );
                //else if (sb.st_mode & !S_ISGID & S_IXUSR)
                //    printf( "x" );
                //else 
                //    printf( "-" );
                switch (sb.st_mode & (S_IXGRP | S_ISGID)) {
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
                //printf( (sb.st_mode & S_IXGRP) ? "x" : "-");
                printf( (sb.st_mode & S_IROTH) ? "r" : "-");
                printf( (sb.st_mode & S_IWOTH) ? "w" : "-");
                //if (sb.st_mode & S_ISVTX & S_IXUSR)
                //    printf( "t" );
                //else if (sb.st_mode & S_ISVTX & !S_IXUSR)
                //    printf( "T" );
                //else if (sb.st_mode & !S_ISVTX & S_IXUSR)
                //    printf( "x" );
                //else 
                //    printf( "-" );
                switch (sb.st_mode & (S_IXOTH | S_ISVTX)) {
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
                //printf( (sb.st_mode & S_IXOTH) ? "x" : "-");

                if(lvalue == 0) {
                    printf("\t%ld", sb.st_nlink);
                    printf("\t%zu", sb.st_size);
                }
                printf("\t");
            }
            printf("%s", argv[i]);
            if(lflag) {
                if(S_ISLNK(sb.st_mode)) {
                    char linkname[1024];
                    ssize_t r;

                    if ((r = readlink(argv[i], linkname, sizeof(linkname)-1)) != -1);
                    linkname[r] = '\0';
                    printf(" -> %s", linkname);
                }
            }
            printf("\n");
        }
    }
    printf("files in input...\n");

    ls(optind, argc, argv, file);
    printf("other...\n");

    return exit_status;
}

