//  MP_Int.cc
//  C++ Class Implementation of the MP_Int Class
//
//  Copyright (C) 1993, John A. Kassebaum
//
//  This software was written by John Kassebaum as a part of his graduate
//  course work at Purdue.  He retains all rights to this software.  
//
//  ----------
//  See MP_Int.H for the Interface description and Usage directions.
//  ----------
//
// Notes: 
//
//  Revision:
//    $Id: MP_Int.cc,v 1.1 1997/09/13 23:38:40 jak Exp $
//
//  ************************************************************
static char rcsid_MP_Int_cc[] = "$Id: MP_Int.cc,v 1.1 1997/09/13 23:38:40 jak Exp $";


#include "MP_Int.h"

//
//  Use these external classes.
//
#include "MP_Float.h"
#include "MP_Ratio.h"

// ----------------------------------------------------------------
//
// MP_Int Class variables
//
rounding_method MP_Int::my_rounding_method = MPZ_TRUNCATE;

// ----------------------------------------------------------------
//
// MP_Int::MP_Int_Storage  Implementation
//

// ==========================================
//
// Default Constructor
//
// ==========================================

MP_Int::MP_Int_Storage::MP_Int_Storage():
    reference_count( 1 )
{
	mpz_init( intVal );
};

// ==========================================
//
// Copy Constructor
//
// ==========================================

MP_Int::MP_Int_Storage::MP_Int_Storage( const MP_Int_Storage & anInt ):
    reference_count( 1 )
{
	mpz_init( intVal );
	mpz_set( intVal, anInt.intVal );
};

// ==========================================
//
// Constructor - initialize with unsigned integer
//
// ==========================================

MP_Int::MP_Int_Storage::MP_Int_Storage( unsigned long num ):
    reference_count( 1 )
{
	mpz_init( intVal );
	mpz_set_ui( intVal, num );
};

// ==========================================
//
// Constructor - initialize with signed integer
//
// ==========================================

MP_Int::MP_Int_Storage::MP_Int_Storage( signed long num ):
    reference_count( 1 )
{
	mpz_init( intVal );
	mpz_set_si( intVal, num );
};

// ==========================================
//
// Constructor - initialize with double precision floating point number
//
// ==========================================

MP_Int::MP_Int_Storage::MP_Int_Storage( double dnum ):
    reference_count( 1 )
{
	mpz_init( intVal );
	mpz_set_d( intVal, dnum );
};

// ==========================================
//
// Constructor - initialize with MP_Ratio
//
// ==========================================

MP_Int::MP_Int_Storage::MP_Int_Storage( const MP_Ratio & aRatio ):
    reference_count( 1 )
{
	mpz_init( intVal );
	mpz_set_q( intVal, aRatio.value() );
};

// ==========================================
//
// Constructor - initialize with MP_Float
//
// ==========================================

MP_Int::MP_Int_Storage::MP_Int_Storage( const MP_Float & aFloat ):
    reference_count( 1 )
{
	mpz_init( intVal );
	mpz_set_f( intVal, aFloat.value() );
};

// ==========================================
//
// Constructor - initialize with string value
//
// ==========================================

MP_Int::MP_Int_Storage::MP_Int_Storage( const char *aString, int aBase ):
    reference_count( 1 )
{
	mpz_init( intVal );
	mpz_set_str( intVal, aString, aBase );
};


// ==========================================
//
// Destructor
//
// ==========================================

MP_Int::MP_Int_Storage::~MP_Int_Storage( void )
{
	if ( --reference_count <= 1 ) 
		mpz_clear( intVal );
};

//
// ----------------------------------------------------------------
//

// ----------------------------------------------------------------
//
// MP_Int Implementation
//

// ==========================================
//
// Default Constructor
//
// ==========================================

MP_Int::MP_Int():
    myInt( new MP_Int_Storage )
{
	;
};

// ==========================================
//
// Copy Constructor
//
// ==========================================

MP_Int::MP_Int( const MP_Int & anInt ):
    myInt( anInt.myInt )
{
    myInt->inc_refcount();
};

// ==========================================
//
// Constructor - initialize with unsigned integer
//
// ==========================================

MP_Int::MP_Int( unsigned long num ):
    myInt( new MP_Int_Storage( num ) )
{
    ;
};

// ==========================================
//
// Constructor - initialize with signed integer
//
// ==========================================

MP_Int::MP_Int( signed long num ):
    myInt( new MP_Int_Storage( num ) )
{
    ;
};

// ==========================================
//
// Constructor - initialize with double precision floating point number
//
// ==========================================

MP_Int::MP_Int( double dnum ):
    myInt( new MP_Int_Storage( dnum ) )
{
    ;
};

// ==========================================
//
// Constructor - initialize with MP_Ratio
//
// ==========================================

MP_Int::MP_Int( const MP_Ratio & aRatio ):
    myInt( new MP_Int_Storage( aRatio ) )
{
    ;
};

// ==========================================
//
// Constructor - initialize with MP_Float
//
// ==========================================

MP_Int::MP_Int( const MP_Float & aFloat ):
    myInt( new MP_Int_Storage( aFloat ) )
{
    ;
};

// ==========================================
//
// Constructor - initialize with string value
//
// ==========================================

MP_Int::MP_Int( const char *aString, int aBase ):
    myInt( new MP_Int_Storage( aString, aBase ) )
{
    ;
};


// ==========================================
//
// Destructor
//
// ==========================================

MP_Int::~MP_Int( void )
{
    myInt->dec_refcount();
};

// ----------------------------------------------------
//
// MP_Int Specific Class Methods
//

// ==========================================
//
// Generate Random Numbers
//   generate random number of at most max_size limbs
//   generate negative numbers when max_size is negative
//
// ==========================================

MP_Int MP_Int:: random ( mp_size_t max_size )
{
    MP_Int rtn;
	mpz_random( rtn.value(), max_size );
	
	return rtn;
};  

// ==========================================
//
// Generate Random Numbers
//   generate random number of at most max_size limbs
//   generate negative numbers when max_size is negative
//
// ==========================================

MP_Int MP_Int::random2( mp_size_t max_size )
{
    MP_Int rtn;
	mpz_random2( rtn.value(), max_size );
	
	return rtn;
};  

// ==========================================
//
// Calculate Factorial of an unsigned long
//
// ==========================================

MP_Int MP_Int:: factorial ( unsigned long anULong )
{
    MP_Int rtn;
	mpz_fac_ui( rtn.value(), anULong );
	return rtn;
}


// ----------------------------------------------------
//
// Common Instance Methods
//

// ==========================================
//
// getStringValue
//
// ==========================================

string MP_Int::getStringValue( void ) const
{
    char result[ mpz_sizeinbase(value(),10) + 2 ];
    mpz_get_str( result, 10, value() );
	return string( result );
};

// ==========================================
//
// Assignment Operation
//
// ==========================================

MP_Int& MP_Int::operator = ( const MP_Int& anInt )
{
    myInt->dec_refcount();
    myInt = anInt.myInt;
	myInt->inc_refcount();
	
	return *this;
};

// ==========================================
//
// Assignment Operation
//
// ==========================================

MP_Int& MP_Int::operator = ( unsigned_long AULval )
{
	mpz_set_ui( value(), AULval );
	return *this;
};

// ==========================================
//
// Assignment Operation
//
// ==========================================

MP_Int& MP_Int::operator = ( signed_long ASLval)
{
	mpz_set_si( value(), ASLval );
	return *this;
};

// ==========================================
//
// Numeric Operation - Add
//
// ==========================================

MP_Int MP_Int::operator + ( const MP_Int& anInt ) const
{
    MP_Int rtn;
	mpz_add( rtn.value(), this->value(), anInt.value() );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Add an Unsigned Long Int
//
// ==========================================

MP_Int MP_Int::operator + ( unsigned long aULong ) const
{
    MP_Int rtn;
	mpz_add_ui( rtn.value(), this->value(), aULong );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Negation
//
// ==========================================

MP_Int MP_Int::operator - ( void ) const
{
    MP_Int rtn;
	mpz_neg( rtn.value(), this->value() );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Absolute Value
//
// ==========================================

MP_Int abs ( const MP_Int& anInt )
{
    MP_Int rtn;
	mpz_abs( rtn.value(), anInt.value() );
	return rtn;
}

// ==========================================
//
// Numeric Operation - Subtract
//
// ==========================================

MP_Int MP_Int::operator - ( const MP_Int& anInt ) const
{
    MP_Int rtn;
	mpz_sub( rtn.value(), this->value(), anInt.value() );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Subtract an Unsigned Long Int
//
// ==========================================

MP_Int MP_Int::operator - ( unsigned long aULong) const
{
    MP_Int rtn;
	mpz_sub_ui( rtn.value(), this->value(), aULong );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Multiply
//
// ==========================================

MP_Int MP_Int::operator * ( const MP_Int& anInt ) const
{
    MP_Int rtn;
	mpz_mul( rtn.value(), this->value(), anInt.value() );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Multiply an Unsigned Long Int
//
// ==========================================

MP_Int MP_Int::operator * ( unsigned long aULong ) const
{
    MP_Int rtn;
	mpz_mul_ui( rtn.value(), this->value(), aULong );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Multiply by power of 2 - (also known as left shift)
//
// ==========================================

MP_Int MP_Int::operator << ( unsigned long aULong ) const
{
    MP_Int rtn;
	mpz_mul_2exp( rtn.value(), this->value(), aULong );
	return rtn;
};

// ==========================================
//
// Numeric Operation - Division - return Quotient
//
// ==========================================

MP_Int MP_Int::operator / ( const MP_Int& anInt) const
{
    MP_Int rtn;
	switch( my_rounding_method )
	{
	    case MPZ_CEILING:
	        mpz_cdiv_q( rtn.value(), this->value(), anInt.value() );
		    break;
			
	    case MPZ_FLOOR:
	        mpz_fdiv_q( rtn.value(), this->value(), anInt.value() );
		    break;
			
	    case MPZ_TRUNCATE:
		default:
	        mpz_tdiv_q( rtn.value(), this->value(), anInt.value() );
		    break;
	}
	return rtn;
};

// ==========================================
//
// Numeric Operation - Division by an unsigned long int - return Quotient
//
// ==========================================

MP_Int MP_Int::operator / ( unsigned long aULong ) const
{
    MP_Int rtn;
	switch( my_rounding_method )
	{
	    case MPZ_CEILING:
	        mpz_cdiv_q_ui( rtn.value(), this->value(), aULong );
		    break;
			
	    case MPZ_FLOOR:
	        mpz_fdiv_q_ui( rtn.value(), this->value(), aULong );
		    break;
			
	    case MPZ_TRUNCATE:
		default:
	        mpz_tdiv_q_ui( rtn.value(), this->value(), aULong );
		    break;
	}
	return rtn;
};

// ==========================================
//
// Numeric Operation - Divide by power of 2 - return Quotient
//
// ==========================================

MP_Int MP_Int::operator >> ( unsigned long aULong ) const
{
    MP_Int rtn;
	switch( my_rounding_method )
	{
	    case MPZ_FLOOR:
	        mpz_fdiv_q_2exp( rtn.value(), this->value(), aULong );
		    break;
			
	    case MPZ_CEILING: // mpz_cdiv_q_2exp does not exist
	    case MPZ_TRUNCATE:
		default:
	        mpz_tdiv_q_2exp( rtn.value(), this->value(), aULong );
		    break;
	}
	return rtn;
};

// ==========================================
//
// Numeric Operation - Division - return Remainder
//
// ==========================================

MP_Int MP_Int::operator % ( const MP_Int& anInt) const
{
    MP_Int rtn;
	switch( my_rounding_method )
	{
	    case MPZ_CEILING:
	        mpz_cdiv_r( rtn.value(), this->value(), anInt.value() );
		    break;
			
	    case MPZ_FLOOR:
	        mpz_fdiv_r( rtn.value(), this->value(), anInt.value() );
		    break;
			
	    case MPZ_TRUNCATE:
		default:
	        mpz_tdiv_r( rtn.value(), this->value(), anInt.value() );
		    break;
	}
	return rtn;
};

// ==========================================
//
// Numeric Operation - Division by an unsigned long int - return Remainder
//
// ==========================================

MP_Int MP_Int::operator % ( unsigned long aULong ) const
{
    MP_Int rtn;
	switch( my_rounding_method )
	{
	    case MPZ_CEILING:
	        mpz_cdiv_r_ui( rtn.value(), this->value(), aULong );
		    break;
			
	    case MPZ_FLOOR:
	        mpz_fdiv_r_ui( rtn.value(), this->value(), aULong );
		    break;
			
	    case MPZ_TRUNCATE:
		default:
	        mpz_tdiv_r_ui( rtn.value(), this->value(), aULong );
		    break;
	}
	return rtn;
};

// ==========================================
//
// Numeric Operation - Divide by power of 2 - return Remainder
//
// ==========================================

MP_Int MP_Int::shift_remainder ( unsigned long aULong ) const
{
    MP_Int rtn;
	switch( my_rounding_method )
	{
	    case MPZ_FLOOR:
	        mpz_fdiv_r_2exp( rtn.value(), this->value(), aULong );
		    break;
			
	    case MPZ_CEILING:  // mpz_cdiv_r_2exp does not exist
	    case MPZ_TRUNCATE:
		default:
	        mpz_tdiv_r_2exp( rtn.value(), this->value(), aULong );
		    break;
	}
	return rtn;
};

// ==========================================
//
// Exponentiation 
//
// ==========================================

MP_Int MP_Int::operator ^ ( unsigned long aULong_exponent ) const
{
    MP_Int rtn;
    mpz_pow_ui( rtn.value(), this->value(), aULong_exponent ); 
	return rtn;
};

// ==========================================
//
// Exponentiation with modulo division
//
// ==========================================

MP_Int MP_Int::exponential_remainder( const MP_Int& exponent, const MP_Int& divisor ) const
{
    MP_Int rtn;
    mpz_powm( rtn.value(), this->value(), exponent.value(), divisor.value() ); 
	return rtn;
};

// ==========================================
//
// Exponentiation with modulo division
//
// ==========================================

MP_Int MP_Int::exponential_remainder( unsigned long aULong_exponent, const MP_Int& divisor) const
{
    MP_Int rtn;
    mpz_powm_ui( rtn.value(), this->value(), aULong_exponent, divisor.value() ); 
	return rtn;
};


// ==========================================
//
// Square Root 
//
// ==========================================

MP_Int sqrt( const MP_Int& anInt )
{
    MP_Int rtn;
    mpz_sqrt( rtn.value(), anInt.value() );
	return rtn;
};

// ==========================================
//
// Square Root remainder
//
// ==========================================

MP_Int sqrt_remainder( const MP_Int& anInt ) 
{
    MP_Int rtn, theSqrt;
    mpz_sqrtrem( theSqrt.value(), rtn.value(), anInt.value() );
	return rtn;
};

// ==========================================
//
// Test for a Perfect Square
//
// ==========================================

int  is_perfect_square( const MP_Int& anInt )
{
    return mpz_perfect_square_p( anInt.value() );
};


// ==========================================
//
// Test for a Probable Primeness
//
// ==========================================

int  is_probably_prime( const MP_Int& anInt, int reps )
{
    return mpz_probab_prime_p( anInt.value(), reps );
};
 
// ==========================================
//
// Find Greatest Common Divisor
//
// ==========================================

MP_Int  greatest_common_divisor( const MP_Int& A, const MP_Int& B)
{
    MP_Int rtn;
	mpz_gcd( rtn.value(), A.value(), B.value() );
	return rtn;
};

// ==========================================
//
// Find Greatest Common Divisor with an unsigned long int
//
// ==========================================

MP_Int  greatest_common_divisor( const MP_Int& A, unsigned long B_ulong )
{
    MP_Int rtn;
	mpz_gcd_ui( rtn.value(), A.value(), B_ulong );
	return rtn;
};

// ==========================================
//
// Find an External Greatest Common Divisor : AS + BT = G = gcd
//
// ==========================================

void  external_gcd( MP_Int& G, MP_Int& S, MP_Int& T, const MP_Int& A, const MP_Int& B)
{
	mpz_gcdext( G.value(), S.value(), T.value(), A.value(), B.value() );
};

// ==========================================
//
// Find an External Greatest Common Divisor : AS + B = G = gcd
//
// ==========================================

void  external_gcd( MP_Int& G, MP_Int& S, const MP_Int& A, const MP_Int& B)
{
	mpz_gcdext( G.value(), S.value(), 0, A.value(), B.value() );
};

// ==========================================
//
// Inverse after Modulo Divide
//
// ==========================================

MP_Int  modulo_invert( const MP_Int& A, const MP_Int& divisor )
{
    MP_Int rtn;
    mpz_invert( rtn.value(), A.value(), divisor.value() ); 
	return rtn;
};

// ==========================================
//
// Jacobi number
//
// ==========================================

MP_Int  jacobi( const MP_Int& anInt )
{
    MP_Int rtn;
	mpz_jacobi( rtn.value(), anInt.value() );
	return rtn;
};

// ==========================================
//
// legendre number
//
// ==========================================

MP_Int  legendre( const MP_Int& anInt )
{
    MP_Int rtn;
	mpz_legendre( rtn.value(), anInt.value() );
	return rtn;
};

// ==========================================
//
// SGN - sign of MP_Int
//
// ==========================================

int sgn( const MP_Int& anInt )
{
	return mpz_sgn( anInt.value() );
};

// ==========================================
//
// CMP - compare two MP_Ints
//
// ==========================================

int cmp( const MP_Int& A, const MP_Int& B)
{
	return mpz_cmp( A.value(), B.value() );
};

// ==========================================
//
// CMP - compare MP_Int and unsigned long
//
// ==========================================

int cmp( const MP_Int& A, unsigned long B_ulong )
{
	return mpz_cmp_ui( A.value(), B_ulong );
};

// ==========================================
//
// CMP - compare MP_Int and signed long
//
// ==========================================

int cmp( const MP_Int& A, signed long B_slong)
{
	return mpz_cmp_si( A.value(), B_slong );
};


// ==========================================
//
// AND - bitwise and 
//
// ==========================================

MP_Int MP_Int::operator & ( const MP_Int& anInt ) const
{
    MP_Int rtn;
	mpz_and( rtn.value(), this->value(), anInt.value() );
	return rtn;
};

// ==========================================
//
// OR - bitwise inclusive or 
//
// ==========================================

MP_Int MP_Int::operator | ( const MP_Int& anInt ) const
{
    MP_Int rtn;
	mpz_ior( rtn.value(), this->value(), anInt.value() );
	return rtn;
};

// ==========================================
//
// COMPLEMENT - bitwise ones complement 
//
// ==========================================

MP_Int MP_Int::operator ~ ( void ) const
{
    MP_Int rtn;
	mpz_com( rtn.value(), this->value() );
	return rtn;
};

// ==========================================
//
// hamming_distance - calculate hamming distance between two MP_Ints 
//
// ==========================================

unsigned long hamming_distance( const MP_Int& A, const MP_Int& B )
{
    return mpz_hamdist( A.value(), B.value() );
};

// ==========================================
//
// Set a bit in an MP_Int
//
// ==========================================

MP_Int MP_Int::set( unsigned int aULong )
{
    mpz_setbit( value(), aULong );
    return *this;
};

// ==========================================
//
// Clear a bit in an MP_Int
//
// ==========================================

MP_Int MP_Int::clr( unsigned int aULong )
{
    mpz_clrbit( value(), aULong );
    return *this;
};

// ==========================================
//
// Scan for a 0 bit in an MP_Int
//
// ==========================================

unsigned long scan_for_0( const MP_Int & A, unsigned int aULong_startAtBit )
{
    return mpz_scan0( A.value(), aULong_startAtBit );
};

// ==========================================
//
// Scan for a 1 bit in an MP_Int
//
// ==========================================

unsigned long scan_for_1( const MP_Int & A, unsigned int aULong_startAtBit )
{
    return mpz_scan1( A.value(), aULong_startAtBit );
};

// ==========================================
//
// Input to an MP_Int from a stream
//
// ==========================================

istream & operator >> (istream &cbuf,       MP_Int & anInt)
{
    string  aString_intval;
	
    cbuf >>  aString_intval;
	anInt = MP_Int( aString_intval.c_str() );
	
	return cbuf;
};

// ==========================================
//
// Output an MP_Int to a stream
//
// ==========================================

ostream & operator << (ostream &cbuf, const MP_Int & anInt)
{
    return ( cbuf <<  anInt.getStringValue() );
};

// ==========================================
//
// Output an MP_Int to a stream file pointer
//
// ==========================================

void fprint( FILE* fp, const MP_Int& anInt ) 
{
    mpz_out_str( fp, 10, anInt.value() );
};
 
// ==========================================
//
// Input an MP_Int from a stream file pointer
//
// ==========================================

MP_Int fread( FILE* fp )
{ 
    MP_Int rtn;
    mpz_inp_str( rtn.value(), fp , 0 );
	return rtn;
};

// ==========================================
//
// Output an MP_Int in raw binary format to a stream file pointer
//
// ==========================================

void MP_Int::writeTo( FILE* fp ) const  // raw (non-ascii) output to file
{
    mpz_out_raw( fp, value() );
};
 
// ==========================================
//
// Input an MP_Int in raw binary format from a stream file pointer
//
// ==========================================

MP_Int& MP_Int::readFrom( FILE* fp )    // raw (non-ascii) input from file
{
    mpz_inp_raw( value(), fp );
};
   

// ==========================================
//
// Get Number of Digits in an MP_Int given a representation Base
//
// ==========================================

size_t MP_Int::sizeInBase( int base )
{
    return mpz_sizeinbase( value(), base );
};

//
// ----------------------------------------------------------------
//

// History:
//   $Log: MP_Int.cc,v $
//   Revision 1.1  1997/09/13 23:38:40  jak
//   Initial revision
//
//

