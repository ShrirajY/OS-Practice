#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct _File{
    char *name;
    struct _File **childs;
    int totalChilds;
    struct _File *parent;
}File;

File *init(char *name)
{
    File *temp=(File *)malloc(sizeof(File));
    temp->totalChilds=0;
    temp->childs=(File **)malloc(0);
    temp->name=(char *)malloc(sizeof(char)*30);
    temp->parent=(File *)malloc(sizeof(File));
    temp->parent=NULL;
    strcpy(temp->name,name);
    return temp;
}

int AddFile(File *parent,File *child)
{
    parent->totalChilds+=1;

    if(parent==NULL || child==NULL)
    {
        return -1;
    }

    if
    ((parent->childs=(File **)realloc(parent->childs,sizeof(File *)*parent->totalChilds))==NULL)
    {
        return -1;
    }

    parent->childs[parent->totalChilds-1]=child;
    child->parent=parent;
    return 1;
}

int SearchFile(File *parent, char *childName, int *ret)
{
    if(parent==NULL || childName==NULL)
    {
        return -1;
    }

    else
    {
        for(int i=0;i<parent->totalChilds;i++)
        {
            if(strcmp(parent->childs[i]->name,childName)==0)
            {
                *ret=i;
                return 1;
            }
        }
    }

    printf("File not found named %s\n",childName);
    return -1;
}


int main()
{
    File *parent=init(strdup("Parent1"));

    File *child1=init(strdup("Child1"));
    File *child2=init(strdup("Child2"));
    File *child3=init(strdup("Child3"));
    File *child4=init(strdup("Child4"));
    File *child5=init(strdup("Child5"));

    int data=-1;
    AddFile(parent,child1);
    AddFile(parent,child2);
    AddFile(parent,child3);
    AddFile(parent,child4);
    AddFile(child3,child5);

    SearchFile(child3,strdup("Child5"),&data);

    printf("%d",data);
}