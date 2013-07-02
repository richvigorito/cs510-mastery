
/********************************************************************
		N queens problem. 

			          		 Rich Vigorito
			         CS510  Mastery in Programming  
					             6/30/2013				
*********************************************************************

This solution uses the back tracking technique in chapter 3  of
the course textbook, 'Algorithms + Data Structures = Programs.'
Specifically pgs 143-147. 

After reading that textbook and reading the homework description, 
implementing the backtracking algorithm seemed more challenging and 
honestly more fun.

The jist of the backtracking algorithm is to sequentially place the 
queen in a square whereas the move is valid, ie cant get caught. If 
after so many places of a queen its deteremined that the current move 
in valid, bubble up the call stack to and try a different solution. The 
data structure is a single array where the index+1 represents the row and the
value represents the column. ie: { 2 4 1 3 } would be:

. Q . .
. . . Q
Q . . .
. . Q .

Originally started doing just 8 queens solution but wasnt getting all 
92 solutions. I then turned the program into an n-queen program to 
verify could do 4-queens, then 5-queens, then 6-queens. Specifically 
solving the 5-queen problem tipped me off on my bug.


NOTE: An additional source i used for help comprehending
the recursive back tracking algorithm is the following youtube video:
http://www.youtube.com/watch?v=p4_QnaTIxkQ

*********************************************************************/


#include <stdio.h>
#include <math.h>


int x[8]; 			// board representation. index = row, value = col
int solutions[500000][8];		// solutions store
int solution_cnt = 0;		// solutions computed
int iterations = 0;		// iterations
const char file[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N'};

void print_solutions(n){
	int i,j,k,p;
	for (i=0;i<solution_cnt;i++){ // iterate over stored solutions
		
  		printf("Successful postion: %d. \n",i+1);
	
		// file header, ie A->H	
  		printf("  +");
		for (p=0;p<n;p++){
  			printf(" %c",file[p]);
		}
  		printf(" +\n");

		
		for (j=0;j<n;j++){ // iterate over columns of an individual solution
  			printf("%d |",j+1);
			for (k=1;k<=n;k++){  // determine to place '.' or 'Q' based on column value
				if(k == solutions[i][j])
  					printf(" Q");
				else
  					printf(" .");
			}
  			printf(" |\n");
  			//printf(" %d, ",solutions[i][j]);
  			//printf("\n");
		}
  	
		// file header, ie A->H	
		printf("  +");
		for (p=0;p<n;p++){
  			printf(" %c",file[p]);
		}
  		printf(" +\n");

  		printf("\n");
	}
  	printf("Iterations via backtracking: %d \n",iterations);
  	printf("Successful positions: %d \n",solution_cnt);
}


void save(n){
	// save solution to solutions array
	int j ;
	for (j=0;j<n;j++)
		solutions[solution_cnt][j] = x[j];
	solution_cnt++;
}


int setQ(int r,int c){
// attempt to set the queen. 
// return true if placement cant result in an attack
	int j;
	for (j=1;j<=r-1;j++){
		if(c == x[j-1]) // invalid due to same column attack
			return 0;
		if (abs(x[j-1] - c) == abs(j-r)) // invalid due to diagonal attack
			return 0;
	}
	return 1;
}

void queens(int r,int n){
// recursive backtracking function
// r = row, n = board dimension
	
	int c ;  // column
	for (c = 1; c <= n ; c++){
		
		if(setQ(r,c)){ 	// if we are allowed to set the queen
			x[r-1] = c;
			if (r == n){ // no more columns to attempt to set
				save(n);
			} else{
				queens(r+1,n);
				x[r-1] = 0; // backtrack! 
			}
		}
	}
	iterations++;
}

void main(){
  int n;

  // solution is n-queens. allow user to select dimensions via 	 
  printf("Enter board size. ie NxN\n");
  scanf("%d", &n);
 
  queens(1,n);

  print_solutions(n);
}
