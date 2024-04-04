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


#include "lock.hh"
#include "string.h"
#include "semaphore.hh"
#include "system.hh"
#include "lib/utility.hh"
#include "thread.hh"

/// Dummy functions -- so we can compile our later assignments.

/// Create lock
Lock::Lock(const char *debugName)
{
    name = debugName;
    semaforo = new Semaphore(debugName, 1);
}

/// Delete lock
Lock::~Lock()
{
    delete semaforo;
}

const char *
Lock::GetName() const
{
    return name;
}

void
Lock::Acquire()
{
    semaforo->P();
    thread_name = currentThread->GetName();
    DEBUG('s',"Lock tomado por thread %s\n", thread_name);
}

void
Lock::Release()
{   
    ASSERT(IsHeldByCurrentThread());
    semaforo->V();
    DEBUG('s',"Lock liberado por thread %s\n", thread_name);
}

bool
Lock::IsHeldByCurrentThread() const
{
    if(strcmp(thread_name, currentThread->GetName()) == 0){
        return true;
    }
    return false;
}
