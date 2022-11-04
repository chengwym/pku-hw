/*
WRITTEN BY Jingbo Cheng, 2100013016
*/

#include <stdio.h>
#include <unistd.h>
#include "cachelab.h"
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

int s, E, b, t;
unsigned long long S, B;
int needVerbose;
int needHelp;
FILE *fp;

char operator[256];

int hit_count = 0;
int miss_count = 0;
int eviction_count = 0;

int sumTime = 0;

typedef struct {
    int valid;
    int tag;
    int time;
}block;

block ***cache;

int getTag(unsigned long long memory) {
    return memory >> (b + s);
}

int getSindex(unsigned long long memory) {
    return memory << t >> (t + b);
}

void printH() {
    printf("123");
}

void getParameters(int argc, char *argv[]) {
    int ch;
    while ((ch = getopt(argc, argv, "vhs:E:b:t:")) != -1) {
        switch (ch)
        {
        case 'v':
            needVerbose = 1;
            break;
        case 'h':
            printH();
            break;
        case 's':
            s = atoi(optarg);
            S = 1 << s;
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            B = 1 << b;
            break;
        case 't':
            fp = fopen(optarg, "r");
            break;
        default:
            break;
        }
    }
    t = 64 - b - s;
}

void initialize() {
    cache = malloc(sizeof(void*) * S);
    for (int i = 0; i < S; i++) {
        cache[i] = malloc(sizeof(void*) * E);
        for (int j = 0; j < E; j++) {
            block * block_ = malloc(sizeof(block));
            block_ -> tag = 0;
            block_ -> time = 0;
            block_ -> valid = 0;
            cache[i][j] = block_;
        }
    }
}

void simulate(unsigned long long memory) {
    int s_index = getSindex(memory);
    int tag = getTag(memory);
    block *emptyBlock = NULL;
    block *LRUBlock = NULL;
    block *hitBlock = NULL;
    for (int i = 0; i < E; i++) {
        if (hitBlock == NULL && cache[s_index][i]->tag == tag && cache[s_index][i]->valid){
            hitBlock = cache[s_index][i];
        }
        if (emptyBlock == NULL && !cache[s_index][i]->valid) {
            emptyBlock = cache[s_index][i];
        }
        if ((LRUBlock == NULL || LRUBlock->time > cache[s_index][i]->time) && cache[s_index][i]->valid){
            LRUBlock = cache[s_index][i];
        }
    }
    if (hitBlock != NULL) {
        hit_count++;
        if (needVerbose) {
            printf(" hit");
        }
        hitBlock->time = sumTime;
    } else {
        miss_count++;
        if (needVerbose) {
            printf(" miss");
        }
        if (emptyBlock) {
            emptyBlock->time = sumTime;
            emptyBlock->tag = tag;
            emptyBlock->valid = 1;
        } else {
            eviction_count++;
            LRUBlock->time = sumTime;
            LRUBlock->tag = tag;
            LRUBlock->valid = 1;
            if (needVerbose) {
                printf(" eviction");
            }                
        }
    }
}

void operatoration() {
    sumTime++;
    char opType;
    unsigned long long address;
    int size;
    int count = sscanf(operator, " %c %llx,%d", &opType, &address, &size);
    if (count == 0) {
        return;
    }
    if (needVerbose) {
        printf("%s", operator);
    }
    switch (opType)
    {
    case 'L':
        simulate(address);
        break;
    case 'S':
        simulate(address);
        break;
    case 'M':
        simulate(address);
        simulate(address);
        break;
    default:
        break;
    }
    if (needVerbose) {
        printf("\n");
    }
}

void readFile() {
    while(fgets(operator, 256, fp)) {
        if (operator[strlen(operator)-1] == '\n') {
            operator[strlen(operator)-1] = '\0';
        }
        operatoration();
    }
}

int main(int argc, char *argv[]) {
    
    getParameters(argc, argv);
    initialize();
    readFile();
    fclose(fp);

    printSummary(hit_count, miss_count, eviction_count);

    return 0;
}