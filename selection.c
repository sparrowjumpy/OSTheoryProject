#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#define ARR_SIZE 1000

int arr[ARR_SIZE];

void ParseParameters(char *argv[])
{
  for (int i = 0, j = 1; j < ARR_SIZE + 1; i++, j++)
    sscanf(argv[j], "%d", &arr[i]);
}

void SelectionSort()
{
  int n = ARR_SIZE;
  int temp = 0;
  int i, j, min_idx;

  for (i = 0; i < n - 1; i++)
  {
    // Find the minimum element in unsorted array
    min_idx = i;
    for (j = i + 1; j < n; j++)
    {
      if (arr[j] < arr[min_idx])
      {
        min_idx = j;
      }
    }

    // Swap the found minimum element with the first element
    temp = arr[min_idx];
    arr[min_idx] = arr[i];
    arr[i] = temp;
  }
}

int main(int argc, char *argv[])
{
  FILE *fp;
  fp = fopen("SelectionFork.csv", "a");
  if(fp == NULL)
    printf("ERROR!\n");

  // fprintf(fp,"time,trial\n");
  int k;
  int id = getpid();
  double times[15];
  double timetaken1;
  clock_t start1 = clock();

  fork();
  fork();
  fork();
  if (getpid() == id)
  {
    clock_t end1 = clock();
    timetaken1 = (double)(end1 - start1) / CLOCKS_PER_SEC;
  }

  for (k = 0; k < 15; k++)
  {
    clock_t start = clock();
    ParseParameters(argv);
    SelectionSort();
    clock_t end = clock();

    double timetaken = ((double)(end - start) / CLOCKS_PER_SEC) + timetaken1;
    if (getpid() == id)
    {
      if(k==14)
        fprintf(fp,"%f,15", timetaken *8);
      else
        fprintf(fp, "%f,%d\n", (timetaken * 8),(k+1));
      
      printf("Time taken by 8 \x1b[1mPROCESSES\x1b[0m:\x1b[92m %f "
             "seconds\x1b[0m\n\n",
             (timetaken * 8));
    }
  }
  fclose(fp);
}
