/*
 * FCFS (First Come, First Served) CPU Scheduling
 * -------------------------------------------------
 * Rule: the process that arrives first is served first.
 * Type: non-preemptive (once a process starts, it runs to completion).
 *
 * Formulas:
 *   Turnaround Time (TAT) = Completion Time (CT) - Arrival Time (AT)
 *   Waiting Time (WT)     = Turnaround Time (TAT) - Burst Time (BT)
 */

#include <stdio.h>

#define MAX_PROCESSES 20

typedef struct {
    int pid;
    int arrival;
    int burst;
    int completion;
    int turnaround;
    int waiting;
} Process;

int main(void)
{
    Process p[MAX_PROCESSES];
    int n, i, j;
    int time = 0;
    float total_waiting = 0.0f;
    float total_turnaround = 0.0f;

    printf("Enter number of processes (1-%d): ", MAX_PROCESSES);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_PROCESSES) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter arrival time and burst time for P%d: ", p[i].pid);
        if (scanf("%d %d", &p[i].arrival, &p[i].burst) != 2 ||
            p[i].arrival < 0 || p[i].burst <= 0) {
            printf("Invalid input: arrival must be >= 0 and burst must be > 0.\n");
            return 1;
        }
    }

    /*
     * Step 1: sort processes by arrival time.
     * If two processes arrive at the same time, the lower PID goes first.
     * This is a simple selection sort, easy to trace by hand.
     */
    for (i = 0; i < n - 1; i++) {
        int earliest = i;

        for (j = i + 1; j < n; j++) {
            if (p[j].arrival < p[earliest].arrival ||
                (p[j].arrival == p[earliest].arrival &&
                 p[j].pid < p[earliest].pid)) {
                earliest = j;
            }
        }

        if (earliest != i) {
            Process temp = p[i];
            p[i] = p[earliest];
            p[earliest] = temp;
        }
    }

    /*
     * Step 2: run the processes in arrival order.
     * If the CPU is idle, jump forward to the next arrival time.
     */
    for (i = 0; i < n; i++) {
        if (time < p[i].arrival) {
            time = p[i].arrival;   /* CPU is idle until this process arrives */
        }

        time += p[i].burst;
        p[i].completion = time;
        p[i].turnaround = p[i].completion - p[i].arrival;
        p[i].waiting = p[i].turnaround - p[i].burst;
    }

    /* Step 3: print the result table. */
    printf("\nFCFS Scheduling Result\n");
    printf("--------------------------------------------------\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    printf("--------------------------------------------------\n");

    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].arrival, p[i].burst,
               p[i].completion, p[i].turnaround, p[i].waiting);

        total_waiting += p[i].waiting;
        total_turnaround += p[i].turnaround;
    }

    printf("--------------------------------------------------\n");
    printf("Average Waiting Time    = %.2f\n", total_waiting / n);
    printf("Average Turnaround Time = %.2f\n", total_turnaround / n);

    return 0;
}
