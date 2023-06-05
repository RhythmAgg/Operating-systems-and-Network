#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef long long ll;

char * extract_file_name(char *path)
{
    int len = strlen(path);
    int flag=0;
    for(int i=len-1; i>0; i--)
    {
        if(path[i]=='/' )
        {
            flag=1;
            path = path+i+1;
            break;
        }
    }
    return path;
}

int main(int argc,char** argv){
    char* path = (char*)malloc(sizeof(char)*100);
    strcpy(path,extract_file_name(argv[1]));
    int fd=open(argv[1],O_RDONLY | O_CREAT);
    if(fd == -1){
        perror("Input file : ");
        exit(1);
    }
    int dir=mkdir("Assignment",0700);
    char des[1000] = "./Assignment/1_";
    strcat(des,path);
    int f_out = open(des,O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if(f_out == -1){
        perror("Output file : ");
        exit(1);
    }
    char c;
    ll file_size = lseek(fd,(off_t)0,SEEK_END);
    ll size = file_size;
    ll read_1=0;
    char arr[1000000];
    char str[1000]="";
    ll count=1;
    while(file_size >= 1000000){
        lseek(fd,-1*count*1000000,SEEK_END);
        write(1,"\r",1);
        read(fd,arr,1000000);
        char temp[1000000];
        for(int i=0;i<1000000;i++) temp[i] = arr[1000000-i-1];
        write(f_out,temp,1000000);
        sprintf(str,"percentage done: %0.2f",100*((double)(read_1)/size));
        read_1+=1000000;
        write(1,str,strlen(str));
        count++;
        file_size=file_size-1000000;
    }
    lseek(fd,(off_t)0,SEEK_SET);
    char buffer[file_size];
    read(fd,arr,file_size);
    char temp[file_size];
    for(int i=0;i<file_size;i++) temp[i] = arr[file_size-i-1];
    write(f_out,temp,file_size);
    write(1,"\rpercentage done: 100%",22);
    close(fd);
    close(f_out);
}