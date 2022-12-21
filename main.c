#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "makecache.h"
#define filename1 "read01.trace"
#define filename2 "read02.trace"
#define filename3 "read03.trace"
#define filename4 "write01.trace"
#define filename5 "write02.trace"
#define filename6 "swim.trace"
#define filename7 "gcc.trace"

int main(int argc, char* argv[])
{
    set = atoi(argv[1]);
    e_way = atoi(argv[2]);
    eachblock_bytes = atoi(argv[3]);
    write_hit = argv[4];
    write_miss = argv[5];
    evict = argv[6]; // LRU / FIFO / random 중 LRU만 구현
    filename = argv[7];

    printf("%d %d %d\n\n", set, e_way, eachblock_bytes);

    cache_simulation (set, e_way, eachblock_bytes, filename);

    return 0;
}