/********************************************************************
                Strongly Connected Components

                                                 Rich Vigorito
                                 CS510  Mastery in Programming  
                                                      8/7/2013
                                                          HW 5                          
*********************************************************************

This program does the following:
1) reads in an input file of a graph, whose node names are integers
2) constructs the graph
3) analyzes for strongly connected components, (scc)
4) output scc nodes, purposely omitting singleton nodes

Example Input:
1 2
2 3
3 1
3 7
8 8

Example Output:
next scc number is: 1
node 2 is in.
node 1 is in.
node 3 is in.

!!! ***NOTE***  input files must have blank row at end of file !!!!

to compile:  /> g++ scc.cpp
to run:      /> a.out < graph-input-file.txt
                    
    
*********************************************************************/

#include <stdio.h>
#include <string>
#include <sstream>
#include <ctype.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define NIL	NULL

// node may have any number of successors
// all need to be retrieved.
// so each node in G has a link pointer,
// pointing to Llist of all successor nodes.
// Last one connected is the first one inserted
typedef struct link_tp * link_ptr_tp;	// forward ref
typedef struct node_tp * node_ptr_tp;	
node_ptr_tp make_node( int name );

node_ptr_tp finger = make_node(-9999999);

typedef struct link_tp
{
	link_ptr_tp	next_link;	// point to next link
	node_ptr_tp	next_node;	// point to successor node
} str_link_tp;

typedef struct node_tp * node_ptr_tp;
#define LINK_SIZE sizeof( str_link_tp )

link_ptr_tp make_link( link_ptr_tp new_link,node_ptr_tp new_node )
{ // make_link
	link_ptr_tp link = (link_ptr_tp) malloc( LINK_SIZE );

	link->next_link = new_link;
	link->next_node = new_node;

	return link;
}
	
 
typedef struct node_tp
{
	link_ptr_tp	link;		// Llist of successors
	node_ptr_tp	finger;		// finger through all nodes
	int	name;			// name given at creation
	int 	lowlink;	// Tarjan's lowlink
	int 	number;		// Tarjan's number
	bool	visited;		// to check connectivity

	node_ptr_tp	scc_pred;	// scc stack predecessor
} str_node_tp;

#define NODE_SIZE sizeof( str_node_tp )


// create a node in graph G, identified by “name”
// connect to the global “finger” at head of Llist
node_ptr_tp make_node( int name )
{ // make_node
	node_ptr_tp node = (node_ptr_tp) malloc( NODE_SIZE );
	
	// check once non-Null here, not on user side!
//	ASSERT( node, ”no space for node in heap!" );
	node->finger	   	= finger;	// re-assign finger!!
	node->lowlink		; //NIL;	// int. not pointer
	node->number	   	; //NIL;	// int type
	node->link	   	= NULL;		// pointer type
	node->scc_pred	   	= NULL;		// pointer type
	node->name	   	= name;		// IDs this node
	node->visited		= false;	// initially
	finger			= node;		// now link to “this”
	return node;
} //end make_node



// check, whether link between these 2 nodes already exists
// if not, return true: New! Else return false, NOT new!
bool new_link( node_ptr_tp first, node_ptr_tp second )
{ // new_link
	int target			= second->name;
	link_ptr_tp link	= first->link;

	while ( link ) {
		if ( target == link->next_node->name ) {
			return false; // it is an existing link, NOT new
		} //end if
		// check next node; if any
		link = link->next_link;
	} //end while
	// none of successors equal the second node's name
	return true; // is a new link
} //end new_link

node_ptr_tp exists( int name ){
	node_ptr_tp tmp = finger;
	do{
		if(tmp->name == name)	
			return tmp;
	} while(tmp = tmp->finger);
	return NULL;
}



void build(){
	// input is list of pairs, each element being a node name
	// craft edge from first to second name = number
	// If a node is new: create it; else use ptr = exists()
	int a,b;
	char str[60];
	node_ptr_tp first,second;
	while( true) {
		fgets(str,60,stdin);
		if(str[0] == '\n'){
			break; 
		} else {
			sscanf(str,"%d%d",&a,&b);
		}

		if ( ! ( first = exists( a ) ) ) {	// ‘a’ new node?
			first = make_node( a );		// allocate ‘a’
		} //end if
		if ( ! ( second = exists( b ) ) ) {	// ‘b’ new node?
			second = make_node( b );	// allocate ‘b’
		} //end if
	// both exist. Either created, or pre-existed: Connect!
		if ( new_link( first, second ) ) {
			link_ptr_tp link = make_link( first->link, second );
	//		ASSERT( link, "no space for link node" );
			first->link = link;
		}else{
			// link was there already, no need to add again!
			printf( "<><> skip duplicate link %d->%d\n", a, b );
		} //end if
	} //end while

}

// globals for scc
int		scc_number	= 0;	// Tarjan's SCC numbers
node_ptr_tp	scc_stack	= NULL;	// stack exists via link in nodes
int		scc_count	= 0;	// tracks # of SCCs

// initial point of stack is global "scc_stack"
// each node has scc_pred link, linking up in fashion of a stack
void push( node_ptr_tp v )
{ // push
//	ASSERT( v, "push() called with NIL vertex v" );
//	ASSERT( !( v->visited ), "pushing vertex again?" );
	v->scc_pred	= scc_stack;	// first time NULL, then stack ptr
	v->visited	= true;		// will be handled now
	scc_stack	= v;		// global pts ID’s head
} //end push

// starting with global scc_stack, we can traverse whole stack
// all elements are connected by node field scc_stack
void pop()
{ // pop
//	ASSERT( scc_stack, "error, empty SCC stack" );
	scc_stack->visited 	= false; 	// remove from stack
	scc_stack 		= scc_stack->scc_pred;
} //end pop

void scc( node_ptr_tp v )
{ // scc
	node_ptr_tp w;

//	ASSERT( v, "calling scc with NULL pointer" );
//	ASSERT( !v->number, “node already has non-null number!" );
	v->number = v->lowlink = ++scc_number;
	push( v );
	for( link_ptr_tp link=v->link; link; link=link->next_link ) {
			w = link->next_node;
//			ASSERT( w, “node w linked as successor must be /= 0" );
			if ( ! w->number ) {		// alias for “->visited”
				scc( w );
				v->lowlink = min( v->lowlink, w->lowlink );
			} else if ( w->number < v->number ) {
				// frond, AKA “cross link”
				if ( w->visited ) {
					v->lowlink = min( v->lowlink, w->number );
				} //end if
			} //end if
	} //end for

	// now see, whether v is part of an SCC
	// and if so, record SC  number and all nodes belonging to it
	if ( v->lowlink == v->number ) {
			// found next scc; but if singleton node SCC, then skip it
			if ( scc_stack == v ) {
				// yes, singleton node; so be silent! Uninteresting!
				pop();
			} else {
				// multi-node scc; THAT we do consider
				scc_count++;
				cout << "next scc number is: " << scc_count << endl;
				while( scc_stack && ( scc_stack->number >= v->number ) ) {
					cout << "node " << scc_stack->name << " is in." << endl;
					pop();
				} //end while
			} //end if
	} //end if
} //end scc


int main()
{
	build();
	node_ptr_tp w = finger;
	do{
		if( ! w->visited )	
			scc(w);
	} while ( w = w->finger );	
  	return 0;
}
