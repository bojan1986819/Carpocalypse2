/* Production payload definitions/copy, with CPU buffers; no game or graphics. */
#include <assert.h>
#include <stdint.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glad/glad.h>
#define BR_MAX_LIGHTS 48
#define BR_MAX_CLIP_PLANES 6
typedef struct {float v[2];} br_vector2;
typedef struct {float v[4];} br_vector4;
typedef struct {float m[4][4];} br_matrix4;
#include "shader_data.h"
#include "snapshot_production.h"
int main(void) {
#ifdef __vita__
    assert(GL_SCENE_UNIFORM_BYTES == 128);
#else
    assert(GL_SCENE_UNIFORM_BYTES == sizeof(shader_data_scene));
#endif

 shader_data_scene scene, replay;
 gl_transparent_draw* draw=calloc(1,sizeof(*draw));
 void* expected=malloc(sizeof(draw->scene));
 memset(&scene,0x36,sizeof(scene));scene.num_clip_planes=3;scene.yon_z=1234;scene.num_lights=4;
 struct {struct {struct {shader_data_scene scene;} cache;} state;} renderer_value2;
 renderer_value2.state.cache.scene=scene;
 __typeof__(renderer_value2)* renderer=&renderer_value2;
 #include "snapshot_copy.h"
 memcpy(expected,&scene,sizeof(draw->scene));memset(&renderer_value2.state.cache.scene,0,sizeof(scene));
 memset(&replay,0xa5,sizeof(replay));memcpy(&replay,&draw->scene,sizeof(draw->scene));
 assert(!memcmp(&replay,expected,sizeof(draw->scene)));
 assert(replay.num_clip_planes==3 && replay.yon_z==1234 && replay.num_lights==4);
#ifdef __vita__
 assert(sizeof(draw->scene)==128);assert(((unsigned char*)replay.lights)[0]==0xa5);
 assert(sizeof(*draw)<1200);
#else
 assert(sizeof(draw->scene)==sizeof(shader_data_scene));
#endif
 free(expected);free(draw);
 puts("PASS production alpha scene snapshot layout/copy: clip/fog/light-count prefix retained after source mutation, correct native/desktop footprint; ASan/UBSan; no GPU");
}
