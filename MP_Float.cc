//  MP_Float.cc
//  C++ Class Implementation of the MP_Float Class
//
//  Copyright (C) 1993, John A. Kassebaum
//
//  This software was written by John Kassebaum as a part of his graduate
//  course work at Purdue.  He retains all rights to this software.  
//
//  ---------------------------------------------------------------------------
//  Author(s) and Credits:
//	  This Software was originally written by John Kassebaum as a part of 
//    his graduate classwork at Purdue University.  
//
//  ----------
//  See MP_Float.H for the Interface description and Usage directions.
//  ----------
//
//  Notes: 
//
//  Revision:
//    $Id: MP_Float.cc,v 1.1.1.1 1997/09/13 23:38:40 jak Exp $
//
// ************************************************************
static char rcsid_MP_Float_cc[] = "$Id: MP_Float.cc,v 1.1.1.1 1997/09/13 23:38:40 jak Exp $";


#include "MP_Float.h"

//
//  Use these external classes.
//
#include "MP_Int.h"
#include "MP_Ratio.h"

// ----------------------------------------------------------------
//
// MP_Float::MP_Float_Storage  Implementation
//

const  MP_Float  MP_FLOAT_EPSILON( "1e-107374181", 10 );

// ==========================================
//
// Default Constructor
//
// ==========================================

MP_Float::MP_Float_Storage::MP_Float_Storage():
    reference_count( 1 )
{
	mpf_init( floatVal );
};

// ==========================================
//
// Copy Constructor
//
// ==========================================

MP_Float::MP_Float_Storage::MP_Float_Storage( const MP_Float & aFloat ):
    reference_count( 1 )
{
	mpf_init( floatVal );
	mpf_set( floatVal, aFloat.value() );
};

// ==========================================
//
// Copy Constructor - specified minimum precision
//
// ==========================================

MP_Float::MP_Float_Storage::MP_Float_Storage( const MP_Float & aFloat, unsigned long precision ):
    reference_count( 1 )
{
	mpf_init2( floatVal, precision );
	mpf_set( floatVal, aFloat.value() );
};

// ==========================================
//
// Constructor - initialize with unsigned integer numerator and denominator
//
// ==========================================

MP_Float::MP_Float_Storage::MP_Float_Storage( unsigned long num, unsigned long denom ):
    reference_count( 1 )
{
	mpf_init_set_ui( floatVal, num );
	mpf_div_ui(floatVal, floatVal, denom );
}

// ==========================================
//
// Constructor - initialize with signed integer numerator and unsigned denominator
//
// ==========================================

MP_Float::MP_Float_Storage::MP_Float_Storage( signed long num, unsigned long denom ):
    reference_count( 1 )
{
	mpf_init_set_si( floatVal, num );
	mpf_div_ui(floatVal, floatVal, denom );
};

// ==========================================
//
// Constructor - initialize with double precision floating point number
//
// ==========================================

MP_Float::MP_Float_Storage::MP_Float_Storage( double dnum ):
    reference_count( 1 )
{
	mpf_init_set_d( floatVal, dnum );
};

// ==========================================
//
// Constructor - initialize with MP_Ints for numerator and demoninator 
//
// ==========================================

MP_Float::MP_Float_Storage::MP_Float_Storage( const MP_Int &num, const MP_Int &denom ):
    reference_count( 1 )
{
    mpf_t tempdenom;
	
	mpf_init( tempdenom );
	mpf_set_z( tempdenom, denom.value() );
	
	mpf_init( floatVal );
	mpf_set_z( floatVal, num.value() );
	
	mpf_div( floatVal, floatVal, tempdenom );
};

// ==========================================
//
// Constructor - initialize with MP_Ratio
//
// ==========================================

MP_Float::MP_Float_Storage::MP_Float_Storage( const MP_Ratio &num ):
    reference_count( 1 )
{	
	mpf_init( floatVal );
	mpf_set_q( floatVal, num.value() );
};

// ==========================================
//
// Constructor - Initialize from string
//
// ==========================================

#define SEGFAULT_SETSTR

MP_Float::MP_Float_Storage::MP_Float_Storage( const char *numString, int aBase ):
    reference_count( 1 )
{
#ifdef SEGFAULT_SETSTR
    string aString;
    string aStringMantissa;
    string aStringExponent;
	int    e_indx, at_indx;
	
	MP_Float  divisor;
	MP_Int    mantissa;
	MP_Int    exponent;
	long      l_exponent;
	
#endif
	mpf_init( floatVal );	
#ifndef SEGFAULT_SETSTR
	mpf_set_str( floatVal, numString, aBase );	// this segmentation faults
#endif
	
#ifdef SEGFAULT_SETSTR
  //
  // Obtain Strings for Mantissa and Exponent
  //
	aString = string( numString );
	if( (e_indx = aString.find( 'e', 0 )) != -1 ){
	
	    aStringMantissa = aString.substr( 0, (e_indx - 1) );
	    aStringExponent = aString[ e_indx + 1 ];

	} else if( (at_indx = aString.find( '@', 0 )) != -1 ) {
	
	    aStringMantissa = aString.substr( 0, (at_indx - 1));
	    aStringExponent = aString[ at_indx + 1 ];

	} else { // assume numstring is an integer

		mantissa = MP_Int( aString.c_str(), aBase );
		mpf_set_z( floatVal, mantissa.value() );
		return;
		
	}
	
  //
  // Obtain Mantissa and Exponent
  //
	mantissa = MP_Int( aStringMantissa.c_str(), aBase );
	mpf_set_z( floatVal, mantissa.value() );
	
	if ( aBase < 0 ) {   // exponent in decimal digits ( base 10 )
		exponent = MP_Int( aStringExponent.c_str(), 10 );
	} else {             // exponent in aBase digits (base 2 - 36)
		exponent = MP_Int( aStringExponent.c_str(), aBase );
	}
	
	l_exponent = mpz_get_si( exponent.value() );
	
  //
  // Make a Divisor 
  //
   divisor = MP_Float( (aBase<0)?10:aBase );
   divisor = divisor ^ l_exponent ;
   
   mpf_div( floatVal, floatVal, divisor.value() );
   
#endif
	
};

// ==========================================
//
// Constructor - Initialize from strings for numerator and demoninator
//
// ==========================================

MP_Float::MP_Float_Storage::MP_Float_Storage( const char *numString, const char *denomString, int aBase ):
    reference_count( 1 )
{
    MP_Float tempdenom( denomString, aBase );
	
	mpf_init_set_str( floatVal, numString, aBase );	
	
	mpf_div( floatVal, floatVal, tempdenom.value() );
};


// ==========================================
//
// Destructor
//
// ==========================================

MP_Float::MP_Float_Storage::~MP_Float_Storage( void )
{
	if ( --reference_count <= 1 ) 
		mpf_clear( floatVal );
};

//
// ----------------------------------------------------------------
//

// ----------------------------------------------------------------
//
// MP_Float Implementation
//

// ==========================================
//
// Set Default Precision - Class Method
//
// ==========================================

void MP_Float::setDefaultPrecision( unsigned long precision )
{
    mpf_set_default_prec( precision );
};

// ==========================================
//
// Generate Random Numbers
//     generate random number of at most max_size limbs
//     generate negative numbers when max_size is negative
//
// ==========================================
MP_Float MP_Float::random2 ( mp_size_t max_size, mp_exp_t max_exp )
{
	MP_Float rtn;
	mpf_random2( rtn.value(), max_size, max_exp );
	return rtn;
};  

// ==========================================
//
// Default Constructor
//
// ==========================================

MP_Float::MP_Float(): 
    myFloat( new MP_Float_Storage )
{
    ;
};

// ==========================================
//
// Copy Constructor
//
// ==========================================

MP_Float::MP_Float( const MP_Float & aFloat ):
    myFloat( aFloat.myFloat )
{
    myFloat->inc_refcount();
};

// ==========================================
//
// Copy Constructor - specified precision
//
// ==========================================

MP_Float::MP_Float( const MP_Float & aFloat, unsigned long precision ):
    myFloat( new MP_Float_Storage( aFloat, precision ) )
{
    ;
};

// ==========================================
//
// Constructor - initialize with unsigned integer numerator and denominator
//
// ==========================================

MP_Float::MP_Float( unsigned long num, unsigned long denom ):
    myFloat( new MP_Float_Storage( num, denom ) )
{
    ;
};

// ==========================================
//
// Constructor - initialize with signed integer numerator and unsigned denominator
//
// ==========================================

MP_Float::MP_Float( signed long num, unsigned long denom ):
    myFloat( new MP_Float_Storage( num, denom ) )
{
    ;
};

// ==========================================
//
// Constructor - initialize with signed integer numerator and unsigned denominator
//
// ==========================================

MP_Float::MP_Float( int num, unsigned long denom ):
    myFloat( new MP_Float_Storage( (long) num, denom ) )
{
    ;
};

// ==========================================
//
// Constructor - initialize with double precision floating point number
//
// ==========================================

MP_Float::MP_Float( double dnum ):
    myFloat( new MP_Float_Storage( dnum ) )
{
    ;
};

// ==========================================
//
// Constructor - initialize with MP_Ints for numerator and demoninator 
//
// ==========================================

MP_Float::MP_Float( const MP_Int &num, const MP_Int &denom ):
    myFloat( new MP_Float_Storage( num, denom ) )
{
    ;
};

// ==========================================
//
// Constructor - initialize with MP_Float
//
// ==========================================

MP_Float::MP_Float( const MP_Ratio &num ):
    myFloat( new MP_Float_Storage( num ) )
{
    ;
};

// ==========================================
//
// Constructor - Initialize from String 
//
// ==========================================

MP_Float::MP_Float( const char *numString, int aBase ):
    myFloat( new MP_Float_Storage( numString, aBase ) )
{
    ;
};

// ==========================================
//
// Constructor - Initialize from strings for numerator and demoninator
//
// ==========================================

MP_Float::MP_Float( const char *numString, const char *denomString, int aBase ):
    myFloat( new MP_Float_Storage( numString, denomString, aBase ) )
{
    ;
};

// ==========================================
//
// Destructor
//
// ==========================================

MP_Float::~MP_Float( void )
{
    myFloat->dec_refcount();
};

// ==========================================
//
// Set Precision 
//
// ==========================================

void MP_Float::setPrecision( unsigned long precision )
{
    MP_Float_Storage *newfloatstorage;
	
	newfloatstorage = new MP_Float_Storage( *myFloat );
	
	myFloat->dec_refcount();
	myFloat = newfloatstorage;
    mpf_set_prec( myFloat->floatVal, precision );
};

// ----------------------------------------------------
//
// Common Instance Methods
//

// ==========================================
//
// getStringValue
//
// ==========================================

string MP_Float::getStringValue( void ) const
{
    string rtn;
    char *mantissa;
	char exponent[100];
	mp_exp_t  expon;
	mantissa = mpf_get_str( (char *)0, &expon, 10, 0, value() );
	
	sprintf(exponent, "%ld", expon );
	rtn = string( mantissa ) + string("e") + string( exponent );
	if( !mantissa )  free( mantissa );
	
	return rtn;
};

// ==========================================
//
// Assignment
//
// ==========================================

MP_Float& MP_Float::operator = ( const MP_Float& aFloat )
{
    myFloat->dec_refcount();
    myFloat = aFloat.myFloat;
	myFloat->inc_refcount();
	
	return *this;
};

// ==========================================
//
// Assignment
//
// ==========================================

MP_Float& MP_Float::operator = ( double aDouble )
{
    myFloat->dec_refcount();
    myFloat = new MP_Float_Storage;
    mpf_init_set_d( value(), aDouble );	
	return *this;
};

// ==========================================
//
// Numeric Operation - Add
//
// ==========================================

MP_Float MP_Float::operator + ( const MP_Float& aFloat ) const
{
    MP_Float rtn;
	mpf_add( rtn.value(), value(), aFloat.value() );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Add an unsigned long
//
// ==========================================

MP_Float MP_Float::operator + ( unsigned long aULong ) const
{
    MP_Float rtn;
	mpf_add_ui( rtn.value(), value(), aULong );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Add an unsigned long
//
// ==========================================

MP_Float MP_Float::operator + ( long aLong ) const
{
    MP_Float rtn( aLong );
	mpf_add( rtn.value(), value(), rtn.value() );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Add a double
//
// ==========================================

MP_Float MP_Float::operator + ( double aDouble ) const
{
    MP_Float rtn( aDouble );
	mpf_add( rtn.value(), value(), rtn.value() );
	return rtn;
};


// ==========================================
//
// Numeric Operation - Negation
//
// ==========================================

MP_Float MP_Float::operator - ( void ) const
{
    MP_Float rtn;
	mpf_neg( rtn.value(), value() );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Absolute Value
//
// ==========================================

MP_Float abs ( const MP_Float& aFloat )
{
    MP_Float rtn;
	mpf_abs( rtn.value(), aFloat.value() );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Subtract
//
// ==========================================

MP_Float MP_Float::operator - ( const MP_Float& aFloat) const
{
    MP_Float rtn;
	mpf_sub( rtn.value(), value(), aFloat.value() );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Subtract unsigned long
//
// ==========================================

MP_Float MP_Float::operator - ( unsigned long aULong ) const
{
    MP_Float rtn;
	mpf_sub_ui( rtn.value(), value(), aULong );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Subtract long
//
// ==========================================

MP_Float MP_Float::operator - ( long aLong ) const
{
    MP_Float rtn( aLong );
	mpf_sub( rtn.value(), value(), rtn.value() );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Subtract a double
//
// ==========================================

MP_Float MP_Float::operator - ( double aDouble ) const
{
    MP_Float rtn( aDouble );
	mpf_sub( rtn.value(), value(), rtn.value() );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Multiply
//
// ==========================================

MP_Float MP_Float::operator * ( const MP_Float& aFloat) const
{
    MP_Float rtn;
	mpf_mul( rtn.value(), value(), aFloat.value() );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Multiply by unsigned long
//
// ==========================================

MP_Float MP_Float::operator * ( unsigned long aULong ) const
{
    MP_Float rtn;
	mpf_mul_ui( rtn.value(), value(), aULong );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Multiply by a long
//
// ==========================================

MP_Float MP_Float::operator * ( long aLong ) const
{
    MP_Float rtn( aLong );
	mpf_mul( rtn.value(), value(), rtn.value() );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Multiply by a double
//
// ==========================================

MP_Float MP_Float::operator * ( double aDouble ) const
{
    MP_Float rtn( aDouble );
	mpf_mul( rtn.value(), value(), rtn.value() );
	return rtn;
};


// ==========================================
//
// Numeric Operation - Multiply by power of 2
//
// ==========================================

MP_Float MP_Float::operator << ( unsigned long aULong ) const
{
    MP_Float rtn;
	mpf_mul_2exp( rtn.value(), value(), aULong );
	return rtn;
};


// ==========================================
//
// Numeric Operation - Division - return Quotient
//
// ==========================================

MP_Float MP_Float::operator / ( const MP_Float& aFloat ) const
{
    MP_Float rtn;
	mpf_div( rtn.value(), value(), aFloat.value() );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Divide by unsigned long - return Quotient
//
// ==========================================

MP_Float MP_Float::operator / ( unsigned long aULong ) const
{
    MP_Float rtn;
	mpf_div_ui( rtn.value(), value(), aULong );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Divide by long - return Quotient
//
// ==========================================

MP_Float MP_Float::operator / ( long aLong ) const
{
    MP_Float rtn( aLong );
	mpf_div( rtn.value(), value(), rtn.value() );
	return rtn;
};


// ==========================================
//
// Numeric Operation - Multiply by a double
//
// ==========================================

MP_Float MP_Float::operator / ( double aDouble ) const
{
    MP_Float rtn( aDouble );
	mpf_div( rtn.value(), value(), rtn.value() );
	return rtn;
};


// ==========================================
//
// Numeric Operation - Divide by power of 2 - return Quotient
//
// ==========================================

MP_Float MP_Float::operator >> ( unsigned long aULong ) const
{
    MP_Float rtn;
	mpf_div_2exp( rtn.value(), value(), aULong );
	return rtn;
};


// ==========================================
//
// Exponentiation 
//
// ==========================================

MP_Float MP_Float::operator ^ ( long p ) const
{
    MP_Float result;
	if ( p < 0 ){ // negative exponent
		MP_Float temp( (*this)^(-p) );
		result = MP_Float( 1.0 ) / temp ;
	} else if ( p == 0 ) { // zero exponent
		result = MP_Float( 1.0 );
	} else if ( (p % 2) == 1 ) { // odd
		result = ( (*this) * ( (*this)^(p-1) ));
	} else { // even
		MP_Float temp( (*this)^(p/2) );
		result = ( temp * temp );
	}
	return result;
};

// ==========================================
//
// Exponentiation to fractional power
//
// ==========================================
#define EXP_MAXITERS 10000

MP_Float MP_Float::operator ^ ( const MP_Float& p ) const
{
    MP_Float        last;
    MP_Float        rtn;
	MP_Float        term;
	MP_Float        factor;
	MP_Float        term_k;
	unsigned long   i;
	
    term    = (p * ln( *this ));
	factor  = 1.0;
	term_k  = term;
	
	rtn     = term_k * factor + 1.0;
	
	last    = rtn;
	term_k  = term_k * term;
	factor *= MP_Float( 0.5 );
	rtn    += (term_k * factor);
	
	for( i = 3; !epsilon_equivalent( rtn, last, 2*getPrecision() ) && (i < EXP_MAXITERS); i++ ){
		last     = rtn;
		term_k   = term_k * term;
		factor  *= ( 1.0 / MP_Float( (double) i ) );
		rtn     += (term_k * factor);
	}

	return rtn;
};

// ==========================================
//
// Exponential function of fractional variable 
// Exp( X ) or E^X
//
// ==========================================

MP_Float exp( const MP_Float &X )
{
    MP_Float        last;
    MP_Float        rtn;
	MP_Float        term;
	MP_Float        factor;
	MP_Float        term_k;
	unsigned long   i;
	
    term    = X;
	factor  = 1.0;
	term_k  = term;
	
	rtn     = term_k / factor + 1.0;
	
	last    = rtn;
	term_k  = term_k * term;
	factor *= MP_Float( 2.0 );
	rtn    += (term_k / factor);
	
	for( i = 3; !epsilon_equivalent( rtn, last, 2*X.getPrecision() ) && (i < EXP_MAXITERS); i++ ){
		last    = rtn;
		term_k  = term_k * term;
		factor *= MP_Float( (double) i );
		rtn    += (term_k / factor);
	}

	return rtn;
}

// ==========================================
//
// Natural Logarithm (Base 'e' logarithm)
//
// ==========================================

#define LN_MAXITERS 10000

MP_Float ln ( const MP_Float& X )
{
    MP_Float        last;
    MP_Float        rtn;
	unsigned long   i;
	unsigned long   precision;
	MP_Float        term;
	MP_Float        factor;
	MP_Float        term_k;
	long            sign;
	
	precision =  2*X.getPrecision();
    if ( X > MP_Float(0.5) ){
	    term    = 1.0 - ( 1.0 / X );
		factor  = 1.0;
		term_k  = term;
		
		rtn     = term_k / factor;
		
		last    = rtn;
		term_k  = term_k * term;
		factor  = MP_Float( 2.0 );
		rtn    += (term_k / factor);
		
		for( i = 3; !epsilon_equivalent( rtn, last, precision ) && (i < LN_MAXITERS); i++ ){
		    last    = rtn;
		    term_k  = term_k * term;
			factor  = MP_Float( (double) i );
		    rtn    += (term_k / factor);
		}
	} else if ( X > MP_Float(0.0) ){
	    term    = X - 1.0;
		factor  = 1.0;
		term_k  = term;
		
		sign    = 1;
		rtn     = sign * term_k / factor;
		
		last    = rtn;
		sign    = sign * -1;
		term_k  = term_k * term;
		factor  = MP_Float( 2.0 );
		rtn    += (sign * term_k / factor);
		
		for( i = 3; !epsilon_equivalent( rtn, last, precision ) && (i < LN_MAXITERS) ; i++ ){
		    last    = rtn;
		    sign    = sign * -1;
		    term_k  = term_k * term;
			factor  = MP_Float( (double) i );
		    rtn    += (sign * term_k / factor);
		}
	} else {
	    // Natural Logarithm undefined - 0 or negative argument
		// NOTE - Should Throw and Exception Here!
		rtn = MP_Float(0.0);
	}
	
	return rtn;
};

// ==========================================
//
// Logarithm with Base ( default: comon logarithm - base 10 )
//
// ==========================================

MP_Float log ( const MP_Float& aFloat, const MP_Float& base )
{
    if( base > MP_Float( 0.0 ) )
        return ( ln( aFloat ) / ln ( base ) );
	else 
	    return MP_Float( 0.0 );  // Base must be positive and not zero!
		                         // NOTE - Should Throw and Exception Here!
};

// ==========================================
//
// Square Root 
//
// ==========================================

MP_Float sqrt( const MP_Float& aFloat )
{
    MP_Float rtn;
	mpf_sqrt( rtn.value(), aFloat.value() );
	return rtn;
};

// ==========================================
//
// STATIC Method - Square Root of an unsigned long
//
// ==========================================

MP_Float MP_Float:: sqrt( unsigned long aULong )
{
    MP_Float rtn;
	mpf_sqrt_ui( rtn.value(), aULong );
	return rtn;
};

	
// ==========================================
//
// Input to an MP_Float from a stream
//
// ==========================================

istream & operator >> (istream &cbuf, MP_Float & aFloat)
{
    string  aString;
	
    cbuf >>  aString;
	mpf_set_str( aFloat.value(), aString.c_str(), 10 );	
	
	return cbuf;
};

// ==========================================
//
// Output an MP_Float to a stream
//
// ==========================================

ostream & operator << (ostream &cbuf, const MP_Float & aFloat)
{
    return ( cbuf <<  aFloat.getStringValue() );
};

// ==========================================
//
// Output an MP_Float to a stream file pointer
//
// ==========================================

void     fprint ( FILE* fp, const MP_Float& aFloat)
{
    mpf_out_str( fp, 10, 0, aFloat.value() );
};
 
// ==========================================
//
// Input an MP_Float from a stream file pointer
//
// ==========================================

MP_Float   MP_Float:: fread ( FILE* fp )
{
    MP_Float rtn;
    mpf_inp_str( rtn.value(), fp , 0 );
	return rtn;
};


//
// ----------------------------------------------------------------
//

//
// ----------Instantiate The Following Templates ------------------
//
#pragma implementation
#include <complex>
#include <std/complext.cc>

template operator!=(complex<MP_Float> const &, complex<MP_Float> const &);
template operator*(complex<MP_Float> const &, complex<MP_Float> const &);
template operator-(complex<MP_Float> const &);
template operator+(complex<MP_Float> const &, complex<MP_Float> const &);
template operator==(complex<MP_Float> const &, complex<MP_Float> const &);
template abs(complex<MP_Float> const &);

template operator<<(ostream &, complex<MP_Float> const &);
template operator/(complex<MP_Float> const &, complex<MP_Float> const &);

//
// ----------------------------------------------------------------
//

//
//  History:
//    $Log: MP_Float.cc,v $
//    Revision 1.1.1.1  1997/09/13 23:38:40  jak
//    Restored MathTools to new CVS repository. -jak
//
//
