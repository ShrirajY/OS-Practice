#include<iostream>
#include "File2.hpp"
#include <cstring>
#include<iostream>
int main()
{
    Files f1(strdup("FirstFile"));
    f1.Write(strdup("New Content trying\n"));
    f1.Write(strdup("Second Content Trying\n"));

    Files f2(strdup("Second File"));
    f2.Write(strdup(" Second File"));

    f1.copy(&f2);
    return 0;
}