/// Copyright (c) 1992-1993 The Regents of the University of California.
///               2007-2009 Universidad de Las Palmas de Gran Canaria.
///               2016-2021 Docentes de la Universidad Nacional de Rosario.
/// All rights reserved.  See `copyright.h` for copyright notice and
/// limitation of liability and disclaimer of warranty provisions.


#include "thread_test_simple.hh"
#include "system.hh"

#include <stdio.h>
#include <string.h>


/// Loop 10 times, yielding the CPU to another ready thread each iteration.
///
/// * `name` points to a string with a thread name, just for debugging
///   purposes.

bool thread2Done = false;
bool thread3Done = false;
bool thread4Done = false;
bool thread5Done = false;

void
SimpleThread(void *name_)
{

    // If the lines dealing with interrupts are commented, the code will
    // behave incorrectly, because printf execution may cause race
    // conditions.
    for (unsigned num = 0; num < 10; num++) {
        printf("*** Thread `%s` is running: iteration %u\n", currentThread->GetName(), num);
        currentThread->Yield();
    }
    if (strcmp(currentThread->GetName(),"2nd")==0) {
	    thread2Done = true;
    }
    if (strcmp(currentThread->GetName(),"3rd")==0) {
	    thread3Done = true;
    }
    if (strcmp(currentThread->GetName(),"4th")==0) {
	    thread4Done = true;
    }
    if (strcmp(currentThread->GetName(),"5th")==0) {
	    thread5Done = true;
    }
    printf("!!! Thread `%s` has finished SimpleThread\n", currentThread->GetName());
 
}

/// Set up a ping-pong between several threads.
///
/// Do it by launching one thread which calls `SimpleThread`, and finally
/// calling `SimpleThread` on the current thread.
void
ThreadTestSimple()
{
    Thread *newThread = new Thread("2nd");
    Thread *newThread3 = new Thread("3rd");
    Thread *newThread4 = new Thread("4th");
    Thread *newThread5 = new Thread("5th");
    newThread->Fork(SimpleThread, NULL);
    newThread3->Fork(SimpleThread, NULL);
    newThread4->Fork(SimpleThread, NULL);
    newThread5->Fork(SimpleThread, NULL);

    //the "main" thread also executes the same function
    SimpleThread(NULL);

   //Wait for the 2nd thread to finish if needed
    while (!thread2Done || !thread3Done || !thread5Done || !thread5Done) {
        currentThread->Yield(); 
    }
    printf("Test finished\n");
}
