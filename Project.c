#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <sys/types.h> 
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define ARR_SIZE 1000 //we are taking an array of size 1000 always
int arr[ARR_SIZE];


//elementart sorting algorithms in their respective functions
void BubbleSort()
{
	int temp = 0;
	int i, j;
	int temparr[ARR_SIZE];

	//temp arr here so each thread dosent recieve an "ALREADY SORTED" array
	//and it dosent disturb the global array
	for (i = 0; i < ARR_SIZE; i++)
	{
		temparr[i] = arr[i];
	}

	for (i = 0; i < ARR_SIZE - 1; i++)
	{
		for (j = 0; j < ARR_SIZE - i - 1; j++)
		{
			if (temparr[j] > temparr[j + 1])
			{
				temp = temparr[j];
				temparr[j] = temparr[j + 1];
				temparr[j + 1] = temp;
			}
		}
	}
}

void SelectionSort()
{
	int n = ARR_SIZE;
	int temp = 0;
	int i, j, min_idx;
	int temparr[ARR_SIZE];

	for (i = 0; i < ARR_SIZE; i++)
	{
		temparr[i] = arr[i];
	}

	for (i = 0; i < n - 1; i++)
	{
		
		min_idx = i;
		for (j = i + 1; j < n; j++)
		{
			if (temparr[j] < temparr[min_idx])
			{
				min_idx = j;
			}
		}

		temp = temparr[min_idx];
		temparr[min_idx] = temparr[i];
		temparr[i] = temp;
	}
}

int ShellSort()
{
	int n = ARR_SIZE;
	int temp = 0;
	int i, j, key;
	int temparr[ARR_SIZE];

	for (i = 0; i < ARR_SIZE; i++)
	{
		temparr[i] = arr[i];
	}

	for (int gap = n / 2; gap > 0; gap /= 2)
	{

		for (int i = gap; i < n; i += 1)
		{

			int temp = temparr[i];

			int j;
			for (j = i; j >= gap && temparr[j - gap] > temp; j -= gap)
				temparr[j] = temparr[j - gap];

			temparr[j] = temp;
		}
	}
	return 0;
}

void InsertionSort()
{
	int n = ARR_SIZE;
	int temp = 0;
	int i, j, key;
	int temparr[ARR_SIZE];

	for (i = 0; i < ARR_SIZE; i++)
	{
		temparr[i] = arr[i];
	}

	for (i = 1; i < n; i++)
	{
		key = temparr[i];
		j = i - 1;


		while (j >= 0 && temparr[j] > key)
		{
			temparr[j + 1] = temparr[j];
			j = j - 1;
		}
		temparr[j + 1] = key;
	}
}

int getNextGap(int gap)
{
	gap = (gap * 10) / 13;

	if (gap < 1)
		return 1;
	return gap;
}

void CombSort()
{
	int n = ARR_SIZE;

	int gap = n;
	int i, temparr[ARR_SIZE], temp;

	for (i = 0; i < ARR_SIZE; i++)
	{
		temparr[i] = arr[i];
	}

	int swapped = 1;

	while (gap != 1 || swapped == 1)
	{

		gap = getNextGap(gap);

		swapped = 0;

		for (i = 0; i < n - gap; i++)
		{
			if (temparr[i] > temparr[i + gap])
			{

				temp = temparr[i];
				temparr[i] = temparr[i + gap];
				temparr[i + gap] = temp;

				swapped = 1;
			}
		}
	}
}

//filing function that initializes file and adds the axis headings needed for later
//in python plotly library
//filingUwU is more for the forking program than the threading one
void filingUwU(char arr[50])
{
	FILE *fio;
	fio = fopen(arr, "w");
	fprintf(fio, "time,trial\n");
	fclose(fio);
	
}


int main()
{
	FILE *fp;

	int i, k, ch;
	
	//initializing the array with random values ranging between
	//0 and 10,000
	srand(time(0));
	for (i = 0; i < ARR_SIZE; i++)
	{
		arr[i] = rand() % 10000;
	}

	// Create a dynamic string array to pass data to other files
	char **strArr = calloc(ARR_SIZE + 2, sizeof(char *));
	for (int i = 0; i < ARR_SIZE + 2; i++)
		strArr[i] = calloc(50, sizeof(char));	//strArr contains our filename, array and NULL
		//these are needed when passing parameters to execvp later on

	// Convert the data to string and populate the string array
	for (int i = 1; i < ARR_SIZE + 1; i++)
		sprintf(strArr[i], "%d", arr[i]);

	strArr[ARR_SIZE + 1] = NULL; // Last index of string array must point to NULL

	pthread_t t[8]; //creating an array of 8 threads

	//menu type program for user to select which type of sorting to run
	printf("Enter which sorting algorithm you would like to utilize through "
		   "Multithreading and Forking: \n");
	printf("1. Bubble Sort\n");
	printf("2. Selection Sort\n");
	printf("3. Insertion Sort\n");
	printf("4. Shell Sort\n");
	printf("5. Comb Sort\n");

	printf("\nEnter choice: ");
	scanf("%d", &ch);

	if (ch == 1)
	{
		//creating a csv file for BubbleSort done in forking
		filingUwU("BubbleFork.csv");
	
		printf("\n\n\x1b[94m~~~~~~~~~BUBBLE SORT~~~~~~~~~~\x1b[0m");
		
		//creating a csv file for BubbleSort done in threading
		//we have not used the filingUwU here because we would need to open it again anyways
		//cuz filingUwU closes the file	
		fp = fopen("BubbleThread.csv", "w");
		fprintf(fp,"time,trial\n");
	
		//for 15 different number of trials we will create and join 8 different threads
		for (k = 0; k < 15; k++)
		{
			//starting the clock
			clock_t start = clock();
			for (i = 0; i < 8; i++)
				pthread_create(&t[i], NULL, (void *)&BubbleSort, NULL);

			for (i = 0; i < 8; i++)
				pthread_join(t[i], NULL);
			
			//ending the clock
			clock_t end = clock();
	
			double timetaken = (double)(end - start) / CLOCKS_PER_SEC;
			if (k == 14)
				fprintf(fp, "%f,15", timetaken);
			else
				fprintf(fp, "%f,%d\n", timetaken,k+1);

			printf(
				"\nTime taken by 8 \x1b[1mTHREADS\x1b[0m:\x1b[92m %f seconds\x1b[0m\n",
				timetaken);
		}
		fclose(fp);

		// First index of string array must be the file name to execute
		strcpy(strArr[0], "./bubble");
		
		//and now after achieving results of threading, we will send the array
		//to a forking program to get results of processes.
		if (execvp("./bubble", strArr) == -1)
			printf("Error!\n");
	}

	else if (ch == 2)
	{
		filingUwU("SelectionFork.csv");
		printf("\n\n\x1b[94m~~~~~~~~~SELECTION SORT~~~~~~~~~~\x1b[0m");
		fp = fopen("SelectionThread.csv", "w");
		fprintf(fp,"time,trial\n");

		for (k = 0; k < 15; k++)
		{

			clock_t start = clock();
			for (i = 0; i < 8; i++)
				pthread_create(&t[i], NULL, (void *)&SelectionSort, NULL);

			for (i = 0; i < 8; i++)
				pthread_join(t[i], NULL);

			clock_t end = clock();

			double timetaken = (double)(end - start) / CLOCKS_PER_SEC;
			if (k == 14)
				fprintf(fp, "%f,15", timetaken);
			else
				fprintf(fp, "%f,%d\n", timetaken,k+1);

			printf(
				"\nTime taken by 8 \x1b[1mTHREADS\x1b[0m:\x1b[92m %f seconds\x1b[0m\n",
				timetaken);
		}
		fclose(fp);
		// First index of string array must be the file name to execute
		strcpy(strArr[0], "./selection");
		if (execvp("./selection", strArr) == -1)
			printf("Error!\n");
	}

	else if (ch == 3)
	{
		filingUwU("InsertionFork.csv");
		printf("\n\n\x1b[94m~~~~~~~~~INSERTION SORT~~~~~~~~~~\x1b[0m");
		fp = fopen("InsertionThread.csv", "w");
		fprintf(fp,"time,trial\n");
		
		for (k = 0; k < 15; k++)
		{
			clock_t start = clock();
			for (i = 0; i < 8; i++)
				pthread_create(&t[i], NULL, (void *)&InsertionSort, NULL);

			for (i = 0; i < 8; i++)
				pthread_join(t[i], NULL);

			clock_t end = clock();
			double timetaken = (double)(end - start) / CLOCKS_PER_SEC;
			if (k == 14)
				fprintf(fp, "%f,15", timetaken);
			else
				fprintf(fp, "%f,%d\n", timetaken,k+1);

			printf(
				"\nTime taken by 8 \x1b[1mTHREADS\x1b[0m:\x1b[92m %f seconds\x1b[0m\n",
				timetaken);
		}
		fclose(fp);

		// First index of string array must be the file name to execute
		strcpy(strArr[0], "./insertion");
		if (execvp("./insertion", strArr) == -1)
			printf("Error!\n");
	}

	else if (ch == 4)
	{
		filingUwU("ShellFork.csv");
		printf("\n\n\x1b[94m~~~~~~~~~SHELL  SORT~~~~~~~~~~\x1b[0m");
		fp = fopen("ShellThread.csv", "w");
		fprintf(fp,"time,trial\n");

		for (k = 0; k < 15; k++)
		{

			clock_t start = clock();
			for (i = 0; i < 8; i++)
				pthread_create(&t[i], NULL, (void *)&ShellSort, NULL);

			for (i = 0; i < 8; i++)
				pthread_join(t[i], NULL);

			clock_t end = clock();

			double timetaken = (double)(end - start) / CLOCKS_PER_SEC;
			if (k == 14)
				fprintf(fp, "%f,15", timetaken);
			else
				fprintf(fp, "%f,%d\n", timetaken,k+1);

			printf(
				"\nTime taken by 8 \x1b[1mTHREADS\x1b[0m:\x1b[92m %f seconds\x1b[0m\n",
				timetaken);
		}
		fclose(fp);

		// First index of string array must be the file name to execute
		strcpy(strArr[0], "./shell");
		if (execvp("./shell", strArr) == -1)
			printf("Error!\n");
	}
	else if (ch == 5)
	{
		filingUwU("CombFork.csv");
		printf("\n\n\x1b[94m~~~~~~~~~COMB SORT~~~~~~~~~~\x1b[0m");
		fp = fopen("CombThread.csv", "w");
		fprintf(fp,"time,trial\n");

		for (k = 0; k < 15; k++)
		{

			clock_t start = clock();
			for (i = 0; i < 8; i++)
				pthread_create(&t[i], NULL, (void *)&CombSort, NULL);

			for (i = 0; i < 8; i++)
				pthread_join(t[i], NULL);

			clock_t end = clock();

			double timetaken = (double)(end - start) / CLOCKS_PER_SEC;
			if (k == 14)
				fprintf(fp, "%f,15", timetaken);
			else
				fprintf(fp, "%f,%d\n", timetaken,k+1);

			printf(
				"\nTime taken by 8 \x1b[1mTHREADS\x1b[0m:\x1b[92m %f seconds\x1b[0m\n",
				timetaken);
		}
		fclose(fp);

		// First index of string array must be the file name to execute
		strcpy(strArr[0], "./comb");
		if (execvp("./comb", strArr) == -1)
			printf("Error!\n");
	}

	else
	{
		printf("INVALID ENTRY!");
	}

	return 0;
}
