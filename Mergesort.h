/**  Cmput379 Lec B1

 *  ID:   1388761

 *  CCID: eric3

 *  Assignment 1 Mergesort.h due on 2016, Jan. 29.

 */

#pregma once
#include "Mergesort.c"

/**  @brief merge_sort_init opens the file to read integers and creates a shared memory pointer.
     
 *          The function will then copy the values into shared memory.
 
 *   @param argc The number of arguments passed into the program.

 *   @param argv The strings of the arguments passed into the program.

 *   @return 1 for an error or failure of the program. 0 for a sucessful run.

 */

int merge_sort_init(int argc, char *argv[]);


/**  @brief merge_sort will track of the upper and lower elements of the array in the process.

 *          It will recursively create a new processes and pass the parameters of the elements

 *   @param A is a pointer to the shared memory array.

 *   @param l_bound is the lower bound element.

 *   @param u_bound is the uppoer bound element.

 *   @return 1 if an error occurs while making a new process. 0 if it is sucessful
 
 */
int merge_sort(int *A, int l_bound, int u_bound);


/**  @brief merge copies the upper and lower section of the array into two temporary arrays.

 *          It will then merge the two arrays while sorting them from least to greatest value

 *   @param A is the array in shared memory.

 *   @param l_bound is the lower bound of the array.

 *   @param mid is the middle element of the array. This splits the array into two parts

 *   @param u_bound is the upper bound of the array.

 *   @return 0 for sucess of the merging.

 */
int merge(int *A, int l_bound, int mid, int u_bound);
