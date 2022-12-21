#ifndef _MAKE_CACHE_
#define _MAKE_CACHE_
#define INF 0x7fffffff

typedef struct cache{
    int valid;
    unsigned int tag;
    int time;
} cache;

int set;
int e_way;
int eachblock_bytes;
char* write_hit;
char* write_miss;
char* evict;
char* filename;
int cur_time;

int total_loads, total_stores;
int load_hits, load_misses;
int store_hits, store_misses;
int total_cycles;

void cache_simulation (int set, int e_way, int eachblock_bytes, char* filename);
void read (unsigned int addr, int set, int e_way, int eachblock_bytes, cache** c);
void write (unsigned int addr, int set, int e_way, int eachblock_bytes, cache** c);
int eviction (unsigned int set, int e_way, cache** c);

#endif

/*
    헤더 파일에 int total_stores = 0; 형태로 0으로 초기화하면 아래와 같이 오류발생함
    makecache.o:makecache.c:(.bss+0x0): multiple definition of `total_stores'
    >>> 초기화가 안된 전역변수들은 .bss영역에서 0으로 초기화 해주는것 같음 (중복 선언 문제 발생)
*/