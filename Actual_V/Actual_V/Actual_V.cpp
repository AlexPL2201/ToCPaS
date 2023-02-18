#include <stdlib.h>
#include <stdio.h>


int findFreeMemory(int memSize, int blockSize, int* flags)
{
    int freeSize = 0;              // размер свободной памяти
    int freeMemory = 0;            // элемент, с которого начинается свободная память

    for (int i = 0; i < memSize; i++)
    {
        if (freeSize == blockSize)
        {
            return freeMemory;
        }

        if (flags[i] == 0)
        {
            if (freeSize == 0)
            {
                freeMemory = i;
            }
            freeSize++;
        }
        else
        {
            freeSize = 0;
            freeMemory = 0;
        }
    }

    return -1;
}

void allocateMem(int memSize, int* p, int* flags, int blockID) {

    int blockSize;
    printf("Введите размер блока памяти: ");
    scanf("%d", &blockSize);


    int freeMemory = findFreeMemory(memSize, blockSize, flags); // находим непрерывную свободную память нужного размера

    if (freeMemory == -1)
    {
        printf("Нет свободной памяти\n");
    }
    else
    {
        for (int i = 0; i < blockSize; i++)
        {
            p[freeMemory + i] = blockID;
            flags[freeMemory + i] = 1;
        }
    }


}



int main(int argc, const char* argv[]) {

    int memSize;

    printf("Введите размер (Байт) необходимой памяти: ");
    scanf("%d", &memSize);

    int* p = (int*)malloc(memSize * sizeof(int)); // выделенная память

    int flags[memSize]; // массив флагов (используется\не используется)

    for (int i = 0; i < memSize; i++)
    {
        p[i] = 0;
        flags[i] = 0;
    }


    int choice;

    int blockID = 1;

    do {
        printf("Память: ");
        for (int i = 0; i < memSize; i++)
        {
            printf("%d ", p[i]);
        }
        printf("\n");
        printf("Статус: ");
        for (int i = 0; i < memSize; i++)
        {
            printf("%d ", flags[i]);
        }
        printf("\n1. Выделение памяти заданного размера (блок)\n2. Освобождение памяти\n3. Выход\nПункт: ");
        scanf("%d", &choice);
        if (choice == 1) {
            allocateMem(memSize, p, flags, blockID);
            blockID++;
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
