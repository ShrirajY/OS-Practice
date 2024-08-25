#include<iostream>
#include "File.hpp"
#include <cstring>
#include<iostream>
int main()
{
    // Files f(strdup("Shriraj Yamkanmardi"));
    // f.Write(strdup("Shriraj Yamkanmardi\n"));
    // f.show();
    // f.clear();
    // f.Write(strdup("Shriraj Yamkanmardi\n"));
    // f.Write(strdup("Adding Another Contents\n"));

    // std::cout<<"Showing Final Outcomes"<<std::endl;
    // f.show();

    // BufVec c;
    // c.write(strdup("This content is going to be vanished\n"));
    // std::cout<<"C Content\n";
    // c.show();
    // BufVec b;
    // b.write(strdup("Shriraj Shridhar Yamkanmardi\n"));
    // c.copy(&b);
    // std::cout<<"C Content\n";
    // c.show();
    // c.ShowBufHeaders();
    // std::cout<<"B Content\n";
    // b.show();
    // b.ShowBufHeaders();
    // // b.show();
    // std::cout<<"After Linking Files\n";
    // c.link(&b);
    // std::cout<<"C Content\n";
    // c.show();
    // c.ShowBufHeaders();
    // std::cout<<"B Content\n";
    // b.show();
    // b.ShowBufHeaders();

    // Files f1(strdup("First File\n"));
    // Files f2(strdup("Second File\n"));

    // f1.Write(strdup("First Line of First File\n"));
    // f2.Write(strdup("First Line of Second File\n"));
    // f1.Info();
    // f2.Info();
    // f1.copy(&f2);
    // std::cout<<"After copying\n";
    // f1.Info();
    // f2.Info();
    // f1.Write(strdup("Now We Will try Linking\n"));
    // f1.Info();
    // f1.link(&f2);
    // std::cout<<"\nAfter Linking\n";
    // f1.Info();
    // f2.Info();

    // f1.Write(strdup("This is going to get appended ABCDEFGHIJKLOMNOP\n"));
    // f2.show();
    // f1.Info();
    // f2.Info();


    // BFList.show();
    // Files f1(strdup("First File\n"));
    // f1.Write(strdup("First Text adding More text\n"));
    // BFList.show();
    // f1.Write(strdup("More More More\n"));
    // // f1.show();
    // BFList.show();

    Files f1(strdup("FirstFile"));
    Files f2(strdup("SecondFile"));
    f1.Write(strdup("This is First File\n"));
    f2.Write(strdup("This is Second File\n"));
    f1.Info();
    f2.Info();
    f1.copy(&f2);
    f1.Info();
    f2.Info();
    f1.Write(strdup("This is First File\n"));
    f2.Write(strdup("This is Second File\n"));
    f1.link(&f2);
    f1.Info();
    f2.Info();
    return 0;
}