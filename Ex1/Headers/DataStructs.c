//
//  DataStructs.c
//  Ex1
//
//  Created by Sasha Plakhotnik on 11.03.2023.
//
#include <stdlib.h>
#include <stdio.h>
#include "DataStructs.h"


typedef struct SturctMeta { // Структура Стэк, Дэк или Очередь
    int Size; // Размер структуры
    char type; // Тип структуры
    void *p; // Указатель на первый элемент
    void *t; // Указатель на последний занятый элемент
} StructMeta;

void HeadAdding(StructMeta **head, StructMeta **elem) {
    return;
}

void TailAdding(StructMeta **tail, StructMeta **elem) {
    return;
}

void HeadDeleting(StructMeta **head, StructMeta **neighbour) {
    return;
}

void TailDeleting(StructMeta **tail, StructMeta **neighbour) {
    return;
}
