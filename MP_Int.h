
//  MP_Int.h
//  C++ Class Interface Definition of the MP_Int Class
//
//  Copyright (C) 1993, John A. Kassebaum
//
//  This software was written by John Kassebaum as a part of his graduate
//  course work at Purdue.  He retains all rights to this software.  
//
//  *************************  Description  **************************
//  MP_Int Object:
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
//  ---------------------------------------------------------------------------
//  Author(s) and Credits:
//	  This Software was originally written by John Kassebaum as a part of 
//    his graduate classwork at Purdue University.  
//
//  Revision:
//    $Id: MP_Int.h,v 1.1 1997/09/13 23:38:40 jak Exp $
//  ******************************************************************
//
#ifndef _MP_Int_h
#define _MP_Int_h

static char rcsid_MP_Int_h[] = "$Id: MP_Int.h,v 1.1 1997/09/13 23:38:40 jak Exp $";

//
// ANSI-C Library Functions
//
#include <iostream.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//
// stdc++ Library Functions
//
#include <string>

//
//  This software requires the GNU MP Library.
//  current version gmp-2.0.2
//
#include <gmp.h>


//
//  Use these external classes.
//
class MP_Float;
class MP_Ratio;


typedef signed long   signed_long ;
typedef unsigned long unsigned_long ;

// ----------------------------------------------------------------
//
// MP_Int Interface Definition
//

enum rounding_method { MPZ_TRUNCATE = 0, MPZ_CEILING = 1, MPZ_FLOOR = 2 };

class MP_Int {

public:
	// ------------------------------------------------------------
	//
	// MP_Int_Storage Interface Definition
	//
	
	class MP_Int_Storage
	{
	  public:
        mpz_t  intVal;
        int    reference_count;
		
		MP_Int_Storage( void );
		MP_Int_Storage( const MP_Int_Storage & );
		MP_Int_Storage( unsigned long );
		MP_Int_Storage( signed long );
		MP_Int_Storage( double );
		MP_Int_Storage( const MP_Ratio & );
		MP_Int_Storage( const MP_Float & );
		MP_Int_Storage( const char *aString, int );

		~MP_Int_Storage( void );
		
		inline void dec_refcount( void ) { if( --reference_count == 0 ) delete this; };
		inline void inc_refcount( void ) { reference_count++; };		
	};
	
	// ------------------------------------------------------------

    MP_Int_Storage *myInt;
	static rounding_method my_rounding_method;
	
public:
  // Constructors
	MP_Int( void );
	MP_Int( const MP_Int & );
	MP_Int( unsigned long );
	MP_Int( signed long );
	MP_Int( double );
	MP_Int( const MP_Ratio & );
	MP_Int( const MP_Float & );
	MP_Int( const char *aString, int = 0 );
	
  // Destructor
    virtual ~MP_Int( void );

  // 
  // MP_Int Specific Class Methods
  // 
  
  // Set Class Rounding Convention
    inline static            void setRoundingMethod( rounding_method aMethod ) { my_rounding_method = aMethod; };
    inline static rounding_method getRoundingMethod(            void         ) { return my_rounding_method;    };
  
  // Generate Random Numbers
    static MP_Int random ( mp_size_t /*max_size*/ );  // generate random number of at most max_size limbs
    static MP_Int random2( mp_size_t /*max_size*/ );  // generate negative numbers when max_size is negative
	
  // Factorial
    static MP_Int factorial ( unsigned long );
    
  // 
  // MP_Int Specific Instance Methods
  // 
  
  // convert internal value to unsigned or signed long
    inline unsigned long ( void ) const { return mpz_get_ui( value() ); };
    inline   signed long ( void ) const { return mpz_get_si( value() ); };
  
  // 
  // Common Instance Methods
  // 
  
  // Evaluation and Conversion
    inline mpz_t& value( void ) const { return myInt->intVal; };
	inline operator double ( void ) const { return mpz_get_d( value() ); };
	string getStringValue(void) const;

  // Assignment
    MP_Int& operator = ( const MP_Int& );
    MP_Int& operator = ( unsigned_long );
    MP_Int& operator = (   signed_long );
    
  // Numeric Operation - Add
    MP_Int operator + ( const MP_Int& ) const;
    MP_Int operator + ( unsigned long ) const;
	inline friend MP_Int operator + ( unsigned long aULong, const MP_Int& anMPInt ) { return (anMPInt + aULong); };
	
  // Numeric Operation - Negation
    MP_Int operator - ( void ) const;

  // Absolute Value
    friend MP_Int abs ( const MP_Int& );
    
  // Numeric Operation - Subtract
    MP_Int operator - ( const MP_Int& ) const;
    MP_Int operator - ( unsigned long ) const;
	inline friend MP_Int operator - ( unsigned long aULong, const MP_Int& anMPInt ) { return (MP_Int(aULong) - anMPInt); };
	
  // Numeric Operation - Multiply
    MP_Int operator * ( const MP_Int& ) const;
    MP_Int operator * ( unsigned long ) const;
	inline friend MP_Int operator * ( unsigned long aULong, const MP_Int& anMPInt ) { return ( anMPInt * aULong ); };
	
  // Numeric Operation - Multiply by power of 2
    MP_Int operator << ( unsigned long ) const;
    
  // Numeric Operation - Division - return Quotient
    MP_Int operator / ( const MP_Int& ) const;
    MP_Int operator / ( unsigned long ) const;
	inline friend MP_Int operator / ( unsigned long aULong, const MP_Int& anMPInt ) { return ( MP_Int(aULong) / anMPInt ); };
	
  // Numeric Operation - Divide by power of 2 - return Quotient
    MP_Int operator >> ( unsigned long ) const;
    
  // Numeric Operation - Division - return Remainder
    MP_Int operator % ( const MP_Int& ) const;
    MP_Int operator % ( unsigned long ) const;
	inline friend MP_Int operator % ( unsigned long aULong, const MP_Int& anMPInt ) { return ( MP_Int(aULong) % anMPInt); };
	
  // Numeric Operation - Divide by power of 2 - return Remainder
    MP_Int shift_remainder ( unsigned long ) const;
	
  // Exponentiation 
    MP_Int operator ^ ( unsigned long ) const;
    MP_Int exponential_remainder( const MP_Int&, const MP_Int& ) const;
    MP_Int exponential_remainder( unsigned long, const MP_Int& ) const;
	
  // Square Root 
    friend MP_Int  sqrt( const MP_Int& );
	friend MP_Int  sqrt_remainder( const MP_Int& );
	friend int     is_perfect_square( const MP_Int& );
	
  // Number Theoretic
    friend int      is_probably_prime( const MP_Int&, int = 25 ); 
	friend MP_Int   greatest_common_divisor( const MP_Int&, const MP_Int& );
	friend MP_Int   greatest_common_divisor( const MP_Int&, unsigned long );
	friend void     external_gcd( MP_Int&, MP_Int&,          const MP_Int&, const MP_Int& );
	friend void     external_gcd( MP_Int&, MP_Int&, MP_Int&, const MP_Int&, const MP_Int& );
	friend MP_Int   modulo_invert( const MP_Int&, const MP_Int& );
	friend MP_Int   jacobi( const MP_Int& );
	friend MP_Int   legendre( const MP_Int& );
	
  // Comparison
    friend int sgn( const MP_Int& );
    friend int cmp( const MP_Int&, const MP_Int& );
    friend int cmp( const MP_Int&, unsigned long );
    friend int cmp( const MP_Int&, signed long );
	inline friend int operator == ( const MP_Int& a, const MP_Int& b) { return (cmp(a,b) == 0) ? 1 : 0 ; };
	inline friend int operator <= ( const MP_Int& a, const MP_Int& b) { return (cmp(a,b) <= 0) ? 1 : 0 ; };
	inline friend int operator >= ( const MP_Int& a, const MP_Int& b) { return (cmp(a,b) >= 0) ? 1 : 0 ; };
	inline friend int operator <  ( const MP_Int& a, const MP_Int& b) { return (cmp(a,b) <  0) ? 1 : 0 ; };
	inline friend int operator >  ( const MP_Int& a, const MP_Int& b) { return (cmp(a,b) >  0) ? 1 : 0 ; };
	inline friend int operator == ( const MP_Int& a, unsigned long b) { return (cmp(a,b) == 0) ? 1 : 0 ; };
	inline friend int operator <= ( const MP_Int& a, unsigned long b) { return (cmp(a,b) <= 0) ? 1 : 0 ; };
	inline friend int operator >= ( const MP_Int& a, unsigned long b) { return (cmp(a,b) >= 0) ? 1 : 0 ; };
	inline friend int operator <  ( const MP_Int& a, unsigned long b) { return (cmp(a,b) <  0) ? 1 : 0 ; };
	inline friend int operator >  ( const MP_Int& a, unsigned long b) { return (cmp(a,b) >  0) ? 1 : 0 ; };
	inline friend int operator == ( const MP_Int& a,   signed long b) { return (cmp(a,b) == 0) ? 1 : 0 ; };
	inline friend int operator <= ( const MP_Int& a,   signed long b) { return (cmp(a,b) <= 0) ? 1 : 0 ; };
	inline friend int operator >= ( const MP_Int& a,   signed long b) { return (cmp(a,b) >= 0) ? 1 : 0 ; };
	inline friend int operator <  ( const MP_Int& a,   signed long b) { return (cmp(a,b) <  0) ? 1 : 0 ; };
	inline friend int operator >  ( const MP_Int& a,   signed long b) { return (cmp(a,b) >  0) ? 1 : 0 ; };
	
  // Assignment with operators
	inline friend MP_Int operator +=  ( MP_Int& A, const MP_Int& B ) { return A = A +  B; };
	inline friend MP_Int operator -=  ( MP_Int& A, const MP_Int& B ) { return A = A -  B; };
	inline friend MP_Int operator *=  ( MP_Int& A, const MP_Int& B ) { return A = A *  B; };
	inline friend MP_Int operator /=  ( MP_Int& A, const MP_Int& B ) { return A = A /  B; };
	inline friend MP_Int operator ^=  ( MP_Int& A, const MP_Int& B ) { return A = A ^  B; };
	inline friend MP_Int operator ^=  ( MP_Int& A, unsigned long B ) { return A = A ^  B; };
	inline friend MP_Int operator >>= ( MP_Int& A, unsigned long B ) { return A = A >> B; };
	inline friend MP_Int operator <<= ( MP_Int& A, unsigned long B ) { return A = A << B; };
	
  // Bit Manipulation
    MP_Int operator & ( const MP_Int& ) const;
    MP_Int operator | ( const MP_Int& ) const;
    MP_Int operator ~ ( void ) const;
	inline unsigned long population_count( void ) const { return mpz_popcount( value() ); };
	friend unsigned long hamming_distance( const MP_Int&, const MP_Int& );
	MP_Int set( unsigned int );
	MP_Int clr( unsigned int );
	friend unsigned long scan_for_0( const MP_Int&, unsigned int = 0);
	friend unsigned long scan_for_1( const MP_Int&, unsigned int = 0);
  
  // Input and Output
    friend istream & operator >> (istream &cbuf,       MP_Int &);
    friend ostream & operator << (ostream &cbuf, const MP_Int &);
	friend void    fprint( FILE* , const MP_Int& ); 
	friend MP_Int  fread( FILE* );
	void writeTo( FILE* ) const; // raw (non-ascii) output to file
	MP_Int& readFrom( FILE* );   // raw (non-ascii) input from file
    
  // Miscellaneous
    size_t sizeInBase( int );
};

#endif

//
//  History:
//    $Log: MP_Int.h,v $
//    Revision 1.1  1997/09/13 23:38:40  jak
//    Initial revision
//
//
