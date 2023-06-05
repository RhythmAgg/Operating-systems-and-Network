#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
typedef long long ll;

void permissions_old(int n)
{
    int arr[9];
    for (int i = 0; i < 9; i++)
    {
        if (n % 2 == 0)
            arr[9 - i - 1] = 0;
        else
            arr[9 - i - 1] = 1;
        n/=2;
    }
    if (arr[0] == 1)
        write(1, "User has read permissions on oldfile: Yes\n", 43);
    else
        write(1, "User has read permissions on oldfile: No\n", 42);
    // fflush(stdout);
    if (arr[1] == 1)
        write(1, "User has write permissions on oldfile: Yes\n", 44);
    else
        write(1, "User has write permissions on oldfile: No\n", 43);
    if (arr[2] == 1)
        write(1, "User has execute permissions on oldfile: Yes\n", 46);
    else
        write(1, "User has execute permissions on oldfile: No\n", 45);
    if (arr[3] == 1)
        write(1, "Group has read permissions on oldfile: Yes\n", 44);
    else
        write(1, "Group has read permissions on oldfile: No\n", 43);
    if (arr[4] == 1)
        write(1, "Group has write permissions on oldfile: Yes\n", 45);
    else
        write(1, "Group has write permissions on oldfile: No\n", 44);
    if (arr[5] == 1)
        write(1, "Group has execute permissions on oldfile: Yes\n", 47);
    else
        write(1, "Group has execute permissions on oldfile: No\n", 46);
    if (arr[6] == 1)
        write(1, "Others has read permissions on oldfile: Yes\n", 45);
    else
        write(1, "Others has read permissions on oldfile: No\n", 44);
    if (arr[7] == 1)
        write(1, "Others has write permissions on oldfile: Yes\n", 46);
    else
        write(1, "Others has write permissions on oldfile: No\n", 45);
    if (arr[8] == 1)
        write(1, "Others has execute permissions on oldfile: Yes\n", 48);
    else
        write(1, "Others has execute permissions on oldfile: No\n", 47);
}
void permissions_new(int n)
{
    int arr[9];
    for (int i = 0; i < 9; i++)
    {
        if (n % 2 == 0)
            arr[9 - i - 1] = 0;
        else
            arr[9 - i - 1] = 1;
        n/=2;
    }
    if (arr[0] == 1)
        write(1, "User has read permissions on newfile: Yes\n", 43);
    else
        write(1, "User has read permissions on newfile: No\n", 42);
    // fflush(stdout);
    if (arr[1] == 1)
        write(1, "User has write permissions on newfile: Yes\n", 44);
    else
        write(1, "User has write permissions on newfile: No\n", 43);
    if (arr[2] == 1)
        write(1, "User has execute permissions on newfile: Yes\n", 46);
    else
        write(1, "User has execute permissions on newfile: No\n", 45);
    if (arr[3] == 1)
        write(1, "Group has read permissions on newfile: Yes\n", 44);
    else
        write(1, "Group has read permissions on newfile: No\n", 43);
    if (arr[4] == 1)
        write(1, "Group has write permissions on newfile: Yes\n", 45);
    else
        write(1, "Group has write permissions on newfile: No\n", 44);
    if (arr[5] == 1)
        write(1, "Group has execute permissions on newfile: Yes\n", 47);
    else
        write(1, "Group has execute permissions on newfile: No\n", 46);
    if (arr[6] == 1)
        write(1, "Others has read permissions on newfile: Yes\n", 45);
    else
        write(1, "Others has read permissions on newfile: No\n", 44);
    if (arr[7] == 1)
        write(1, "Others has write permissions on newfile: Yes\n", 46);
    else
        write(1, "Others has write permissions on newfile: No\n", 45);
    if (arr[8] == 1)
        write(1, "Others has execute permissions on newfile: Yes\n", 48);
    else
        write(1, "Others has execute permissions on newfile: No\n", 47);
}
void permissions_dire(int n)
{
    int arr[9];
    for (int i = 0; i < 9; i++)
    {
        if (n % 2 == 0)
            arr[9 - i - 1] = 0;
        else
            arr[9 - i - 1] = 1;
        n/=2;
    }
    if (arr[0] == 1)
        write(1, "User has read permissions on directory: Yes\n", 45);
    else
        write(1, "User has read permissions on directory: No\n", 44);
    // fflush(stdout);
    if (arr[1] == 1)
        write(1, "User has write permissions on directory: Yes\n", 46);
    else
        write(1, "User has write permissions on directory: No\n", 45);
    if (arr[2] == 1)
        write(1, "User has execute permissions on directory: Yes\n", 48);
    else
        write(1, "User has execute permissions on directory: No\n", 47);
    if (arr[3] == 1)
        write(1, "Group has read permissions on directory: Yes\n", 46);
    else
        write(1, "Group has read permissions on directory: No\n", 45);
    if (arr[4] == 1)
        write(1, "Group has write permissions on directory: Yes\n", 47);
    else
        write(1, "Group has write permissions on directory: No\n", 46);
    if (arr[5] == 1)
        write(1, "Group has execute permissions on directory: Yes\n", 49);
    else
        write(1, "Group has execute permissions on directory: No\n", 48);
    if (arr[6] == 1)
        write(1, "Others has read permissions on directory: Yes\n", 47);
    else
        write(1, "Others has read permissions on directory: No\n", 46);
    if (arr[7] == 1)
        write(1, "Others has write permissions on directory: Yes\n", 48);
    else
        write(1, "Others has write permissions on directory: No\n", 47);
    if (arr[8] == 1)
        write(1, "Others has execute permissions on directory: Yes\n", 50);
    else
        write(1, "Others has execute permissions on directory: No\n", 49);
}
int main(int argc, char **argv)
{
    DIR *directory = opendir(argv[3]);
    if (directory)
    {
        write(1, "Directory is created: Yes\n", 27);
    }
    else
        write(1, "Directory is created: No\n", 26);
    struct stat oldfile, newfile,dir;
    int fd_old=open(argv[1],O_RDONLY);
    if(fd_old == -1){
        perror("Old file : ");
        exit(1);
    }
    int fd_new=open(argv[2],O_RDONLY);
    if(fd_new == -1){
        perror("New file : ");
        exit(1);
    }
    // int fd_temp=open("temp.txt",O_RDWR);
    ll file_old = lseek(fd_old,0,SEEK_END);
    ll file_new = lseek(fd_new,0,SEEK_END);
    if(file_old != file_new) write(1,"File contents are reversed in newfile: No\n",43);
    else{
        lseek(fd_new,0,SEEK_SET);
        ll size = file_old;
        int flag = 1;
        char arr[1000000];
        lseek(fd_old,-1000000,SEEK_END);
        while(file_old > 1000000){
            read(fd_old,arr,1000000);
            char temp[1000000];
            for(int i=0;i<1000000;i++) temp[i] = arr[1000000-i-1];
            read(fd_new,arr,1000000);
            for(int i=0;i<1000000;i++){
                if(temp[i] != arr[i]){
                    flag = 0;
                    break;
                }
            }
            if(flag == 0) break;
            lseek(fd_old,-2000000,SEEK_CUR);
            file_old=file_old-1000000;
        }
        if(flag == 0){
            write(1,"File contents are reversed in newfile: No\n",43);
        }
        else{
            char c;
            lseek(fd_old,(off_t)file_old-1,SEEK_SET);
            while(file_old--){
                read(fd_old,&c,1);
                char tem;
                read(fd_new,&tem,1);
                if(c != tem){
                    flag = 0;
                    break;
                }
                lseek(fd_old,file_old-1,SEEK_SET);
                // index++;
            }
            if(flag == 0){
                write(1,"File contents are reversed in newfile: No\n",43);
            }
            else write(1,"File contents are reversed in newfile: Yes\n",44);
        }
    }
    write(1,"\n",1);
    close(fd_old);
    close(fd_new);
    int old, new, dire;
    old = stat(argv[2], &oldfile);
    if(old == -1){
        perror("old file : ");
    }
    new = stat(argv[1], &newfile);
    if(new == -1){
        perror("new file : ");
    }
    dire = stat(argv[3],&dir);
    if(dire == -1){
        perror("directory");
    }
    permissions_old(oldfile.st_mode);
    write(1,"\n\n",2);
    permissions_new(newfile.st_mode);
    write(1,"\n\n",2);
    permissions_dire(dir.st_mode);
    // printf("Permission bits: %d\n", oldfile.st_mode);
    // printf("Permission bits: %d\n", newfile.st_mode);
}