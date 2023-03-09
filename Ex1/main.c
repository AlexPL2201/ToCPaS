//
//  main.c
//  Ex1
//
//  Created by Sasha Plakhotnik on 03.03.2023.
//

#include <stdlib.h>
#include <stdio.h>
#include "Headers/MemManagement.h"

int main(int argc, const char * argv[])
{
    int memSize;
    FILE *fp = NULL;
    
    printf("Введите размер (Байт) необходимой памяти: ");
    scanf("%d", &memSize);

    int *p = (int*) malloc(memSize * sizeof(int)); // выделенная память
    
    if (p != NULL) {
        logPush(fp, "[Main] Allocated memory(bytes):", memSize);
        for (int i = 0; i < memSize; i++)
        {
            p[i] = 0;
        }
        
        BlockMeta *freeMem = NULL; // указатель на список из инфы о блоках свободной памяти
        BlockMeta *usedMem = NULL; // указатель на список из инфы о блоках занятой памяти
        
        // так как в начале вся память является свободной, создаём блок инфы о свободной памяти и добавляем его в список
        
        freeMem = CreateBlockMeta(&p[0], 0, 0, memSize);
        
        int choice;
        
        int blockID = 1;
        
        do {
            printf("Память: ");
            for (int i = 0; i < memSize; i++)
            {
                printf("%d ", p[i]);
            }
            printf("\nИспользуемая память\n");
            Print(usedMem);
            printf("\nСвободная память\n");
            Print(freeMem);
            printf("\n1. Выделение памяти заданного размера (блок)\n2. Освобождение памяти\n3. Выход\nПункт: ");
            scanf("%d", &choice);
            if (choice == 1) {
                if (AllocateMem(fp, memSize, p, blockID, &freeMem, &usedMem))
                {
                    blockID++;
                }
            }
            
            else if (choice == 2) {
                FreeMemFunc(fp, p, &freeMem, &usedMem);
            }
            else if (choice == 3) {
                free(p);
                logPush(fp, "[Free] Memory deallocated. Exit code", 0);
                printf("Выход");
            }
            else {
                printf("Error 0. No such comand");
            }
        } while (choice != 3);
    }
    else {
        logPush(fp, "[Main] Memory allocation error", 0);
        
    }
}

