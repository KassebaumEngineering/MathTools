
//  IntArray.cc
//  C++ Class Implementation of the IntArray Class
//
//  Copyright (C) 1993, John A. Kassebaum
//
//  This software was written by John Kassebaum as a part of his graduate
//  course work at Purdue.  He retains all rights to this software.  
//
//  ----------
//  See IntArray.H for the Interface description and Usage directions.
//  ----------
//
//  Revision:
//    $Id: IntArray.cc,v 1.2 1997/09/14 01:01:32 jak Exp $
//
//  Notes: 
//
//  ************************************************************

static char rcsid_IntArray_cc[] = "$Id: IntArray.cc,v 1.2 1997/09/14 01:01:32 jak Exp $";


#include "IntArray.h"

//
// -------- IntArray Implementation ------------------------
//

//
// special preprocessor functions
//
#define ABS(x) ((x)>=0?(x):-(x))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// ======================================
//
// Constructors
//
// ======================================

IntArray:: IntArray( unsigned int len ): 
    length(len), first_index(0)
{
    allocate();
};

IntArray:: IntArray( int fi, unsigned int len): 
    length(len), first_index(fi)
{
    allocate();
};

IntArray:: IntArray( const IntArray &ar ): 
    length(ar.length), first_index(ar.first_index)
{
    register int r;
    register int *ptrA, *ptrB;

    allocate();

    ptrA = &(array[first_index]);
    ptrB = &(ar[first_index]);
    for( r=first_index; r < first_index+length; r++){
         *ptrA++ = *ptrB++;
    }
};

// ======================================
//
// Destructor
//
// ======================================

IntArray:: ~IntArray( void )
{
    deallocate();
};

// ======================================
//
// Private Allocation/Deallocation Methods
//
// ======================================

void IntArray:: allocate()
{
    register int r;
    register int *ptr;

    if( length > 0 ){
        array = (int *) new int [ length ];
		if (array == (int *)0 ) {
			cerr << "IntArray::allocate(): couln't allocate space for an ";
			cerr << "IntArray( " << length  << " )\n" ;
			abort();
		}
		for( ptr = array, r=0; r < length; r++){
			*ptr++ = 0;
		}	
		array = array - first_index;
    } else {
        array = (int *)0;
    }
};

void IntArray:: deallocate()
{
	if ( array != (int *)0 ){
		delete [] &(array[first_index]);
	}
};

// ======================================
//
// Assignment Operations
//
// ======================================

IntArray& IntArray:: operator = (const IntArray &ar)
{
    register int r;
    register int *ptrA, *ptrB;

    if (length != ar.length){
 		deallocate();
        length = ar.length;
        first_index = ar.first_index;
        allocate();
    } else if(ar.first_index != first_index) {
        this->shift_to(ar.first_index);
    }

    ptrA = &(array[first_index]);
    ptrB = &(ar[first_index]);
    for( r=first_index; r < first_index+length; r++){
         *ptrA++ = *ptrB++;
    }
   
    return *this;
};

// ======================================
//
//  Sub array Access Methods
//
// ======================================

IntArray  IntArray:: operator() ( int fi, unsigned int len ) const 
{
    IntArray result( fi, len );
    register int r, start, stop;
    register int *ptrA, *ptrB;

    start = MAX( fi, first_index );
    stop  = MIN( fi+len, first_index+length );

    ptrA = &(result[first_index]);
    ptrB = &(array[first_index]);
    for( r=start; r < stop; r++){
         *ptrA++ = *ptrB++;
    }

    return result;
};

IntArray& IntArray:: shift_to ( int fi )
{
    register int i;

    for(i=first_index; i<first_index+length; i++)
        array[i] += ( fi - first_index );

	array -= ( fi - first_index );
    first_index = fi;

    return *this;
};

// ======================================
//
//  Stream Methods
//
// ======================================

ostream & operator << (ostream &cbuf, const IntArray &Array)
{
    register int r;

    cbuf << "at ( " << Array.first_index << " )\n";
    if( Array.is_empty() )
        cbuf << "Null Array\n";
    else {
		for (r=Array.first_index; r < Array.first_index + Array.length; r++){
			cbuf << Array[r] << "  ";
		}
		cbuf << "\n";
    }
    return cbuf;
};

istream & operator >> (istream &cbuf, const IntArray &Array)
{
    register int r;

    for (r=Array.first_index; r < Array.first_index + Array.length; r++){
        cbuf >> Array[r];
    }
    return cbuf;
};

//
// ---------------------------------------------------------------------
//

//
// History:
//   $Log: IntArray.cc,v $
//   Revision 1.2  1997/09/14 01:01:32  jak
//   Some purely cosmetic changes. -jak
//
//   Revision 1.1.1.1  1997/09/13 23:38:38  jak
//   Restored MathTools to new CVS repository. -jak
//
//

