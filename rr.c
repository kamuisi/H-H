#include <stdio.h>
#include <stdlib.h>
#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3
typedef struct
{
    int iPID;
    int iArrival, iBurst;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;
void inputProcess(int n, PCB P[])
{
    printf("Nhap thong tin process theo trinh tu sau: PID Arrival Burst\n");
    for (int i = 0; i < n; i++)
    {
        printf("Nhap thong tin process thu %d: ", i + 1);
        scanf("%d\n%d\n%d", &P[i].iPID, &P[i].iArrival, &P[i].iBurst);
    }
}
void printProcess(int n, PCB P[])
{
    if (n == 1)
    {
        printf("\n%-20s %-20s %-20s %-20s %-20s %-20s", "PID", "Start time", "Finish time", "Waiting time", "Response time", "Turnaround time");
    }
    for (int i = 0; i < n; i++)
    {
        printf("\n%-20d %-20d %-20d %-20d %-20d %-20d", P[i].iPID, P[i].iStart, P[i].iFinish, P[i].iWaiting, P[i].iResponse, P[i].iTaT);
    }
}
void exportGanttChart(int n, PCB P[], int Root)
{
    int count = Root;
    printf("|");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < P[i].iBurst / 2; j++)
            printf(" ");
        printf("%d", P[i].iPID);
        for (int j = 0; j < P[i].iBurst / 2; j++)
            printf(" ");
        printf("|");
    }
    printf("\n%d", Root);
    for (int i = 0; i < n; i++)
    {
        count += P[i].iBurst;
        for (int j = 0; j <= P[i].iBurst; j++)
            printf(" ");
        if (count > 9)
            printf("\b");
        if (P[i].iBurst % 2 != 0)
            printf("\b");
        printf("%d", count);
    }
    printf("\n");
}
void pushProcess(int *n, PCB P[], PCB Q)
{
    P[*n] = Q;
    *n = *n + 1;
}
void removeProcess(int *n, int index, PCB P[])
{
    PCB a = {0};
    P[index] = a;
    *n = *n - 1;
    for (int i = index; i < *n; i++)
    {
        P[i] = P[i + 1];
    }
}
int swapProcess(PCB *P, PCB *Q)
{
    PCB A = *P;
    *P = *Q;
    *Q = A;
    return 0;
}
int partition(PCB P[], int low, int high, int iCriteria)
{
    int i = low - 1;
    switch (iCriteria)
    {
    case SORT_BY_ARRIVAL:
    {
        int pivot = P[high].iArrival;
        for (int j = low; j <= high; j++)
        {
            if (P[j].iArrival < pivot)
            {
                i++;
                swapProcess(&P[i], &P[j]);
            }
        }
        swapProcess(&P[i + 1], &P[high]);
        break;
    }
    case SORT_BY_BURST:
    {
        int pivot = P[high].iBurst;
        for (int j = low; j <= high; j++)
        {
            if (P[j].iBurst < pivot)
            {
                i++;
                swapProcess(&P[i], &P[j]);
            }
        }
        swapProcess(&P[i + 1], &P[high]);
        break;
    }
    case SORT_BY_PID:
    {
        int pivot = P[high].iPID;
        for (int j = low; j <= high; j++)
        {
            if (P[j].iPID < pivot)
            {
                i++;
                swapProcess(&P[i], &P[j]);
            }
        }
        swapProcess(&P[i + 1], &P[high]);
        break;
    }
    case SORT_BY_START:
    {
        int pivot = P[high].iStart;
        for (int j = low; j <= high; j++)
        {
            if (P[j].iStart < pivot)
            {
                i++;
                swapProcess(&P[i], &P[j]);
            }
        }
        swapProcess(&P[i + 1], &P[high]);
        break;
    }
    }
    return (i + 1);
}
void quickSort(PCB P[], int low, int high, int iCriteria)
{
    if (low < high)
    {
        int pi = partition(P, low, high, iCriteria);
        quickSort(P, low, pi - 1, iCriteria);
        quickSort(P, pi + 1, high, iCriteria);
    }
}
void calculateAWT(int n, PCB P[])
{
    float result = 0;
    int count_id = 1;
    int id = P[0].iPID;
    for (int i = 0; i < n; i++)
    {
        result += P[i].iWaiting;
        if (id != P[i].iPID)
        {
            count_id++;
            id = P[i].iPID;
        }
    }
    result /= count_id;
    printf("Average Waiting time: %f\n", result);
}
void calculateATaT(int n, PCB P[])
{
    float result = 0;
    int count_id = 1;
    int id = P[0].iPID;
    for (int i = 0; i < n; i++)
    {
        result += P[i].iTaT;
        if (id != P[i].iPID)
        {
            count_id++;
            id = P[i].iPID;
        }
    }
    result /= count_id;
    printf("Average Turnaround time: %f\n", result);
}
int main()
{
    PCB Input[10];
    PCB ReadyQueue[10];
    PCB TerminatedArray[10];
    int iNumberOfProcess;
    int RR;
    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);
    printf("Please input RR: ");
    scanf("%d", &RR);
    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0;
    inputProcess(iNumberOfProcess, Input);
    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);
    int Root = Input[0].iArrival;
    pushProcess(&iReady, ReadyQueue, Input[0]);
    removeProcess(&iRemain, 0, Input);
    ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
    ReadyQueue[0].iResponse = 0;
    printf("\nReady queue");
    printProcess(1, ReadyQueue);
    int Time_Passed = ReadyQueue[0].iArrival;

    while (iReady > 0 || iRemain > 0)
    {
        if (iRemain > 0)
        {
            while (iRemain > 0 && Input[0].iArrival <= Time_Passed + RR)
            {
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);
            }
        }
        if (iReady > 0)
        {
            pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);
            TerminatedArray[iTerminated - 1].iStart = Time_Passed;
            if (TerminatedArray[iTerminated - 1].iBurst > RR)
            {
                TerminatedArray[iTerminated - 1].iBurst = RR;
                ReadyQueue[0].iArrival = RR + Time_Passed;
                ReadyQueue[0].iBurst -= RR;
                pushProcess(&iReady, ReadyQueue, ReadyQueue[0]);
            }
            TerminatedArray[iTerminated - 1].iFinish = TerminatedArray[iTerminated - 1].iBurst + Time_Passed;
            Time_Passed += TerminatedArray[iTerminated - 1].iBurst;
            TerminatedArray[iTerminated - 1].iResponse = TerminatedArray[iTerminated - 1].iStart - TerminatedArray[iTerminated - 1].iArrival;
            TerminatedArray[iTerminated - 1].iWaiting = TerminatedArray[iTerminated - 1].iResponse;
            TerminatedArray[iTerminated - 1].iTaT = TerminatedArray[iTerminated - 1].iFinish - TerminatedArray[iTerminated - 1].iArrival;
            removeProcess(&iReady, 0, ReadyQueue);
        }
    }

    printf("\n===== RR Scheduling =====\n");
    exportGanttChart(iTerminated, TerminatedArray, Root);
    quickSort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID);
    calculateAWT(iTerminated, TerminatedArray);
    calculateATaT(iTerminated, TerminatedArray);
    return 0;
}