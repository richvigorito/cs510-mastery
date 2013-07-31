#include <stdio.h>
#include <string>
#include <sstream>
#include <ctype.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define BLANK	' '
#define TAB	'\t'

// each node has 1 of these class states:
// a Literal, an Identifier (for variable), or an Operator.
// Parenthesized expressions have been reduced
typedef enum { Literal, Identifier, Operator } NodeClass;

char next_char = BLANK;	// globally used for "token"

typedef struct NodeType * NodePtr;	// forward announcement



NodePtr Expression(); // forward declaration
NodePtr Make( NodeClass Class, char Symbol, int value,  NodePtr Left, NodePtr Right ); // forward declaration



NodePtr OneNode  =  Make( Literal, '1', 1, NULL, NULL ) ;
NodePtr NullNode =  Make( Literal, '0', 0, NULL, NULL ) ;


// now comes the actual node-type structure,
// using the forward declared pointer type: NodePtr
typedef struct NodeType
{
	NodeClass	Class;			// 1 of the 3 classes.
	char		Symbol;		// stores ident or literal
	int			LitVal;		// if Literal, this is its value
	NodePtr		Left;			// subtree
	NodePtr		Right;			// subtree
} s_node_tp;

void scan( )
{ // scan out spaces, to get next character
	next_char = getchar();
	if ( BLANK == next_char || TAB == next_char) {
			scan();
	} else {
			// echo non-blank found
			//printf( "%c\n", next_char );	
	} 
}


// assume global: char next_char, void function scan()
void Must_Be( char expected )
{ // must_be
	if ( next_char != expected ) {
			// next_char not expected. Throw exception	
			std::ostringstream oss ;
			oss << " Expect " << expected <<  " is " << next_char << "\n";
			std::string err =  oss.str() ;
			throw err;

	} 
}

int IsLit(char expected, NodePtr node){
	if (node->Class != Literal)
		return 0;
	else if ( node->Symbol != expected ) 
		return 0;
	return 1;
}


int BothLit(NodePtr a, NodePtr b){
	return a->Class == Literal && b->Class == Literal; 
}
// malloc() new node from heap. All fields are passed in;
// return the pointer to the new node to caller

NodePtr Make( NodeClass Class, char Symbol, int value,  NodePtr Left, NodePtr Right )
{ // Make
	NodePtr Node = (NodePtr) malloc( sizeof( struct NodeType ) );
//	ASSERT( ... node’s space is really there ... );
	Node->Class  = Class;
	Node->Symbol = Symbol;
	Node->LitVal = value;
	Node->Left   = Left;
	Node->Right  = Right;
	return Node;
} //end Make


// recursively copy tree pointed to by Root.
// Pass pointer to the copy to its caller
NodePtr Copy( NodePtr Root )
{ // Copy
	if ( Root == NULL ) {
		return NULL;
	} else {
		return Make( Root->Class, Root->Symbol,
			Root->LitVal,
			Copy( Root->Left ),
			Copy( Root->Right )
		);
	} //end if
} //end Copy

void PrintTree( NodePtr Root )
{ // PrintTree
	if ( Root != NULL ) {
		if ( Root->Class == Operator ) {
			printf( "(" );
		} //end if
		PrintTree( Root->Left );
		if ( Root->Class == Literal ) {
			printf( "%d", Root->LitVal ); // prints ints > 9
		}else{
			printf( "%c", Root->Symbol );
		} //end if
		PrintTree( Root->Right );
		if ( Root->Class == Operator ) {
			printf( ")" );
		} //end if
   	} //end if
} //end PrintTree

// now multiple ^ operators are handled right-to-left
// in line with common precedence of exponentiation
NodePtr Primary( )
{ // primary
	char Symbol = next_char;		// first_set = { ‘(‘, ‘&’, IDENT, LIT } 
   	NodePtr Temp;

	scan();

   	if ( isdigit( Symbol ) ) {
		// end node: don’t recurse
		return Make( Literal, Symbol, (int)(Symbol-'0'), NULL, NULL );
   	} else if ( isalpha( Symbol ) ) {
		// also end node: don’t recurse
     		return Make( Identifier, tolower( Symbol ), 0, NULL, NULL );
   	} else if ( '(' == Symbol ) {
     		Temp = Expression();
     		Must_Be( ')' );
		scan();
     		return Temp;
	} else if ( Symbol == '&' ) {
		return Make(  Operator, '&', 0, NULL, Primary() );
	} else {
     		printf( "Illegal character '%c'.\n", Symbol );
			return NULL;
	} //end if

} //end primary



// exponentiation operator ‘^’ right-associative
NodePtr Factor(){
	 // Factor
	NodePtr Left = Primary();
	if ( next_char == '^' ) {
		scan();
     		Left = Make( Operator, '^', 0, Left, Factor() );
   	} //end if
   	return Left;
}

// multiply operators ‘*’ and ‘/’, later add ‘%’
NodePtr Term()
{ // Term
	char Op;						// remember ‘*’ or ‘/’
	NodePtr Left = Factor();
	while ( next_char == '*' || next_char == '/' ) {
     		Op = next_char;			// remember ‘*’ or ‘/’
     	
		scan();
		// note 0 below for LitVal is just a dummy
     		Left = Make( Operator, Op, 0, Left, Factor() );
    	} //end while
    return Left;
} //end Term



// parse expression and build tree
// using Term() and higher priority functions/ops
// all returning pointers to nodes
// in Expression() handle ‘+’ and ‘-’ operators
NodePtr Expression()
{ // Expression
	char Op;						// remember ‘+’ or ‘-’
	NodePtr Left = Term();	// handle all higher prior.
	while ( next_char == '+' || next_char == '-') {
		Op = next_char;			// remember ‘+’ or ‘-’
     		//getchar();			// skip Op
		scan();
		// note 0 below for LitVal is just a dummy
		Left = Make( Operator, Op, 0, Left, Term() );
	} //end while
	return Left;
} //end Expression

 //end Factor

NodePtr Derive( NodePtr Root )
{ // Derive
	if ( NULL == Root ) {
			return NULL;
	}else{
		switch ( Root->Class ) {
	    		case Literal: 
				return Make( Literal, '0', 0, NULL, NULL );
		    	case Identifier:
				if ( ( Root->Symbol == 'x' ) || ( Root->Symbol == 'X' ) ) {
			    		return Make( Literal, '1', 1, NULL, NULL );
				} else {
			    		return Make( Literal, '0', 0, NULL, NULL );
				} //end if
			case Operator:
       				switch ( Root->Symbol ) {
					case '+': 
					case '-':
	 					return Make( Operator, Root->Symbol, 0,
	  						Derive( Root->Left ), Derive( Root->Right ) );
					case '*':
	 					return Make( Operator, '+', 0,
	  						Make( Operator, '*', 0, Derive( Root->Left ),
								Copy( Root->Right ) ),
	  						Make( Operator, '*', 0, Copy( Root->Left ),
								Derive( Root->Right ) ) );
					case '/':
	 					return Make( Operator, '/', 0,
							Make( Operator, '-', 0,
	   							Make( Operator, '*', 0, Derive( Root->Left ),
									Copy( Root->Right ) ),
	   							Make( Operator, '*', 0, Copy( Root->Left ),
									Derive( Root->Right ) ) ),
	  						Make( Operator, '*', 0,
								Copy( Root->Right ), Copy( Root->Right ) ) );
					case '^':
  						return
    							Make( Operator, '+', 0,
      								Make( Operator, '*', 0,	Derive( Root->Left ),
        								Make( Operator, '*', 0,	Copy( Root->Right ),
          									Make( Operator, '^', 0,	Copy(Root->Left ),
            										Make( Operator, '-', 0, 
											Copy( Root->Right ), Copy ( OneNode)))
              								  ) 
								),
      								Make( Operator, '*', 0, 
									Make( Operator, '*', 0,
          									Make( Operator, '&', 0,	NULL, Copy( Root->Left ) ),
										Derive( Root->Right ) 
									),
        								Make( Operator, '^', 0,
          									Copy( Root->Left ), Copy( Root->Right ) 
									) 
								) 
							);
					case '&':
  						if ( Root->Left != NULL ) {
    							printf( "ln has only one operand.\n" );
  						} //end if
  						return Make(Operator, '/',0,						
							Derive(Root->Right),Copy(Root->Right));
					default:
						// base case.  IF here ERROR
    						printf( "DERIVE ERROR, Bad operator\n" );
						return Root;
						break;
				}
		} // endswitch
	}
} // Derivce


int is_associative( char symbol)
{
	if (symbol == '*' ||  symbol == '+')
		return 1;
	else
		return 0;

}



// return true only if both subtrees left and right are equal
int IsEqual( NodePtr Left, NodePtr Right )
{ // IsEqual
  	if ( ( !Left ) && ( !Right ) ) {
    		return 1;
  	} else if ( NULL == Left ) {
    		// Right is known to be not NULL
    		return 0;
  	} else if ( NULL == Right ) {
    		// Left is known to be NOT NULL
    		return 0;
  	} else if ( ( Left->Class == Literal ) && ( Right->Class == Literal ) ) {
    		return ( Left->LitVal ) == ( Right->LitVal );
  	} else if ( ( Left->Class == Identifier ) && ( Right->Class == Identifier )){
    		return ( Left->Symbol ) == ( Right->Symbol );
  	} else {
    		// must be Operator; same?
    		if ( ( Left->Symbol ) == ( Right->Symbol ) ) {
      			// IsEqual yields true, only if both subtrees are equal
      			return ( IsEqual( Left->Left, Right->Left ) && 	IsEqual( Left->Right, Right->Right ) ) ||
        			( is_associative( Left->Symbol ) &&
			 		IsEqual( Left->Left,  Right->Right ) &&
			 		IsEqual( Left->Right, Right->Left ) );
    		} else {
    			return 0;
    		} //end if
  	} //end if
  	printf( "Impossible to reach in IsEqual.\n" );
} //end IsEqual



NodePtr Simplify( NodePtr Root ) 
{ // Simplify
	int val = 0;			// accumulate integer values from + - * etc.
	if ( !Root ) {
		  return Root;
	} else{
       		switch ( Root->Class ) {
      			case Literal:
      			case Identifier:
				return Root;
      			case Operator:
       				Root->Left  = Simplify( Root->Left );
       				Root->Right = Simplify( Root->Right );
       				switch ( Root->Symbol ) {
        				case '+':
         					if ( IsLit( '0', Root->Left ) ) {
							return Root->Right;
         					} else if ( IsLit( '0', Root->Right ) ) {
							return Root->Left;
						} else if ( BothLit( Root->Left, Root->Right ) ) {
							val = Root->Left->LitVal + Root->Right->LitVal;
							return Make( Literal, (char)( val + '0' ), val,NULL, NULL );
						} else {
							return Root; // no other simplifiction for ‘+’
						} //end if
					case '-':
  						if ( IsLit( '0', Root->Right ) ) {
    							return Root->Left;
  						} else if ( BothLit( Root->Left, Root->Right ) ) {
    							val = Root->Left->LitVal - Root->Right->LitVal;
    							return Make( Literal, (char)( val + '0' ), val, NULL, NULL );
  						} else if ( IsEqual( Root->Left, Root->Right ) ) {
    							return  NullNode;
  						} else {
    							return Root;
  						} //end if
					case '*':
  						if ( IsLit( '1', Root->Left ) ) {
    							return Root->Right;
  						} else if ( IsLit( '1', Root->Right ) ) {
    							return Root->Left;
  						} else if ( IsLit( '0', Root->Left ) || IsLit( '0', Root->Right ) ) {
    							return  NullNode;
  						} else 	{
    							return Root;
  						}//end if
					case '/':
   						if ( IsLit( '1', Root->Right ) ) {
     							return Root->Left;
   						} else if ( IsLit( '0', Root->Left ) ) {
     							return NullNode;
   						} else if ( IsEqual( Root->Left, Root->Right ) ) {
     							return OneNode;
   						} else {
     							return Root;
   						} //end if
  					case '^':
    						if ( IsLit( '0', Root->Right ) ) {
      							return OneNode;
    						} else if ( IsLit( '1', Root->Right ) ) {
      							return Root->Left;
    						}else{
      							return Root;
    						} //end if
  					case '&':
    						if ( IsLit( '1', Root->Right ) ) {
     							return NullNode;
   						} else {
     							return Root;
   						} //end if
					default:
						printf("invalid operator: %c\n",Root->Symbol);
						return 0;
				}//endswitch
		}//endswitch
	}// endelse
}// end simplify

void Initialize( )
{ // Initialize
	printf( " Enter f(x), ended with $:  " );
	next_char = ' ';	// just assure it is NOT '$'
	next_char = getchar();
} //end Initialize
	
int main()
{
	try{
		NodePtr root = NULL;
 		Initialize();

		root = Expression();
		Must_Be( '$' );
    		printf( "Original   f(x) = " );
		PrintTree(root);
    		printf( "\n");
	
		root = Simplify( root );
		printf( "Simplified f(x) = " );
		PrintTree(root);
  		printf( "\n");
	
		root = Derive( root );
		printf( "Derived   f'(x) = " );
		PrintTree(root);
    		printf( "\n");
 
		root = Simplify( root );
		printf( "Reduced   f'(x) = " );
		PrintTree(root);
  		printf( "\n ");
	} catch( std::string ex ) {
       		cerr << endl << "Error: " << ex << endl;
		return 0;
   	}

  	return 0;
}
