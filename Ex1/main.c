//
//  main.c
//  Ex1
//
//  Created by Sasha Plakhotnik on 10.02.2023.
//

#include <stdlib.h>
#include <stdio.h>

int allocateMem(int* blockSizesArray, int memSize, int blockCount) {
    int *p = NULL;
    int endBlock = 0, startBlock = 0;
    p = (int*) malloc(memSize * sizeof(int));
    for (int i = 0; i < blockCount; i++) {
        endBlock += blockSizesArray[i];
        for (int j = startBlock; j < endBlock; j++) {
            p[j] = i;
        }
        startBlock += blockSizesArray[i];
    }
    
//    for (int i = 0; i < memSize; i++) {
//        printf("%d ", p[i]);
//    }
    
    int res;
    printf("Очистить?");
    scanf("%d", &res);
    if (res == 1) {
        free(p);
    }
    return 0;
}

int main(int argc, const char * argv[]) {
    int choice;
    do {
        printf("\n1. Выделение памяти заданного размера (блок)\n2. Освобождение памяти\n3. Выход\nПункт: ");
        scanf("%d", &choice);
        if (choice == 1) {
            int memSize, blockCount, blockSize, memInUse = 0;
            printf("Введите размер (Байт) необходимой памяти: ");
            scanf("%d", &memSize);
            printf("Введите количесвто блоков памяти: ");
            scanf("%d", &blockCount);
            int blockSizesArray[blockCount];
            for (int i = 0; i < blockCount; i++) {
                printf("Введите размер (Байт) блока %d (Осталсоь %d Байт): ", i, memSize - memInUse);
                scanf("%d", &blockSize);
                memInUse += blockSize;
                blockSizesArray[i] = blockSize;
            }
            for (int i = 0; i < blockCount; i++) {
                printf("%d", blockSizesArray[i]);
            }
            
            allocateMem(blockSizesArray, memSize, blockCount);
        }
        else if (choice == 2) {
            printf("2");
        }
        else if (choice == 3) {
            printf("Выход");
        }
        else {
                printf("Error 0. No such comand");
        }
    } while (choice != 3);
    
}
