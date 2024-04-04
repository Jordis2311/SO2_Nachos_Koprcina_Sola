/// Copyright (c) 1992-1993 The Regents of the University of California.
///               2007-2009 Universidad de Las Palmas de Gran Canaria.
///               2016-2021 Docentes de la Universidad Nacional de Rosario.
/// All rights reserved.  See `copyright.h` for copyright notice and
/// limitation of liability and disclaimer of warranty provisions.


#include "thread_test_prod_cons.hh"
#include "condition.hh"
#include "lock.hh"

#include <stdio.h>

#define max 3
int buffer[max];
int items = 0;

Lock *lock = new Lock("bufferlock");
Condition *condicionNE = new Condition("condNE", lock);
Condition *condicionNF = new Condition("condNF", lock);

void productor(void *name_){
    for(int i = 1;i <= 1000;i++){
        lock->Acquire();
        while(items == max){
            printf("Productor esperando (buffer lleno)\n");
            condicionNF->Wait();
        }
        printf("Productor produce: %d en %d\n", i, items);
        buffer[items] = i;
        items++;
        condicionNE->Signal();
        lock->Release();
    }
}

void consumidor(){
    for(int i = 1;i <= 1000;i++){
        lock->Acquire();
        while(items == 0){
            printf("Consumidor esperando (buffer vacio)\n");
            condicionNE->Wait();
        }
        items--;
        printf("Consumidor consume: %d en %d\n", buffer[items], items);
        condicionNF->Signal();
        lock->Release();
    }
}

void
ThreadTestProdCons()
{
    Thread *newThread = new Thread("Productor");
    newThread->Fork(productor, NULL);

    consumidor();

}
