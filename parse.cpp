
/********************************************************************
                Regular Expression Parser

                                                 Rich Vigorito
                                 CS510  Mastery in Programming  
                                                     7/16/2013                          
*********************************************************************

parser the followling LL(1) grammar:


expression	:	term [ + expression ]
		|	term [ - expression ]

term		:	factor [ * term ]
		|	factor [ / term ]
		|	factor [ % term ]

factor		:	power [ ^ factor ]

power		:	primary 
		|	( expression )

primary		: 	ident
		| 	number
		|	- expression
		|	+ expression

ident		: 	letter { let_git }

let_git		: 	letter 
		| 	digit
	    	| 	‘_’ letter 
	  	| 	‘_’ digit

letter		: 	‘a’ | ‘A’ | ‘b’ | ‘B’ | ...

number		: 	digit { un_digit }

un_digit	:	digit 
		| 	‘_’ digit

digit		: 	‘0’ | ‘1’ | ‘2’ | ... | ‘9’

to compile:
shell> g++ parse.cpp 

*********************************************************************/


#include <stdio.h>
#include <string>
#include <sstream>
#include <ctype.h>
#include <stdlib.h>
#include <iostream>
using namespace std;


#define BLANK	' '
#define TAB	'\t'
#define EOL	'\n'
#define	OPEN	'('
#define	CLOSED	')'

char next_char = BLANK;	// globally used for "token"


void scan( )
{ // scan out spaces, to get next character
	next_char = getchar();
	if ( BLANK == next_char || TAB == next_char) {
			scan();
	}else{
			// echo non-blank found
			printf( "%c", next_char );	
	} 
}

// assume global: char next_char, void function scan()
void must_be( char expected )
{ // must_be
	if ( next_char != expected ) {
			// next_char not expected. Throw exception	
			std::ostringstream oss ;
			oss << " Expect " << expected <<  " is " << next_char << "\n";
			std::string err =  oss.str() ;
			throw err;
	} 
	scan(); 	// next_char is what we expect so scan
} 

void expression(); // forward declaration

// really just scans a single digit; if not found: exception
void digit()
{ 
	if (isdigit(next_char)){
		  scan();
	} else {
		std::ostringstream oss ;
		oss << " Expect 0,1,2 ... 9 got: " << next_char << "\n";
		std::string err =  oss.str() ;
		throw err;
	} 
} 

// really just scans a single letter; if not found: exception
void letter(){
	int next_char_int = next_char;
	// A-Z is have ascii value of 97 -> 122   and 
	// a-z  is have ascii value of 65 -> 90
	if (next_char_int >= 97 && next_char_int <= 122){
		scan();
	} else if (next_char_int >= 65 && next_char_int <= 90){
  		scan();
	} else {
		std::ostringstream oss ;
		oss << " Expect a,b,c,d... z,A,B,C..Z got: " << next_char << "\n";
		std::string err =  oss.str() ;
		throw err;
	}
}

// allow letter, digit, '_' + digit or '_' + digit
void let_git(){
	// cannot have trailing '_', last char in token is
	// '_' then throw exception
	if ((next_char == '_') && cin.peek() == BLANK){
		std::ostringstream oss ;
		oss << " Expect 0,1,2,3 _. 9 got: " << next_char << "\n";
		std::string err =  oss.str() ;
		throw err;
	} 


	// allow for '_'
	if ((next_char == '_'))
		  scan();

	// here it can be a letter or it can be a digit
	// if letter thows an exception then give digit 
	// a chance to work. if digit throws an exception
	// then we know that it is neither a letter or a digit
	try{
		letter();
	} catch (...){
		digit();
	}
}

// start with letter have any number of let_git's
void ident(){
	letter();
	try{
		while(true)
			let_git();
	} catch (...){
		// we know that let_git in a loop will always thfrow
		// an exception. If the last char was a '_' then its
		// legit, throw exception.  otherwise allow the exception
		if (next_char == '_'){
			std::ostringstream oss ;
			oss << " Expect a,b,c,d... z,A,B,C..Z got: " << next_char << "\n";
			std::string err =  oss.str() ;
			throw err;
	
		} else {
			//ignore excepeption
		}
	}

}

// allow digit or '_' +  digit
void un_digit()
{ 
	if ((next_char == '_')) {
		scan();
		digit();
	} else { 
		digit();
	}
} 

// allow digit plus any number of un_digits
void number(){
	digit();
	while (isdigit(next_char) || (next_char == '_')){
		un_digit();
	} 
}


// allow ident or number or either '-' or ''+' plus expression
void primary(){
	if ((next_char == '-') || (next_char == '+')) {
		scan();
		expression();
	} else {

		// here it can be an ident or it can be a number
		// if ident thows an exception then give number
		// a chance to work. if number throws an exception
		// then we know that it is neither an ident or a number
		try{
			ident();
		} catch (...){
			number();
		}
	}
}

// allow primary or ( expression ) 
void power(){
	if ( next_char == OPEN ) {	// if '(' then must have expression + ')'
		scan();
		expression();		
		must_be( CLOSED );
	} else {			// else primary 
		primary();		
	}
}

// power or power plus '^' plus factor
void factor()
{ 
	power();
	if ( ( next_char == '^' )){
		  scan();
		  factor();
	} 
} 


// allow '*','/' or '%' followed by a term
void term()
{ 
	factor();
	if ( ( next_char == '*' ) || ( next_char == '/' ) || ( next_char == '%' ) ) {
		  scan();
		  term();
	} 
} 




// parse adding operators + and 0, skip plus_op nonterminal
void expression()
{ 
	term();
	if ((next_char == '+') || (next_char == '-')) {
		  scan();
		  expression();
	} 
} 

int main()
{ 
	cout << "Enter arithmetic expression." << endl;
	try{
		scan();		// get first ever token
		expression();
		//must_be( '\n' );
		must_be( EOL );
 	} catch( std::string ex ) {
       		cerr << endl << "Syntax Error: " << ex << endl;
		return 0;
   	}
	cout << "Your input expression is correct." << endl;
	return 1;
} 
