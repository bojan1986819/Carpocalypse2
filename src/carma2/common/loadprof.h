#ifndef C2_LOADPROF_H
#define C2_LOADPROF_H

/* Main-thread PF I/O only: counters exclude diagnostics, audio and raw platform
 * I/O. Bytes are returned stdio bytes, including pack read-ahead, not syscalls.
 * Nested scopes report inclusive deltas. No retail code/data is emitted. */
#ifdef CARPOCALYPSE2_MATCHING
#define C2LoadBegin(name) ((void)0)
#define C2LoadEnd() ((void)0)
#define C2LoadFopen fopen
#define C2LoadFread fread
#define C2LoadFgetc fgetc
#define C2LoadFgets fgets
#else
#include <stdio.h>
typedef struct {
    unsigned long long pfopen, packed, fopen, reads, bytes;
} C2LoadCounts;
extern C2LoadCounts gC2LoadCounts;
void C2LoadBegin(const char* name);
/* test (C2_TEST_READSITES): reads tallied by the game code calling the PF read functions */
extern int gC2LoadSites;
#ifdef _MSC_VER
#include <intrin.h>
#define C2_LOAD_CALLER() _ReturnAddress()
#else
#define C2_LOAD_CALLER() __builtin_return_address(0)
#endif
void C2LoadSite(void* pSite);
void C2LoadEnd(void);
/* test (C2_TEST_LOADDETAIL): time finer load phases too */
int C2LoadDetail(void);
unsigned long long C2LoadMicros(void);
void C2LoadNote(const char* pName, unsigned long long pMicros);
FILE* C2LoadFopen(const char* path, const char* mode);
size_t C2LoadFread(void* out, size_t size, size_t count, FILE* file);
int C2LoadFgetc(FILE* file);
char* C2LoadFgets(char* out, int size, FILE* file);
#endif
#endif
