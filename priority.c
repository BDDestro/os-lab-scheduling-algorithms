/*
 * Priority CPU Scheduling - Non-Preemptive
 * -----------------------------------------
 * Rule: among the processes that have already arrived, run the one
 *       with the smallest priority number.
 *       Smaller number = higher priority (1 is higher than 3).
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
    int priority;
    int completion;
    int turnaround;
    int waiting;
} Process;

int main(void)
{
    Process p[MAX_PROCESSES];
    int done[MAX_PROCESSES] = {0};
    int n, i;
    int time = 0;
    int completed = 0;
    float total_waiting = 0.0f;
    float total_turnaround = 0.0f;

    printf("Enter number of processes (1-%d): ", MAX_PROCESSES);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_PROCESSES) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter arrival time, burst time and priority for P%d: ", p[i].pid);
        if (scanf("%d %d %d", &p[i].arrival, &p[i].burst, &p[i].priority) != 3 ||
            p[i].arrival < 0 || p[i].burst <= 0 || p[i].priority < 0) {
            printf("Invalid input: arrival >= 0, burst > 0, priority >= 0.\n");
            return 1;
        }
    }

    /*
     * Repeat until every process has run:
     *   1. Look at the processes that have arrived and are not done.
     *   2. Pick the one with the smallest priority number.
     *   3. Run it to completion.
     */
    while (completed < n) {
        int highest = -1;

        for (i = 0; i < n; i++) {
            if (done[i] || p[i].arrival > time) {
                continue;   /* not arrived yet, or already finished */
            }

            if (highest == -1 ||
                p[i].priority < p[highest].priority ||
                (p[i].priority == p[highest].priority &&
                 p[i].arrival < p[highest].arrival) ||
                (p[i].priority == p[highest].priority &&
                 p[i].arrival == p[highest].arrival &&
                 p[i].pid < p[highest].pid)) {
                highest = i;
            }
        }

        if (highest == -1) {
            /* CPU is idle: jump to the next arrival. */
            int next = -1;

            for (i = 0; i < n; i++) {
                if (!done[i] &&
                    (next == -1 || p[i].arrival < p[next].arrival)) {
                    next = i;
                }
            }

            time = p[next].arrival;
            continue;
        }

        time += p[highest].burst;
        p[highest].completion = time;
        p[highest].turnaround = p[highest].completion - p[highest].arrival;
        p[highest].waiting = p[highest].turnaround - p[highest].burst;

        done[highest] = 1;
        completed++;
    }

    printf("\nPriority (Non-Preemptive) Scheduling Result\n");
    printf("----------------------------------------------------------\n");
    printf("PID\tAT\tBT\tPR\tCT\tTAT\tWT\n");
    printf("----------------------------------------------------------\n");

    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].arrival, p[i].burst, p[i].priority,
               p[i].completion, p[i].turnaround, p[i].waiting);

        total_waiting += p[i].waiting;
        total_turnaround += p[i].turnaround;
    }

    printf("----------------------------------------------------------\n");
    printf("Average Waiting Time    = %.2f\n", total_waiting / n);
    printf("Average Turnaround Time = %.2f\n", total_turnaround / n);

    return 0;
}
