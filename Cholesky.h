
//  Cholesky.h
//  C++ Class Definition of the Cholesky Class
//
//  Copyright (C) 1993, John A. Kassebaum
//
//  This software was written by John Kassebaum as a part of his graduate
//  course work at Purdue.  He retains all rights to this software.  
//
//  *************************  Description  **************************
//  Cholesky Class:
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
//    $Id: Cholesky.h,v 1.2 1997/09/14 01:01:30 jak Exp $
//
//  ******************************************************************
//
#ifndef _Cholesky_h
#define _Cholesky_h

static char rcsid_Cholesky_h[] = "$Id: Cholesky.h,v 1.2 1997/09/14 01:01:30 jak Exp $";

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
// NA

// ---------------------------------------------------------------------
// Cholesky<NUMTYPE> : Template Class
// ---------------------------------------------------------------------

#define L              0
#define L_transpose    1

template <class NUMTYPE>
class Cholesky : public MatrixComposition<NUMTYPE>
{
public:
    Matrix<NUMTYPE>     cholmat;

    Cholesky( void );
    Cholesky( const Cholesky<NUMTYPE>& );
    Cholesky( Matrix<NUMTYPE>& );
    virtual ~Cholesky( void );

// Assignment Operations
    Cholesky<NUMTYPE>&  operator = (const Cholesky<NUMTYPE> &);

    inline int is_empty() const 
	{
        return cholmat.is_empty();
    };
    inline Cholesky<NUMTYPE>&  shift_to (int fr, int fc)
	{
        cholmat.shift_to( fr, fc );
        return *this;
    };

    NUMTYPE determinant ( void );

    Matrix<NUMTYPE> operator[] ( int ) const;
    Matrix<NUMTYPE> solve_for ( const Matrix<NUMTYPE> & );

};

// ---------------------------------------------------------------------
// Cholesky<NUMTYPE> : Template Class Implementation
// ---------------------------------------------------------------------

// ======================================
//
// Use the following Template Classes
// 
#include "MatrixComposition.h"
#include "Matrix.h"

// ======================================
//
// Default Contructor
//
// ======================================

template <class NUMTYPE>
Cholesky<NUMTYPE>::Cholesky( void ):
    cholmat()
{
    ;
};

// ======================================
//
// Copy Constructor
//
// ======================================

template <class NUMTYPE>
Cholesky<NUMTYPE>::Cholesky( const Cholesky<NUMTYPE> &chol ):
     cholmat( chol.cholmat )
{
    ;
};

// ======================================
//
// Constructor - Calculate Cholesky Decomposition of a Matrix
//
// ======================================

template <class NUMTYPE>
Cholesky<NUMTYPE>::Cholesky( Matrix<NUMTYPE> &matA )
{
    int first_row, first_col, size;
    register int row, col, k;
    Cholesky<NUMTYPE> *temp;
	NUMTYPE sum;

    if (( !matA.is_symmetric() )||( matA.is_empty() )){
        return;
    }

// Get MatA's stored Decomposition if available
    temp = (Cholesky<NUMTYPE> *)( matA.getDecompose( CHOL_DECOMP ) );
    if ( temp != (Cholesky<NUMTYPE> *)0 ) {
        *this = *temp;
        return;
    }

//
// Cholesky routine initialization
//
    size =  matA.Rows();
    first_row = matA.FirstRow();
    first_col = matA.FirstCol();

    cholmat = Matrix<NUMTYPE>( size, size, first_row, first_col, 0, "Cholesky_Factor");  

    for( row = first_row; row < first_row + size; row++ ){
		for( col = row-first_row+first_col; col < first_col + size; col++ ){
		    for( sum = matA[row][col], k = row -1; k>=first_row; k-- ){
			    sum -= matA[row][k-first_row+first_col] * matA[col][k-first_row+first_col];
			}
			if( row == col ){
			    if( sum <= NUMTYPE(0) ){  // Matrix is NOT POSITIVE DEFINITE
				    cholmat = Matrix<NUMTYPE>();
					return;
				} else {
				    cholmat[row][row-first_row+first_col] = sqrt( sum );
				}
			} else {
				cholmat[col][row] = sum / matA[row][row-first_row+first_col];
			}
		}
	}

//
// End Cholesky routine
// 
	matA.setDecompose( new Cholesky<NUMTYPE>( *this ), CHOL_DECOMP );
};

// ======================================
//
// Destructor
//
// ======================================

template <class NUMTYPE>
Cholesky<NUMTYPE>::~Cholesky()
{
    ;
};


// ======================================
//
// Assignment Operator
//
// ======================================

template <class NUMTYPE>
Cholesky<NUMTYPE>&  Cholesky<NUMTYPE>::operator= (const Cholesky<NUMTYPE> & chol)
{
    if ( ! chol.is_empty() ){
		cholmat = chol.cholmat;
    }
	return *this;
};


// ======================================
//
// Matrix Solution via Cholesky Decomposition
//
// ======================================

template <class NUMTYPE>
Matrix<NUMTYPE> Cholesky<NUMTYPE>::solve_for( const Matrix<NUMTYPE> &matA )
{
    register int row, col, k;
    int first_row, first_col, size;
    NUMTYPE sum;
    Matrix<NUMTYPE> result;

    if( matA.Rows () != cholmat.Rows () ){
        Abort("Cholesky<NUMTYPE>::solve_for(const Matrix<NUMTYPE>&): Incompatible Row Dimensions!\n");
    }
	
    if((matA.FirstRow() != cholmat.FirstRow())
           ||(matA.FirstCol() != cholmat.FirstCol())){
        this->shift_to( matA.FirstRow(), matA.FirstCol());
    }

    size =  matA.Rows();
    first_row = matA.FirstRow();
    first_col = matA.FirstCol();

//
// backsubstitution
//
    result = matA;
    for( col=first_col; col < first_col + size; col++ ){
	
//
// forward substitution
//
		for( row = first_row; row < first_row + size; row++ ){
			for( sum = result[row][col], k = row - 1; k >= first_row; k-- ){
			    sum -= cholmat[row][k-first_row+first_col] * result[k][col];
			}
			result[row][col] = sum / cholmat[row][row-first_row+first_col];
		}
//
// backsubstitution
//
		for( row = first_row + size - 1; row >= first_row; row-- ){
			for( sum = result[row][col], k = row + 1; k >= first_row; k++ ){
			    sum -= cholmat[k][row-first_row+first_col] * result[k][col];
			}
			result[row][col] = sum / cholmat[row][row-first_row+first_col];
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
NUMTYPE  Cholesky<NUMTYPE>:: determinant() 
{
    register int i, j;
    NUMTYPE result;

    result = NUMTYPE( 1 );
    for( i = cholmat.FirstRow(), j = cholmat.FirstCol(); i < cholmat.FirstRow() + cholmat.Rows(); i++, j++ )
        result *= cholmat[i][j];

    return result;
};

// ======================================
//
// Get a DeComposition Sub Matrix
//
// ======================================

template <class NUMTYPE>
Matrix<NUMTYPE> Cholesky<NUMTYPE>:: operator[] ( int  key ) const
{
    register int i,j;
    Matrix<NUMTYPE> rtn( cholmat );
    Matrix<NUMTYPE> temp;
    NUMTYPE value;

 // Default (key == L)
 //	temp = Matrix<NUMTYPE>::LowerTriangle( rtn.Rows () );
 //	temp.shift_to( rtn.FirstRow(), rtn.FirstCol() );
 //	for( i = rtn.FirstRow(); i < rtn.FirstRow() + rtn.Rows(); i++)
 //	    temp[i][i] = NUMTYPE( 1 );
 //	rtn = rtn & temp;
		
    if ( key == L_transpose ){
	    rtn = !rtn;
	} else {
	    rtn = Matrix<NUMTYPE>();
	}
		
    return rtn;
};


//
// ---------------------------------------------------------------------
//


#endif // _Cholesky_h

//
//  History:
//    $Log: Cholesky.h,v $
//    Revision 1.2  1997/09/14 01:01:30  jak
//    Some purely cosmetic changes. -jak
//
//    Revision 1.1.1.1  1997/09/13 23:38:38  jak
//    Restored MathTools to new CVS repository. -jak
//
//
