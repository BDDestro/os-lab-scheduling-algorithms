/*
 * SRTF (Shortest Remaining Time First) CPU Scheduling
 * ----------------------------------------------------
 * SRTF is the preemptive version of SJF.
 * Rule: at every time unit, run the arrived process with the smallest
 *       remaining burst time. A new short process can preempt a running one.
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
    int remaining[MAX_PROCESSES];
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
        remaining[i] = p[i].burst;
    }

    /*
     * Repeat until every process is finished:
     *   1. Among arrived processes with work left, pick the smallest
     *      remaining time.
     *   2. Run it for exactly one time unit.
     *   3. If it reaches zero, record its completion time.
     */
    while (completed < n) {
        int shortest = -1;

        for (i = 0; i < n; i++) {
            if (p[i].arrival > time || remaining[i] == 0) {
                continue;   /* not arrived yet, or already finished */
            }

            if (shortest == -1 ||
                remaining[i] < remaining[shortest] ||
                (remaining[i] == remaining[shortest] &&
                 p[i].arrival < p[shortest].arrival) ||
                (remaining[i] == remaining[shortest] &&
                 p[i].arrival == p[shortest].arrival &&
                 p[i].pid < p[shortest].pid)) {
                shortest = i;
            }
        }

        if (shortest == -1) {
            /* CPU is idle: jump to the next arrival. */
            int next = -1;

            for (i = 0; i < n; i++) {
                if (remaining[i] > 0 &&
                    (next == -1 || p[i].arrival < p[next].arrival)) {
                    next = i;
                }
            }

            time = p[next].arrival;
            continue;
        }

        remaining[shortest]--;
        time++;

        if (remaining[shortest] == 0) {
            p[shortest].completion = time;
            p[shortest].turnaround = p[shortest].completion - p[shortest].arrival;
            p[shortest].waiting = p[shortest].turnaround - p[shortest].burst;
            completed++;
        }
    }

    printf("\nSRTF (Preemptive SJF) Scheduling Result\n");
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
