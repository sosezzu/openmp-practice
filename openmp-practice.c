#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <string.h>

//globals
int N;				//upper bound from input
int t;				//number threads
int floorVal;		//floor((N+1)/2)
int *allNums;		//all numbers 2-N

int main(int argc, char *argv[])
{
    double tstart = 0.0, tend= 0.0, ttaken;

 	//GETING INPUT // INITIALIZING VARS
  	N = atoi(argv[1]);
  	t = atoi(argv[2]);
  	int i = 0;
  	int floorVal = (N+1)/2;
  	allNums = (int *) malloc((N - 1) * sizeof(int));
	if(!allNums)
	{
		printf("Cannot allocate allNums!\n");
		exit(1);
	}

	//GENERATING PRIMES
  	tstart = omp_get_wtime(); 

  	//1. Generate all numbers from 2 to N.
  	int counter = 0;
	for(i = 2; i <= N; i++)
	{
		allNums[counter] = i;
		counter++;
	}

	//2. Crossing out all multiples of the next prime number
    for (i = 2; i < N; i++)
    {
    	//next prime number
        if (allNums[i] != 0)
        {
            int j;
            # pragma omp parallel num_threads(t)
            {
                #pragma omp parallel for
                //crosses out multiples
                for (j = (i + i); j < N; j += i)
                {
                    allNums[j] = 0;
                }
            }
        }
    }

    ttaken = omp_get_wtime() - tstart;
    printf("Time taken for the main part: %f\n", ttaken);

  //OUTPUT
  char output[11] = "";
  sprintf(output,"%d.txt", N);
	FILE * fp = fopen(output, "w");
	if(!fp)
  	{
    	printf("Cannot create the file %s\n", output);
    	exit(1);
  	}
  	int prevPrime = 2;
    counter = 1;
    for (i = 0; i < (N - 1); i++)
    {
    	if(allNums[i]!= 0)
    	{
    		fprintf(fp, "%d %d %d\n", counter, allNums[i], allNums[i] - prevPrime);
    		prevPrime = allNums[i];
        counter++;
    	}
    }
    fclose(fp);
    free(allNums);
}








