#include<iostream>
#include "File2.hpp"
#include <cstring>
#include<iostream>
int main()
{
    Files f1(strdup("FirstFile"));
    f1.Write(strdup("New Content trying\n"));
    f1.Write(strdup("Second Content Trying\n"));

    Files f2(strdup("SecondFile"));
    f2.Write(strdup("Second Conetnt Trying\n"));

    f1.link(&f2);
    f1.Write(strdup("This is the new Content Hopefully\n"));
    
    // std::cout<<"Content"<<std::endl;
    f1.show();
    f2.show();
    return 0;
}