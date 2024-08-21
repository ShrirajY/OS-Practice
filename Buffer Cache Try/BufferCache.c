#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
char MyBuffer[15];

int main()
{
    int fdold=open("abc.txt",O_RDONLY);
    char ch='Y' | 'y';
    int count;
    int buffer=0;
    while((ch=='y' || ch=='y') && (count=read(fdold,MyBuffer,sizeof(MyBuffer)))>0)
    {
        printf("In Buffer %d travel:\t",buffer++);
        printf("%s\n",MyBuffer);
        printf("Size of buffer used %d\n",strlen(MyBuffer));
        strcpy(MyBuffer,"\0");
    }
}