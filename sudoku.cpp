
#include <stdio.h>
#include <string>
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


enum row_col_anal {ROW_ANALYSIS,COL_ANALYSIS};

typedef struct board_tp * square_ptr_tp;

typedef struct board_tp
{
	unsigned field; 		// 0 if empty, else one of 1..n*n
	unsigned option_count; 		// initially, all n*n are options
	unsigned can_be[SMALL_N+1]; 	// if false, number 'i' is impossible
} struct_board_tp;


struct_board_tp sudoku[ SMALL_N ][ SMALL_N ];


unsigned i(unsigned block){
	if(block == 0) return 0;	
	if(block == 1) return 7;	
	if(block == 2) return 14;	
}

unsigned f(unsigned index){
	ASSERT ((index <=9 ),index,"index cant be larger than 9");
	ASSERT ((index >= 0 ),index,"index cant be less than 0");

	if (index < 3)
		return 0;
	else if (index < 6)
		return 3;
	else if (index < 9)
		return 6;
}


unsigned count_fields(unsigned row, unsigned col){
	unsigned options = 0;
	for (int i = 1; i <= SMALL_N; i++){
		if(sudoku[ row ][ col ].can_be[i] == TRUE)
			++options; 
	}
	return options;
}

unsigned find_1_field(unsigned row, unsigned col){
	for (int i = 1; i <= SMALL_N; i++){
		if(sudoku[ row ][ col ].can_be[i] == TRUE)
			return i; 
	}
}

bool sanity_check(){
	for ( int row = 0; row < SMALL_N; row++ ) {
		for ( int col = 0; col < SMALL_N; col++ ) {
			if(!sudoku[ row ][ col ].field)
				return 0;
		}			
	}
	return 1;
}

void FILL(unsigned row, unsigned col,unsigned field){
	sudoku[ row ][ col ].field 		= field;
	sudoku[ row ][ col ].option_count 	= 0;
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
				ASSERT( ( SRC.option_count == 0 ),SRC.option_count, " has # + option?" );
			}else{
				// field is EMPTY. Goal to count down options to 1
				ASSERT( ( SRC.option_count ),SRC.option_count,"0 field must have opt" );
				// go thru each field. For # found, exclude # from can_be[]
				// go thru each field. For # found, exclude # from can_be[]
				for ( int i = 0; i < SMALL_N; i++ ) {
					if ( row_or_col == ROW_ANALYSIS ) {
						field = sudoku[ row ][ i ].field;
					}else{
						// column analysis
						field = sudoku[ i ][ col ].field;
					} //end if
					if ( field ) {
						// we found a field
						SRC.can_be[ field ] = FALSE;
					} //end if
					SRC.option_count = options = count_fields( row, col );
					if ( options == 1 ) {
						// plug in only 1 of BIG_N numbers
						// and set option_count to 0
						field = find_1_field( row, col );
						FILL(row,col,field);
						changes++;
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
				ASSERT( ( SRC.option_count == 0 ),SRC.option_count, "has # + option?" );
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
						if ( options == 1 ) {
							// plug in only 1 of SMALL_N numbers
							// and set option_count to 0
							field = find_1_field( row, col );
							FILL(row,col,field);
							changes++;
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
	unsigned changes;			// count fieldsfilled in
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
	//try_single_option();
#	ifdef DEBUG
				printf( "Iterated level0 %d times.\n", iterations );
				errors = sanity_check();
#	endif // DEBUG
	return changes;
} //end sudoku_level0

// simplest sudoku strategy by eliminating options for a field
// that would conflict with existing numbers in row, column, subsquare
unsigned sudoku_level1()
{ //sudoku_level0
	unsigned changes;			// count fieldsfilled in

	changes = sudoku_level0();
	if (sanity_check())
		return changes;
	return changes;

} //end sudoku_level0


void set_board(struct_board_tp &square,unsigned field){
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

	char one,two,three;

	int bar_cnt = 0 ;
	unsigned _1,_2,_3;

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
  	char header[]  	= "  0 1 2  3 4 5  6 7 8";
  	char div[] 	= "+------+------+------+";

	std::ostringstream board_str ;
	board_str << header << std::endl << div << std::endl;	 
		// set up a blank board
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

	std::cout << board_str.str() << div << std::endl << std::endl;
}


int main()
{
	init_board();
	print_board();

	read_board();
	print_board();

	sudoku_level1();
	print_board();
  	return 0;
}

