#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#define LIST_DECLARATIONS(ListType, Type, constructor, add, expand) \
  typedef struct ListType {                                         \
    Type *elements;                                                 \
    int size;                                                       \
    int count;                                                      \
  } ListType;                                                       \
  ListType *constructor(int N);                                     \
  void expand(ListType *list);                                      \
  int add(ListType *list, Type e);


#define LIST_IMPLEMENTATION(ListType, Type, constructor, add, expand) \
  ListType *constructor(Type N) {                                     \
    ListType *list = (ListType*)malloc(sizeof(ListType));             \
    list->size = N;                                                   \
    list->count = 0;                                                  \
    list->elements = (Type*)malloc(sizeof(Type)*N);                   \
    return list;                                                      \
  }                                                                   \
  void expand(ListType *list) {                                       \
    int i, oldsize = list->size;                                      \
    list->size = 2*list->size;                                        \
    Type *newElements = (Type*)malloc(sizeof(Type)*list->size);       \
    for (i=0; i<list->count; ++i) {                                   \
      newElements[i] = list->elements[i];                             \
    }                                                                 \
    free(list->elements);                                             \
    list->elements = newElements;                                     \
  }                                                                   \
  int addInt(ListType *list, Type e) {                                \
    int i = list->count;                                              \
    ++(list->count);                                                  \
    if (list->count >= list->size) {                                  \
      expand(list);                                                   \
    }                                                                 \
    list->elements[i] = e;                                            \
    return i;                                                         \
  }

#endif // LIST_H_INCLUDED
