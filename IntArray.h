//  IntArray.h
//  C++ Class Interface Definition of the IntArray Class
//
//  Copyright (C) 1993, John A. Kassebaum
//
//  This software was written by John Kassebaum as a part of his graduate
//  course work at Purdue.  He retains all rights to this software.  
//
//  *************************  Description  **************************
//  IntArray Object:
//
//  Public Data:
//
//  Responding Messages:
//
//  Original Messages:
//
//  Side Effects:
//	None.
//
//  Known Bugs:
//	None. 
//
//  ---------------------------------------------------------------------
//  Author(s) and Credits:
//	  This Software was originally written by John Kassebaum as a part of 
//    his graduate classwork at Purdue University.  
//
//  Revision:
//    $Id: IntArray.h,v 1.1 1997/09/13 23:38:39 jak Exp $
//
//  ******************************************************************
//
#ifndef _IntArray_H
#define _IntArray_H

static char rcsid_IntArray_h[] = "$Id: IntArray.h,v 1.1 1997/09/13 23:38:39 jak Exp $";

//
// ANSI-C Library Functions
//
#include <iostream.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <strings.h>
//#include <stdlib.h>



/// ---------------------------------------------------------------------
// IntArray : Class
// ---------------------------------------------------------------------

class IntArray {
    int           first_index;
    unsigned int  length;
    int          *array;

    void allocate(void);
    void deallocate(void);

public:

// Constructors and Destructors
    IntArray( unsigned int = 0 );
    IntArray( int, unsigned int);
    IntArray( const IntArray & );
    ~IntArray( void );

// Assignment Operations
    IntArray&  operator = (const IntArray &);

//  Sub array Access Methods
    IntArray operator() ( int, unsigned int ) const;
    IntArray&  shift_to ( int );

    inline unsigned int getLength( void ) const { return length;    };
    inline int first( void ) const { return first_index;    };

    inline int& operator [] ( int i ) const { return array[ i ]; };
    inline int has_element( int i ) const {
        if ( (i <  first_index) || (i >= first_index + length ))
            return 0;
        else
            return 1;
    };
    inline int is_empty() const {
        return array?0:1;
    };

// Input Output
    friend ostream & operator << (ostream &, const IntArray&);
    friend istream & operator >> (istream &, const IntArray&);
};

//
// ---------------------------------------------------------------------
//

#endif

//
//  History:
//    $Log: IntArray.h,v $
//    Revision 1.1  1997/09/13 23:38:39  jak
//    Initial revision
//
//
