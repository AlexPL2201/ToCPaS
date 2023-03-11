//
//  MemManagement.c
//  Ex1
//
//  Created by Sasha Plakhotnik on 03.03.2023.
//
#include <stdlib.h>
#include <stdio.h>
#include "MemManagement.h"


void logPush(FILE *fp, char dataC[], int dataI) {
    char name[] = "Desktop/log.txt";
    fp = fopen(name, "a+");
    if (fp != NULL) {
        fprintf(fp, "%s %d;\n", dataC, dataI);
    }
    fclose(fp);
}

// BlockMeta - информация о блоке памяти
typedef struct BlockMeta {
    int *pointer; // указатель на место в памяти, где хранится описываемый блок
    int index; // индекс этого места в памяти (так как память представляет собой массив, можно получить доступ к первой ячейке блока через индекс массива)
    int IDBlock; // id блока - определяется номером очерёдности создания блока
    int size; // размер блока памяти
    struct BlockMeta* next; // указатель на следующую структуру
} BlockMeta;


void Optimization (int* p, BlockMeta **freeMem, BlockMeta **usedMem, FILE *fp)
{
    BlockMeta *tmp = *usedMem; // начинаем проходиться по занятой памяти
    int fstIter = 1; // первая итерация цикла или нет
    int sEmp, sFull, sizeEmp, sizeFull;

    logPush(fp, "[Optimize] Request to stack all blocks to the left", 0);

    do {

        if (tmp->index != 0 && fstIter == 1) // если пустой блок стоит первее занятого
        {
            sEmp = 0; // начало пустого блока
            sFull = tmp->index; // начало первого занятого блока после пустого
            sizeFull = tmp->size; // размер занятого
        }
        else
        {
            sEmp = tmp->index + tmp->size;
            sFull = tmp->next->index;
            sizeFull = tmp->next->size;
            tmp = tmp->next; // перемещаемся на первый занятый блок после пустого
        }
        sizeEmp = sFull - sEmp; // размер пустого блока

        if (sizeEmp != 0) // если пустой блок есть на самом деле
        {
            tmp->pointer = &p[sEmp]; // сдвигаем указатель занятого блока влево
            tmp->index = sEmp; // меняем индекс занятого блока
            for (int i = 0; i < sizeFull; i++) // сдвигаем сами данные влево
            {
                p[sEmp + i] = p[sEmp + i + sizeEmp];
            }
            BlockMeta *empty = *freeMem;
            while (empty->index != sEmp) // находим в списке пустых блоков нужный
            {
                empty = empty->next;
            }
            empty->pointer = &p[sEmp + sizeFull]; // сдвигаем указатель пустого блока вправо
            empty->index = sEmp + sizeFull; // меняем индекс пустого блока
        }

        fstIter = 0;
    } while (tmp->next != NULL);

    BlockMeta *block = *freeMem;
    int *pointer = block->pointer;
    int index = block->index;
    int size = 0;
    while (block != NULL) // считаем суммарный размер пустого места, удаляем записи о блоках
    {
        size += block->size;
        tmp = block->next;
        DeleteBlockMeta(freeMem, block);
        block = tmp;
    }
    block = CreateBlockMeta(pointer, index, 0, size); // новая запись об одном пустом блоке
    Push(freeMem, block); // добавляем в список пустой памяти
    for (int i = 0; i < size; i++) { // для наглядности меняем пустое на нолики
        p[index + i] = 0;
    }

    logPush(fp, "[Optimize] Metadata changed. Defragmentation of empty space completed", 0);
}


BlockMeta* CreateBlockMeta(int *Pointer, int Index, int Id, int Size)
{
    BlockMeta *tmp = (BlockMeta*) malloc(sizeof(BlockMeta));
    tmp->pointer = Pointer;
    tmp->index = Index;
    tmp->IDBlock = Id;
    tmp->size = Size;
    tmp->next = NULL;
    return tmp;
}

void DeleteBlockMeta(BlockMeta **head, BlockMeta *elem)
{
    BlockMeta *pnow = *head; // текущий узел
    BlockMeta *prev = NULL;   // предыдущий узел
    
    while (pnow != NULL)
    {
        if (elem == pnow)
        {
            if (elem == *head)
            {
                *head = elem->next;
            }
            else
            {
                prev->next=elem->next;
            }
            free(elem);
            return;
        }
        prev = pnow;
        pnow = pnow->next;
    }
}

BlockMeta* SearchBlockMeta(BlockMeta *usedMem, int IdBlock) // поиск структуры по ID блока
{
    while (usedMem != NULL)
    {
        if (usedMem->IDBlock == IdBlock)
        {
            return usedMem;
        }
        usedMem = usedMem->next;
    }
    
    return NULL;
}

void Push(BlockMeta **head, BlockMeta *elem) // добавление структуры в список (в соответствии с местом блока в памяти)
{
    if (*head == NULL)
    {
        *head = elem;
    }
    else
    {
        if ((*head)->index > elem->index)
        {
            elem->next = *head;
            *head = elem;
        }
        else
        {
            BlockMeta* tmp = *head;
            
            while (tmp->next != NULL)
            {
                if (tmp->index < elem->index && tmp->next->index > elem-> index)
                {
                    break;
                }
                tmp = tmp->next;
            }
            
            elem->next = tmp->next;
            tmp->next = elem;
        }
    }
}

BlockMeta* FindFreeMemory(int blockSize, BlockMeta* freeMem) // поиск в списке freeMem свободного блока памяти, вмещающего блок размером blockSize
{
    while (freeMem != NULL)
    {
        if (freeMem->size >= blockSize)
        {
            return freeMem;
        }
        
        freeMem = freeMem->next;
    }
    
    return NULL;
}

// функция выделения памяти, возвращает 1, если смогли выделить память, 0 - если не смогли выделить память
int AllocateMem(FILE *fp, int memSize, int* p, int blockID, BlockMeta **p_freeMem, BlockMeta **p_usedMem)
// memSize - размер общей памяти
// *p - указатель на общую память
// blockID - id блока, который хотим добавить
// **p_freeMem - указатель на указатель на список инфы о свободной памяти
// **p_usedMem - указатель на указатель на список инфы о занятой памяти

// !!! указатель на указатель необходим, потому что в Си указатели передаются в функцию по значению, а не по ссылке, то есть если мы поменяем значение указателя внутри метода, при выходе из функции значение указателя останется прежним
{
    int blockSize;
    printf("Введите размер блока памяти: ");
    scanf("%d", &blockSize);
    
    if (blockSize <= 0)
    {
        printf("Некорректный ввод. Введите положительное число\n");
        return 0;
    }
    
    BlockMeta* freeMemory = FindFreeMemory(blockSize, *p_freeMem); // находим непрерывную свободную память нужного размера
    logPush(fp, "[Allocate] Request for memory allocation for block", blockID);
    
    if (freeMemory == NULL)
    {
        printf("Попытка оптимизации памяти\n");
        Optimization(p, p_freeMem, p_usedMem, fp);
        freeMemory = FindFreeMemory(blockSize, *p_freeMem); // находим непрерывную свободную память нужного размера
        if (freeMemory == NULL) {
            printf("Нет свободной памяти\n");
            logPush(fp, "[Allocate] Not enough memory", 0);
            return 0;
        }
        printf("Память оптимизирована\n");
        
    }
        // меняем значения ячеек в самой памяти (чисто для наглядности, чтобы нам самим было видно, что мы память чем-то заняли)
    for (int i = 0; i < blockSize; i++)
    {
        p[freeMemory->index+i] = blockID;
    }
    
    // создаём структуру с инфой о новом используемом блоке памяти
    BlockMeta *tmp = CreateBlockMeta(freeMemory->pointer, freeMemory->index, blockID, blockSize);
    // добавляем структуру в список
    Push(p_usedMem, tmp);
    
    // если размер блока свободной памяти равен размеру блока, который мы хотим занять значит данного блока свободной памяти большей не существует, удаляем инфу о нём
    if (freeMemory->size == blockSize)
    {
        DeleteBlockMeta(p_freeMem, freeMemory);
    }
    // иначе мы лишь уменьшаем наш блок свободной памяти
    else
    {
        freeMemory->pointer += blockSize;
        freeMemory->index += blockSize;
        freeMemory->size -= blockSize;
    }
    logPush(fp, "[Allocate] Metadata changed. Memory allocated (byte):", blockSize);
    return 1;
}

// функция Join необходима для освобождения памяти, вызывается из функции FreeMemFunc(), которая описана ниже

// она проверяет, есть ли рядом с будущим блоком свободной памяти такие блоки памяти, с которыми его можно объединить
// если есть, функция объединяет их и возвращает 1
// если нет, функция возращает 0

// у нас есть 3 ситуации, когда блоки надо объединить:
// 1 - перед текущим блоком есть блок свободной памяти
// 2 - после текущего блока есть блок свободной памяти
// 3 - и перед, и после текущего блока есть блоки свободной памяти, то есть он находится между ними
int Join(BlockMeta** freeMem, BlockMeta* block)
{
    BlockMeta *tmp = *freeMem;
    
    int ThereIsBlockBefore = 0; // флаг о том, что сущестует блок свободной памяти перед текущим
    BlockMeta* blockbefore = NULL; // указатель на этот блок
    
    while (tmp != NULL)
    {
        if (tmp->index + tmp->size == block->index) // если нашли свободный блок памяти перед текущим
        {
            tmp->size += block->size;
            blockbefore = tmp;
            ThereIsBlockBefore = 1;
            continue;
        }
        
        if (tmp->index == block->index + block->size) // если нашли свободный блок памяти после текущего
        {
            if (ThereIsBlockBefore) // и при этом мы уже знаем, что сущестует блок свободной памяти перед текущим
            {
                blockbefore->size += tmp->size;
                DeleteBlockMeta(freeMem, tmp);
                return 1;
            }
            
            tmp->pointer = block->pointer;
            tmp->index = block->index;
            tmp->size += block->size;
            return 1;
        }

        tmp = tmp->next;
    }
    
    if (ThereIsBlockBefore)
    {
        return 1;
    }
    
    return 0;
}

// функция освобождения памяти
void FreeMemFunc(FILE *fp, int* p, BlockMeta **p_freeMem, BlockMeta **p_usedMem)
{
    int IdBlock;
    printf("Введите ID блока памяти: ");
    scanf("%d", &IdBlock);
    logPush(fp, "[Release] Request to release block", IdBlock);
    
    BlockMeta* block = SearchBlockMeta(*p_usedMem, IdBlock);
    
    if (block == NULL)
    {
        printf("Блок не найден\n");
        logPush(fp, "[Release] Not found block", IdBlock);
        return;
    }
    
    if (!Join(p_freeMem, block)) // если объединение не произошло
    {
        BlockMeta *newBlock = CreateBlockMeta(block->pointer, block->index, 0, block->size); // создаём новую инфу о свободном блоке
        Push(p_freeMem, newBlock); // добавляем инфу в список
    }
    
    // отмечаем в памяти освободившийся блок (опять же, чисто для наглядности)
    for (int i = 0; i < block->size; i++)
    {
        p[block->index+i] = 0;
    }
    
    // удаляем из списка занятой памяти инфу о блоке, который уже освободили
    DeleteBlockMeta(p_usedMem, block);
    logPush(fp, "[Release] Metadata changed. Released block", IdBlock);
}

void Print(BlockMeta *head)
{
    if (head == NULL) {
        printf("Нет данных\n");
        return;
    }
    printf("Адрес памяти\t|\tИндекс\t|\tID\t|\tРазмер\n");
    do
    {
        printf("%p\t|\t%d\t|\t%d\t|\t%d\n", head->pointer, head->index, head->IDBlock, head->size);
        head = head->next;
    } while (head);
}


