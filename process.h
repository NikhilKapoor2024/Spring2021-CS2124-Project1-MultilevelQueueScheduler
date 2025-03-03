#ifndef _process_h
#define _process_h

typedef enum priority { FOREGROUND, BACKGROUND } priority;

/* DO NOT access any data inside of a processData struct */
typedef struct processData{ int heap[  30        ]; char       PN21[       21];char TLN        [4  ];}   processData ;

/* struct process (only accessed in student written code)
 *
 * The data associated with a specific process.
 * At minimum you need to track the process name,
 * the processData struct returned by initializeProcessData,
 * and the number of time steps process still needs to run.
 *
 * Hint: It may help to store how many time steps happened before
 * a process was added to the schedule.
 *
 * You may also want to track the number of steps remaining until your FOREGROUND process is moved to the back of the queue.
 */
typedef struct process
{
    //TODO: Put the data for your process here!
    char* processName; // name of process
    int stepsUntilComplete; // number of steps a process needs to run
    processData* processData; // an array of data that belongs to a process
    int stepsBeforeAdd; // the time step when the process was added to a FOREGROUND/BACKGROUND queue
    int stepsFtoR; // number of steps remaining until a FOREGROUND process is moved to the rear of the FOREGROUND queue
    priority pr; // priority struct object for FOREGROUND or BACKGROUND
    int s; // step comparer

}  process;

#endif
