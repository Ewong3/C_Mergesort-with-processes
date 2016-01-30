/**  Cmput379 Lec B1

 *  ID:   1388761

 *  CCID: eric3

 *  Assignment 1 Mergesort.c due on 2016, Jan. 29.

 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[]){
  if (merge_sort_init(argc, argv) == 0) {
    return 0;
  } else {
    return 1;}
}
// Reads file and allocates shared memory
int merge_sort_init(int argc, char *argv[]) {
  if( argc == 2) {         	// If the filename argument is given
    printf("\nSorting file: %s\n",argv[1]);
    int count = 0;
    
    int *array;
    array = (int *) malloc(1*sizeof(int));

    FILE *fp;
    fp = fopen(argv[1], "r");

    if (fp == NULL) {     				// Failed file open
      printf("Error opening the file\n");
      return 1;

    } else {            				// Successful file open
      while(fscanf(fp, "%d", &array[count]) != EOF) {
        count++;
        array = (int *) realloc(array,(count + 1) * (sizeof(int)));
      }
      fclose(fp);
    }

    printf("%d elements read\n\n", count);		// If no integers are in the file
    if (count == 0) {
        free(array);
    	return 0;
    }

    int shmid;						// Allocate Shared Memory
    int *shmptr;
    shmid = shmget(IPC_PRIVATE, count *(sizeof(int)),0600);
    if (shmid < 0) {					
        printf("shmget error");
	return 1;
    }

    shmptr = shmat(shmid, 0, 0);			//  Shared Memory pointer
    if (shmptr == (void *)-1) {
	printf("shmat error");
	return 1;
    }
    int i;						// Copy local array to Shared Memory
    for (i = 0; i < count; i++) {
	shmptr[i] = array[i];
    }
    
    free(array);

    
    printf("Input Numbers:\n");
    int k;
    for (k = 0; k < count; k++) {       	 	// Read numbers before sort
      printf("%d ", shmptr[k]);
    }
    printf("\n\n");

    
    if (merge_sort(shmptr, 0, count - 1) == 1) {     	// Call recursive merge_sort
    	printf("Merge sort error");
	return 1;
    }

    printf("Sorted Numbers:\n");
    for (k = 0; k < count; k++) {      		     	// Read numbers after sort
      printf("%d ", shmptr[k]);
    }
    printf("\n\n");

    shmdt(&shmid);
    
  } else {               			    	// If the filename argument is not given
    printf("This program requires one filename argument\n");
    return 1;
  }
  
  return 0;
}


// The merge_sort function will take the array and create a process for each half of the array
int merge_sort(int *A, int l_bound, int u_bound) {
  if (l_bound == u_bound){				// Stop when the array holds 1 element
    return 0;
  }
  
  int status = 0;
  pid_t pid;
  int mid = ((l_bound + u_bound) / 2);
  pid = fork();

  if (pid < 0) {                       		    	// Fork error
    printf("Forking Error");
    return 1;

  } else if (pid == 0) {               			// Child Process
    merge_sort(A, l_bound, mid);
    exit(0);
    
  } else {                            			// Parent Process
    merge_sort(A, mid + 1, u_bound);

    while (waitpid(pid , &status, 0) > 0) {   		// Wait for child process to finish before merge
      merge(A, l_bound, mid, u_bound);
    } 
  }
  
  return 0;
}


// The merge function will merge two sections of the array while sorting
int merge(int *A, int l_bound, int mid, int u_bound){
  int *B, *C;						// Allocate memory for temporary arrays
  B = (int *) malloc(sizeof(int));
  C = (int *) malloc(sizeof(int));

  int a_elem = l_bound;					/* Initialize values */
  int b_count = 0;					/* These values are for looping */
  int b_elem = 0;
  int c_count = 0;
  int c_elem = 0;
  
  // Copy the lower portion of the array into arrayB
  for (b_count = 0; b_count < 1 + mid - l_bound ; b_count++) {
    B = (int*) realloc(B, (1 + b_count) * sizeof(int));
    B[b_count] = A[b_count + l_bound];
  }
  // Copy the upper portion of the array into arrayC
  for (c_count = 0; c_count <= u_bound - (mid + 1); c_count++) {
    C = (int*) realloc(C, (1 + c_count) * sizeof(int));
    C[c_count] = A[c_count + (mid + 1)];
  }

  // Merge and sort into array A depending on which element is lesser
  while(b_elem < b_count && c_elem < c_count) {
    if (B[b_elem] <= C[c_elem]) {
      A[a_elem] = B[b_elem];
      b_elem++;
    } else {
      A[a_elem] = C[c_elem];
      c_elem++;
    }
    a_elem++;
  }

  // Put rest of array B into array A if there are no elements in arrayC
  while(b_elem < b_count) {
    A[a_elem] = B[b_elem];
    b_elem++;
    a_elem++;
  }

  // Put rest of array C into array A if there are no elements in array B
  while(c_elem < c_count) {
    A[a_elem] = C[c_elem];
    c_elem++;
    a_elem++;
  }

  free(B);					// Deallocate arrays
  free(C);
  return 0;
}

