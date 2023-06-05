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
ll convert(char* str){
    ll x=0;
    ll k = 1;
    for(int i=strlen(str)-1;i>=0;i--){
        x+=(str[i]-'0')*1LL*k;
        k*=10;
    }
    return x;
}
int main(int argc,char** argv)
{
    char* path = (char*)malloc(sizeof(char)*100);
    strcpy(path,extract_file_name(argv[1]));
    int fd=open(argv[1],O_RDONLY | O_CREAT);
    if(fd == -1){
        perror("Input file : ");
        exit(1);
    }
    int dir=mkdir("Assignment",0700);
    char des[1000] = "./Assignment/2_";
    strcat(des,path);
    int f_out = open(des,O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if(f_out == -1){
        perror("Output file : ");
        exit(1);
    }
    ll index_1 = convert(argv[2]);
    ll index_2 = convert(argv[3]);

    ll size= lseek(fd,(off_t)0,SEEK_END);
    ll read_1=0;
    char arr[1000000];
    ll file_size = lseek(fd,index_1,SEEK_SET);
    char str[1000]="";
    lseek(fd,-1000000,SEEK_CUR);
    while(file_size > 1000000){
        write(1,"\r",1);
        read(fd,arr,1000000);
        char temp[1000000];
        for(int i=0;i<1000000;i++) temp[i] = arr[1000000-i-1];
        write(f_out,temp,1000000);
        lseek(fd,-2000000,SEEK_CUR);
        sprintf(str,"percentage done: %lf%%",100*((double)(read_1)) / size);
        read_1+=1000000;
        write(1,str,strlen(str));
        file_size=file_size-1000000;
    }
    lseek(fd,(off_t)0,SEEK_SET);
    char buffer[file_size];
    read(fd,arr,file_size);
    char temp[file_size];
    for(int i=0;i<file_size;i++) temp[i] = arr[file_size-i-1];
    write(f_out,temp,file_size);
    read_1+=file_size;
    sprintf(str,"\rpercentage done: %lf%%",100*((double)(read_1)) / size);
    write(1,str,strlen(str));
    // printf("percentage done: %lf%%", (double)(size-file_size)*100/size);

    file_size = lseek(fd,index_2,SEEK_SET) - index_1+1;
    lseek(fd,index_1,SEEK_SET);
    while(file_size > 1000000){
        write(1,"\r",1);
        read(fd,arr,1000000);
        write(f_out,arr,1000000);
        sprintf(str,"percentage done: %lf%%",100*((double)(read_1)) / size);
        read_1+=1000000;
        write(1,str,strlen(str));
        file_size=file_size-1000000;
    }
    char buffer_2[file_size];
    read(fd,buffer_2,file_size);
    write(f_out,buffer_2,file_size);
    read_1+=file_size;
    sprintf(str,"\rpercentage done: %lf%%",100*((double)(read_1)) / size);
    write(1,str,strlen(str));

    file_size = lseek(fd,0,SEEK_END) - index_2;
    lseek(fd,-1000000,SEEK_CUR);
    while(file_size > 1000000){
        read(fd,arr,1000000);
        char temp_3[1000000];
        for(int i=0;i<1000000;i++) temp_3[i] = arr[1000000-i-1];
        write(f_out,temp_3,1000000);
        sprintf(str,"\rpercentage done: %lf%%",100*((double)(read_1)) / size);
        read_1+=1000000;
        write(1,str,strlen(str));
        lseek(fd,-2000000,SEEK_CUR);
        file_size=file_size-1000000;
    }
    lseek(fd,(off_t)index_2+1,SEEK_SET);
    file_size--;
    char buffer_3[file_size];
    read(fd,buffer_3,file_size);
    char temp_3[file_size];
    for(int i=0;i<file_size;i++) temp_3[i] = buffer_3[file_size-i-1];
    write(f_out,temp_3,file_size);
    write(1,"\rpercentage done: 100%",23);
}