#ifndef C2_VITA_DATA_H
#define C2_VITA_DATA_H

/* Return zero from progress to cancel; completed files survive for a retry. */
typedef int (*C2VitaCopyProgress)(void* context, const char* path, unsigned int bytes);
int C2VitaCopyMissingTree(const char* source, const char* destination,
    C2VitaCopyProgress progress, void* context);

#endif
