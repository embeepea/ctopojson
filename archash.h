#ifndef ARCHASH_H_INCLUDED
#define ARCHASH_H_INCLUDED

#include "arc.h"
#include "simplehash.h"

SIMPLEHASH_DECLARATIONS(ArcHash, Arc, newArcHash, getArcIndex)

int getReverseArcIndex(ArcHash *hash, Arc *a);

#endif // ARCHASH_H_INCLUDED
