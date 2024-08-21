#include<iostream>
#include "Inodes.hpp"

int main()
{
    Inode *temp;
    temp=IPool.AllocatInode();
    std::cout<<temp->inodeNo;
}