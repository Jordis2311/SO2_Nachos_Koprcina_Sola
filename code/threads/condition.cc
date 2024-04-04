/// Routines for synchronizing threads.
///
/// The implementation for this primitive does not come with base Nachos.
/// It is left to the student.
///
/// When implementing this module, keep in mind that any implementation of a
/// synchronization routine needs some primitive atomic operation.  The
/// semaphore implementation, for example, disables interrupts in order to
/// achieve this; another way could be leveraging an already existing
/// primitive.
///
/// Copyright (c) 1992-1993 The Regents of the University of California.
///               2016-2021 Docentes de la Universidad Nacional de Rosario.
/// All rights reserved.  See `copyright.h` for copyright notice and
/// limitation of liability and disclaimer of warranty provisions.


#include "condition.hh"
#include "lock.hh"
#include "semaphore.hh"
#include "lib/utility.hh"

/// Dummy functions -- so we can compile our later assignments.
///

Condition::Condition(const char *debugName, Lock *conditionLock)
{
    name = debugName;
    cd_lock = conditionLock;
    queue = new List<Semaphore *>;
}

Condition::~Condition()
{
    delete queue;
}

const char *
Condition::GetName() const
{
    return name;
}

void
Condition::Wait()
{
    ASSERT(cd_lock->IsHeldByCurrentThread());

    // Creamos un semaforo en 0, lo agregamos a la lista y lo ponemos a esperar
    Semaphore *semaforo = new Semaphore(name,0);

    queue->Append(semaforo);
    
    DEBUG('s',"Soltando el Lock y Esperando condicion\n");
    // Liberamos el lock antes de esperar y lo tomamos cuando se da la condicion
    cd_lock->Release();
    
    semaforo->P();

    DEBUG('s',"Condicion cumplida, Tomamos el Lock\n");
    cd_lock->Acquire();

}

void
Condition::Signal()
{
    Semaphore *semaforo = queue->Pop();

    // Activamos un semaforo de la lista
    if (semaforo != nullptr) {
        semaforo->V();
    }

}

void
Condition::Broadcast()
{
    Semaphore *semaforo = queue->Pop();

    // Activamos los semaforos de la lista
    while (semaforo != nullptr) {
        semaforo->V();
        semaforo = queue->Pop();
    }
}
