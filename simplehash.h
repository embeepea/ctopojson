#ifndef SIMPLEHASH_H_INCLUDED
#define SIMPLEHASH_H_INCLUDED

/*
 * Bare bones implementation of a hash whose keys are some
 * arbitrary Type and whose values are integers.  You don't
 * get to choose the value for a key;
 * it's assigned automatically when the key is inserted.
 *
 * This hash does not support removing a key.
 *
 * Furthermore, `get` and `insert` are really the same operation,
 * since we'd want to insert any key being queried if it's not
 * already there, and inserting a key returns the corresponding (int)
 * value associated with it.
 *
 * So, this implementation just provides a single `accessor` method.
 */

#define SIMPLEHASH_DECLARATIONS(HashType, Type, constructor, accessor) \
  typedef struct HashType {                                            \
    int M;        /* declared size */                                  \
    int count;    /* number actually occupied */                       \
    Type **keys;                                                       \
  } HashType;                                                          \
  HashType *constructor(int M);                                        \
  int accessor(HashType *hash, Type *p);

#define SIMPLEHASH_IMPLEMENTATION(HashType, Type, constructor, accessor, modularHash, equal)  \
  HashType *constructor(int M) {                                                              \
    HashType *hash = (HashType*)malloc(sizeof(HashType));                                     \
    hash->M = M;                                                                              \
    hash->keys = (Type**)malloc(sizeof(Type*)*M);                                             \
    int i;                                                                                    \
    for (i=0; i<M; ++i) {                                                                     \
      hash->keys[i] = NULL;                                                                   \
    }                                                                                         \
    hash->count = 0;                                                                          \
    return hash;                                                                              \
  }                                                                                           \
  int accessor(HashType *hash, Type *p) {                                                     \
    int i = modularHash(p, hash->M);                                                          \
    int n = 0;                                                                                \
    while (hash->keys[i] != NULL) {                                                           \
      ++n;                                                                                    \
      if (n >= hash->M) {                                                                     \
        fprintf(stderr, "Error: Hash capacity exceeded\n");                                   \
        exit(-1);                                                                             \
      }                                                                                       \
      if (equal(p, hash->keys[i])) {                                                          \
        return i;                                                                             \
      }                                                                                       \
      i = (i + 1) % hash->M;                                                                  \
    }                                                                                         \
    hash->keys[i] = p;                                                                        \
    ++(hash->count);                                                                          \
    return i;                                                                                 \
  }

#endif // SIMPLEHASH_H_INCLUDED
