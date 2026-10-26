#pragma once
#include <stdint.h>
typedef struct {float v[3];} br_vector3;
typedef struct {float m[3][2];} br_matrix23;
typedef struct br_pixelmap {int origin_x,origin_y,base_x,base_y,width,height;} br_pixelmap;
typedef struct br_material {void* user;br_pixelmap* colour_map;br_matrix23 map_transform;void* extra_prim;} br_material;
typedef struct br_model {int id;} br_model;
typedef struct br_actor {struct br_actor *parent,*children,*next;br_model* model;br_material* material;struct {struct {struct {br_vector3 t;} translate;} t;} t;} br_actor;
void BrZbSceneRender(br_actor*,br_actor*,br_pixelmap*,br_pixelmap*);
void BrZbSceneRenderBegin(br_actor*,br_actor*,br_pixelmap*,br_pixelmap*);
void BrZbSceneRenderAdd(br_actor*);
void BrZbSceneRenderEnd(void);
