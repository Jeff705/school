#include <ucontext.h>
#include <stdlib.h>
#include <assert.h>
#include "mythreads.h"
// DATA STRUCTURE DECLARATIONS

typedef struct thread {
	int id;
	void *returnValue;
	ucontext_t context;
	struct thread *next;
	struct thread *prev;

} thread;

typedef struct thread_queue {
	int count;
	thread *currentThread;
	thread *head;
	thread *tail;
} tqueue;

typedef struct retVal {
	int id;
	void *returnValue;
	struct retVal *next;
} retVal;

typedef struct rtvList {
	retVal *head;
	retVal *tail;
} rtvList;

// GLOBAL VARIABLES

int interruptsAreDisabled = 0;
tqueue *queue;
thread *mainthread;
rtvList returnList;

// LIBRARY METHODS

// HELPER FUNCTIONS: these aren't part of the mythreads API

static void interruptDisable() 
{
	assert(!interruptsAreDisabled) ;
	interruptsAreDisabled = 1;
}

static void interruptEnable() 
{
	assert(interruptsAreDisabled) ;
	interruptsAreDisabled = 0;
}

int isQueueTail(thread *checkMe) {
	if(queue->tail == checkMe) {
		return 1;
	}
	else return 0;
}
//TODO: Figure a way to safely deallocate the thread

/* removeThread()
 *
 * pulls thread from the circular queue and fixes the links
 *
 * also sets currentThread to the next available thread
 * EFFECTIVELY ATOMIC
 * this is called from within a protected section
 */

void removeThread(thread *removeMe) {
	if(isQueueTail(removeMe)) 
	{
		thread *newTail = queue->tail->prev;
		queue->head->prev = newTail;
		newTail->next = queue->head;
		queue->currentThread = queue->head;
	}

	else
	{
		thread *forward = queue->currentThread->next;
		thread *reverse = queue->currentThread->prev;
		forward->prev = reverse;
		reverse->next = forward;
		queue->currentThread = forward;
	}
}

void managedThreadRun(thFuncPtr func, void *arg)
{
	void *result = func(arg);
	threadExit(result);
}

/* threadMake()
 *
 * Allocates space for a new thread type, sets ID, and stores
 * the context passed by the caller.
 *
 */

thread* threadMake(int newid, ucontext_t *threadContext)
{
	thread* newthread = (thread*)malloc(sizeof(thread));
	newthread->id = newid;
	newthread->context = *threadContext;
	return newthread;
}

/* contextSetup()
 *
 * takes a context pointer, function pointer (type), and argument pointer
 *
 * Initializes and allocates a new stack
 * Sets up context to run function with specified argument
 *
 */

void contextSetup(ucontext_t *newcontext, thFuncPtr func, void* arg) 
{
	
	getcontext(newcontext);
	newcontext->uc_stack.ss_sp = malloc(STACK_SIZE);
	newcontext->uc_stack.ss_size = STACK_SIZE;
	newcontext->uc_stack.ss_flags = 0;
	makecontext(newcontext, (void (*)(void))managedThreadRun, 2, func, arg);

}
/*
 * EFFECTIVELY ATOMIC
 * this is called from inside a protected section
 *
 */
void returnListAppend(retVal *toAppend)
{
	if(returnList.head == NULL)
	{
		returnList.head = toAppend;
		returnList.tail = toAppend;
	}

	else
	{	
		returnList.tail->next = toAppend;
		returnList.tail = toAppend;
	}
}

// API DEFINED FUNCTIONS: These are specified in the API for the library

void threadInit() 
{
	
	interruptDisable();

	ucontext_t *currentContext = malloc(sizeof(ucontext_t));
	getcontext(currentContext);
	queue = malloc(sizeof(tqueue));
	queue->head = threadMake(0, currentContext);
	queue->tail = queue->head;
	queue->currentThread = queue->head;
	queue->count = 1;

	mainthread = queue->head; //we want this for easy access later!

	interruptEnable();

}

/* retValFinder()
 *
 * iterates through the list of retVal structures and finds the one that
 * corresponds to thread_id, if it exists.
 * 
 * stores retVal.returnValue in 'result' if it is found and returns 1
 *
 * else returns 0
 * 
 */

int retValFinder(int thread_id, void **result) 
{
	int found = 0;
	retVal *current;
	if(returnList.head != NULL) 
	{

		current = returnList.head;
		//iterate over list of retVal nodes in the list, grab the value if
		//id matches
		while(current != NULL && found == 0) 
		{
			if(current->id == thread_id)
			{
				result = &(current->returnValue);
				found = 1;
			}
			current = current->next;
		}
	}

	return found;
}

// thread management functions

/* threadCreate()
 * 
 * ATOMIC *
 * Creates a context and thread node for the queue
 * Appends node to tail of the queue
 * Points the 'next' pointer of the tail to the head (circularizing)
 *
 */

int threadCreate( thFuncPtr funcPtr , void *argPtr ) 
{
	interruptDisable();
	ucontext_t *newContext = malloc(sizeof(ucontext_t));
	ucontext_t *prevContext = &(queue->currentThread->context);
	contextSetup(newContext, funcPtr, argPtr);
	thread* newthread = threadMake(queue->count, newContext);
	queue->count++;
	queue->tail->next = newthread;
	newthread->prev = queue->tail;
	queue->tail = newthread;
	queue->tail->next = queue->head;
	queue->head->prev = queue->tail;
	queue->currentThread = queue->tail;
	interruptEnable();
	swapcontext(prevContext, newContext);
	return newthread->id;
}

void threadYield()
{
	if(!interruptsAreDisabled) 
	{
		ucontext_t *prevContext = &(queue->currentThread->context);
		queue->currentThread = queue->currentThread->next;
		ucontext_t *runThisContext = &(queue->currentThread->context);
		swapcontext(prevContext, runThisContext);
	}
}

void threadJoin( int thread_id , void ** result )
{
	
	int found = 0;

	while(!found) 
	{
		interruptDisable();
		found = retValFinder(thread_id, result);
		if(!found) {
			interruptEnable();
			threadYield();
		}
	}
	interruptEnable();

}

void threadExit(void *result)
{
	interruptDisable();
	if(queue->currentThread->id == 0) 
	{
		exit(0);
	}

	else
	{
		//add current thread's stuff to list of return values
		retVal *toAppend = malloc(sizeof(retVal));
		toAppend->id = queue->currentThread->id;
		toAppend->returnValue = result;
		returnListAppend(toAppend);
		removeThread(queue->currentThread);
		ucontext_t *nextContext = &(queue->currentThread->context);
		interruptEnable();
		setcontext(nextContext);
	}
}

// synchronization functions
void threadLock( int lockNum )
{

}

void threadUnlock( int lockNum )
{

}

void threadWait( int lockNum , int conditionNum)
{

}

void threadSignal( int lockNum , int conditionNum)
{

}

// control atomicity



