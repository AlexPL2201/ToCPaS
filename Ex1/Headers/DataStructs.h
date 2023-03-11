//
//  DataStructs.h
//  Ex1
//
//  Created by Sasha Plakhotnik on 11.03.2023.
//
#include <stdio.h>

#ifndef DataStructs_h
#define DataStructs_h
typedef struct SturctMeta StructMeta;
void HeadAdding(StructMeta **head, StructMeta **elem);
void TailAdding(StructMeta **tail, StructMeta **elem);
void HeadDeleting(StructMeta **head, StructMeta **neighbour);
void TailDeleting(StructMeta **tail, StructMeta **neighbour);

#endif /* DataStructs_h */
