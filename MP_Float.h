//  MP_Float.h
//  C++ Class Interface Definition of the MP_Float Class
//
//  Copyright (C) 1993, John A. Kassebaum
//
//  This software was written by John Kassebaum as a part of his graduate
//  course work at Purdue.  He retains all rights to this software.  
//
//  *************************  Description  **************************
//  MP_Float Object:
//
//  Public Data:
//
//  Responding Messages:
//
//  Original Messages:
//
//  Side Effects:
//	  None.
//
//  Known Bugs:
//	  None. 
//
//  Author(s) and Credits:
//	  This Software was originally written by John Kassebaum as a part of 
//    his graduate classwork at Purdue University.  
//
//  Revision:
//    $Id: MP_Float.h,v 1.2 1997/09/14 01:01:42 jak Exp $
//
//  ******************************************************************
//
#ifndef _MP_Float_h
#define _MP_Float_h

static char rcsid_MP_Float_h[] = "$Id: MP_Float.h,v 1.2 1997/09/14 01:01:42 jak Exp $";

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
class MP_Ratio;

// ----------------------------------------------------------------
//
// MP_Float Interface Definition
//

// exponent is 1/20th of largest negative exponent value (signed long exponent)
//#define MP_FLOAT_EPSILON      ( MP_Float( "1e-107374181", 10 ) )  
class MP_Float;
extern const  MP_Float  MP_FLOAT_EPSILON;

class MP_Float {

public:
	// ------------------------------------------------------------
	//
	// MP_Float_Storage Interface Definition
	//
	
	class MP_Float_Storage
	{
	  public:
        mpf_t   floatVal;
        int     reference_count;
		
		MP_Float_Storage( void );		
		MP_Float_Storage( const MP_Float & );
		MP_Float_Storage( const MP_Float &, unsigned long );
		MP_Float_Storage( unsigned long, unsigned long = 1 );
		MP_Float_Storage( signed long, unsigned long = 1 );
		MP_Float_Storage( double );
		MP_Float_Storage( const MP_Int &, const MP_Int & = 1 );
		MP_Float_Storage( const MP_Ratio & );
		MP_Float_Storage( const char *, int = 10 );
		MP_Float_Storage( const char *, const char *, int = 10 );
		
		~MP_Float_Storage( void );
		
		inline void dec_refcount( void ) { if( --reference_count == 0 ) delete this; };
		inline void inc_refcount( void ) { reference_count++; };		
	};
	
	// ------------------------------------------------------------

    MP_Float_Storage *myFloat;
	
public:
  // Constructors
	MP_Float( void );		
	MP_Float( const MP_Float & );
	MP_Float( const MP_Float &, unsigned long );
	MP_Float( unsigned long, unsigned long = 1 );
	MP_Float( signed long, unsigned long = 1 );
	MP_Float( int, unsigned long = 1 );
	MP_Float( double );
	MP_Float( const char *, int = 10 );
	MP_Float( const char *, const char *, int = 10 );
	MP_Float( const MP_Int &, const MP_Int & = 1 );
	MP_Float( const MP_Ratio & );
	
  // Destructor
    virtual ~MP_Float( void );

  //
  // Class Methods
  //
  
  // set class precision
    static void setDefaultPrecision( unsigned long );
	
  // generate random numbers
  //     generate random number of at most max_size limbs
  //     generate negative numbers when max_size is negative
    static MP_Float random2 ( mp_size_t /*max_size*/, mp_exp_t /*max_exp*/ );  
	
  //
  // MP_Float Specific Instance Methods
  //
  
  // set/get instance precision
	inline void setPrecision( unsigned long );
	inline unsigned long getPrecision( void ) const { return mpf_get_prec( myFloat->floatVal ); };

  //
  // Common Instance Methods
  //
  
  // Evaluation and Conversion
    inline mpf_t& value( void ) const { return myFloat->floatVal; };
	inline operator double ( void ) { return mpf_get_d( this->value() ); };
	string getStringValue(void) const;

  // Assignment
    MP_Float& operator = ( const MP_Float& );
    MP_Float& operator = ( double );
    
  // Numeric Operation - Add
    MP_Float operator + ( const MP_Float& ) const;
    MP_Float operator + ( unsigned long ) const;
    MP_Float operator + ( long ) const;
    MP_Float operator + ( double ) const;
    inline MP_Float operator + ( int anInt ) const { return ((*this) + (long) anInt); };
	inline friend MP_Float operator + ( unsigned long aULong, const MP_Float& anMPFloat ) { return (anMPFloat + aULong); };
	inline friend MP_Float operator + (          long  aLong, const MP_Float& anMPFloat ) { return (anMPFloat +  aLong); };
	inline friend MP_Float operator + (          int   anInt, const MP_Float& anMPFloat ) { return (anMPFloat + (long) anInt); };
	inline friend MP_Float operator + ( double aDouble, const MP_Float& anMPFloat ) { return (anMPFloat + aDouble); };
	
	
  // Numeric Operation - Negation
    MP_Float operator - ( void ) const;
    
  // Absolute Value
    friend MP_Float abs ( const MP_Float& );
    
  // Numeric Operation - Subtract
    MP_Float operator - ( const MP_Float& ) const;
    MP_Float operator - ( unsigned long ) const;
    MP_Float operator - (          long ) const;
    MP_Float operator - ( double ) const;
    inline MP_Float operator - ( int anInt ) const { return ((*this) - (long) anInt); };
	inline friend MP_Float operator - ( unsigned long aULong, const MP_Float& anMPFloat ) { return (MP_Float(aULong) - anMPFloat); };
	inline friend MP_Float operator - (          long  aLong, const MP_Float& anMPFloat ) { return (MP_Float(aLong)  - anMPFloat); };
	inline friend MP_Float operator - (          int   anInt, const MP_Float& anMPFloat ) { return (anMPFloat - (long) anInt); };
	inline friend MP_Float operator - ( double aDouble, const MP_Float& anMPFloat ) { return (MP_Float(aDouble) - anMPFloat); };
	
  // Numeric Operation - Multiply
    MP_Float operator * ( const MP_Float& ) const;
    MP_Float operator * ( unsigned long ) const;
    MP_Float operator * (          long ) const;
    MP_Float operator * ( double ) const;
    inline MP_Float operator * ( int anInt ) const { return ((*this) * (long) anInt); };
	inline friend MP_Float operator * ( unsigned long aULong, const MP_Float& anMPFloat ) { return ( anMPFloat * aULong ); };
	inline friend MP_Float operator * (          long  aLong, const MP_Float& anMPFloat ) { return ( anMPFloat *  aLong ); };
	inline friend MP_Float operator * (          int   anInt, const MP_Float& anMPFloat ) { return (anMPFloat * (long) anInt); };
	inline friend MP_Float operator * ( double aDouble, const MP_Float& anMPFloat ) { return ( anMPFloat * aDouble ); };
	
  // Numeric Operation - Division - return Quotient
    MP_Float operator / ( const MP_Float& ) const;
    MP_Float operator / ( unsigned long ) const;
    MP_Float operator / (          long ) const;
    MP_Float operator / ( double ) const;
    inline MP_Float operator / ( int anInt ) const { return ((*this) / (long) anInt); };
	inline friend MP_Float operator / ( unsigned long aULong, const MP_Float& anMPFloat ) { return ( MP_Float(aULong) / anMPFloat ); };
	inline friend MP_Float operator / (          long  aLong, const MP_Float& anMPFloat ) { return ( MP_Float(aLong)  / anMPFloat ); };
	inline friend MP_Float operator / (          int   anInt, const MP_Float& anMPFloat ) { return ( MP_Float(anInt)  / anMPFloat ); };
	inline friend MP_Float operator / ( double aDouble, const MP_Float& anMPFloat ) { return ( MP_Float(aDouble) / anMPFloat ); };
	
  // Numeric Operation - Multiply by power of 2
    MP_Float operator << ( unsigned long ) const;
    
  // Numeric Operation - Divide by power of 2 - return Quotient
    MP_Float operator >> ( unsigned long ) const;
    
  // Numeric Operation - Inversion 
    inline friend MP_Float invert ( const MP_Float& aFloat ) { return ( MP_Float( 1.0 ) / aFloat ); };
    
  // Exponentiation 
    MP_Float operator ^ ( long ) const;
	inline MP_Float operator ^ ( int anInt ) const { return ((*this) ^ (long) anInt ); };
    MP_Float operator ^ ( const MP_Float& ) const;
    inline friend MP_Float pow ( const MP_Float &X, const MP_Float &p ) { return (X ^ p); };
	friend MP_Float exp( const MP_Float & );
	
  // Logarithm 
    friend MP_Float ln ( const MP_Float& );
    friend MP_Float log ( const MP_Float&, const MP_Float& = 10.0 );
	
  // Square Root 
    friend MP_Float sqrt( const MP_Float& );
	static MP_Float sqrt( unsigned long );
		
  // Comparison
    inline friend int sgn( const MP_Float& aFloat ) { return mpf_sgn( aFloat.value() ); };
    inline friend int cmp( const MP_Float& A, const MP_Float& B ) { return mpf_cmp( A.value(), B.value() ); };
    inline friend int cmp( const MP_Float& A, unsigned long anULong ) { return mpf_cmp_ui( A.value(), anULong );};
    inline friend int cmp( const MP_Float& A,   signed long anSLong ) { return mpf_cmp_si( A.value(), anSLong );};
    inline friend int cmp( const MP_Float& A,        double aDouble ) 
	{ 
	    MP_Float B( aDouble );
	    return cmp( A, B );
	};
	inline friend int epsilon_equivalent( const MP_Float& A, const MP_Float& B, unsigned long bits) 
	{
	    return mpf_eq( A.value(), B.value(), bits );
	};
	inline friend MP_Float relative_difference( const MP_Float& A, const MP_Float& B ) 
	{ 
	    MP_Float rtn;
	    mpf_reldiff( rtn.value(), A.value(), B.value() );
		return rtn;
	};
	
	inline friend int operator != ( const MP_Float& a, const MP_Float& b) { return (cmp(a,b) != 0) ? 1 : 0 ; };
	inline friend int operator == ( const MP_Float& a, const MP_Float& b) { return (cmp(a,b) == 0) ? 1 : 0 ; };
	inline friend int operator <= ( const MP_Float& a, const MP_Float& b) { return (cmp(a,b) <= 0) ? 1 : 0 ; };
	inline friend int operator >= ( const MP_Float& a, const MP_Float& b) { return (cmp(a,b) >= 0) ? 1 : 0 ; };
	inline friend int operator <  ( const MP_Float& a, const MP_Float& b) { return (cmp(a,b) <  0) ? 1 : 0 ; };
	inline friend int operator >  ( const MP_Float& a, const MP_Float& b) { return (cmp(a,b) >  0) ? 1 : 0 ; };
	inline friend int operator != ( const MP_Float& a,   unsigned long b) { return (cmp(a,b) != 0) ? 1 : 0 ; };
	inline friend int operator == ( const MP_Float& a,   unsigned long b) { return (cmp(a,b) == 0) ? 1 : 0 ; };
	inline friend int operator <= ( const MP_Float& a,   unsigned long b) { return (cmp(a,b) <= 0) ? 1 : 0 ; };
	inline friend int operator >= ( const MP_Float& a,   unsigned long b) { return (cmp(a,b) >= 0) ? 1 : 0 ; };
	inline friend int operator <  ( const MP_Float& a,   unsigned long b) { return (cmp(a,b) <  0) ? 1 : 0 ; };
	inline friend int operator >  ( const MP_Float& a,   unsigned long b) { return (cmp(a,b) >  0) ? 1 : 0 ; };
	inline friend int operator != ( const MP_Float& a,     signed long b) { return (cmp(a,b) != 0) ? 1 : 0 ; };
	inline friend int operator == ( const MP_Float& a,     signed long b) { return (cmp(a,b) == 0) ? 1 : 0 ; };
	inline friend int operator <= ( const MP_Float& a,     signed long b) { return (cmp(a,b) <= 0) ? 1 : 0 ; };
	inline friend int operator >= ( const MP_Float& a,     signed long b) { return (cmp(a,b) >= 0) ? 1 : 0 ; };
	inline friend int operator <  ( const MP_Float& a,     signed long b) { return (cmp(a,b) <  0) ? 1 : 0 ; };
	inline friend int operator >  ( const MP_Float& a,     signed long b) { return (cmp(a,b) >  0) ? 1 : 0 ; };
	inline friend int operator != ( const MP_Float& a,          double b) { return (cmp(a,b) != 0) ? 1 : 0 ; };
	inline friend int operator == ( const MP_Float& a,          double b) { return (cmp(a,b) == 0) ? 1 : 0 ; };
	inline friend int operator <= ( const MP_Float& a,          double b) { return (cmp(a,b) <= 0) ? 1 : 0 ; };
	inline friend int operator >= ( const MP_Float& a,          double b) { return (cmp(a,b) >= 0) ? 1 : 0 ; };
	inline friend int operator <  ( const MP_Float& a,          double b) { return (cmp(a,b) <  0) ? 1 : 0 ; };
	inline friend int operator >  ( const MP_Float& a,          double b) { return (cmp(a,b) >  0) ? 1 : 0 ; };
	
  // Assignment with operators
	inline friend MP_Float operator +=  ( MP_Float& A, const MP_Float& B ) { return A = A +  B; };
	inline friend MP_Float operator -=  ( MP_Float& A, const MP_Float& B ) { return A = A -  B; };
	inline friend MP_Float operator *=  ( MP_Float& A, const MP_Float& B ) { return A = A *  B; };
	inline friend MP_Float operator /=  ( MP_Float& A, const MP_Float& B ) { return A = A /  B; };
	inline friend MP_Float operator ^=  ( MP_Float& A, const MP_Float& B ) { return A = A ^  B; };
	inline friend MP_Float operator +=  ( MP_Float& A,          double B ) { return A = A +  B; };
	inline friend MP_Float operator -=  ( MP_Float& A,          double B ) { return A = A -  B; };
	inline friend MP_Float operator *=  ( MP_Float& A,          double B ) { return A = A *  B; };
	inline friend MP_Float operator /=  ( MP_Float& A,          double B ) { return A = A /  B; };
	inline friend MP_Float operator ^=  ( MP_Float& A,          double B ) { return A = A ^ MP_Float( B ); };
	inline friend MP_Float operator ^=  ( MP_Float& A,           long  B ) { return A = A ^  B; };
	inline friend MP_Float operator >>= ( MP_Float& A,  unsigned long  B ) { return A = A >> B; };
	inline friend MP_Float operator <<= ( MP_Float& A,  unsigned long  B ) { return A = A << B; };
	
  // Input and Output
    friend istream & operator >> (istream &cbuf,       MP_Float &);
    friend ostream & operator << (ostream &cbuf, const MP_Float &);
	friend void       fprint ( FILE* , const MP_Float& ); 
	static MP_Float   fread ( FILE* );
    
};

#endif

//
//  History:
//    $Log: MP_Float.h,v $
//    Revision 1.2  1997/09/14 01:01:42  jak
//    Some purely cosmetic changes. -jak
//
//    Revision 1.1.1.1  1997/09/13 23:38:40  jak
//    Restored MathTools to new CVS repository. -jak
//
//
