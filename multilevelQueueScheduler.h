#ifndef _multilevelQueueScheduler_h
#define _multilevelQueueScheduler_h
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "process.h"
#include "queue.h"
#include "processSimulator.h"

/* struct schedule (only accessed in student written code)
 *
 * Data related to the order the processes should be scheduled in.
 * At minimum it should store the FOREGROUND and BACKGROUND queues.
 *
 * Hint: It may also help to track the total number of time steps that have been executed so far.
 */
typedef struct schedule
{
    //TODO: Put the data for your schedule program here!
    Queue* FOREGROUND; // FOREGROUND processes queue
    Queue* BACKGROUND; // BACKGROUND processes queue
    int stepsSoFar; // number of time steps since the simulation started

}  schedule;

schedule* createSchedule( );
bool isScheduleUnfinished( schedule *ps );
void addNewProcessToSchedule( schedule *ps, char *processName, int runtime, priority p );
char* runNextProcessInSchedule( schedule *ps );
void freeSchedule( schedule *ps );

int min( int x, int y ) ;

#endif
