/********************************************************************
                         Sudoku

                                                 Rich Vigorito
                                 CS510  Mastery in Programming  
                                                     8/14/2013
                                                          HW 6                         
*********************************************************************

This program does the following:
1) reads in an input file of a sudoku
2) attempt to solve the board as a level0 sudoku board
	ie at any given point there is field who as at most
	1 viable possibility for its field value
3)  EXTRA CREDIT: if level0 doesnt solve the board, solve as
	as a level1 board. ie there are fields who have 
	at most 2 viable possible values for its board

Sample input:
  0 1 2  3 4 5  6 7 8
+------+------+------+
|     5|      | 9   1| 0
|      | 5   2|      | 1
|     1|     8| 7 5  | 2
+------+------+------+
| 3    | 1 8  |   2  | 3
| 5 8  |   6  |   1 7| 4
|   7  |   2 5|     9| 5
+------+------+------+
|   4 7| 2    | 1    | 6
|      | 9   4|      | 7
| 9   3|      | 2    | 8
+------+------+------+

Output:
1) Print the initial board with statistics about how my fields
need solving. 
2) Print solved board and display what level board was solved at.

Sample:
Statistics initially:
Total # of fields: 81
Fields filled:     31
empty fields:      50

Sudoku level 1 sudoku board.
	  0 1 2  3 4 5  6 7 8
	+------+------+------+
	| 6 2 5| 7 4 3| 9 8 1| 0
	| 7 9 8| 5 1 2| 6 3 4| 1
	| 4 3 1| 6 9 8| 7 5 2| 2
	+------+------+------+
	| 3 6 9| 1 8 7| 4 2 5| 3
	| 5 8 2| 4 6 9| 3 1 7| 4
	| 1 7 4| 3 2 5| 8 6 9| 5
	+------+------+------+
	| 8 4 7| 2 5 6| 1 9 3| 6
	| 2 1 6| 9 3 4| 5 7 8| 7
	| 9 5 3| 8 7 1| 2 4 6| 8
	+------+------+------+
The board was solved with sudoku level 1


!!! ***NOTE***  input files must have blank row at end of file !!!!

to compile:  /> g++ sudoku.cpp
to run:      /> a.out < sudoku-input-file.txt
*********************************************************************/

#include <stdio.h>
#include <string>
#include <cstring>
#include <sstream>
#include <ctype.h>
#include <stdlib.h>
#include <iostream>

#define  TINY_N 3
#define  SMALL_N (TINY_N * TINY_N)
#define BIG_N ( SMALL_N * SMALL_N )
#define EOL '\n'
#define EMPTY 0
#define NO_CHAR ' '
#define FALSE 0
#define TRUE  1

#define P_BAR		printf( "|" )
#define P_LINE		printf( "-" )
#define P_PLUS		printf( "+" )
#define P_EOL   	printf( "\n" )
#define P_TAB		printf( "\t" )
#define P_BLANK	printf( " " )

#define ASSERT( b,i,msg )				\
	if ( !b ) {					\
  		printf( "ERROR (%c): %s\n", i,msg );	\
	} //end if 
 //end if


enum row_col_anal {ROW_ANALYSIS,COL_ANALYSIS}; // dictates what type of analysis we are performing

typedef struct board_tp * square_ptr_tp;

typedef struct board_tp
{
	unsigned field; 		// 0 if empty, else one of 1..n*n
	unsigned option_count; 		// initially, all n*n are options
	unsigned can_be[SMALL_N+1]; 	// if false, number 'i' is impossible
} struct_board_tp;


struct_board_tp sudoku[ SMALL_N ][ SMALL_N ];

void print_board();

unsigned i(unsigned block){ 
// returns the start index of an individual block in a row from the input file
	if(block == 0) return 0;	
	if(block == 1) return 7;	
	if(block == 2) return 14;	
}

unsigned f(unsigned index){
// used for subsquare indexing
	ASSERT ((index <=9 ),index,"index cant be larger than 9");
	ASSERT ((index >= 0 ),index,"index cant be less than 0");

	if (index < 3) return 0;
	if (index < 6) return 3;
	if (index < 9) return 6;
}


unsigned count_fields(unsigned row, unsigned col){
// determine how many options are still viable field values
// for the field data structure
	unsigned options = 0;
	for (int i = 1; i <= SMALL_N; i++){
		if(sudoku[ row ][ col ].can_be[i] == TRUE)
			++options; 
	}
	return options;
}

unsigned find_1_field(unsigned row, unsigned col){
// find first available viable field value for a field
	for (int i = 1; i <= SMALL_N; i++){
		if(sudoku[ row ][ col ].can_be[i] == TRUE)
			return i; 
	}
}

int get_empty_count(){
// how many fields are empty in the entire board
	int cnt = 0;
	for ( int row = 0; row < SMALL_N; row++ ) {
		for ( int col = 0; col < SMALL_N; col++ ) {
			if(!sudoku[ row ][ col ].field)
				cnt++;
		}			
	}
	return cnt;
}

int valid_move(int row,int col,int set_field){
// checks to see that all are set, ie have values
	// determine numbers hasnt already set in row
	for ( int c = 0,i=1; c < SMALL_N; c++,i++ ) {
		if (set_field == sudoku[row][c].field){
			return 0;
		}
		
	}

	// determine numbers hasnt already set in col
	for ( int r = 0,i=1; r < SMALL_N; r++,i++ ) {
		if (set_field == sudoku[r][col].field)
			return 0;
	}

	for ( int i = f( row ); i < ( f( row ) + TINY_N ); i++ ) {
		ASSERT( ( i <= row+SMALL_N ), i,"wrong i_row in [][]" );
		for ( int j = f( col ); j < ( f( col ) + TINY_N ); j++ ) {
			ASSERT( (j <= col+SMALL_N),i, "wrong j_col in [][]" );
			if (set_field == sudoku[i][j].field)
				return 0;
		}
	}

	return 1;
}


int is_board_filled(){
	// all fields must be set	
	for ( int row = 0; row < SMALL_N; row++ ) {
		for ( int col = 0; col < SMALL_N; col++ ) {
			if(!sudoku[ row ][ col ].field)
				return 0;
		}			
	}
	return 1;
}

int sanity_check(){
// checks to see that all are set, ie have values

	if (!is_board_filled())	return 0;

	
	int sum = 0	;
	
	// all rows must have all numbers
	for ( int row = 0; row < SMALL_N; row++ ) {
		sum = 0;
		for ( int col = 0,i=1; col < SMALL_N; col++,i++ ) {
			sum += sudoku[row][col].field;
		}
		if (sum != 45){
			std::cout << "row" << row+1 << ": " << sum << std::endl ;
	  		return 0;
		}
	}
	
	for ( int col = 0; col < SMALL_N; col++ ) {
		sum = 0;
		for ( int row = 0; row < SMALL_N; row++ ) {
			sum += sudoku[row][col].field;
		}
		if (sum != 45){
			std::cout << "col" << col+1 << ": " << sum << std::endl ;
	  		return 0;
		}
	}

	// all subsquare must have all numbers
	for ( int row = 0; row < SMALL_N; row++ ) {
		for ( int col = 0; col < SMALL_N; col++ ) {
			sum = 0;
			for ( int i = f( row ); i < ( f( row ) + TINY_N ); i++ ) {
			ASSERT( ( i <= row+SMALL_N ), i,"wrong i_row in [][]" );
				for ( int j = f( col ); j < ( f( col ) + TINY_N ); j++ ) {
					ASSERT( (j <= col+SMALL_N),i, "wrong j_col in [][]" );
					sum += sudoku[i][j].field;
				}
			}
			if (sum != 45){
				std::cout << "col" << col+1 << ": " << sum << std::endl ;
	  			return 0;
			}
		}
	}

	return 1;
}

void FILL(unsigned row, unsigned col,unsigned field){
// set a field with a value
	sudoku[ row ][ col ].field 		= field;
	sudoku[ row ][ col ].option_count 	= 0;
	
	for ( int n = 1; n <= SMALL_N; n++ ) 
		sudoku[ row ][ col ].can_be[n] = 0;
		
	sudoku[ row ][ col ].can_be[field] = 1;
	int z = 0;	
}


// check all horizontal and vertical lines for empty spaces.
// each empty space initially has BIG_N options
// but for each existing value in that row or col, decrease the option.
// if per chance the options end up just 1, then we can plug in a number.
// return the number of fields changed from empty to a new value
unsigned horiz_vert( row_col_anal row_or_col )
{ // horiz_vert
	unsigned changes = 0;
	unsigned options = 0;
	unsigned field   = 0;	// remember the next number to be excluded

	for ( int row = 0; row < SMALL_N; row++ ) {
		for ( int col = 0; col < SMALL_N; col++ ) {
			struct_board_tp& SRC =  sudoku[ row ][ col ];
			if ( SRC.field ) {	// there is a number
				//ASSERT( ( SRC.option_count == 0 ),SRC.option_count, " has # + option?" );
			}else{
				// field is EMPTY. Goal to count down options to 1
				ASSERT( ( SRC.option_count ),SRC.option_count,"0 field must have opt" );
				// go thru each field. For # found, exclude # from can_be[]
				for ( int i = 0; i < SMALL_N; i++ ) {
					if ( row_or_col == ROW_ANALYSIS ) {
						field = sudoku[ row ][ i ].field;
					}else{
						// column analysis
						field = sudoku[ i ][ col ].field;
					} 
					if ( field ) {
						// we found a field
						SRC.can_be[ field ] = FALSE;
					} 
					SRC.option_count = options = count_fields( row, col );
					if ( options == 1 ) {
						// plug in only 1 of BIG_N numbers
						// and set option_count to 0
						field = find_1_field( row, col );
						if ( valid_move(row,col,field)) {
							FILL(row,col,field);
							changes++;
						} //end if
					} //end if
				} //end for i
			   } //end if
		 } //end for col
	} //end for row
	return changes;
} //end horiz_vert


// check all horizontal and vertical lines for empty spaces.
// each empty space initially has SMALL_N options
// But for each field value in subsquare, decrease options.
// if per chance the options end up just 1, then we can plug in a number.
// return the number of fields changed from empty to a new value
unsigned subsquare( )
{ // subsquare
	unsigned changes = 0;
	unsigned options = 0;
	unsigned field   = 0;	// remember the next number to be excluded

	for ( int row = 0; row < SMALL_N; row++ ) {
		for ( int col = 0; col < SMALL_N; col++ ) {
			struct_board_tp& SRC =  sudoku[ row ][ col ];
			if ( SRC.field ) {	// there is a number
				//ASSERT( ( SRC.option_count == 0 ),SRC.option_count, "has # + option?" );
			}else{
				// field is EMPTY. Goal to count down options to 1jj
				ASSERT( ( SRC.option_count ),SRC.option_count, "subsquare must have opt" );
				// analyze all fields in subsquare, exclude from can_be[]
				for ( int i = f( row ); i < ( f( row ) + TINY_N ); i++ ) {
					ASSERT( ( i <= row+SMALL_N ), i,"wrong i_row in [][]" );
					for ( int j = f( col ); j < ( f( col ) + TINY_N ); j++ ) {
						ASSERT( (j <= col+SMALL_N),i, "wrong j_col in [][]" );
						field = sudoku[ i ][ j ].field;
						if ( field ) {
							// we found a non-zero field
							SRC.can_be[ field ] = FALSE;
						} //end if
						SRC.option_count = options = count_fields( row, col );
						if ( options == 1) {
							// plug in only 1 of SMALL_N numbers
							// and set option_count to 0
							field = find_1_field( row, col );
							if(valid_move(row,col,field)) {
								FILL(row,col,field);
								changes++;
							}
						} //end if
					} //end for j
				} //end for i
			   } //end if
	  } //end for col
	} //end for row
	return changes;
} //end subsquare

// simplest sudoku strategy by eliminating options for a field
// that would conflict with existing numbers in row, column, subsquare
unsigned sudoku_level0()
{ //sudoku_level0
	unsigned changes;		// count fieldsfilled in
	unsigned iterations = 0;	// count times we go around
	unsigned errors = 0;		// do final sanity check

	row_col_anal row_analysis = ROW_ANALYSIS;
	row_col_anal col_analysis = COL_ANALYSIS;

	do {
				changes  = 0;
				changes  = horiz_vert( row_analysis );
				changes += horiz_vert( col_analysis );
				changes += subsquare();
				++iterations;
	} while ( changes );

#	ifdef DEBUG
				printf( "Iterated level0 %d times.\n", iterations );
				errors = sanity_check();
#	endif // DEBUG
	return changes;
} //end sudoku_level0

int solved_at_level = 0; // used to determine what level board was solved at

// level1 sudoku solver.
// first attempt to solve at level0. if that works return without level1 processing.
// IF level0 didnt solve find all fields with options_count=2. 
//    -- LOOP over the 2 solutions try solution via FILL
//    -- recurse 
//    -- IF a solution works, return
//    -- ELSE backtrack, ie set the field back to the state prior to the recursive call

struct_board_tp temp[SMALL_N][SMALL_N] ;

bool find_unassigned(int &row, int &col)
{
    for (row = 0; row < SMALL_N; row++)
        for (col = 0; col < SMALL_N; col++)
            if (sudoku[row][col].field == 0)
                return true;
    return false;
}

//unsigned sudoku_level1(int r, int c, int try_option=0)
unsigned sudoku_level1(int try_option=0)
{ //sudoku_level1

	int row,col;
	
	if (0 == solved_at_level){
		int changes = sudoku_level0();
	}
	
	if (!find_unassigned(row,col))
			return 1;

	solved_at_level = 1;
		

	for (int field = 1;field <= SMALL_N;field++){
		if (0 == sudoku[row][col].can_be[field]) continue;

		if (sudoku[row][col].can_be[field]){
			if (valid_move(row,col,field)){
                      		sudoku[ row ][ col ].field = field ;
				if(sudoku_level1(field))
					return 1;
                       		sudoku[ row ][ col ].field = 0 ;
			}
		}
	}
	return 0;
} //end sudoku_level1


void set_board(struct_board_tp &square,unsigned field){
// set a field in the sudoku board
	square.field 		= field;
	square.option_count 	= 0;
	for ( int j = 1; j <= SMALL_N+1; j++ ) {
		if(j != field)	
			square.can_be[j]  = FALSE;
		else 
			square.can_be[j]  = TRUE;
	}
}

void parse_line(unsigned row,char * str){
// used to parse the read in lines form the board file

	char one,two,three; // chars for the field values
	unsigned _1,_2,_3;  // ints that are created from the chars parsed

	for(int n = 0 ,j = 0; n < SMALL_N; n+=3) {

		one 	= str[i(n/3)+2];
		two 	= str[i(n/3)+4];
		three 	= str[i(n/3)+6];
	
		_1 = ( one   != ' ') ? one   - '0' : 0 ;
		_2 = ( two   != ' ') ? two   - '0' : 0 ;
		_3 = ( three != ' ') ? three - '0' : 0 ;
	
		if(_1)
			set_board(sudoku[row][j],_1);	
		j++;

		if(_2)
			set_board(sudoku[row][j],_2);	
		j++;

		if(_3)
			set_board(sudoku[row][j],_3);	
		j++;

	}
	
}

void init_board(){
// set up a blank board
	for ( int row = 0; row < SMALL_N; row++ ) {
		for ( int col = 0; col < SMALL_N; col++ ) {
			sudoku[row][col].field 	= 0;
			sudoku[row][col].option_count  = 9;
			for ( int j = 0; j < SMALL_N; j++ ) 
				sudoku[row][col].can_be[1+j]  = TRUE;
		}
	}
}

void read_board(){
// read board from stdin line at a time

	char str[60];	
	int a,b;
	bool go = false;

	fgets(str,60,stdin);
	fgets(str,60,stdin);
	for(int i = 0 ,j = 0; i < SMALL_N; i+=3) {
		fgets(str,60,stdin);
		parse_line(j++,str);
		fgets(str,60,stdin);
		parse_line(j++,str);
		fgets(str,60,stdin);
		parse_line(j++,str);
		fgets(str,60,stdin);
	}
}

void print_board(){
// print board representation of struct_board_tp
  	char header[]  	= "  0 1 2  3 4 5  6 7 8";
  	char div[] 	= "+------+------+------+";

	std::ostringstream board_str ;
	board_str << header << std::endl << div << std::endl;	 

	for ( int row = 0; row < SMALL_N; row++ ) {
		if (row%3 == 0 && row != 0)
			board_str << div << std::endl;
		for ( int col = 0; col < SMALL_N; col++ ) {
			if (col%3 == 0)
				board_str << "|" ;
			if (sudoku[row][col].field)
				board_str << " " << sudoku[row][col].field  ;
			else 
				board_str << "  " ;
			if (col == SMALL_N -1)
				board_str << "| " << row << std::endl;
		}
	}

	std::cout << board_str.str() << div << std::endl;
	std::cout << std::endl;
}

int main()
{
	init_board(); // initialize

	read_board(); // read in baord

	std::cout << "Initial, incomplete input field:" << std::endl;
	print_board();

	int cnt = get_empty_count(); 
	
	std::cout << "Statistics initially:" << std::endl;
	std::cout << "Total # of fields: 81" << std::endl;
	std::cout << "Fields filled: " << 81-cnt << std::endl;
	std::cout << "Fields Empty: " << cnt << std::endl << std::endl;

	//sudoku_level1(0,0); // solve board
	sudoku_level1(); // solve board
	print_board();

	if(sanity_check()){
		std::cout << "Board was solvable with level " << solved_at_level << std::endl;
	} else {
		std::cout << "Board wasnt solvable with level "  << solved_at_level << std::endl;
	}
  	return 0;
}

