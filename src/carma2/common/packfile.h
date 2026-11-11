#ifndef GUARD_PACKFILE_H
#define GUARD_PACKFILE_H

#include "carpocalypse2_types.h"
#include "c2_hooks.h"

#include <stdio.h>

extern int gDisableTiffConversion;

extern void C2_HOOK_FASTCALL InitPackFiles(void);

extern tTWTVFS C2_HOOK_FASTCALL OpenPackFile(const char* path);

extern void C2_HOOK_FASTCALL ClosePackFile(tTWTVFS twt);

#ifndef CARMA2_PF_FCLOSE_DECLARED
#define CARMA2_PF_FCLOSE_DECLARED
extern int C2_HOOK_FASTCALL PFfclose(FILE* pFile);
#endif

extern FILE* C2_HOOK_FASTCALL PFfopen(const char* pPath, const char* mode);
#ifndef CARPOCALYPSE2_MATCHING
/* port: while set, PFfopen reads a loose (unpacked) file opened for reading into memory and serves
 * it like a pack entry; only for readers that use the PF functions alone */
extern int gPF_loose_into_memory;
int PFIsInMountedPack(const char* pPath);
#ifdef __vita__
/* reads every file of pDir into the session cache of gPF_loose_into_memory files, in the background */
void PFPrefetchDirectory(const char* pDir);
#endif
#endif

extern int C2_HOOK_FASTCALL PFfgetc(FILE* pFile);

// PFgetc

extern char* C2_HOOK_FASTCALL PFfgets(char* buffer, br_size_t size, FILE* pFile);

int C2_HOOK_FASTCALL PFungetc(int ch, FILE* pFile);

extern br_size_t C2_HOOK_FASTCALL PFfread(void* buf, br_size_t size, unsigned int n, void* f);

extern int C2_HOOK_FASTCALL PFfwrite(const void* buf, br_size_t size, unsigned int n, void* f);

extern int C2_HOOK_FASTCALL PFftell(FILE* pF);

extern int C2_HOOK_FASTCALL PFfseek(FILE* pF, int offset, int whence);

extern void C2_HOOK_FASTCALL PFrewind(FILE* pF);

extern int C2_HOOK_FASTCALL PFfeof(FILE* pFile);

extern void C2_HOOK_FASTCALL PFForEveryFile(const char* pThe_path, tPDForEveryFileRecurse_cbfn pAction_routine);

extern void C2_HOOK_FASTCALL PFForEveryFile2(const char* path, tEnumPathCallback pCallback, void* data);

extern tTWTVFS C2_HOOK_FASTCALL OpenPackFileAndSetTiffLoading(const char* path);

extern void C2_HOOK_FASTCALL ClosePackFileAndSetTiffLoading(tTWTVFS twt);

extern void C2_HOOK_FASTCALL PackFileRevertTiffLoading(void);

extern void C2_HOOK_FASTCALL PackFileRerevertTiffLoading(void);

void C2_HOOK_FASTCALL PackfileNoOp(void);

#endif // GUARD_PACKFILE_H
