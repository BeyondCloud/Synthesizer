#ifndef ShareMem_H
#define ShareMem_H
#include <windows.h>
using namespace std;
class ShareMem
{
    public:
        ShareMem(TCHAR*);
        ShareMem(TCHAR*,int);
        void     writeMem(void*);
        void*    readMem();
        int      getBufferSize();
        TCHAR*   getName();
        HANDLE hMapFile;
        void* bBuf;
    private:
        int buffer_size;
        TCHAR* name;


};

#endif
