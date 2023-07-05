/**
 * @file Filemanagement.c
 * @author Sachin Pandey
 * @brief  This is a simple file management program.It has 4 functions.
- Creating a file.
- Reading from a specified file, amount of data to read (in bytes), and from where to read
- Writing to a specified file, amount of data to write (in bytes), and from where to write
- Display statistical information of a specified file including owner, permissions, and inode.
- Create a copy of a given file, and the user will provide the source and destination files.

To peform the above operations we have 4 options:
-c	creates a file 
SYNTAX	:	./objectfile -c [filename] [mode] 
example	:	./a -c demo.txt 0666


-r	read from a file
SYNTAX	:	./objectfile -r [filename] [amount of data to read in bytes] [offset in the file] [offset w.r.t to file's start or end (put 0 for start,2 for end)]
example :	./a -r demo.txt 5 2 0


-w	write to a file taking input from console
SYNTAX	:	./objectfile -w [filename] [optionflag(Use 0 to write,1 to append,2 to truncate)] [amount of data to write in bytes] [offset in file] [offset w.r.t to file's start or end (put 0 for start,2 for end)]
example	:	./a -w demo.txt 0 10 0 0


-s	Display statistics of the file
SYNTAX	:	./objectfile -s [filename]
example	:	./a -s demo.txt


-v	copy content of source file to the destniation file
SYNTAX	:	./objectfile -v [source file] [destination file]
example	:	./a -v demo.txt copyofdemo.txt


--help	this will print information about how to give arguments in command line. 
 * 
 * 
 * 
 * 
 * 
 * 
 * @version 0.1
 * @date 2023-03-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define S_IROTH 00004
#define S_IWOTH 00002
#define S_IXOTH 00001

#define S_IRGRP 00040
#define S_IWGRP 00020
#define S_IXGRP 00010

#define S_IRWXO  S_IROTH | S_IWOTH | S_IXOTH
#define S_IRWXG  S_IRGRP | S_IWGRP | S_IXGRP

#define S_ISUID 0004000    /* set user id on execution */
#define S_ISGID 0002000    /* set group id on execution */
#define S_ISVTX 0001000    /* save swapped text even after use */


int myCreate(int argc, char *argv[]);
int myRead(int argc, char *argv[]);
int myWrite(int argc, char *argv[]);
int myStats(int argc, char *argv[]);
int myCopy(int argc, char *argv[]);
void myhelp();
mode_t myMode(char *argv);

int main(int argc, char *argv[])
{
    if (argc==1)
    {
        printf("\nType ./objectfile --help for more information");
        exit(1);
    }

    /*if the option is to create a file*/
    if (strcmp(argv[1], "-c") == 0)
    {
        myCreate(argc, argv);
    }

    /*if the option is to read from a file*/
    else if (strcmp(argv[1], "-r") == 0)
    {
        myRead(argc, argv);
    }

    /*if the option is to write to a file*/
    else if (strcmp(argv[1], "-w") == 0)
    {
        myWrite(argc, argv);
    }

    /*if the option is to display stats of a file*/
    else if (strcmp(argv[1], "-s") == 0)
    {
        myStats(argc, argv);
    }

    /*if the option is to copy the source file into destination file*/
    else if (strcmp(argv[1], "-v") == 0)
    {
        myCopy(argc, argv);
    }
    else if (strcmp(argv[1], "--help") == 0)
    {
        myhelp();
    }
    else{
        printf("No such option exist.");
        printf("\nType ./objectfile --help for more information");
        exit(1);
    }

    return 0;
}

/*Function : myCreate
 *-----------------------------------------
 *This function will create a file with the name and permissions given by user in command line.
 *
 * argc : count of argument passsed in the argument vector
 * argv : argument vector
 *
 * returns : None,
 *          error in case if number of expected argument are not meet or,
 *          error in case creat() is unable to create a file
 *
 * Written by:Sachin Pandey
 *
 */
int myCreate(int argc, char *argv[])
{
    int fd;
    if (argc != 4)
    {
        printf("Expected number of argument = 2, with the -c option");
        exit(1);
    }
    mode_t mode = myMode(argv[3]);

    char *filename = argv[2];
    fd = creat(filename, mode);
    if (fd == -1)
    {
        printf("cannot create file with file name %s\n", filename);
        exit(1);
    }
    printf("File created Successfully!!");
    close(fd);
}

/*Function : myRead
 *-----------------------------------------
 *This function will read from a file given by user in command line.
 *
 * argc : count of argument passsed in the argument vector
 * argv : argument vector
 *
 * returns : None,
 *          error in case if number of expected argument are not meet or,
 *          error in case read() is unable to read from a file
 *
 * Written by : Sachin Pandey
 *
 */
int myRead(int argc, char *argv[])
{
    int fd;
    char *buffer;
    if (argc != 6)
    {
        printf("Expected number of argument 4 but found %d, with the -r option", argc - 2);
        exit(1);
    }
    char *readfrom = argv[2];
    fd = open(readfrom, O_RDONLY, 0666);
    if (fd == -1)
    {
        printf("cannot open the file : %s\n", readfrom);
        exit(1);
    }
    int offset = atoi(argv[4]);
    int whence = 0;
    whence = (atoi(argv[5]) == 0) ? 0 : 2;
    // moving pointer in the file
    lseek(fd, offset, whence);
    int amount = atoi(argv[3]);
    buffer = (char *)malloc(amount * sizeof(char));
    read(fd, buffer, amount);
    for (int i = 0; i < amount; i++)
    {
        printf("%c", buffer[i]);
    }
    free(buffer);
    close(fd);
}

/*Function : myWrite
 *-----------------------------------------
 *This function will write to a file given by user in command line.
 *
 * argc : count of argument passsed in the argument vector
 * argv : argument vector
 *
 * returns : None,
 *          error in case if number of expected argument are not meet or,
 *          error in case open() is unable to open a file,
 *
 * Written by : Sachin Pandey
 *
 */
int myWrite(int argc, char *argv[])
{
    int fd;
    char *buffer;
    if (argc < 5)
    {
        printf("atleast 3 argument are required, with the -w option");
        exit(1);
    }
    char *readfrom = argv[2];
    int oflag = atoi(argv[3]);
    switch (oflag)
    {
    case 0:
        fd = open(readfrom, O_WRONLY | O_CREAT, 0666);
        if (fd == -1)
        {
            printf("cannot open the file : %s\n", readfrom);
            exit(1);
        }
        if (argc != 7)
        {
            printf("Expected 5 argument but found %d, for the given mode in -w option ", argc - 2);
            exit(1);
        }
        int offset = atoi(argv[5]);
        int whence = 0;
        whence = (atoi(argv[6]) == 0) ? 0 : 2;
        //moving pointer in the file
        lseek(fd, offset, whence);
        break;
    case 1:
        fd = open(readfrom, O_WRONLY | O_CREAT | O_APPEND, 0666);
        break;
    case 2:
        fd = open(readfrom, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        break;
    default:
        exit(1);
    }
    if (fd == -1)
    {
        printf("cannot open the file : %s\n", readfrom);
        exit(1);
    }
    int amount = atoi(argv[4]);
    buffer = (char *)malloc(amount * sizeof(char));
    printf("Enter the text you want to write : \n");
    scanf("%s", buffer);
    write(fd, buffer, amount);
    free(buffer);
    close(fd);
}

/*Function : myStats
 *-----------------------------------------
 *This function will print statistical information of the specified file including owner, permissions, and inode.
 *
 * argc : count of argument passsed in the argument vector
 * argv : argument vector
 *
 * returns : None,
 *          error in case if number of expected argument are not meet or,
 *          error in case read() is unable to read from a file
 *
 * Written by : Sachin Pandey
 *
 */
int myStats(int argc, char *argv[])
{
    struct stat stfile;
    struct tm dt;
    if (stat(argv[2], &stfile) == -1)
    {
        printf("Error occurred ");
        exit(1);
    }
    printf("Statistics of the given file\n");
    printf("------------------------------------\n");
    printf("mode : %d\n", stfile.st_mode);
    printf("inode number :%lld\n", stfile.st_ino);
    printf("Device ID : %d\n", stfile.st_dev);
    printf("File Device ID : %d\n", stfile.st_rdev);
    printf("Size of the file in bytes :%d\n", stfile.st_size);
    dt = *(gmtime(&stfile.st_atime));
    printf("Last accessed time : %d-%d-%d %d:%d:%d\n", dt.tm_mday, dt.tm_mon + 1, dt.tm_year + 1900, dt.tm_hour, dt.tm_min, dt.tm_sec);
    dt = *(gmtime(&stfile.st_ctime));
    printf("Last permission changed time : %d-%d-%d %d:%d:%d\n", dt.tm_mday, dt.tm_mon + 1, dt.tm_year + 1900, dt.tm_hour, dt.tm_min, dt.tm_sec);
    dt = *(gmtime(&stfile.st_mtime));
    printf("Last modified time : %d-%d-%d %d:%d:%d\n", dt.tm_mday, dt.tm_mon + 1, dt.tm_year + 1900, dt.tm_hour, dt.tm_min, dt.tm_sec);
    printf("Number of hard links : %d\n", stfile.st_nlink);
    printf("User ID : %d\n", stfile.st_uid);
    printf("Group ID : %d\n", stfile.st_gid);
    printf("Permissions on File : %o\n", stfile.st_mode&0777);
  
}

/*Function : myCopy
 *-----------------------------------------
 *This function will copy the source file into the destination file.
 *
 * argc : count of argument passsed in the argument vector
 * argv : argument vector
 *
 * returns : None,
 *           Error if expected number of arguments are not meet,
 *           Error if unable to open the source file,
 *           Error if unable to create the destination file.
 *
 * Written by : Sachin Pandey
 *
 */
int myCopy(int argc, char *argv[])
{

    if (argc != 4)
    {
        printf("Expected argument 2 but found %d with the -v option.\n", argc - 2);
        exit(1);
    }
    int fsource, fdest;
    char *sfile = argv[2];
    char *dfile = argv[3];

    // printf("Copying from '%s' to '%s'. \n",argv[2], argv[3]);
    if ((fsource = open(sfile, O_RDONLY)) == -1)
    {
        printf("Cannot open file %s\n", sfile);
        exit(1);
    }
    if ((fdest = creat(dfile, 0666)) == -1)
    {
        printf("Cannot create file %s\n", dfile);
        exit(1);
    }

    char c;
    while (read(fsource, &c, 1) == 1)
    {
        write(fdest, &c, 1);
    }
    close(fsource);
    close(fdest);
}


void myhelp(){
    int fd;
    if ((fd = open("Documentation.txt", O_RDONLY)) == -1)
    {
        printf("Cannot open documentation file : Documentation.txt\n");
        exit(1);
    }
    char c;
    while (read(fd, &c, 1) == 1)
    {
        printf("%c",c);
    }
    close(fd);
    
}




mode_t myMode(char *argv){
    int x = atoi(argv);
    mode_t mode = 0000000;
    int other = x%10;
    x /=10;
    int group = x%10;
    x /=10;
    int user = x%10;
    x /=10;
    int ids = x%10;

    mode = (other == 7)?mode|S_IRWXO:
           (other == 6)?mode|S_IROTH | S_IWOTH:
           (other == 5)?mode|S_IROTH|S_IXOTH:
           (other == 4)?mode|S_IROTH:
           (other == 3)?mode|S_IWOTH|S_IXOTH:
           (other == 2)?mode|S_IWOTH:
           (other == 1)?mode|S_IXOTH:mode;
    
    mode = (group == 7)?mode|S_IRWXG:
           (group == 6)?mode|S_IRGRP | S_IWGRP:
           (group == 5)?mode|S_IRGRP|S_IXGRP:
           (group == 4)?mode|S_IRGRP:
           (group == 3)?mode|S_IWGRP|S_IXGRP:
           (group == 2)?mode|S_IWGRP:
           (group == 1)?mode|S_IXGRP:mode;

    mode = (user == 7)?mode|S_IRWXU:
           (user == 6)?mode|S_IRUSR | S_IWUSR:
           (user == 5)?mode|S_IRUSR|S_IXUSR:
           (user == 4)?mode|S_IRUSR:
           (user == 3)?mode|S_IWUSR|S_IXUSR:
           (user == 2)?mode|S_IWUSR:
           (user == 1)?mode|S_IXUSR:mode;
    

    // mode = (other == 7)?mode|S_ISUID |S_ISGID |S_ISVTX:
    //        (other == 6)?mode|S_ISUID |S_ISGID:
    //        (other == 5)?mode|S_ISUID |S_ISVTX :
    //        (other == 4)?mode|S_ISUID:
    //        (other == 3)?mode|S_ISVTX |S_ISGID:
    //        (other == 2)?mode|S_ISGID:
    //        (other == 1)?mode|S_ISVTX:mode;    
    
    return mode;
}