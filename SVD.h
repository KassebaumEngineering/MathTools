
//  SVD.h
//  C++ Class Definition of the SVD Class
//
//  Copyright (C) 1993, John A. Kassebaum
//
//  This software was written by John Kassebaum as a part of his graduate
//  course work at Purdue.  He retains all rights to this software.  
//
//  *************************  Description  **************************
//  SVD Class:
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
//	  NOT YET READY FOR USE! 
//
//  Author(s) and Credits:
//	  This Software was originally written by John Kassebaum as a part of 
//    his graduate classwork at Purdue University.  
//
//  Revision:
//    $Id: SVD.h,v 1.1 1997/09/14 04:20:40 jak Exp $
//
//  ******************************************************************
//
#ifndef _SVD_h
#define _SVD_h

static char rcsid_SVD_h[] = "$Id: SVD.h,v 1.1 1997/09/14 04:20:40 jak Exp $";

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
// SVD<NUMTYPE> : Template Class
// ---------------------------------------------------------------------

#define L              0
#define L_transpose    1

template <class NUMTYPE>
class SVD : public MatrixComposition<NUMTYPE>
{
public:
    Matrix<NUMTYPE>     svdmat;

    SVD( void );
    SVD( const SVD<NUMTYPE>& );
    SVD( Matrix<NUMTYPE>& );
    virtual ~SVD( void );

// Assignment Operations
    SVD<NUMTYPE>&  operator = (const SVD<NUMTYPE> &);

    inline int is_empty() const 
	{
        return svdmat.is_empty();
    };
    inline SVD<NUMTYPE>&  shift_to (int fr, int fc)
	{
        svdmat.shift_to( fr, fc );
        return *this;
    };

    NUMTYPE determinant ( void );

    Matrix<NUMTYPE> operator[] ( int ) const;
    Matrix<NUMTYPE> solve_for ( const Matrix<NUMTYPE> & );

};

// ---------------------------------------------------------------------
// SVD<NUMTYPE> : Template Class Implementation
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
SVD<NUMTYPE>::SVD( void ):
    svdmat()
{
    ;
};

// ======================================
//
// Copy Constructor
//
// ======================================

template <class NUMTYPE>
SVD<NUMTYPE>::SVD( const SVD<NUMTYPE> &chol ):
     svdmat( chol.svdmat )
{
    ;
};

// ======================================
//
// Constructor - Calculate SVD Decomposition of a Matrix
//
// ======================================

template <class NUMTYPE>
SVD<NUMTYPE>::SVD( Matrix<NUMTYPE> &matA )
{
    int first_row, first_col, size;
    register int row, col, k;
    SVD<NUMTYPE> *temp;
	NUMTYPE sum;

    if (( !matA.is_symmetric() )||( matA.is_empty() )){
        return;
    }

// Get MatA's stored Decomposition if available
    temp = (SVD<NUMTYPE> *)( matA.getDecompose( SVD_DECOMP ) );
    if ( temp != (SVD<NUMTYPE> *)0 ) {
        *this = *temp;
        return;
    }

//
// SVD routine initialization
//
    size =  matA.Rows();
    first_row = matA.FirstRow();
    first_col = matA.FirstCol();

    svdmat = Matrix<NUMTYPE>( size, size, first_row, first_col, 0, "SVD_Factor");  

    for( row = first_row; row < first_row + size; row++ ){
		for( col = row-first_row+first_col; col < first_col + size; col++ ){
		    for( sum = matA[row][col], k = row -1; k>=first_row; k-- ){
			    sum -= matA[row][k-first_row+first_col] * matA[col][k-first_row+first_col];
			}
			if( row == col ){
			    if( sum <= NUMTYPE(0) ){  // Matrix is NOT POSITIVE DEFINITE
				    svdmat = Matrix<NUMTYPE>();
					return;
				} else {
				    svdmat[row][row-first_row+first_col] = sqrt( sum );
				}
			} else {
				svdmat[col][row] = sum / matA[row][row-first_row+first_col];
			}
		}
	}

//
// End SVD routine
// 
	matA.setDecompose( new SVD<NUMTYPE>( *this ), SVD_DECOMP );
};

// ======================================
//
// Destructor
//
// ======================================

template <class NUMTYPE>
SVD<NUMTYPE>::~SVD()
{
    ;
};


// ======================================
//
// Assignment Operator
//
// ======================================

template <class NUMTYPE>
SVD<NUMTYPE>&  SVD<NUMTYPE>::operator= (const SVD<NUMTYPE> & chol)
{
    if ( ! chol.is_empty() ){
		svdmat = chol.svdmat;
    }
	return *this;
};


// ======================================
//
// Matrix Solution via SVD Decomposition
//
// ======================================

template <class NUMTYPE>
Matrix<NUMTYPE> SVD<NUMTYPE>::solve_for( const Matrix<NUMTYPE> &matA )
{
    register int row, col, k;
    int first_row, first_col, size;
    NUMTYPE sum;
    Matrix<NUMTYPE> result;

    if( matA.Rows () != svdmat.Rows () ){
        Abort("SVD<NUMTYPE>::solve_for(const Matrix<NUMTYPE>&): Incompatible Row Dimensions!\n");
    }
	
    if((matA.FirstRow() != svdmat.FirstRow())
           ||(matA.FirstCol() != svdmat.FirstCol())){
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
			    sum -= svdmat[row][k-first_row+first_col] * result[k][col];
			}
			result[row][col] = sum / svdmat[row][row-first_row+first_col];
		}
//
// backsubstitution
//
		for( row = first_row + size - 1; row >= first_row; row-- ){
			for( sum = result[row][col], k = row + 1; k >= first_row; k++ ){
			    sum -= svdmat[k][row-first_row+first_col] * result[k][col];
			}
			result[row][col] = sum / svdmat[row][row-first_row+first_col];
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
NUMTYPE  SVD<NUMTYPE>:: determinant() 
{
    register int i, j;
    NUMTYPE result;

    result = NUMTYPE( 1 );
    for( i = svdmat.FirstRow(), j = svdmat.FirstCol(); i < svdmat.FirstRow() + svdmat.Rows(); i++, j++ )
        result *= svdmat[i][j];

    return result;
};

// ======================================
//
// Get a DeComposition Sub Matrix
//
// ======================================

template <class NUMTYPE>
Matrix<NUMTYPE> SVD<NUMTYPE>:: operator[] ( int  key ) const
{
    register int i,j;
    Matrix<NUMTYPE> rtn( svdmat );
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


#endif // _SVD_h

//
//  History:
//    $Log: SVD.h,v $
//    Revision 1.1  1997/09/14 04:20:40  jak
//    Skeletons of the QR and SVD now built from Cholesky - lot of work left. -jak
//
//
