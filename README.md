# OS Lab: CPU Scheduling Algorithms

Five CPU scheduling algorithms implemented in C for an Operating Systems lab.
Each program is self-contained, reads its input from the keyboard (or a file),
prints a result table, and calculates the average waiting and turnaround times.

| # | Algorithm | File | Preemptive? | Next process is chosen by |
|---|-----------|------|-------------|---------------------------|
| 1 | First Come, First Served (FCFS) | `fcfs.c` | No | Earliest arrival time |
| 2 | Shortest Job First (SJF) | `sjf.c` | No | Smallest burst time among arrived processes |
| 3 | Shortest Remaining Time First (SRTF) | `srtf.c` | Yes | Smallest remaining time |
| 4 | Round Robin (RR) | `round_robin.c` | Yes (time slice) | FIFO ready queue + time quantum |
| 5 | Priority Scheduling | `priority.c` | No | Smallest priority number |

## Formulas

For every process:

```text
Turnaround Time (TAT) = Completion Time (CT) - Arrival Time (AT)
Waiting Time    (WT)  = Turnaround Time (TAT) - Burst Time (BT)
```

The programs print `CT`, `TAT`, and `WT` for each process, then the averages.

## Assumptions

- Arrival times are non-negative and burst times are positive.
- **Priority: a smaller number means higher priority.** Priority `1` runs before
  priority `3`. If your class uses the opposite convention, change `<` to `>` in
  the priority comparison in `priority.c`.
- SJF and Priority are **non-preemptive**. SRTF is the preemptive version of SJF.
- Round Robin uses a circular ready queue. Processes that arrive during a time
  slice are queued before the preempted process is put back in the queue.
- Ties are broken by earlier arrival time, then lower PID.
- Maximum 20 processes per run.

## Repository structure

```text
.
├── fcfs.c
├── sjf.c
├── srtf.c
├── round_robin.c
├── priority.c
├── build.bat            # Windows build script
├── Makefile             # Linux / macOS build
├── samples/             # sample input for each program
└── examples/            # output produced by the sample inputs
```



## Sample input

All five programs use the same five processes in the sample files:

```text
5
0 4
1 3
2 1
3 2
4 5
```

`round_robin.txt` adds a time quantum of `2` at the end. `priority.txt` adds a
priority to each line.

## Verified sample results

These are the actual outputs from the sample inputs (full transcripts are in
`examples/`).

### FCFS

```text
PID     AT      BT      CT      TAT     WT
P1      0       4       4       4       0
P2      1       3       7       6       3
P3      2       1       8       6       5
P4      3       2       10      7       5
P5      4       5       15      11      6

Average Waiting Time    = 3.80
Average Turnaround Time = 6.80
```

### SJF (non-preemptive)

```text
PID     AT      BT      CT      TAT     WT
P1      0       4       4       4       0
P2      1       3       10      9       6
P3      2       1       5       3       2
P4      3       2       7       4       2
P5      4       5       15      11      6

Average Waiting Time    = 3.20
Average Turnaround Time = 6.20
```

### SRTF

```text
PID     AT      BT      CT      TAT     WT
P1      0       4       5       5       1
P2      1       3       10      9       6
P3      2       1       3       1       0
P4      3       2       7       4       2
P5      4       5       15      11      6

Average Waiting Time    = 3.00
Average Turnaround Time = 6.00
```

### Round Robin (quantum = 2)

```text
PID     AT      BT      CT      TAT     WT
P1      0       4       7       7       3
P2      1       3       12      11      8
P3      2       1       5       3       2
P4      3       2       9       6       4
P5      4       5       15      11      6

Average Waiting Time    = 4.60
Average Turnaround Time = 7.60
```

### Priority (non-preemptive, lower number = higher priority)

```text
PID     AT      BT      PR      CT      TAT     WT
P1      0       4       2       4       4       0
P2      1       3       1       7       6       3
P3      2       1       4       10      8       7
P4      3       2       3       9       6       4
P5      4       5       5       15      11      6

Average Waiting Time    = 4.00
Average Turnaround Time = 7.00
```

### Comparison

| Algorithm | Average WT | Average TAT |
|-----------|-----------:|------------:|
| FCFS | 3.80 | 6.80 |
| SJF | 3.20 | 6.20 |
| SRTF | 3.00 | 6.00 |
| Round Robin (q = 2) | 4.60 | 7.60 |
| Priority | 4.00 | 7.00 |

## How each algorithm works

### FCFS

The processes are sorted by arrival time and run in that order. If the CPU is
idle, it waits until the next process arrives. FCFS is simple, but a long process
at the front can delay everyone behind it; this is called the **convoy effect**.

Gantt chart for the sample:

```text
| P1 | P2 | P3 | P4 | P5 |
0    4    7    8    10   15
```

### SJF

At each step, look only at processes that have already arrived, then pick the
one with the smallest burst time. It is non-preemptive, so the chosen process
runs to completion. SJF gives the minimum average waiting time when all burst
times are known in advance, but real operating systems usually cannot know them.

Gantt chart for the sample:

```text
| P1 | P3 | P4 | P2 | P5 |
0    4    5    7    10   15
```

### SRTF

SRTF is SJF with preemption. At every time unit, the scheduler compares the
remaining times of all arrived processes and runs the smallest one. A newly
arrived short process can interrupt the current process.

Gantt chart for the sample:

```text
| P1 | P3 | P1 | P4 | P2 | P5 |
0    2    3    5    7    10   15
```

### Round Robin

Ready processes wait in a FIFO queue. Each process gets at most one time quantum.
If it does not finish, it is placed at the back of the queue. New arrivals are
added before the preempted process, which is the usual convention.

Gantt chart for the sample (quantum = 2):

```text
| P1 | P2 | P3 | P1 | P4 | P5 | P2 | P5 | P5 |
0    2    4    5    7    9    11   12   14   15
```

A very small quantum causes many context switches; a very large quantum makes
Round Robin behave like FCFS.

### Priority

At each step, pick the arrived process with the smallest priority number. This
implementation is non-preemptive. Priority scheduling can starve low-priority
processes; **aging** (gradually increasing the priority of waiting processes)
is the standard fix.

Gantt chart for the sample:

```text
| P1 | P2 | P4 | P3 | P5 |
0    4    7    9    10   15
```

## Notes

- These programs use fixed-size arrays (`MAX_PROCESSES = 20`) to keep the code
  simple for a lab submission. No dynamic memory allocation is used.
- All arithmetic is integer except the final averages, which are `float`.
- Input is validated: the program exits with an error message on invalid values.
