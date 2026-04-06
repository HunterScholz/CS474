#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "queue.h"

#define MAX_PROG_LEN 10 // Max terms in a "program"
#define MAX_PROCS 10 // Max number of processes
#define QUANTUM 40 // Time quantum, ms

#define MIN(x,y) ((x)<(y)?(x):(y)) // Compute the minimum

// TODO: all the code!

// Process information.
struct process {
    int pid;
    int time_awake_remaining;
    int time_asleep_remaining;
    int instructions[MAX_PROG_LEN+1];
    int counter;
    enum state {
        READY,
        SLEEPING,
        EXITED
    } state;
};

struct process table[MAX_PROCS];

// Initialize the process table.
void init_proc_table(void){
    for (int i = 0; i < MAX_PROCS; i++) {
        table[i].pid = i;
        table[i].time_awake_remaining = 0;
        table[i].time_asleep_remaining = 0;
        table[i].counter = 0;
        table[i].state = READY;
    }
}

// Parse the command line.
void parse_command_line(int argc, char **argv){
    for (int i = 0; i < argc-1; i++){

        char *token;
        int j = 0;

        if ((token = strtok(argv[i+1], ",")) != NULL) do {
            table[i].instructions[j++] = atoi(token);
        } while ((token = strtok(NULL, ",")) != NULL);

        table[i].time_awake_remaining = table[i].instructions[0];
    }
}

// Simulate the passage of time
void pass_time(int *clock, int argc){

    int lowest_time_to_wake = INT_MAX;

    for (int i = 0; i < argc-1; i++){
        if (table[i].state == SLEEPING){
            lowest_time_to_wake = MIN(lowest_time_to_wake, table[i].time_asleep_remaining);
        }
    }

    if (lowest_time_to_wake == INT_MAX)
        return;

    for (int i = 0; i < argc-1; i++){
        if (table[i].state == SLEEPING){
            table[i].time_asleep_remaining -= lowest_time_to_wake;
        }
    }

    *clock += lowest_time_to_wake;
}

void pass_sleeping(int runtime, int argc) {
    for (int i = 0; i < argc-1; i++) {
        if (table[i].state == SLEEPING) {
            table[i].time_asleep_remaining -= runtime;
        }
    }
}

void wake_process(struct queue *q, int i){
    struct process *p = table+i;
    p->counter += 1;

    // Exit if out of instructions
    if (p->instructions[p->counter] == 0){
        p->state = EXITED;
        printf("PID %d: Exiting\n", p->pid);
    }

    // Otherwise requeue
    else {
        p->state = READY;
        p->time_awake_remaining = p->instructions[p->counter];

        printf("PID %d: Waking up for %d ms\n", p->pid, p->instructions[p->counter]);
        queue_enqueue(q, p);
    }
}

// Main.
int main(int argc, char **argv)
{
    int clock = 0;
    struct queue *q = queue_new();
    int exited = 0;
    int num_jobs = argc-1;
    int runtime = 0;

    init_proc_table();
    parse_command_line(argc, argv);

    // Add Jobs
    for (int i = 0; i < num_jobs; i++){
        queue_enqueue(q, table+i);
    }

    // Main Loop
    while (exited != num_jobs){

        if(queue_is_empty(q)){
            pass_time(&clock, argc);
        }

        printf("=== Clock %d ms ===\n", clock);


        // Check any awake processes
        for (int i = 0; i < argc-1; i++){
            if (table[i].time_asleep_remaining <= 0 && table[i].state == SLEEPING){
                wake_process(q, i);
            }
        }

        if (queue_is_empty(q))
            continue;

        struct process *p = queue_dequeue(q);
        printf("PID %d: Running\n", p->pid);

        // Current job has time remaining
        if(p->time_awake_remaining > QUANTUM){
            p->time_awake_remaining -= QUANTUM;

            runtime = QUANTUM;
            clock += QUANTUM;
            pass_sleeping(runtime, argc);

            p->state = READY;
            queue_enqueue(q, p);
        }

        // Current job is completed
        else {
            clock += p->time_awake_remaining;
            runtime = p->time_awake_remaining;
            pass_sleeping(runtime, argc);

            p->counter++;
            p->time_awake_remaining = 0;

            // Exit if there are no more instructions
            if(p->instructions[p->counter] == 0){
                p->state = EXITED;
                printf("PID %d: Exited\n", p->pid);
                exited += 1;
            }

            // Otherwise go to sleep
            else {
                p->state = SLEEPING;
                p->time_asleep_remaining = p->instructions[p->counter];
                printf("PID %d: Sleeping for %d\n", p->pid, p->time_asleep_remaining);
            }
        }

        printf("PID %d: Ran for %d ms\n", p->pid, runtime);

    }

    queue_free(q);
}
