#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#define ARR_SIZE 1000

int arr[ARR_SIZE];	//creating global array 

//HERE the parameter of func converts the elements which it recieved as string form
//from previous file into integer type
void ParseParameters(char *argv[])
{
  for (int i = 0, j = 1; j < ARR_SIZE + 1; i++, j++)
    sscanf(argv[j], "%d", &arr[i]);
}

//same sorting algorithm as in previous/main file
void BubbleSort()
{
  int temp;
  for (int i = 0; i < ARR_SIZE; i++)
  {
    for (int j = 0; j < ARR_SIZE - i - 1; j++)
    {
      if (arr[j] > arr[j + 1])
      {
        temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}

int main(int argc, char *argv[])
{
  FILE *fp;
  
  //THIS TIME we open the file in append mode rather than write
  //because we want to append the results for 15 trials
  fp = fopen("BubbleFork.csv", "a");
  if(fp == NULL)
    printf("ERROR!\n");

  int k;
  int id = getpid();
  
  double timetaken1;
  //the clock starts here
  clock_t start1 = clock();

  //running 3 fork statements will create 2^3=8 processes, this is y we used 8 threads aswell
  fork();
  fork();
  fork();
  
  //the clock stop for parent process BECAUSE
  //we need the time taken for process CREATION, and we are assumign here all processes
  //take the same time for creation so we will just add it everytime our process runs the algo
  if (getpid() == id)
  {
    clock_t end1 = clock();
    timetaken1 = (double)(end1 - start1) / CLOCKS_PER_SEC;
  }

  //similarly 15 trials here aswell
  for (k = 0; k < 15; k++)
  {
    clock_t start = clock();
    
    //we parse the parameters EVERYTIME SO EACH PROCESS DOSENT RECIEVE AN "ALREADY SORTED" ARRAY
    ParseParameters(argv);
    BubbleSort();
    
    //stopping the clock once the process has finished sorting
    clock_t end = clock();

	//here we add the time taken for PROCESS CREATIONS aswell BECAUSE in threading we included
	//the time taken for THREAD CREATION aswell.
    double timetaken = ((double)(end - start) / CLOCKS_PER_SEC) + timetaken1;
    
	
	if (getpid() == id)
    {
      if(k==14)
        fprintf(fp,"%f,15", timetaken *8);
      else
        fprintf(fp, "%f,%d\n", (timetaken * 8),(k+1));
      
      //here we print the time taken for 8 different processes 
      printf("Time taken by 8 \x1b[1mPROCESSES\x1b[0m:\x1b[92m %f "
             "seconds\x1b[0m\n\n",
             (timetaken * 8));
    }
  }
  
  //and finally close the file
  fclose(fp);
}
