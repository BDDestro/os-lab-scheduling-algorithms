/*
 * SJF (Shortest Job First) CPU Scheduling - Non-Preemptive
 * ---------------------------------------------------------
 * Rule: among the processes that have already arrived, run the one
 *       with the smallest burst time.
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
        printf("Enter arrival time and burst time for P%d: ", p[i].pid);
        if (scanf("%d %d", &p[i].arrival, &p[i].burst) != 2 ||
            p[i].arrival < 0 || p[i].burst <= 0) {
            printf("Invalid input: arrival must be >= 0 and burst must be > 0.\n");
            return 1;
        }
    }

    /*
     * Repeat until every process has run:
     *   1. Look at the processes that have arrived and are not done.
     *   2. Pick the one with the smallest burst time.
     *   3. Run it to completion.
     */
    while (completed < n) {
        int shortest = -1;

        for (i = 0; i < n; i++) {
            if (done[i] || p[i].arrival > time) {
                continue;   /* not arrived yet, or already finished */
            }

            if (shortest == -1 ||
                p[i].burst < p[shortest].burst ||
                (p[i].burst == p[shortest].burst &&
                 p[i].arrival < p[shortest].arrival) ||
                (p[i].burst == p[shortest].burst &&
                 p[i].arrival == p[shortest].arrival &&
                 p[i].pid < p[shortest].pid)) {
                shortest = i;
            }
        }

        if (shortest == -1) {
            /*
             * No process is ready. Jump to the next arrival instead of
             * increasing time one unit at a time.
             */
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

        time += p[shortest].burst;
        p[shortest].completion = time;
        p[shortest].turnaround = p[shortest].completion - p[shortest].arrival;
        p[shortest].waiting = p[shortest].turnaround - p[shortest].burst;

        done[shortest] = 1;
        completed++;
    }

    printf("\nSJF (Non-Preemptive) Scheduling Result\n");
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
