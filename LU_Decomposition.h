
//  LU_Decomposition.h
//  C++ Class Definition of the LU_Decomposition Class
//
//  Copyright (C) 1993, John A. Kassebaum
//
//  This software was written by John Kassebaum as a part of his graduate
//  course work at Purdue.  He retains all rights to this software.  
//
//  *************************  Description  **************************
//  LU_Decomposition Class:
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
//  ---------------------------------------------------------------------
//  Author(s) and Credits:
//	  This Software was originally written by John Kassebaum as a part of 
//    his graduate classwork at Purdue University.  
//
//  Revision:
//    $Id: LU_Decomposition.h,v 1.1.1.1 1997/09/13 23:38:39 jak Exp $
//
//  ******************************************************************
//
#ifndef _LU_Decomposition_h
#define _LU_Decomposition_h

static char rcsid_LU_Decomposition_h[] = "$Id: LU_Decomposition.h,v 1.1.1.1 1997/09/13 23:38:39 jak Exp $";

#pragma interface

//
// ANSI-C Library Functions
//
#include <iostream.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <strings.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

//
// Libg++ Library Functions
//

//
// stdc++ Library Functions
//
#include <string>
#include <complex>

//
// Use the following Template Classes
// 
template <class NUMTYPE> class Matrix;
template <class NUMTYPE> class MatrixComposition;

//
// Use the following Classes
// 
#include "IntArray.h"
#include "MP_Float.h"

// ---------------------------------------------------------------------
// LU_Decomposition<NUMTYPE> : Template Class
// ---------------------------------------------------------------------

#define P_OF_LU    0
#define L_OF_LU    1
#define U_OF_LU    2

template <class NUMTYPE>
class LU_Decomposition : public MatrixComposition<NUMTYPE>
{
public:
    IntArray            row_permute;
    Matrix<NUMTYPE>     lumat;

    LU_Decomposition( void );
    LU_Decomposition( Matrix<NUMTYPE>& );
    LU_Decomposition( const LU_Decomposition<NUMTYPE>& );
    virtual ~LU_Decomposition( void );

// Assignment Operations
    LU_Decomposition<NUMTYPE>&  operator = (const LU_Decomposition<NUMTYPE> &);

    inline int is_empty() const {
        return lumat.is_empty();
    };
    inline LU_Decomposition<NUMTYPE>&  shift_to (int fr, int fc){
        row_permute.shift_to( fr );
        lumat.shift_to( fr, fc );
        return *this;
    };

    NUMTYPE determinant ( void );

    Matrix<NUMTYPE> operator[] ( int ) const;
    Matrix<NUMTYPE> solve_for ( const Matrix<NUMTYPE> & );

    const NUMTYPE Tiny( void ) const;
};

// ---------------------------------------------------------------------
// LU_Decomposition<NUMTYPE> : Template Class Implementation
// ---------------------------------------------------------------------

// ======================================
//
// Use the following Template Classes
// 
#include "MatrixComposition.h"
#include "Matrix.h"


// ======================================
//
// Special Functions - Tiny()
//
typedef long double         longdouble;
typedef complex<MP_Float>   mp_float_complex;
typedef complex<MP_Ratio>   mp_ratio_complex;

inline const          longdouble            LU_Decomposition<longdouble>::Tiny( void ) const { return LDBL_EPSILON; };
inline const               float                 LU_Decomposition<float>::Tiny( void ) const { return FLT_EPSILON; };
inline const              double                LU_Decomposition<double>::Tiny( void ) const { return DBL_EPSILON; };
inline const       float_complex         LU_Decomposition<float_complex>::Tiny( void ) const { return float_complex( FLT_EPSILON, 0.0 ); };
inline const      double_complex        LU_Decomposition<double_complex>::Tiny( void ) const { return double_complex( DBL_EPSILON, 0.0 ); };
inline const long_double_complex LU_Decomposition< long_double_complex >::Tiny( void ) const { return long_double_complex( DBL_EPSILON, 0.0 ); };

inline const            MP_Float          LU_Decomposition<MP_Float>::Tiny( void ) const { return MP_FLOAT_EPSILON; };
inline const    mp_float_complex  LU_Decomposition<mp_float_complex>::Tiny( void ) const { return mp_float_complex( MP_FLOAT_EPSILON, 0.0 ); };

inline const            MP_Ratio          LU_Decomposition<MP_Ratio>::Tiny( void ) const { return MP_RATIO_EPSILON; };
inline const    mp_ratio_complex  LU_Decomposition<mp_ratio_complex>::Tiny( void ) const { return mp_ratio_complex( MP_RATIO_EPSILON, 0.0 ); };

inline longdouble real( const longdouble   value ) { return value; };
inline      float real( const      float   value ) { return value; };
inline     double real( const     double   value ) { return value; };
inline   MP_Float real( const   MP_Float & value ) { return value; };
inline   MP_Ratio real( const   MP_Ratio & value ) { return value; };

// ======================================
//
// Default Contructor
//
// ======================================

template <class NUMTYPE>
LU_Decomposition<NUMTYPE>::LU_Decomposition( void ):
    lumat(), row_permute()
{
    ;
};

// ======================================
//
// Copy Constructor
//
// ======================================

template <class NUMTYPE>
LU_Decomposition<NUMTYPE>::LU_Decomposition( const LU_Decomposition<NUMTYPE> &lu ):
     lumat( lu.lumat ), row_permute( lu.row_permute )
{
    ;
};

// ======================================
//
// Constructor - Calculate LU Decomposition of a Matrix of Real Non-Complex Values
//
// ======================================

template <class NUMTYPE>
LU_Decomposition<NUMTYPE>::LU_Decomposition( Matrix<NUMTYPE> &matA )
{
// This algorithm is a distinct implementation of an algorithm found
// in Numerical Recipes in C.  The ideas expressed as code here, were 
// derived from that work, which this programmer highly recommends.
// This routine is based on Crout's method with implicit partial pivoting 
// as described by Numerical Recipes in C.

    int first_row, first_col, col_to_row, size;
    register int row, col, k;
    double permute_parity;  // even # of row permutations = +1.0, odd = -1.0
    LU_Decomposition<NUMTYPE> *temp;
    NUMTYPE *row_scaling;

    permute_parity = 1.0;
	
    if ( matA.is_empty() ){
        return;
    }

// Get MatA's stored LU Decomposition if available
    temp = (LU_Decomposition<NUMTYPE> *)( matA.getDecompose( LU_DECOMP ) );
    if ( temp != (LU_Decomposition<NUMTYPE> *)0 ) {
        *this = *temp;
        return;
    }

//
// LU routine initialization
//
    if ( matA.Rows () != matA.Cols () ) 
        Abort("LU_Decompose( const Matrix<NUMTYPE>& ): Square Matrix Required!");
    size =  matA.Rows (); // == matA.Cols ()
    first_row = matA.FirstRow();
    first_col = matA.FirstCol();

    lumat = matA;  
    row_permute = IntArray( first_row,  matA.Rows ());

    row_scaling = new NUMTYPE[ size ];
    if (row_scaling  == (NUMTYPE *)0 ){
        Abort("LU_Decompose( const Matrix<NUMTYPE>& ): Could not allocate memory for row_scaling[]!\n");
    }
    row_scaling = row_scaling - first_row;

//
// Find the Maximum element value in each row and record a scaling factor
//
    for( row=first_row; row < first_row + size; row++){
		NUMTYPE maxval;
        typeof( real( maxval )) max, tempval;

        max =  abs( lumat[row][first_col] );
		maxval = lumat[row][first_col];
        for( col=first_col; col < first_col + size; col++ ){
            tempval = abs( lumat[row][col] );
            if(tempval > max){
			    max = tempval;
			    maxval = lumat[row][col];
			}
        }
        if (max == typeof( max )( 0 ) ) {
		    cerr << "Matrix<NUMTYPE>::LU_Decomp - Singular Matrix\n";
            row_scaling[ row ] = NUMTYPE( 0 );
		} else {
            row_scaling[ row ] = NUMTYPE( 1 ) / maxval;
		}
    }

//
// Crout's Method of Gaussian Elimination
//
    col_to_row = first_row - first_col;
    for( col=first_col; col < first_col + size; col++){
        NUMTYPE sum;
		typeof( real( sum ))  max, tempval;
        unsigned int max_index;

        for( row=first_row; row < col+col_to_row; row++) {
            sum = lumat[row][col];
            for( k=0; k<row-first_row ; k++)
                sum -= lumat[row][k+first_col] * lumat[k+first_row][col];
            lumat[row][col] = sum;
        }
        max = abs( Tiny() );
        max_index = col;
        for( row = col+col_to_row; row < first_row + size; row++){
            sum = lumat[row][col];
            for( k=0; k<col-first_col; k++)
                sum -= lumat[row][k+first_col] * lumat[k+first_row][col];
            lumat[row][col] = sum;
            if((tempval = abs( row_scaling[row] * sum )) >= max ){
                max_index = row;
                max = tempval;
            }
        }
        if ( col != max_index ){
		    NUMTYPE swapval;
            for( k=0; k<size; k++ ){
                swapval = lumat[max_index][k+first_col];
                lumat[max_index][k+first_col] 
                    = lumat[ col+col_to_row ][k+first_col];
                lumat[ col+col_to_row ][k+first_col] = swapval;
            }
            permute_parity = -permute_parity;
            row_scaling[ max_index ] = row_scaling[ col+col_to_row ];
        }
        row_permute[ col+col_to_row ] = max_index ;
        if ( lumat[ col+col_to_row ][col] == NUMTYPE( 0 ) )
            lumat[ col+col_to_row ][col] = Tiny();
        if ( col != size-1 ){
		    NUMTYPE scaleval;
            scaleval = NUMTYPE( 1 ) / (lumat[ col+col_to_row ][col]);
            for( row=col+col_to_row+1; row < first_row + size; row++) 
                lumat[row][col] *= scaleval;
        }
    }

//
// End LU routine
// 

	matA.setDecompose( new LU_Decomposition<NUMTYPE>( *this ), LU_DECOMP );

    delete &(row_scaling[first_row]); 
};

// ======================================
//
// Destructor
//
// ======================================

template <class NUMTYPE>
LU_Decomposition<NUMTYPE>::~LU_Decomposition()
{
    ;
};


// ======================================
//
// Assignment Operator
//
// ======================================

template <class NUMTYPE>
LU_Decomposition<NUMTYPE>&  LU_Decomposition<NUMTYPE>::operator= (const LU_Decomposition<NUMTYPE> & lu)
{
    if ( ! lu.is_empty() ){
		lumat = lu.lumat;
		row_permute = IntArray( lu.row_permute );
    }
	return *this;
};


// ======================================
//
// Matrix Solution via LU Decomposition
//
// ======================================

template <class NUMTYPE>
Matrix<NUMTYPE> LU_Decomposition<NUMTYPE>::solve_for( const Matrix<NUMTYPE> &matA )
{
    Matrix<NUMTYPE> result;
    register int row, col;
    int first_row, first_col;
    int Rows, Cols;

    if( matA.Rows () != lumat.Rows () ){
        Abort("LU_Decomposition<NUMTYPE>::solve_for(const Matrix<NUMTYPE>&): Incompatible Row Dimensions!\n");
    }
    if((matA.FirstRow() != lumat.FirstRow())
           ||(matA.FirstCol() != lumat.FirstCol())){
        this->shift_to( matA.FirstRow(), matA.FirstCol());
    }

    first_row = matA.FirstRow();
    Rows = matA.Rows ();
    first_col = matA.FirstCol();
    Cols = matA.Cols ();

    result = matA;
    for( col=first_col; col < first_col + Cols; col++ ){
        int flag, index;
        register unsigned short k;
        unsigned short row_p;
        NUMTYPE sum;
//
// forward substitution
//
        flag = 0; index = -100000;
        for( row=first_row; row < first_row + Rows; row++ ){
            row_p = row_permute[row];
// --
            sum = result[ row_p ][ col ];
            result[ row_p ][ col ] = result[ row ][ col ];
            if (flag)
                for( k=index; k < row;  k++) 
                    sum -= lumat[row][k+first_col-first_row] * result[k][col];
            else if (sum != NUMTYPE(0.0)){
                flag = 1; index = row;
            }
            result[row][col] = sum;
        }
//
// backsubstitution
//
        for( row=first_row + Rows - 1; row >= first_row; row--){
            sum = result[row][col];
            for( k=row+1; k < first_row + Rows; k++){
                sum -= lumat[row][k+first_col-first_row] * result[k][col];
            }
            result[row][col] = sum / lumat[row][row+first_col-first_row];
        }
    }

    result.setLabel("Solution");
    return result;
};

// ======================================
//
// Compute Determinant
//
// ======================================

template <class NUMTYPE>
NUMTYPE  LU_Decomposition<NUMTYPE>:: determinant() 
{
    register int i, col_to_row;
    NUMTYPE result;

    col_to_row = lumat.FirstRow() - lumat.FirstCol();
    result = NUMTYPE( 1.0 );
    for(i=lumat.FirstRow(); i< lumat.FirstRow()+lumat.Rows ();i++)
        result *= lumat[i+col_to_row][i];

    return result;
};

// ======================================
//
// Get a DeComposition Sub Matrix
//
// ======================================

template <class NUMTYPE>
Matrix<NUMTYPE> LU_Decomposition<NUMTYPE>:: operator[] ( int  key ) const
{
    register int i,j;
    Matrix<NUMTYPE> rtn( lumat );
    Matrix<NUMTYPE> temp;
    NUMTYPE value;

    if ( key == L_OF_LU ){
        temp = Matrix<NUMTYPE>::LowerTriangle( rtn.Rows () );
        temp.shift_to( rtn.FirstRow(), rtn.FirstCol() );
        rtn = rtn & temp;
        for( i = rtn.FirstRow(); i < rtn.FirstRow() + rtn.Rows(); i++)
            rtn[i][i] = NUMTYPE( 1.0 );
    } else if (key == U_OF_LU) {
        temp = Matrix<NUMTYPE>::UpperTriangle( rtn.Rows() );
        temp.shift_to( rtn.FirstRow(), rtn.FirstCol() );
        rtn = rtn & temp;
    } else if (key == P_OF_LU) {
        temp = Matrix<NUMTYPE>::Identity( rtn.Rows() );
        temp.shift_to( rtn.FirstRow(), rtn.FirstCol() );
        for( i = rtn.FirstRow() + rtn.Rows() - 1; i >= rtn.FirstRow(); i--){
            for( j = rtn.FirstCol(); j< rtn.FirstCol() + rtn.Cols(); j++){
                value = temp[i][j]; 
                temp[i][j] = temp[ row_permute[i] ][j]; 
                temp[ row_permute[i] ][j] = value;
            }
        }
        rtn = temp;
    } else {
        rtn = Matrix<NUMTYPE>();
    }

    return rtn;
};


//
// ---------------------------------------------------------------------
//


#endif // _LU_Decomposition_h

//
//  History:
//    $Log: LU_Decomposition.h,v $
//    Revision 1.1.1.1  1997/09/13 23:38:39  jak
//    Restored MathTools to new CVS repository. -jak
//
//
