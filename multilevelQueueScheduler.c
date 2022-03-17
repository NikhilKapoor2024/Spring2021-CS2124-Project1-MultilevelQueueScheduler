#include <stdlib.h>

#include "multilevelQueueScheduler.h"

/*
    ---------------------------------
    THIS PROJECT WAS COMPLETED BY:
      Nikhil Kapoor
      Tristan Smith
    ---------------------------------
*/

static const int STEPS_TO_PROMOTION = 50;
static const int FOREGROUND_QUEUE_STEPS = 5;

/* createSchedule
 * input: none
 * output: a schedule
 *
 * Creates and return a schedule struct.
 */
schedule* createSchedule( ) {
    /* TODO: malloc a schedule and initialize the data in schedule */
    schedule* s; // schedule struct pointer
    
    s = (schedule*)malloc(sizeof(schedule)); // mallocs a schedule
    if (s == NULL)
        return NULL; // returns NULL if malloc produces no results
    
    s->FOREGROUND = createQueue(); // creates FOREGROUND queue
    s->BACKGROUND = createQueue(); // creates BACKGROUND queue
    s->stepsSoFar = 0; // sets the number of steps taken to 0
    
    printf("\n");
    return s; /* TODO: Replace with your return value */
}

/* isScheduleUnfinished
 * input: a schedule
 * output: bool (true or false)
 *
 * Check if there are any processes still in the queues.
 * Return TRUE if there is.  Otherwise false.
 */
bool isScheduleUnfinished( schedule *ps ) {
    /* TODO: check if there are any process still in a queue.  Return TRUE if there is. */
    
    if (isEmpty(ps->FOREGROUND) == false || isEmpty(ps->BACKGROUND) == false) // if the FOREGROUND OR BACKGROUND queues are NOT empty
        return true; // return false
    
    return false; // otherwise, return true
}

/* addNewProcessToSchedule
 * input: a schedule, a string, an int, a priority
 * output: void
 *
 * Create a new process with the provided name, stepsUntilComplete, and priority.
 * Add that process to the appropriate queue
 */
void addNewProcessToSchedule( schedule *ps, char *processName, int stepsUntilComplete, priority p ) {
    /* TODO: complete this function.
    The function "initializeProcessData" in processSimulator.c will be useful in completing this. */
    process* proc = NULL; // creates a pointer to a process struct variable
    queueType temp = NULL; // creates queueType var initialized to NULL
    
    // creating process
    temp = malloc(sizeof(queueType));
    proc = (process*)malloc(sizeof(process)); // mallocs memory for a process
    proc->processName = processName; // gets process name
    proc->processData = initializeProcessData(proc->processName);
    proc->stepsUntilComplete = stepsUntilComplete; // gets the steps until the process is complete
    //printf("%s-%d\n", proc->processName, proc->stepsUntilComplete);
    proc->pr = p; // gets what queue is the process's priority (FOREGROUND or BACKGROUND)
    temp = proc; // transfers all data to the queueType var
    
    // assigning process to proper schedule (a queue)
    if (temp->pr == FOREGROUND) {
        temp->stepsFtoR = FOREGROUND_QUEUE_STEPS; // gets the steps until a FOREGROUND process moves to the rear of the FOREGROUND queue
        enqueue(ps->FOREGROUND, temp); // adds process to FOREGROUND queue using getNext function in second parameter of enqueue function
        getNext(ps->FOREGROUND)->stepsBeforeAdd = ps->stepsSoFar; // sets the step number the BACKGROUND process was at before getting added to the schedule
        getNext(ps->FOREGROUND)->s = 0; // sets the number of time steps actually done equal to 0
    }
    else {
        enqueue(ps->BACKGROUND, temp);
        getNext(ps->BACKGROUND)->stepsBeforeAdd = ps->stepsSoFar; // sets the step number the BACKGROUND process was at before getting added to the schedule
        getNext(ps->BACKGROUND)->s = 0; // sets the number of time steps actually done equal to 0
    }
    
    // free( processName ); // TODO: This is to prevent a memory leak but you should remove it once you create a process to put processName into
}

/* runNextProcessInSchedule
 * input: a schedule
 * output: a string
 *
 * Use the schedule to determine the next process to run and for how many time steps.
 * Call "runProcess" to attempt to run the process.  You do not need to print anything.
 * You should return the string "runProcess" returns.  You do not need to use/modify this string in any way.
 */
char* runNextProcessInSchedule( schedule *ps ) {
    /* TODO: complete this function.
    The function "runProcess", "promoteProcess", "loadProcessData", and "freeProcessData"
    in processSimulator.c will be useful in completing this.
    You may want to write a helper function to handle promotion */
    char *ret = NULL; // string to be returned
    int numSteps = 0; // creates int for number of steps and initializes it to 0;
    queueType temp = NULL; // queueType var for dequeueing and enqueueing
    
    // running processes
    while (isScheduleUnfinished(ps) == true) { // as long as there's a schedule unfinished
        while (isEmpty(ps->FOREGROUND) == false) { // as long as one process exists in FOREGROUND
            loadProcessData(getNext(ps->FOREGROUND)->processData); // loads the process
            numSteps = min(FOREGROUND_QUEUE_STEPS, getNext(ps->FOREGROUND)->stepsUntilComplete - getNext(ps->FOREGROUND)->s); // sets numSteps to minimum ammount of time steps
            ret = runProcess(getNext(ps->FOREGROUND)->processName, &numSteps); // calls runProcess to run process numSteps number of times (passes numSteps by reference)
            getNext(ps->FOREGROUND)->s += numSteps; // updates the number of time steps actually done on the process
            ps->stepsSoFar += numSteps;
            if (numSteps != FOREGROUND_QUEUE_STEPS && getNext(ps->FOREGROUND)->s < getNext(ps->FOREGROUND)->stepsUntilComplete) { // numSteps has actually changed and total steps done are still less than the number of steps needed to be done
                numSteps = min(FOREGROUND_QUEUE_STEPS, getNext(ps->FOREGROUND)->stepsUntilComplete - getNext(ps->FOREGROUND)->s); // sets numSteps equal to the total steps needed minus the steps actually done
                ret = runProcess(getNext(ps->FOREGROUND)->processName, &numSteps); // calling runProcess again
                getNext(ps->FOREGROUND)->s += numSteps;
                ps->stepsSoFar += numSteps;
            } // end of if statement
            else if (numSteps == FOREGROUND_QUEUE_STEPS) { // numSteps haven't changed
                temp = dequeue(ps->FOREGROUND);
                enqueue(ps->FOREGROUND, temp);
            } // end of else if statement
            if (getNext(ps->FOREGROUND)->s >= getNext(ps->FOREGROUND)->stepsUntilComplete) {
                temp = dequeue(ps->FOREGROUND);
                freeProcessData();
                free(temp);
            } // end of if statement
        }  // end of while loop
        if (ps->stepsSoFar > STEPS_TO_PROMOTION) { // there have been 50 time steps in total
            while (isEmpty(ps->BACKGROUND) == false) {
                temp = dequeue(ps->BACKGROUND);
                temp->pr = FOREGROUND;
                enqueue(ps->FOREGROUND, temp);
                promoteProcess(temp->processName, temp->processData);
            }
        } // end of if statement
        if (isEmpty(ps->BACKGROUND) == false) {
            loadProcessData(getNext(ps->BACKGROUND)->processData); // loads the process
            numSteps = min(getNext(ps->BACKGROUND)->stepsUntilComplete, STEPS_TO_PROMOTION - ps->stepsSoFar);
            ret = runProcess(getNext(ps->BACKGROUND)->processName, &numSteps); // calls runProcess to run process numSteps number of times (passes numSteps by reference)
            getNext(ps->BACKGROUND)->s += numSteps;
            ps->stepsSoFar += numSteps;
            if (ps->stepsSoFar >= STEPS_TO_PROMOTION) { // there have been 50 time steps in total
                while (isEmpty(ps->BACKGROUND) == false) { // as long as there's a process in the BACKGROUND queue
                  temp = dequeue(ps->BACKGROUND);
                  enqueue(ps->FOREGROUND, temp);
                  promoteProcess(temp->processName, temp->processData);
                } // end of if statement
            } // end of if statement
            else if (getNext(ps->BACKGROUND)->s < getNext(ps->BACKGROUND)->stepsUntilComplete) {
                numSteps = min(getNext(ps->BACKGROUND)->stepsUntilComplete, getNext(ps->BACKGROUND)->stepsUntilComplete - getNext(ps->BACKGROUND)->s); // sets numSteps equal to the total steps needed minus the steps actually done
                ret = runProcess(getNext(ps->BACKGROUND)->processName, &numSteps); // calling runProcess again
                getNext(ps->BACKGROUND)->s += numSteps;
                ps->stepsSoFar += numSteps;
            } // end of if statement
        } // end of if statement
    } // end of while loop
    
    // freeing and returning
    freeSchedule(ps);
    return ret; /* TODO: be sure to store the value returned by runProcess in ret */
}

/* freeSchedule
 * input: a schedule
 * output: none
 *
 * Free all of the memory associated with the schedule.
 */
void freeSchedule( schedule *ps ) {
    /* TODO: free any data associated with the schedule.
    the function "freeQueue" in queue.c will be useful in completing this.
    You may also want to call "freeProcessData" for any processes still left in your queues */
    if (isEmpty(ps->FOREGROUND) == false) {
        while (isEmpty(ps->FOREGROUND) == false) {
            freeProcessData();
        }
        freeQueue(ps->FOREGROUND);
        printf("FOREGROUND schedule freed\n");
    }
    else if (isEmpty(ps->BACKGROUND) == false) {
        while (isEmpty(ps->BACKGROUND) == false) {
            freeProcessData();
        }
        freeQueue(ps->BACKGROUND);
        printf("BACKGROUND schedule freed\n");
    }
    else {
        printf("All schedules freed\n");
    }
}

int min( int x, int y ){
    if( x<y )
        return x;
    return y;
}
