
//  Range.h
//  C++ Class Template Definition of the Range Class
//
//  Copyright (C) 1993, John A. Kassebaum
//
//  This software was written by John Kassebaum as a part of his graduate
//  course work at Purdue.  He retains all rights to this software.  
//
//  *************************  Description  **************************
//  Range<NUMTYPE> Class:
//
//  Public Data:
//    None.
//
//  Responding Messages:
//    Constructor:
//        Range(NUMTYPE start, NUMTYPE end, NUMTYPE step); 
//
//    Destructor:
//        ~Range( void );
//
//    Evaluation:
//        NUMTYPE start( void ) const;
//        NUMTYPE end( void ) const;
//        NUMTYPE step( void ) const;
//
//        unsigned int RangeCount( void ) const;
//
//    Re-Initialization:
//        void setStart( NUMTYPE start ) const;
//        void setEnd( NUMTYPE end ) const;
//        void setStep( NUMTYPE step ) const;
//
//    Use:
//        NUMTYPE value() const;
//        NUMTYPE next();
//        NUMTYPE reset(); 
//        int     done( void ) const;
//
//  Original Messages:
//    None.
//
//  Side Effects:
//	  None.
//
//  Known Bugs:
//	  None. 
//
//  Note(s):
//    User must define:
//        <NUMTYPE>::operator += ()
//        <NUMTYPE>::operator > ()
//    note: these should be already defined for 
//    builtin numeric types (e.g. int, char, double)
//
//  Author(s) and Credits:
//	  This Software was originally written by John Kassebaum as a part of 
//    his graduate classwork at Purdue University.  
//
//  Revision:
//    $Id: Range.h,v 1.2 1997/09/14 01:02:03 jak Exp $
//
//
#ifndef _Range_H
#define _Range_H

static char rcsid_Range_h[] = "$Id: Range.h,v 1.2 1997/09/14 01:02:03 jak Exp $";

#pragma interface

// ---------------------------------------------------------------------
// Range<NUMTYPE>  : Template Class
// ---------------------------------------------------------------------

template <class NUMTYPE>
class Range {
private:
    NUMTYPE the_start;
    NUMTYPE the_end;
    NUMTYPE by_step;
	
    NUMTYPE the_current;
	int atEnd;

public:
    Range(NUMTYPE s, NUMTYPE e, NUMTYPE st): 
         the_start(s), the_end(e), by_step(st), the_current(s), atEnd(0) {};
    ~Range(){};

    inline NUMTYPE start( void ) const { return the_start; }
    inline void setStart( NUMTYPE s ) { the_start = s; }

    inline NUMTYPE end( void ) const { return the_end; }
    inline void setEnd( NUMTYPE e ) { the_end = e; }

    inline NUMTYPE step( void ) const { return by_step; }
    inline void setStep( NUMTYPE st ) { by_step = st; }

    inline unsigned int RangeCount( void ) const {
	    register int count;
		NUMTYPE val;
		
		for(count = 0, val=the_start; !( val>the_end ); val += by_step){
		    count++;
		}
		
		return count;
	}

    inline NUMTYPE value( void ) const { return the_current; }
    inline NUMTYPE next( void ) { 
        register NUMTYPE rtn;
        if (the_current > the_end){
			 atEnd = 1;
		} else {
            the_current += by_step;
		}
        rtn = the_current;
        return rtn; 
    };
    inline NUMTYPE reset( void ) { 
        the_current = the_start; 
		atEnd = 0;
        return the_current; 
    };
	
	inline int done( void ) const { return atEnd; };
};

//
// ---------------------------------------------------------------------
//

#endif  // _Range_H

//
//  History:
//    $Log: Range.h,v $
//    Revision 1.2  1997/09/14 01:02:03  jak
//    Some purely cosmetic changes. -jak
//
//    Revision 1.1.1.1  1997/09/13 23:38:42  jak
//    Restored MathTools to new CVS repository. -jak
//
//
