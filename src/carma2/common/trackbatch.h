#ifndef C2_TRACKBATCH_H
#define C2_TRACKBATCH_H

#include "c2_hooks.h"
#include "carpocalypse2_types.h"

/* br_model flag (unused by BRender): this model is drawn through a merged track block */
#define C2_MODF_BATCHED 0x4000

/* Port: merges the fixed track geometry into a few models per block of columns (see trackbatch.c). */
void C2TrackBatchBuild(tTrack_spec* pTrack_spec);
void C2TrackBatchDispose(void);

/* BrDbModelRender: the model to draw for this actor (a merged actor draws only its blended
 * faces, or nothing: NULL). */
br_model* C2TrackBatchRenderModel(const br_actor* pActor, br_model* pModel);

/* BrModelUpdate of a C2_MODF_BATCHED model: its blocks go back to the original models. */
void C2TrackBatchModelChanged(br_model* pModel);

/* RenderTrack: adds the enabled blocks overlapping the visible column rectangle. */
void C2TrackBatchRender(int pMin_x, int pMax_x, int pMin_z, int pMax_z);

#endif
