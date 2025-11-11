//#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <sys/stat.h>

int main(void)
{
    struct stat sbuf;
    if (stat("./big.file", &sbuf))
    {
        // stat: Value too large for defined data type
        perror("stat");
        return 1;
    }
}
