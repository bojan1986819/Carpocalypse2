#ifndef VITA_SNDPREFETCH_H
#define VITA_SNDPREFETCH_H

/* Starts a worker thread that reads the WAV files of DATA/SOUND into memory (Vita boot). */
void C2VitaSoundPrefetchStart(void);

/* Hands over the prefetched bytes of pPath (malloc'ed, the caller frees them) and returns 1, or
 * returns 0 when the caller must read the file itself. Waits if the worker is reading that file. */
int C2VitaSoundPrefetchTake(const char* pPath, void** pData, int* pSize);

/* After the sound preload: waits for the worker and frees whatever nothing asked for. */
void C2VitaSoundPrefetchFinish(void);

#endif
