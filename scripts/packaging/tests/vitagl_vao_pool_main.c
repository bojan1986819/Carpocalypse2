/* Exact pinned native allocator setup; allocation is mocked with malloc. */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VERTEX_ATTRIBS_NUM 16
#define vgl_memset memset
#define SCE_GXM_ATTRIBUTE_FORMAT_F32 1
#define SCE_GXM_INDEX_SOURCE_INDEX_16BIT 2
typedef struct {
    uint32_t vertex_attrib_offsets[16],vertex_attrib_vbo[16],vertex_attrib_size[16];
    struct {int regIndex,componentCount,offset,format,streamIndex;} vertex_attrib_config[16];
    struct {int stride,indexSource;} vertex_stream_config[16];
    uint32_t vertex_attrib_state,vertex_attrib_divisor,index_array_unit;
    float *vertex_attrib_pool,*vertex_attrib_pool_ptr,*vertex_attrib_pool_limit;
    float* vertex_attrib_value[16];
} vao;
static vao default_vao,*cur_vao=&default_vao;
static uint32_t main_vertex_attrib_pool_size,aux_vertex_attrib_pool_size;
static size_t allocated;
static void* gpu_alloc_mapped_for_cpu(size_t n) {allocated+=n;void* p=malloc(n);assert(p);return p;}
#include "vao_pool_production.h"
static void check(vao* v,size_t capacity) {
    reset_vao(v);
    assert(cur_vao==&default_vao);
    assert((uint8_t*)v->vertex_attrib_pool_limit-(uint8_t*)v->vertex_attrib_pool==capacity);
    assert(v->vertex_attrib_pool_ptr==v->vertex_attrib_pool+16*4);
    for(unsigned i=0;i<16;++i) {
        assert(v->vertex_attrib_value[i]==v->vertex_attrib_pool+i*4);
        assert(v->vertex_attrib_value[i]+4<=v->vertex_attrib_pool_limit);
        assert(v->vertex_attrib_size[i]==4 && v->vertex_attrib_config[i].regIndex==(int)i);
    }
}
int main(void) {
    vglSetVertexAttribPoolSize(256*1024,1024);
    check(&default_vao,256*1024);
    vao models[256]={0};
    for(unsigned i=0;i<256;++i) check(&models[i],1024);
    assert(allocated==512*1024); /* Previously16.25MiB for this batch. */
    for(unsigned i=0;i<256;++i) free(models[i].vertex_attrib_pool);
    free(default_vao.vertex_attrib_pool);
    puts("PASS exact pinned native reset_vao/reserve/setter: default256KiB retained,256 temporary VBO VAOs use256KiB total,16 float4 defaults fit each1KiB pool; GPU allocator mocked; ASan/UBSan");
}
