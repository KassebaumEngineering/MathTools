//  MP_Ratio.cc
//  C++ Class Implementation of the MP_Ratio Class
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
//  See MP_Ratio.H for the Interface description and Usage directions.
//  ----------
//
// Notes: 
//
//  Revision:
//    $Id: MP_Ratio.cc,v 1.1.1.1 1997/09/13 23:38:41 jak Exp $
//
//  ************************************************************
static char rcsid_MP_Ratio_cc[] = "$Id: MP_Ratio.cc,v 1.1.1.1 1997/09/13 23:38:41 jak Exp $";

#include <float.h>
#ifdef HPPA
#include <values.h>
#endif
#include <math.h>

#include "MP_Ratio.h"

//
//  Use these external classes.
//
#include "MP_Int.h"
#include "MP_Ratio.h"

// ----------------------------------------------------------------
//
// MP_Ratio::MP_Ratio_Storage  Implementation
//

const  MP_Ratio  MP_RATIO_EPSILON( 0 );

// Epsilon to be used for Power Series convergence formulas
MP_Ratio MP_Ratio:: epsilon( MINFLOAT ); // or MP_Ratio MP_Ratio:: epsilon( DBL_EPSILON );
unsigned long MP_Ratio::max_iterations = 10000;

// ==========================================
//
// Default Constructor
//
// ==========================================

MP_Ratio::MP_Ratio_Storage::MP_Ratio_Storage():
    reference_count( 1 ), infinity( 0 )
{
	mpq_init( ratio );
	mpq_set_ui( ratio, (unsigned long) 0, (unsigned long) 1 );
	mpq_canonicalize( ratio );
};

// ==========================================
//
// Copy Constructor
//
// ==========================================

MP_Ratio::MP_Ratio_Storage::MP_Ratio_Storage( const MP_Ratio & aRatio ):
    reference_count( 1 )
{
	mpq_init( ratio );
	mpq_set( ratio, aRatio.value() );
	is_infinite() = aRatio.is_infinite();
};

// ==========================================
//
// Constructor - initialize with unsigned integer numerator and denominator
//
// ==========================================

MP_Ratio::MP_Ratio_Storage::MP_Ratio_Storage( unsigned long num, unsigned long denom ):
    reference_count( 1 ), infinity( 0 )
{
	mpq_init( ratio );
	mpq_set_ui( ratio, num, denom );
	mpq_canonicalize( ratio );
};

// ==========================================
//
// Constructor - initialize with signed integer numerator and unsigned denominator
//
// ==========================================

MP_Ratio::MP_Ratio_Storage::MP_Ratio_Storage( signed long num, unsigned long denom ):
    reference_count( 1 ), infinity( 0 )
{
	mpq_init( ratio );
	mpq_set_si( ratio, num, denom );
	mpq_canonicalize( ratio );
};

// ==========================================
//
// Constructor - initialize with double precision floating point number
//
// ==========================================

MP_Ratio::MP_Ratio_Storage::MP_Ratio_Storage( double dnum ):
    reference_count( 1 ), infinity( 0 )
{
    int exponent;
    double mantissa;
    mpz_t numerator;
	mpz_t denominator;

    mantissa = frexp( dnum, &exponent);
	
	switch ( (exponent <= 0)?1:0 ){
		case 1:  // zero or negative exponent
		    mpz_init_set_d(   numerator, ldexp( mantissa,  DBL_MANT_DIG + 1 ));
			mpz_init_set_d( denominator, ldexp(  1.0,  -exponent + DBL_MANT_DIG + 1));
		    break;
		case 0:  // positive exponent
		    mpz_init_set_d(   numerator, ldexp( mantissa,  DBL_MANT_DIG + exponent + 1 ));
			mpz_init_set_d( denominator, ldexp(  1.0,  DBL_MANT_DIG + 1 ));
		    break;
	}

	mpq_init( ratio );
	mpq_set_num( ratio,   numerator ); 
	mpq_set_den( ratio, denominator );
    mpq_canonicalize( ratio );
};

// ==========================================
//
// Constructor - initialize with MP_Ints for numerator and demoninator 
//
// ==========================================

MP_Ratio::MP_Ratio_Storage::MP_Ratio_Storage( const MP_Int &num, const MP_Int &denom ):
    reference_count( 1 ), infinity( 0 )
{
	mpq_init( ratio );
	mpq_set_num( ratio, num.value() );
	mpq_set_den( ratio, denom.value() );
	mpq_canonicalize( ratio );
};

// ==========================================
//
// Constructor - initialize with MP_Float
//
// ==========================================

MP_Ratio::MP_Ratio_Storage::MP_Ratio_Storage( const MP_Float &num ):
    reference_count( 1 ), infinity( 0 )
{
    MP_Int temp( num );
	
	mpq_init( ratio );
	mpq_set_z( ratio, temp.value() );
};

// ==========================================
//
// Constructor - Initialize from string for numerator
//
// ==========================================

MP_Ratio::MP_Ratio_Storage::MP_Ratio_Storage( char *numString, int aBase ):
    reference_count( 1 ), infinity( 0 )
{
    string  aString( numString );
    MP_Int  numerator;
    MP_Int  denominator( (unsigned long) 1 );
	int div_indx;
	
	mpq_init( ratio );

	numerator = MP_Int( aString.c_str(), aBase );
	mpq_set_num( ratio, numerator.value() );
	
	if( (div_indx = aString.find( '/', 0 )) != -1 ){
	    aString = aString[ div_indx + 1 ] ;
	    denominator = MP_Int( aString.c_str(), aBase );
	    mpq_set_den( ratio, denominator.value() );
	} // else - numerator only
	
	mpq_canonicalize( ratio );
};

// ==========================================
//
// Constructor - Initialize from strings for numerator and demoninator
//
// ==========================================

MP_Ratio::MP_Ratio_Storage::MP_Ratio_Storage( char *numString, char *denomString, int aBase ):
    reference_count( 1 ), infinity( 0 )
{
    MP_Int tempnum( numString, aBase );
    MP_Int tempdenom( denomString, aBase );
	
	mpq_init( ratio );
	mpq_set_num( ratio, tempnum.value() );	
	mpq_set_den( ratio, tempdenom.value() );	
	mpq_canonicalize( ratio );
};


// ==========================================
//
// Destructor
//
// ==========================================

MP_Ratio::MP_Ratio_Storage::~MP_Ratio_Storage( void )
{
	if ( --reference_count <= 1 ) 
		mpq_clear( ratio );
};

//
// ----------------------------------------------------------------
//

// ----------------------------------------------------------------
//
// MP_Ratio Implementation
//

// ==========================================
//
// Default Constructor
//
// ==========================================

MP_Ratio::MP_Ratio(): 
    myRatio( new MP_Ratio_Storage )
{
    ;
};

// ==========================================
//
// Copy Constructor
//
// ==========================================

MP_Ratio::MP_Ratio( const MP_Ratio & aRatio ):
    myRatio( aRatio.myRatio )
{
    myRatio->inc_refcount();
};

// ==========================================
//
// Constructor - initialize with unsigned integer numerator and denominator
//
// ==========================================

MP_Ratio::MP_Ratio( unsigned long num, unsigned long denom ):
    myRatio( new MP_Ratio_Storage( num, denom ) )
{
    ;
};

// ==========================================
//
// Constructor - initialize with signed integer numerator and unsigned denominator
//
// ==========================================

MP_Ratio::MP_Ratio( signed long num, unsigned long denom ):
    myRatio( new MP_Ratio_Storage( num, denom ) )
{
    ;
};

// ==========================================
//
// Constructor - initialize with signed integer numerator and unsigned denominator
//
// ==========================================

MP_Ratio::MP_Ratio( int num, unsigned long denom ):
    myRatio( new MP_Ratio_Storage( (signed long) num, denom ) )
{
    ;
};

// ==========================================
//
// Constructor - initialize with double precision floating point number
//
// ==========================================

MP_Ratio::MP_Ratio( double dnum ):
    myRatio( new MP_Ratio_Storage( dnum ) )
{
    ;
};

// ==========================================
//
// Constructor - initialize with MP_Ints for numerator and demoninator 
//
// ==========================================

MP_Ratio::MP_Ratio( const MP_Int &num, const MP_Int &denom ):
    myRatio( new MP_Ratio_Storage( num, denom ) )
{
    ;
};

// ==========================================
//
// Constructor - initialize with MP_Float
//
// ==========================================

MP_Ratio::MP_Ratio( const MP_Float &num ):
    myRatio( new MP_Ratio_Storage( num ) )
{
    ;
};

// ==========================================
//
// Constructor - Initialize from string for numerator
//
// ==========================================

MP_Ratio::MP_Ratio( char *numString, int aBase ):
    myRatio( new MP_Ratio_Storage( numString, aBase ) )
{
    ;
};

// ==========================================
//
// Constructor - Initialize from strings for numerator and demoninator
//
// ==========================================

MP_Ratio::MP_Ratio( char *numString, char *denomString, int aBase ):
    myRatio( new MP_Ratio_Storage( numString, denomString, aBase ) )
{
    ;
};

// ==========================================
//
// Destructor
//
// ==========================================

MP_Ratio::~MP_Ratio( void )
{
    myRatio->dec_refcount();
};

// ==========================================
//
// Destructor
//
// ==========================================

// ----------------------------------------------------
//
// MP_Ratio Specific Instance Methods
//

// ==========================================
//
// setNumerator
//
// ==========================================

void MP_Ratio::setNumerator( const MP_Int &anInt )
{
    MP_Ratio_Storage *oldp;
	oldp = myRatio;
    myRatio = new MP_Ratio_Storage( *this );
	oldp->dec_refcount();
	
	is_infinite() = 0;
	mpq_set_num( myRatio->ratio, anInt.value() );
	mpq_canonicalize( myRatio->ratio );
};

// ==========================================
//
// setDenominator
//
// ==========================================

void MP_Ratio::setDenominator( const MP_Int &anInt )
{
    MP_Ratio_Storage *oldp;
	oldp = myRatio;
    myRatio = new MP_Ratio_Storage( *this );
	oldp->dec_refcount();
	
	is_infinite() = 0;
	mpq_set_den( myRatio->ratio, anInt.value() );
	mpq_canonicalize( myRatio->ratio );
};

// ==========================================
//
// getNumerator
//
// ==========================================

MP_Int MP_Ratio::getNumerator()
{
    MP_Int result;
	mpq_get_num( result.value(), value() );
	return result;
};

// ==========================================
//
// getDenominator
//
// ==========================================

MP_Int MP_Ratio::getDenominator()
{
    MP_Int result;
	mpq_get_den( result.value(), value() );
	return result;
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

string MP_Ratio::getStringValue( void ) const
{
    string rtn;
	int inf;
	
	if( (inf = is_infinite()) != 0 ){
	    if( inf < 0 )
		    rtn = string( "-Infinity" );
		else 
		    rtn = string( "+Infinity" );
	} else {
		char numerator[ mpz_sizeinbase( mpq_numref(value()),10) + 2 ];
		char denominator[ mpz_sizeinbase( mpq_denref(value()),10) + 2 ];
		
		mpz_get_str( numerator, 10, mpq_numref(value()) );
		mpz_get_str( denominator, 10, mpq_denref(value()) );
		
		rtn = (string( numerator ) + string("/") + string( denominator ));
	}
	
	return rtn;
};

// ==========================================
//
// Assignment
//
// ==========================================

MP_Ratio& MP_Ratio::operator = ( const MP_Ratio& aRatio )
{
    myRatio->dec_refcount();
    myRatio = aRatio.myRatio;
	myRatio->inc_refcount();
	
	return *this;
};

// ==========================================
//
// Numeric Operation - Add
//
// ==========================================

MP_Ratio MP_Ratio::operator + ( const MP_Ratio& aRatio ) const
{
    MP_Ratio rtn( 0 );
	
	if( is_infinite() != 0 ){
		rtn.is_infinite() = is_infinite();
	} else if( aRatio.is_infinite() != 0 ){
	    rtn.is_infinite() = aRatio.is_infinite();
	} else {
	    mpq_add( rtn.value(), value(), aRatio.value() );
	}
	return rtn;
};

// ==========================================
//
// Numeric Operation - Negation
//
// ==========================================

MP_Ratio MP_Ratio::operator - ( void ) const
{
    MP_Ratio rtn( 0 );
	if( is_infinite() != 0 ){
	    rtn.is_infinite() = -is_infinite();
	} else {
	    mpq_neg( rtn.value(), value() );
	}
	return rtn;
};

// ==========================================
//
// Numeric Operation - Subtract
//
// ==========================================

MP_Ratio MP_Ratio::operator - ( const MP_Ratio& aRatio ) const
{
    MP_Ratio rtn( 0 );
	if( is_infinite() != 0 ){
		rtn.is_infinite() = is_infinite();
	} else if( aRatio.is_infinite() != 0 ){
	    rtn.is_infinite() =  -(aRatio.is_infinite());
	} else {
	    mpq_sub( rtn.value(), value(), aRatio.value() );
	}
	return rtn;
};

// ==========================================
//
// Numeric Operation - Multiply
//
// ==========================================

MP_Ratio MP_Ratio::operator * ( const MP_Ratio& aRatio ) const
{
    MP_Ratio rtn( 0 );
	
	if( is_infinite() != 0 ){  // A * B -> A infinite
	
	    if( aRatio.is_infinite() != 0){ // A * B -> A and B infinite
	        rtn.is_infinite() = is_infinite() * aRatio.myRatio->infinity;
		} else {                    
	        if( aRatio != rtn )     // A * B -> A is infinite, B is zero 
	            rtn.is_infinite() = is_infinite() * sgn( aRatio );
			else                    // A * B -> only A is infinite
				rtn.is_infinite() = is_infinite();
        }
		
	} else if( aRatio.is_infinite() != 0){ // A * B -> only B is infinite
	
	    if( *this != rtn )             // A * B -> B is infinite
	        rtn.is_infinite() = aRatio.is_infinite() * sgn( *this );
		else                           // A * B -> B is infinite, A is zero
	        rtn.is_infinite() = aRatio.is_infinite();
			
	} else {   // A * B -> A and B both finite
	
		rtn.is_infinite() = 0;
	    mpq_mul( rtn.value(), value(), aRatio.value() );
		
	}
	
	return rtn;
};

// ==========================================
//
// Numeric Operation - Division - return Quotient
//
// ==========================================

MP_Ratio MP_Ratio::operator / ( const MP_Ratio& aRatio ) const
{
    MP_Ratio rtn( 0 );
	if( is_infinite() != 0 ){
	    if( aRatio.is_infinite() != 0 ){
	        rtn.is_infinite() = is_infinite() * aRatio.is_infinite();
		} else {
		    if (aRatio != rtn) 
 	            rtn.is_infinite() = is_infinite() * sgn( aRatio );
           else
	            rtn.is_infinite() = is_infinite();
		}
	} else if( aRatio.is_infinite() != 0 ){
	    if( *this != rtn )
	        rtn.is_infinite() = aRatio.is_infinite() * sgn( *this );
		else
	        rtn.is_infinite() = aRatio.is_infinite();
	} else if (aRatio == rtn){
	    rtn.is_infinite() = sgn( *this );
	} else {
	    mpq_div( rtn.value(), value(), aRatio.value() );
	}
	return rtn;
};

// ==========================================
//
// Exponentiation 
//
// ==========================================

MP_Ratio MP_Ratio::operator ^ ( unsigned long p ) const
{
    MP_Ratio rtn;
	if ( p < 0 ){ // negative exponent
		MP_Ratio temp( (*this)^(-p) );
		rtn = MP_Ratio( (long) 1, 1 ) / temp ;
	} else if ( p == 0 ) { // zero exponent
		rtn = MP_Ratio( (long) 1, 1 );
	} else if ( (p % 2) == 1 ) { // odd
		rtn = ( (*this) * ( (*this)^(p-1) ));
	} else { // even
		MP_Ratio temp( (*this)^(p/2) );
		rtn = ( temp * temp );
	}
	return rtn;
};

// ==========================================
//
// Exponentiation to fractional power
//
// ==========================================

MP_Ratio pow ( const MP_Ratio & X,const MP_Ratio& p, const MP_Ratio& eps_value )
{
    MP_Ratio        last;
    MP_Ratio        rtn;
	MP_Ratio        term;
	MP_Ratio        factor;
	MP_Ratio        term_k;
	unsigned long   i;
	
    term   = (p * ln( X ));
	factor = 1;
	term_k = term;
	
	rtn    = (term_k * factor) + 1;
	
	last   = rtn;
	term_k = term_k * term;
	factor = factor * MP_Ratio( 1, 2 );
	rtn    = rtn + (term_k * factor);
	
	for( i = 3; (abs( rtn - last ) > eps_value) && (i < MP_Ratio::max_iterations); i++ ){
		last   = rtn;
		term_k = term_k * term;
		factor = factor * MP_Ratio( 1, i );
		rtn = rtn + (term_k * factor);
	}

	return rtn;
}


// ==========================================
//
// Exponentiation to fractional power
//
// ==========================================

MP_Ratio MP_Ratio::operator ^ ( const MP_Ratio& p ) const
{
    MP_Ratio        last;
    MP_Ratio        rtn;
	MP_Ratio        term;
	MP_Ratio        factor;
	MP_Ratio        term_k;
	unsigned long   i;
	
    term    = (p * ln( *this ));
	factor  = 1;
	term_k  = term;
	
	rtn     = (term_k * factor) + 1;
	
	last    = rtn;
	term_k  = term_k * term;
	factor *= MP_Ratio( 1, 2 );
	rtn    += (term_k * factor);
	
	for( i = 3; (abs( rtn - last ) > Epsilon()) && (i < max_iterations); i++ ){
		last    = rtn;
		term_k  = term_k * term;
		factor *= MP_Ratio( 1, i );
		rtn    += (term_k * factor);
	}

	return rtn;
};

// ==========================================
//
// Exponential function of fractional variable 
// Exp( X ) or E^X
//
// ==========================================

MP_Ratio exp( const MP_Ratio &X, const MP_Ratio& eps_value )
{
    MP_Ratio        last;
    MP_Ratio        rtn;
	MP_Ratio        term;
	MP_Ratio        factor;
	MP_Ratio        term_k;
	unsigned long   i;
	
    term    = X;
	factor  = 1;
	term_k  = term;
	
	rtn     = (term_k / factor) + 1;
	
	last    = rtn;
	term_k  = term_k * term;
	factor *= MP_Ratio( 2 );
	rtn    += (term_k / factor);
	
	for( i = 3; (abs( rtn - last ) > eps_value) && (i < MP_Ratio::max_iterations); i++ ){
		last    = rtn;
		term_k  = term_k * term;
		factor *= MP_Ratio( i );
		rtn    += (term_k / factor);
	}

	return rtn;
}


// ==========================================
//
// Natural Logarithm (Base 'e' logarithm)
//
// ==========================================

MP_Ratio ln ( const MP_Ratio& X, const MP_Ratio& eps_value )
{
    MP_Ratio        last;
    MP_Ratio        rtn;
	unsigned long   i;
	MP_Ratio        term;
	MP_Ratio        factor;
	MP_Ratio        term_k;
	long            sign;
	
    if ( X > MP_Ratio(1,2) ){
	    term    = 1 - ( 1 / X );
		factor  = 1;
		term_k  = term;
		
		rtn     = term_k * factor;
		
		last    = rtn;
		term_k  = term_k * term;
		factor  = MP_Ratio( 1, 2 );
		rtn    += (term_k * factor);
		
		for( i = 3; (abs( rtn - last ) > eps_value) && (i < MP_Ratio::max_iterations); i++ ){
		    last    = rtn;
		    term_k  = term_k * term;
			factor  = MP_Ratio( 1, i );
		    rtn    += (term_k * factor);
		}
	} else if ( X > MP_Ratio( 0 ) ){
	    term     = X - 1.0;
		factor   = 1.0;
		term_k   = term;
		
		sign     = 1;
		rtn      = sign * term_k * factor;
		
		last     = rtn;
		sign     = sign * -1;
		term_k   = term_k * term;
		factor   = MP_Ratio( 1, 2 );
		rtn     += (sign * term_k * factor);
		
		for( i = 3; (abs( rtn - last ) > eps_value) && (i < MP_Ratio::max_iterations) ; i++ ){
		    last     = rtn;
		    sign     = sign * -1;
		    term_k   = term_k * term;
			factor   = MP_Ratio( 1, i );
		    rtn     += (sign * term_k * factor);
		}
	} else {
	    // Natural Logarithm undefined - 0 or negative argument
		// NOTE - Should Throw and Exception Here!
		rtn = MP_Ratio( 0 );
	}
	
	return rtn;
};

// ==========================================
//
// Logarithm with Base ( default: comon logarithm - base 10 )
//
// ==========================================

MP_Ratio log ( const MP_Ratio& aRatio, const MP_Ratio& base, const MP_Ratio& eps_value )
{
    if( base > MP_Ratio( 0 ) )
        return ( ln( aRatio, eps_value ) / ln ( base, eps_value ) );
	else 
	    return MP_Ratio( 0 );  // Base must be positive and not zero!
		                       // NOTE - Should Throw and Exception Here!
};

// ==========================================
//
// Input to MP_Ratio 
//
// ==========================================

istream & operator >> (istream &cbuf, MP_Ratio & aRatio )
{
    string  aString;
    MP_Int  numerator;
    MP_Int  denominator( (unsigned long) 1 );
	int div_indx;
	
    cbuf >>  aString;
	
	if( aString == string("+Infinity") ){
	    aRatio = MP_Ratio( 0 );
	    aRatio.is_infinite() = 1;
	} else if ( aString == string("-Infinity") ){
	    aRatio = MP_Ratio( 0 );
	    aRatio.is_infinite() = -1;
	} else {
		numerator = MP_Int( aString.c_str() );
		if( (div_indx = aString.find( '/', 0 )) != -1 ){
			aString = aString[ div_indx + 1 ];
			denominator = MP_Int( aString.c_str() );
		} // else // numerator only
	
	    aRatio = MP_Ratio( numerator, denominator );
	}

	return cbuf;
};

// ==========================================
//
// Output from an MP_Ratio 
//
// ==========================================

ostream & operator << (ostream &cbuf, const MP_Ratio & aRatio)
{
    return ( cbuf <<  aRatio.getStringValue() );
};

// ==========================================
//
// Output from an MP_Ratio 
//
// ==========================================
//
//void       fprint ( FILE* , const MP_Ratio& aRatio )
//{
//    mpq_out_str( fp, 10, 0, aFloat.value() );
//};

// ==========================================
//
// Input to MP_Ratio 
//
// ==========================================

//MP_Ratio  MP_Ratio:: fread ( FILE* )
//{
//    MP_Float rtn;
//    mpq_inp_str( rtn.value(), fp , 0 );
//	return rtn;
//};


//
// ----------------------------------------------------------------
//

//
// ----------Instantiate The Following Templates ------------------
//
#pragma implementation
#include <complex>
#include <std/complext.cc>

template abs(complex<MP_Ratio> const &);
template operator+(complex<MP_Ratio> const &, complex<MP_Ratio> const &);
template operator-(complex<MP_Ratio> const &);
template operator!=(complex<MP_Ratio> const &, complex<MP_Ratio> const &);
template operator*(complex<MP_Ratio> const &, complex<MP_Ratio> const &);
template operator==(complex<MP_Ratio> const &, complex<MP_Ratio> const &);

template operator<<(ostream &, complex<MP_Ratio> const &);
template operator/(complex<MP_Ratio> const &, complex<MP_Ratio> const &);

//
// ----------------------------------------------------------------
//

//
// History:
//    $Log: MP_Ratio.cc,v $
//    Revision 1.1.1.1  1997/09/13 23:38:41  jak
//    Restored MathTools to new CVS repository. -jak
//
//

