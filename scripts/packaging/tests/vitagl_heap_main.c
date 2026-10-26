/* Exact pinned custom allocator; OS mutex services mapped to pthreads. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#define VGL_MEM_ALL 5
#define VGL_MEM_VRAM 0
#define GL_FALSE 0
#define GL_TRUE 1
#define MEM_ALIGNMENT 16
#define VGL_ALIGN(x,a) (((x)+(a)-1)&~((a)-1))
#define vgl_memset memset
#define vgl_fast_memcpy memcpy
#define vgl_log(...) ((void)0)
typedef unsigned char GLboolean;
typedef unsigned char vglMemType; /* SDK default enum width, same native ABI */
typedef pthread_mutex_t SceKernelLwMutexWork;
#define sceKernelLockLwMutex(p,a,b) pthread_mutex_lock(p)
#define sceKernelUnlockLwMutex(p,a) pthread_mutex_unlock(p)
static void *mempool_addr[VGL_MEM_ALL], *mempool_end[VGL_MEM_ALL];
#include "native_heap.h"
static void checkheap(unsigned n) {
 unsigned total=0,used=0,avail=0;
 for (uintptr_t a=(uintptr_t)mempool_addr[0];a<(uintptr_t)mempool_end[0];) {
  tm_block_t *b=*(tm_block_t**)a;
  if(!b || b->base!=a || b->size<8 || *(tm_block_t**)(a+b->size-4)!=b) {
   fprintf(stderr,"bad heap op=%u addr=%x b=%p size=%u used=%d\n",n,(unsigned)a,b,b?b->size:0,b?b->used:0);abort();
  }
  assert(a+b->size<=(uintptr_t)mempool_end[0]);
  total+=b->size;
  if(b->used) used++; else avail+=b->size;
  a+=b->size;
 }
 assert(avail==tm_free[0]);
 unsigned cnt=0; tm_block_t *prev=NULL;
 for(tm_block_t *b=tm_alloclist[0];b;b=b->next) {assert(b->used && b->prev==prev);prev=b;assert(++cnt<=used);}
 assert(cnt==used);
}
int main(void) {
 _Static_assert(sizeof(void*)==4,"exact 32-bit native allocator");
 _Static_assert(sizeof(tm_block_t)==20,"native block layout");
 pthread_mutex_init(&header_mutex,NULL);pthread_mutex_init(&tm_mutexes[0],NULL);
 unsigned heapsize=32*1024*1024;
 void *area=aligned_alloc(4096,heapsize);
 mempool_addr[0]=area;mempool_end[0]=(char*)area+heapsize;
 heap_init();heap_extend(0,area,heapsize);
 for(unsigned tail=1;tail<8;tail++) {
  void *a=heap_alloc(0,16,16),*b=heap_alloc(0,32,16),*c=heap_alloc(0,16,16);
  assert(a&&b&&c);memset(a,0x71,16);heap_blk_free((uintptr_t)b,0);
  tm_block_t *ab=*(tm_block_t**)((uintptr_t)a-4);
  tm_block_t *next=*(tm_block_t**)(ab->base+ab->size);
  unsigned wanted=ab->size+next->size-8-tail;
  assert(heap_realloc(0,a,wanted)==a);
  checkheap(tail);for(unsigned j=0;j<16;j++)assert(((unsigned char*)a)[j]==0x71);
  heap_blk_free((uintptr_t)a,0);heap_blk_free((uintptr_t)c,0);
  assert(tm_free[0]==heapsize);
 }
 enum {N=16000};static void* slots[N];static unsigned sizes[N];
 srand(42);
 for(unsigned i=0;i<250000;i++) {
  unsigned k=rand()%N;
  if(slots[k]) {
   for(unsigned j=0;j<sizes[k];j++) assert(((unsigned char*)slots[k])[j]==(unsigned char)k);
   if(rand()%3==0) {
    unsigned newsize=sizes[k]+rand()%400;
    void *a=heap_realloc(0,slots[k],newsize);
    if(a) {slots[k]=a;sizes[k]=newsize;memset(a,(unsigned char)k,newsize);}
   } else {heap_blk_free((uintptr_t)slots[k],0);slots[k]=NULL;}
  } else {
   unsigned size=rand()%1600;
   slots[k]=heap_alloc(0,size,1<<(2+rand()%8));sizes[k]=size;
   if(slots[k]) memset(slots[k],(unsigned char)k,size);
  }
  if(i%100==0)checkheap(i);
 }
 for(unsigned k=0;k<N;k++) if(slots[k])heap_blk_free((uintptr_t)slots[k],0);
 checkheap(250000);assert(tm_free[0]==heapsize);
 puts("PASS exact 32-bit native heap:250000 odd-sized aligned allocation/reallocation/free operations, data preservation, headers/footers/lists/free-byte accounting; ASan/UBSan");
}
