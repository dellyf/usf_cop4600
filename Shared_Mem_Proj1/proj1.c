/* Operating Systems Project 1: Shared Memory
Name & UID: Delroy Fong  U94257843
Username?: dfong

Project Objective: Introduction to shared memory & problems 
		 		   that may occur if shared memory is not 
		 		   protected adequately
*/

//header files to be used
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

/* key number to identify shared memory segments*/
#define SHMKEY ((key_t) 3987)

/* struct for shared memory value */
typedef struct
{
  int value;
} shared_mem;

shared_mem *total;

//creates first of four processes to be created
//increments shared memory value to 100,000
void process1(){
	int pr = 0;
	for (pr = 0; pr<100000; pr++){
		total->value = total->value + 1;
	}
	printf("\nProcess 1: counter = %d", total->value);
}

//creates second of four processes to be created
//increments shared memory value to 200,000
void process2(){
	int pr = 0;
	for (pr = 0; pr<200000; pr++){
		total->value = total->value + 1;
	}
	printf("\nProcess 2: counter = %d", total->value);
}

//creates third of four processes to be created
//increments shared memory value to 300,000
void process3(){
	int pr = 0;
	for (pr = 0; pr<300000; pr++){
		total->value = total->value + 1;
	}
	printf("\nProcess 3: counter = %d", total->value);
}

//creates fourth of four processes to be created
//increments shared memory value to 500,000
void process4(){
	int pr = 0;
	for (pr = 0; pr<500000; pr++){
		total->value = total->value + 1;
	}
	printf("\nProcess 4: counter = %d", total->value);
}


int main()
{
	int  shmid, pid1,pid2, pid3,pid4, ID, status;
	char *shmadd;
	shmadd = (char *) 0;

	// creating & connecting to a shared memory segment 
	if ((shmid = shmget (SHMKEY, sizeof(int), IPC_CREAT | 0666)) < 0){
	     	perror ("shmget");
      		exit (1);     
	}
 	if ((total = (shared_mem *) shmat (shmid, shmadd, 0)) == (shared_mem*)-1){
   	   perror ("shmat");
      	   exit (0);
    	}
    
    // initializing shared memory value to 0
    total->value = 0;

    // creating processes
    if((pid1 = fork()) == 0){
		process1();
	}
    else if((pid1 != 0) && ((pid2 = fork()) == 0)){
		process2();
	}
	else if((pid1 != 0) && (pid2 != 0) && ((pid3 = fork()) == 0)){
		process3();
	}
	else if((pid1 != 0) && (pid2 != 0) && (pid3 != 0) && ((pid4 = fork()) == 0)){
		process4();
	}	

	// parent wait for child processes to finish
	if((pid1 != 0) && (pid2 != 0) && (pid3 != 0) && (pid4 != 0)){
		while((wait(NULL)) != -1);

		if((shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0)) == -1){
			perror("shmctl");
			exit(-1);
		}
		printf("\n\n");
		// print ID of each child
		printf("Child with ID: %d has just exited\n", pid1);
        printf("Child with ID: %d has just exited\n", pid2);
        printf("Child with ID: %d has just exited\n", pid3);
        printf("Child with ID: %d has just exited\n", pid4);
		printf("\nEnd of Simulation\n");

		// detaching shared memory
		if(shmdt(total)==-1){
    			perror ("shmdt");
      			exit (-1);
    		}

    	// removing shared memory
    	shmdt((void *) total);
    	shmctl(shmid, IPC_RMID, NULL);
	}
	return 0;	
}
