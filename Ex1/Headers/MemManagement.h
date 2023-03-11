//
//  MemManagement.h
//  Ex1
//
//  Created by Sasha Plakhotnik on 03.03.2023.
//
#include <stdlib.h>
#include <stdio.h>

#ifndef MemManagement_h
#define MemManagement_h
void logPush(FILE *fp, char dataC[], int dataI);
typedef struct BlockMeta BlockMeta;
BlockMeta* CreateBlockMeta(int *Pointer, int Index, int Id, int Size);
void DeleteBlockMeta(BlockMeta **head, BlockMeta *elem);
BlockMeta* SearchBlockMeta(BlockMeta *usedMem, int IdBlock); // поиск структуры по ID блока
void Push(BlockMeta **head, BlockMeta *elem); // добавление структуры в список (в соответствии с местом блока в памяти)
BlockMeta* FindFreeMemory(int blockSize, BlockMeta* freeMem); // поиск в списке freeMem свободного блока памяти, вмещающего блок размером blockSize
int AllocateMem(FILE *fp, int memSize, int* p, int blockID, BlockMeta **p_freeMem, BlockMeta **p_usedMem);
int Join(BlockMeta** freeMem, BlockMeta* block);
void FreeMemFunc(FILE *fp, int* p, BlockMeta **p_freeMem, BlockMeta **p_usedMem);
void Print(BlockMeta *head);
#endif /* MemManagement_h */
