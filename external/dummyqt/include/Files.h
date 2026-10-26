#ifndef __FILES___
#define __FILES___

#include "MacTypes.h"

#pragma pack(push, 2)
typedef struct FSSpec {
    short               vRefNum;
    long                parID;
    StrFileName         name;
} FSSpec;
#pragma pack(pop)

#endif /* __FILES___ */
