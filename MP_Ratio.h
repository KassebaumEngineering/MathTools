//  MP_Ratio.h
//  C++ Class Interface Definition of the MP_Ratio Class
//
//  Copyright (C) 1993, John A. Kassebaum
//
//  This software was written by John Kassebaum as a part of his graduate
//  course work at Purdue.  He retains all rights to this software.  
//
//  *************************  Description  **************************
//  MP_Ratio Object:
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
//  Author(s) and Credits:
//	  This Software was originally written by John Kassebaum as a part of 
//    his graduate classwork at Purdue University.  
//
//  Revision:
//    $Id: MP_Ratio.h,v 1.2 1997/09/14 01:01:51 jak Exp $
//
//  ******************************************************************
//
#ifndef _MP_Ratio_h
#define _MP_Ratio_h

static char rcsid_MP_Ratio_h[] = "$Id: MP_Ratio.h,v 1.2 1997/09/14 01:01:51 jak Exp $";

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
class MP_Int;
class MP_Float;

// ----------------------------------------------------------------
//
// MP_Ratio Interface Definition
//
class MP_Ratio;
extern const  MP_Ratio  MP_RATIO_EPSILON;

class MP_Ratio {

public:
	// ------------------------------------------------------------
	//
	// MP_Ratio_Storage Interface Definition
	//
	
	class MP_Ratio_Storage
	{
	  public:
        mpq_t   ratio;    // is zero if number is infinite
        int     reference_count;
		int     infinity; // -1 -> neg infinity, +1 -> pos infinity, 0 - not infinite
		
		MP_Ratio_Storage( void );		
		MP_Ratio_Storage( const MP_Ratio & );
		MP_Ratio_Storage( unsigned long, unsigned long = 1 );
		MP_Ratio_Storage( signed long, unsigned long = 1 );
		MP_Ratio_Storage( double );
		MP_Ratio_Storage( const MP_Int &, const MP_Int & = 1 );
		MP_Ratio_Storage( const MP_Float & );
		MP_Ratio_Storage( char *, int = 10 );
		MP_Ratio_Storage( char *, char *, int = 10 );
		
		~MP_Ratio_Storage( void );
		
		inline void dec_refcount( void ) { if( --reference_count == 0 ) delete this; };
		inline void inc_refcount( void ) { reference_count++; };		
		
		inline int & is_infinite( void ) { return infinity; };
	};
	
	// ------------------------------------------------------------

    MP_Ratio_Storage *myRatio;
	
	static MP_Ratio epsilon;
	static unsigned long max_iterations;
	
public:
  // Constructors
	MP_Ratio( void );		
	MP_Ratio( const MP_Ratio & );
	MP_Ratio( unsigned long, unsigned long = 1 );
	MP_Ratio( signed long, unsigned long = 1 );
	MP_Ratio( int, unsigned long = 1 );
	MP_Ratio( double );
	MP_Ratio( const MP_Int &, const MP_Int & = 1 );
	MP_Ratio( const MP_Float & );
	MP_Ratio( char *, int = 10 );
	MP_Ratio( char *, char *, int = 10 );
	
  // Destructor
    virtual ~MP_Ratio( void );

  // MP_Ratio Specific Instance Methods
    void       setNumerator( const MP_Int & );
    void     setDenominator( const MP_Int & );
    MP_Int     getNumerator( void );
    MP_Int   getDenominator( void );
	
	inline int & is_infinite( void ) const { return myRatio->infinity; };
	
	static inline void setEpsilon( const MP_Ratio & aRatio ) { epsilon = aRatio; };
	static inline MP_Ratio & Epsilon( void ) { return epsilon; };
	static inline void setMaxIterations( unsigned long aValue ) { max_iterations = aValue; };
	static inline unsigned long MaxIterations( void ) { return max_iterations; };
  
  // Common Instance Methods
    inline mpq_t& value( void ) const { return myRatio->ratio; };
	inline operator double ( void ) const { return mpq_get_d( value() ); };
	string getStringValue(void) const;

  // Assignment
    MP_Ratio& operator = ( const MP_Ratio& );
    inline MP_Ratio& operator = (        double A ) { return (*this = MP_Ratio( A )); };
    inline MP_Ratio& operator = (           int A ) { return (*this = MP_Ratio( A )); };
    inline MP_Ratio& operator = (   signed long A ) { return (*this = MP_Ratio( A )); };
    inline MP_Ratio& operator = ( unsigned long A ) { return (*this = MP_Ratio( A )); };
    
  // Numeric Operation - Add
    MP_Ratio operator + ( const MP_Ratio& ) const;
    inline MP_Ratio operator + (        double A ) const { return (*this + MP_Ratio( A )); };
    inline MP_Ratio operator + (           int A ) const { return (*this + MP_Ratio( A )); };
    inline MP_Ratio operator + (   signed long A ) const { return (*this + MP_Ratio( A )); };
    inline MP_Ratio operator + ( unsigned long A ) const { return (*this + MP_Ratio( A )); };
	inline friend MP_Ratio operator + (        double A, const MP_Ratio& anMPRatio ) { return (anMPRatio + A); };
	inline friend MP_Ratio operator + (           int A, const MP_Ratio& anMPRatio ) { return (anMPRatio + A); };
	inline friend MP_Ratio operator + (   signed long A, const MP_Ratio& anMPRatio ) { return (anMPRatio + A); };
	inline friend MP_Ratio operator + ( unsigned long A, const MP_Ratio& anMPRatio ) { return (anMPRatio + A); };
	
  // Numeric Operation - Negation
    MP_Ratio operator - ( void ) const;
    
  // Numeric Operation - Subtract
    MP_Ratio operator - ( const MP_Ratio& ) const;
    inline MP_Ratio operator - (        double A ) const { return (*this - MP_Ratio( A )); };
    inline MP_Ratio operator - (           int A ) const { return (*this - MP_Ratio( A )); };
    inline MP_Ratio operator - ( unsigned long A ) const { return (*this - MP_Ratio( A )); };
    inline MP_Ratio operator - (   signed long A ) const { return (*this - MP_Ratio( A )); };
	inline friend MP_Ratio operator - (        double A, const MP_Ratio& anMPRatio ) { return ( MP_Ratio( A ) - anMPRatio ); };
	inline friend MP_Ratio operator - (           int A, const MP_Ratio& anMPRatio ) { return ( MP_Ratio( A ) - anMPRatio ); };
	inline friend MP_Ratio operator - (   signed long A, const MP_Ratio& anMPRatio ) { return ( MP_Ratio( A ) - anMPRatio ); };
	inline friend MP_Ratio operator - ( unsigned long A, const MP_Ratio& anMPRatio ) { return ( MP_Ratio( A ) - anMPRatio ); };
	
  // Numeric Operation - Multiply
    MP_Ratio operator * ( const MP_Ratio& ) const;
    inline MP_Ratio operator * (        double A ) const { return (*this * MP_Ratio( A )); };
    inline MP_Ratio operator * (           int A ) const { return (*this * MP_Ratio( A )); };
    inline MP_Ratio operator * ( unsigned long A ) const { return (*this * MP_Ratio( A )); };
    inline MP_Ratio operator * (   signed long A ) const { return (*this * MP_Ratio( A )); };
	inline friend MP_Ratio operator * (        double A, const MP_Ratio& anMPRatio ) { return (anMPRatio * A); };
	inline friend MP_Ratio operator * (           int A, const MP_Ratio& anMPRatio ) { return (anMPRatio * A); };
	inline friend MP_Ratio operator * (   signed long A, const MP_Ratio& anMPRatio ) { return (anMPRatio * A); };
	inline friend MP_Ratio operator * ( unsigned long A, const MP_Ratio& anMPRatio ) { return (anMPRatio * A); };
	
  // Numeric Operation - Multiply by power of 2
    MP_Ratio operator << ( unsigned long ) const;
    
  // Numeric Operation - Division - return Quotient
    MP_Ratio operator / ( const MP_Ratio& ) const;
    inline MP_Ratio operator / (        double A ) const { return (*this / MP_Ratio( A )); };
    inline MP_Ratio operator / (           int A ) const { return (*this / MP_Ratio( A )); };
    inline MP_Ratio operator / ( unsigned long A ) const { return (*this / MP_Ratio( A )); };
    inline MP_Ratio operator / (   signed long A ) const { return (*this / MP_Ratio( A )); };
	inline friend MP_Ratio operator / (        double A, const MP_Ratio& anMPRatio ) { return ( MP_Ratio(A) / anMPRatio ); };
	inline friend MP_Ratio operator / (           int A, const MP_Ratio& anMPRatio ) { return ( MP_Ratio(A) / anMPRatio ); };
	inline friend MP_Ratio operator / (   signed long A, const MP_Ratio& anMPRatio ) { return ( MP_Ratio(A) / anMPRatio ); };
	inline friend MP_Ratio operator / ( unsigned long A, const MP_Ratio& anMPRatio ) { return ( MP_Ratio(A) / anMPRatio ); };
	
  // Numeric Operation - Inversion 
    inline friend MP_Ratio invert ( const MP_Ratio& aRatio ) { return ( MP_Ratio( 1 ) / aRatio ); };
    
  // Exponentiation 
    friend MP_Ratio pow ( const MP_Ratio &, const MP_Ratio &, const MP_Ratio & = epsilon );
    MP_Ratio operator ^ ( const MP_Ratio & ) const;
    MP_Ratio operator ^ ( unsigned long ) const;
	friend MP_Ratio exp( const MP_Ratio &, const MP_Ratio & = epsilon );
	
  // Exponentiation 
    friend MP_Ratio ln ( const MP_Ratio&, const MP_Ratio & = epsilon );
    friend MP_Ratio log ( const MP_Ratio&, const MP_Ratio& = 10, const MP_Ratio & = epsilon );

  // Square Root 
    inline friend MP_Ratio sqrt( const MP_Ratio& aRatio ) { return pow( aRatio, MP_Ratio(1,2) ); };
		
  // Comparison
    inline friend int sgn( const MP_Ratio& aRatio ) { return mpq_sgn( aRatio.value() ); };
    inline friend int cmp( const MP_Ratio& A, const MP_Ratio& B ) { return mpq_cmp( A.value(), B.value() ); };
	
	inline friend int operator == ( const MP_Ratio& a, const MP_Ratio& b) { return mpq_equal( a.value(), b.value() ); };
	inline friend int operator == ( const MP_Ratio& a,          double b) { return ( a == MP_Ratio( b ) ); };
	inline friend int operator == ( const MP_Ratio& a,             int b) { return ( a == MP_Ratio( b ) ); };
	inline friend int operator == ( const MP_Ratio& a,     signed long b) { return ( a == MP_Ratio( b ) ); };
	inline friend int operator == ( const MP_Ratio& a,   unsigned long b) { return ( a == MP_Ratio( b ) ); };
	inline friend int operator == (          double b, const MP_Ratio& a) { return ( a == MP_Ratio( b ) ); };
	inline friend int operator == (             int b, const MP_Ratio& a) { return ( a == MP_Ratio( b ) ); };
	inline friend int operator == (     signed long b, const MP_Ratio& a) { return ( a == MP_Ratio( b ) ); };
	inline friend int operator == (   unsigned long b, const MP_Ratio& a) { return ( a == MP_Ratio( b ) ); };
	
	inline friend int operator != ( const MP_Ratio& a, const MP_Ratio& b) { return !mpq_equal( a.value(), b.value() ); };
	inline friend int operator != ( const MP_Ratio& a,          double b) { return ( a != MP_Ratio( b ) ); };
	inline friend int operator != ( const MP_Ratio& a,             int b) { return ( a != MP_Ratio( b ) ); };
	inline friend int operator != ( const MP_Ratio& a,     signed long b) { return ( a != MP_Ratio( b ) ); };
	inline friend int operator != ( const MP_Ratio& a,   unsigned long b) { return ( a != MP_Ratio( b ) ); };
	inline friend int operator != (          double b, const MP_Ratio& a) { return ( a != MP_Ratio( b ) ); };
	inline friend int operator != (             int b, const MP_Ratio& a) { return ( a != MP_Ratio( b ) ); };
	inline friend int operator != (     signed long b, const MP_Ratio& a) { return ( a != MP_Ratio( b ) ); };
	inline friend int operator != (   unsigned long b, const MP_Ratio& a) { return ( a != MP_Ratio( b ) ); };

	inline friend int operator <= ( const MP_Ratio& a, const MP_Ratio& b) { return (cmp(a,b) <= 0) ? 1 : 0 ; };
	inline friend int operator <= ( const MP_Ratio& a,          double b) { return ( a <= MP_Ratio( b ) ); };
	inline friend int operator <= ( const MP_Ratio& a,             int b) { return ( a <= MP_Ratio( b ) ); };
	inline friend int operator <= ( const MP_Ratio& a,     signed long b) { return ( a <= MP_Ratio( b ) ); };
	inline friend int operator <= ( const MP_Ratio& a,   unsigned long b) { return ( a <= MP_Ratio( b ) ); };
	inline friend int operator <= (          double a, const MP_Ratio& b) { return ( MP_Ratio( a ) <= b ); };
	inline friend int operator <= (             int a, const MP_Ratio& b) { return ( MP_Ratio( a ) <= b ); };
	inline friend int operator <= (     signed long a, const MP_Ratio& b) { return ( MP_Ratio( a ) <= b ); };
	inline friend int operator <= (   unsigned long a, const MP_Ratio& b) { return ( MP_Ratio( a ) <= b ); };

	inline friend int operator >= ( const MP_Ratio& a, const MP_Ratio& b) { return (cmp(a,b) >= 0) ? 1 : 0 ; };
	inline friend int operator >= ( const MP_Ratio& a,          double b) { return ( a >= MP_Ratio( b ) ); };
	inline friend int operator >= ( const MP_Ratio& a,             int b) { return ( a >= MP_Ratio( b ) ); };
	inline friend int operator >= ( const MP_Ratio& a,     signed long b) { return ( a >= MP_Ratio( b ) ); };
	inline friend int operator >= ( const MP_Ratio& a,   unsigned long b) { return ( a >= MP_Ratio( b ) ); };
	inline friend int operator >= (          double a, const MP_Ratio& b) { return ( MP_Ratio( a ) >= b ); };
	inline friend int operator >= (             int a, const MP_Ratio& b) { return ( MP_Ratio( a ) >= b ); };
	inline friend int operator >= (     signed long a, const MP_Ratio& b) { return ( MP_Ratio( a ) >= b ); };
	inline friend int operator >= (   unsigned long a, const MP_Ratio& b) { return ( MP_Ratio( a ) >= b ); };

	inline friend int operator <  ( const MP_Ratio& a, const MP_Ratio& b) { return (cmp(a,b) <  0) ? 1 : 0 ; };
	inline friend int operator < ( const MP_Ratio& a,          double b) { return ( a < MP_Ratio( b ) ); };
	inline friend int operator < ( const MP_Ratio& a,             int b) { return ( a < MP_Ratio( b ) ); };
	inline friend int operator < ( const MP_Ratio& a,     signed long b) { return ( a < MP_Ratio( b ) ); };
	inline friend int operator < ( const MP_Ratio& a,   unsigned long b) { return ( a < MP_Ratio( b ) ); };
	inline friend int operator < (          double a, const MP_Ratio& b) { return ( MP_Ratio( a ) < b ); };
	inline friend int operator < (             int a, const MP_Ratio& b) { return ( MP_Ratio( a ) < b ); };
	inline friend int operator < (     signed long a, const MP_Ratio& b) { return ( MP_Ratio( a ) < b ); };
	inline friend int operator < (   unsigned long a, const MP_Ratio& b) { return ( MP_Ratio( a ) < b ); };
	
	inline friend int operator >  ( const MP_Ratio& a, const MP_Ratio& b) { return (cmp(a,b) >  0) ? 1 : 0 ; };
	inline friend int operator > ( const MP_Ratio& a,          double b) { return ( a > MP_Ratio( b ) ); };
	inline friend int operator > ( const MP_Ratio& a,             int b) { return ( a > MP_Ratio( b ) ); };
	inline friend int operator > ( const MP_Ratio& a,     signed long b) { return ( a > MP_Ratio( b ) ); };
	inline friend int operator > ( const MP_Ratio& a,   unsigned long b) { return ( a > MP_Ratio( b ) ); };
	inline friend int operator > (          double a, const MP_Ratio& b) { return ( MP_Ratio( a ) > b ); };
	inline friend int operator > (             int a, const MP_Ratio& b) { return ( MP_Ratio( a ) > b ); };
	inline friend int operator > (     signed long a, const MP_Ratio& b) { return ( MP_Ratio( a ) > b ); };
	inline friend int operator > (   unsigned long a, const MP_Ratio& b) { return ( MP_Ratio( a ) > b ); };
	
  // Assignment with operators
	inline friend MP_Ratio operator +=  ( MP_Ratio& A, const MP_Ratio& B ) { return A = A +  B; };
	inline friend MP_Ratio operator +=  ( MP_Ratio& A,          double B ) { return A = A +  B; };
	inline friend MP_Ratio operator +=  ( MP_Ratio& A,             int B ) { return A = A +  B; };
	inline friend MP_Ratio operator +=  ( MP_Ratio& A,   unsigned long B ) { return A = A +  B; };
	inline friend MP_Ratio operator +=  ( MP_Ratio& A,     signed long B ) { return A = A +  B; };
	
	inline friend MP_Ratio operator -=  ( MP_Ratio& A, const MP_Ratio& B ) { return A = A -  B; };
	inline friend MP_Ratio operator -=  ( MP_Ratio& A,          double B ) { return A = A -  B; };
	inline friend MP_Ratio operator -=  ( MP_Ratio& A,             int B ) { return A = A -  B; };
	inline friend MP_Ratio operator -=  ( MP_Ratio& A,   unsigned long B ) { return A = A -  B; };
	inline friend MP_Ratio operator -=  ( MP_Ratio& A,     signed long B ) { return A = A -  B; };
	
	inline friend MP_Ratio operator *=  ( MP_Ratio& A, const MP_Ratio& B ) { return A = A *  B; };
	inline friend MP_Ratio operator *=  ( MP_Ratio& A,          double B ) { return A = A *  B; };
	inline friend MP_Ratio operator *=  ( MP_Ratio& A,             int B ) { return A = A *  B; };
	inline friend MP_Ratio operator *=  ( MP_Ratio& A,   unsigned long B ) { return A = A *  B; };
	inline friend MP_Ratio operator *=  ( MP_Ratio& A,     signed long B ) { return A = A *  B; };
	
	inline friend MP_Ratio operator /=  ( MP_Ratio& A, const MP_Ratio& B ) { return A = A /  B; };
	inline friend MP_Ratio operator /=  ( MP_Ratio& A,          double B ) { return A = A /  B; };
	inline friend MP_Ratio operator /=  ( MP_Ratio& A,             int B ) { return A = A /  B; };
	inline friend MP_Ratio operator /=  ( MP_Ratio& A,   unsigned long B ) { return A = A /  B; };
	inline friend MP_Ratio operator /=  ( MP_Ratio& A,     signed long B ) { return A = A /  B; };
	
    inline friend MP_Ratio operator ^=  ( MP_Ratio& A, const MP_Ratio& B ) { return A = A ^  B; };
	inline friend MP_Ratio operator ^=  ( MP_Ratio& A,  unsigned long  B ) { return A = A ^  B; };
	
  // Absolute Value
    inline friend MP_Ratio abs( const MP_Ratio& aRatio ) { return (sgn( aRatio )>=0)?aRatio:-aRatio; };

  // Input and Output
    friend istream & operator >> (istream &cbuf,       MP_Ratio &);
    friend ostream & operator << (ostream &cbuf, const MP_Ratio &);
  //  friend void       fprint ( FILE* , const MP_Ratio& ); 
  //  static MP_Ratio   fread ( FILE* );
    

};


#endif

//
//  History:
//    $Log: MP_Ratio.h,v $
//    Revision 1.2  1997/09/14 01:01:51  jak
//    Some purely cosmetic changes. -jak
//
//    Revision 1.1.1.1  1997/09/13 23:38:41  jak
//    Restored MathTools to new CVS repository. -jak
//
//