/*
 * Round Robin (RR) CPU Scheduling
 * --------------------------------
 * Rule: ready processes are kept in a FIFO queue. Each process runs for
 *       at most one time quantum. If it is not finished, it goes to the
 *       back of the queue.
 * Type: preemptive (time-slice based).
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
    int queue[MAX_PROCESSES];
    int n, quantum, i, j;
    int time = 0;
    int completed = 0;
    int next = 0;                 /* next process to arrive (after sorting) */
    int head = 0, tail = 0;       /* circular queue pointers */
    int count = 0;                /* number of processes in the queue */
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

    printf("Enter time quantum: ");
    if (scanf("%d", &quantum) != 1 || quantum <= 0) {
        printf("Invalid time quantum: it must be > 0.\n");
        return 1;
    }

    /*
     * Sort by arrival time so processes enter the ready queue in the
     * correct order. Ties are broken by lower PID.
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

    for (i = 0; i < n; i++) {
        remaining[i] = p[i].burst;
    }

    while (completed < n) {
        /*
         * If the queue is empty, the CPU is idle. Jump to the next
         * arrival and put every process that has arrived into the queue.
         */
        if (count == 0) {
            if (time < p[next].arrival) {
                time = p[next].arrival;
            }

            while (next < n && p[next].arrival <= time) {
                queue[tail] = next;
                tail = (tail + 1) % n;
                count++;
                next++;
            }
        }

        /* Take the process at the front of the queue. */
        {
            int current = queue[head];
            int slice;

            head = (head + 1) % n;
            count--;

            /* Run for one quantum, or less if the process finishes first. */
            slice = (remaining[current] < quantum) ? remaining[current] : quantum;
            time += slice;
            remaining[current] -= slice;

            /*
             * Processes that arrived during this slice join the queue
             * before the preempted process is put back.
             */
            while (next < n && p[next].arrival <= time) {
                queue[tail] = next;
                tail = (tail + 1) % n;
                count++;
                next++;
            }

            if (remaining[current] > 0) {
                queue[tail] = current;
                tail = (tail + 1) % n;
                count++;
            } else {
                p[current].completion = time;
                p[current].turnaround = p[current].completion - p[current].arrival;
                p[current].waiting = p[current].turnaround - p[current].burst;
                completed++;
            }
        }
    }

    printf("\nRound Robin Scheduling Result (Quantum = %d)\n", quantum);
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
