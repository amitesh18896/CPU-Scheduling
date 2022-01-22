#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_PROCESS 100

int min(int a, int b)
{
    return (a<b ? a : b);
}


typedef struct{
    char name[10];
    int arrival;
    int cpburst;
    int turnaround;
    int wait;
    int flag;
}Process;

void b_sort(Process temp[],int n)
{
	Process t;
	int i,j;
	for(i=1;i<n;i++)
		for(j=0;j<n-i;j++){
			if(temp[j].arrival > temp[j+1].arrival){
				t = temp[j];
				temp[j] = temp[j+1];
				temp[j+1] = t;
			}
		}
}

int comparator(const void* p, const void* q){
    Process *a = (Process *)p;
    Process *b = (Process *)q;
    int cmp =  (a->arrival - b->arrival);
    if(cmp == 0)
        return(a->cpburst - b->cpburst);
    return cmp;
}

//function to return the minimum index of the process
int min_porcess(int p[], int n)
{
    int min = -1;
    int min_index=-1;
    for(int i=0; i<n; ++i)
    {
        if(p[i] == 0)
            continue;
        if(min == -1 || p[i] < min)
        {
            min = p[i];
            min_index = i;
        }
    }
    return min_index;
}
//To print out the statistics on the turnaround times and wait times
//This function  works independently on the scheduling algorithm used (FCFS or RR).
void printStatistics(Process p[], int n)
{
    float avgta=0, avgwt = 0;
    
    printf("\nTurnaround Times: ");
    for(int i=0; i<n; ++i)
    {
        printf("%s[%d] ", p[i].name, p[i].turnaround);
        avgta += p[i].turnaround;
    }
   
    printf("\nWait times: ");
    for(int i=0; i<n; ++i)
    {
        printf("%s[%d] ", p[i].name, p[i].wait);
        avgwt += p[i].wait;
    }
    printf("\n");
    //Counting average turaround and average wait time 
    avgta = avgta/n;
    avgwt = avgwt/n;

    printf("\nAverage turnaround time: %.2f", avgta);
    printf("\nAverage wait time: %.2f\n", avgwt);
    printf("\n");
    printf("Hit ENTER key to continue...");
}

void FCFS(Process p[], int n){
    printf("-------------------------------------------------\n");
    printf("      First Come First Served Scheduling\n");
    printf("-------------------------------------------------\n");
    int current_time = 0;
    //b_sort(p,n);
    qsort(p, n, sizeof(Process), comparator);

    for(int i=0; i<n; ++i)
    {
        //You will need to keep track of the current time by mecmp of a variable (global or local) 
        if(current_time >= p[i].arrival)
        {
            p[i].wait = current_time - p[i].arrival;
            p[i].turnaround = current_time + p[i].cpburst - p[i].arrival;
            current_time += p[i].cpburst;
        }
        else
        {
            p[i].turnaround = p[i].cpburst;
            p[i].wait = 0;
            current_time = p[i].arrival + p[i].cpburst;
        }
    }
   
    for(int i=0; i<n; ++i)
    {
        printf("[%d-%d]\t%s\trunning\n", p[i].arrival + p[i].wait, p[i].arrival + p[i].turnaround, p[i].name);
    }
    //using printStatistics for printing the stats
    printStatistics(p, n);
}

int cmpltprcess(int a[], int n)
{
    for(int i=0; i<n; ++i)
    {
        if(a[i] > 0)
            return 0;
    }
    return 1;
}

//Round Robin Process
void RR(Process p[], int n)
{
    //b_sort(p,n);
    qsort(p, n, sizeof(Process), comparator);
    int time, current_time = p[0].arrival;
    printf("Enter the time quantum:");
    scanf("%d", &time);

    int cpuburstcopy[n];
    for(int i=0; i<n; ++i)
    {
        cpuburstcopy[i] = p[i].cpburst;
    }
    printf("-------------------------------------------------\n");
    printf("            Round Robin Scheduling\n");
    printf("-------------------------------------------------\n");
    while(!cmpltprcess(cpuburstcopy, n))
    {
        int rounds = 0;
        for(int i=0; i<n; ++i)
        {
            if(cpuburstcopy[i] <= 0)
                continue;

            if(current_time >= p[i].arrival)
            {
                int t = min(time, cpuburstcopy[i]);
                p[i].wait += (current_time - p[i].turnaround + p[i].arrival);
                //printf("%d",p[i].wait);
                current_time += t;
                p[i].turnaround = current_time - p[i].arrival;
                cpuburstcopy[i] -= t;
                printf("[%d-%d]\t%s\trunning\n", p[i].arrival+p[i].turnaround-t, p[i].arrival + p[i].turnaround, p[i].name);    
            }
            else
            {
                i=-1;
            }
            
        }
    }
    //using printStatistics for printing the stats
    printStatistics(p, n);
}

void SRBF(Process P[],int n){
	int i,t_total=0,tcurr,b[MAX_PROCESS],min_at,j,x,min_bt;
	int sumw=0,sumt=0;
    int at=0;
	float avgwt=0.0,avgta=0.0;
	Process temp[MAX_PROCESS],t;
    printf("-----------------------------------------------\n");
    printf("      Shortest Remaining Burst First\n");
    printf("-----------------------------------------------\n");
	for(i=0;i<n;i++){
		temp[i]=P[i];
		t_total+=P[i].cpburst;
	}

	b_sort(temp,n);

	for(i=0;i<n;i++)
		b[i] = temp[i].cpburst;

	i=j=0;
	for(tcurr=0;tcurr<t_total;tcurr++){

		if(b[i] > 0 && temp[i].arrival <= tcurr)
			b[i]--;
        
        if(i!=j)
        {
			//printf(" %d %s",tcurr,temp[i].name);
            printf("[%d-%d]\t%s\trunning\n", at, tcurr, temp[j].name);
            at=tcurr;
        }
		

		if(b[i]<=0 && temp[i].flag != 1){
		
			temp[i].flag = 1;
			temp[i].wait = (tcurr+1) - temp[i].cpburst - temp[i].arrival;
			temp[i].turnaround = (tcurr+1) - temp[i].arrival;
             //printf("%d\n",temp[i].turnaround);
			sumw+=temp[i].wait;
			sumt+=temp[i].turnaround;
		}

        

		j=i;	min_bt = 999;
		for(x=0;x<n;x++){
		
			if(temp[x].arrival <= (tcurr+1) && temp[x].flag != 1){
			
				if(min_bt != b[x] && min_bt > b[x]){
					min_bt = b[x];
					i=x;
				}
			}
		}
		
	}
    printf("[%d-%d]\t%s\trunning\n", at, tcurr, temp[j].name);

	avgwt = (float)sumw/n;	avgta = (float)sumt/n;
	printf("\n");
    printf("\nAverage turnaround time: %.2f", avgta);
    printf("\nAverage wait time: %.2f\n", avgwt);
    printf("Hit ENTER key to continue...");
}


//reads the input file into a global data structure. This structure may be an array of processes
int ReadProcessTable(char *filename, Process p[])
{
     int arrival, cpburst;
     int index = 0;
     char processname[20];
    //read file
    FILE *file = fopen(filename, "r");
    if(file == NULL)
    {
        fprintf(stderr,"Error! Cannot open file\n");
        exit(1);
    }
    while(fscanf(file, "%s %d %d", processname, &arrival, &cpburst) != EOF)
    {
        strcpy(p[index].name, processname);
        p[index].wait = 0;
        p[index].turnaround = 0;
        p[index].arrival = arrival;
        p[index].cpburst = cpburst;
        ++index;
    }
    fclose(file);
    return index;
}

// that simply outputs the data stored in the global processtable structure filled in by the ReadProcessTable call. 
void printProcessTable(Process p[], int n)
{
    for(int i=0; i<n; ++i)
    {
        printf("%s  %d  %d\n", p[i].name, p[i].arrival, p[i].cpburst);
    }
}


int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "Error ! Less Number of Arguments\n");
        return 1;
    }
    Process processTable[MAX_PROCESS];
    int n = ReadProcessTable(argv[1], processTable);

    //simply outputs the data stored in the global processtable structure filled in by the ReadProcessTable call
    printProcessTable(processTable, n);

    while(1)
    {
        printf("-------------------------------------------------\n");
        printf("           CPU Scheduling Simulation\n");
        printf("-------------------------------------------------\n");
        printf("Select the scheduling algorithm [1,2,3 or 4]:\n");
        printf("1. First Come First Served (FCFS)\n2. Round Robin (RR)\n3. SRBF\n4. Exit\n\n");

        int ch;
        scanf("%d", &ch);

        switch (ch)
        {
        case 1:
            FCFS(processTable, n);
            break;
        case 2:
            RR(processTable, n);
            break;
        case 3:
            SRBF(processTable, n);
            break;
        case 4:
            exit(0);
            break;
        default:
            printf("Error ! Enter value in range 1-4 \n");
            break;
        }
        getchar();
        getchar();
    }
}