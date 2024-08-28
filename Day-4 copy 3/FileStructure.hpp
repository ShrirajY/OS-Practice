#include "File2.hpp"
#include<fcntl.h>
#include<cstdio>
class FileStructure{
    private:
        Files *Current;
    public:
        char *FOLDER_PATH = strdup("E://Current Work//OS Practice//AllFiles//");
        Files *Second;
        FileStructure()
        {
            Current=new Files(strdup("root"),'d',NULL);
            
            new Files(strdup("FirstFile"),'f',Current);
            new Files(strdup("SecondFile"),'f',NULL);
        }
};