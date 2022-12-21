#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "makecache.h"

void cache_simulation (int set, int e_way, int eachblock_bytes, char* filename)
{
    FILE* fp = fopen(filename, "r");

    if (fp == NULL)
    {
        perror("The file does not exist.\n");
        return;
    }

    cache** c = (cache**)malloc(sizeof(struct cache*) * set);
    for (int i = 0; i < set; i++)
        c[i] = (cache*)malloc(sizeof(struct cache) * e_way);

    for (int i = 0; i < set; i++)
    {
        for (int j = 0; j < e_way; j++)
        {
            c[i][j].valid = 0;  c[i][j].tag = 0;    c[i][j].time = 0;
        }
    }

    char mode;  // load or store
    unsigned int addr;   // 32-bit memory address given in hexadecimal, unsigned로 하는 이유는 int로 하면 주소값이 음수가 나와서 인덱싱할 때 오류 발생
    int temp;
    while (!feof(fp))
    {
        fscanf(fp, "%c %x %d\n", &mode, &addr, &temp);
        
        switch (mode)
        {
            case 'l':
                read(addr, set, e_way, eachblock_bytes, c);
                total_loads++;
                break;

            case 's':
                write(addr, set, e_way, eachblock_bytes, c);
                total_stores++;
                break;
        }
        cur_time++;
    }

    printf("Total loads: %d\n", total_loads);
    printf("Total stores: %d\n", total_stores);
    printf("Load hits: %d\n", load_hits);
    printf("Load misses: %d\n", load_misses);
    printf("Store hits: %d\n", store_hits);
    printf("Store misses: %d\n", store_misses);

    for (int i = 0; i < set; i++)
        free(c[i]);
    free(c);

    fclose(fp);
}

void read (unsigned int addr, int set, int e_way, int eachblock_bytes, cache** c)
{
    unsigned int sbits = (addr / eachblock_bytes) % set;
    unsigned int tbits = (addr / eachblock_bytes) / set;   // If you express this decimal value in binary, this is t bits.

    //printf("sbits %d  %u, tbits %d\n", sbits, sbits, tbits);

    for (int i = 0; i < e_way; i++)
    {
        if (c[sbits][i].valid == 1 && c[sbits][i].tag == tbits) // HIT : valid = 1 and tag = tbits
        {
            c[sbits][i].time = cur_time;
            load_hits++;
            return;
        }

        else if (c[sbits][i].valid == 0)    // cold miss
        {
            c[sbits][i].valid = 1;
            c[sbits][i].tag = tbits;
            c[sbits][i].time = cur_time;
            load_misses++;
            return;
        }
    }

    //if (c[sbits][i].valid == 1 && c[sbits][i].tag != tbits)
    load_misses++;  // conflict miss
    int old_time_line = eviction(sbits, e_way, c);
    
    c[sbits][old_time_line].tag = tbits;
    c[sbits][old_time_line].time = cur_time;
    return;
}

void write (unsigned int addr, int set, int e_way, int eachblock_bytes, cache** c)
{
    unsigned int sbits = (addr / eachblock_bytes) % set;
    unsigned int tbits = (addr / eachblock_bytes) / set;   // If you express this decimal value in binary, this is t bits.

    if (strcmp(write_hit, "no-write-allocate") == 0)   // write_miss 발생 시, no-write-allocate로 설정했으면 이 구문으로
    {
        for (int i = 0; i < e_way; i++)
        {
            if (c[sbits][i].valid == 1 && c[sbits][i].tag == tbits) // write_hit (write-through, write-back)
            {
                c[sbits][i].time = cur_time;
                store_hits++;
                return;
            }
        }
    }
    else
    {
        for (int i = 0; i < e_way; i++)
        {
            if (c[sbits][i].valid == 1 && c[sbits][i].tag == tbits) // write_hit (write-through, write-back)
            {
                c[sbits][i].time = cur_time;
                store_hits++;
                return;
            }

            else if (c[sbits][i].valid == 0)    // write_miss (write-allocate)
            {
                c[sbits][i].valid = 1;
                c[sbits][i].tag = tbits;
                c[sbits][i].time = cur_time;
                store_misses++;
                return;
            }
        }  

        store_misses++;  // write_miss (write-allocate)
        int old_time_line = eviction(sbits, e_way, c);
        
        c[sbits][old_time_line].tag = tbits;
        c[sbits][old_time_line].time = cur_time;
        return;
    }
}

int eviction (unsigned int set, int e_way, cache** c)   // Replacement policy : Least Recently Used (LRU)
{
    int min_time = INF;
    int min_idx;

    for (int i = 0; i < e_way; i++)
    {
        if (min_time > c[set][i].time)
        {
            min_time = c[set][i].time;
            min_idx = i;
        }
    }

    return min_idx;
}