#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define MAX_PROCS 10 // Max number of processes
#define QUANTUM 40 // Time quantum, ms
#define MIN(x,y) ((x)<(y)?(x):(y)) // Compute the minimum

/**
 * Process information.
 */
struct process {
    int pid;
    int time_awake_remaining;
};

/**
 * The process table.
 */
struct process table[MAX_PROCS];

/**
 * Initialize the process table.
 */
void init_proc_table(void)
{
    for (int i = 0; i < MAX_PROCS; i++) {
        table[i].pid = i;
        table[i].time_awake_remaining = 0;
    }
}

/**
 * Parse the command line.
 */
void parse_command_line(int argc, char **argv)
{
    for (int i = 0; i < argc-1; i++){
        table[i].time_awake_remaining = atoi(argv[i+1]);
    }
}

/**
 * Main.
 */
int main(int argc, char **argv)
{
    struct queue *q = queue_new();
    int clock = 0;
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
        printf("=== Clock %d ms ===\n", clock);

        struct process *p = queue_dequeue(q);
        printf("PID %d: Running\n", p->pid);

        // Job has time remaining
        if(p->time_awake_remaining > QUANTUM){
            p->time_awake_remaining -= QUANTUM;

            runtime = QUANTUM;
            clock += QUANTUM;

            queue_enqueue(q, p);
        }

        // Job is completed
        else {
            clock += p->time_awake_remaining;
            runtime = p->time_awake_remaining;

            p->time_awake_remaining = 0;
            exited += 1;
        }

        printf("PID %d: Ran for %d ms\n", p->pid, runtime);

    }

    queue_free(q);
}
