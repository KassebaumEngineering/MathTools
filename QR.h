
//  QR.h
//  C++ Class Definition of the QR Class
//
//  Copyright (C) 1993, John A. Kassebaum
//
//  This software was written by John Kassebaum as a part of his graduate
//  course work at Purdue.  He retains all rights to this software.  
//
//  *************************  Description  **************************
//  QR Class:
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
//    $Id: QR.h,v 1.1 1997/09/14 04:20:36 jak Exp $
//
//  ******************************************************************
//
#ifndef _QR_h
#define _QR_h

static char rcsid_QR_h[] = "$Id: QR.h,v 1.1 1997/09/14 04:20:36 jak Exp $";

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
// QR<NUMTYPE> : Template Class
// ---------------------------------------------------------------------

#define Q    0
#define R    1

template <class NUMTYPE>
class QR : public MatrixComposition<NUMTYPE>
{
public:
    Matrix<NUMTYPE>     Qmat;
    Matrix<NUMTYPE>     Rmat;

    QR( void );
    QR( const QR<NUMTYPE>& );
    QR( Matrix<NUMTYPE>& );
    virtual ~QR( void );

// Assignment Operations
    QR<NUMTYPE>&  operator = (const QR<NUMTYPE> &);

    inline int is_empty() const 
	{
        return qr_mat.is_empty();
    };
    inline QR<NUMTYPE>&  shift_to (int fr, int fc)
	{
        qr_mat.shift_to( fr, fc );
        return *this;
    };

    NUMTYPE determinant ( void );

    Matrix<NUMTYPE> operator[] ( int ) const;
    Matrix<NUMTYPE> solve_for ( const Matrix<NUMTYPE> & );

};

// ---------------------------------------------------------------------
// QR<NUMTYPE> : Template Class Implementation
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
QR<NUMTYPE>::QR( void ):
    qr_mat()
{
    ;
};

// ======================================
//
// Copy Constructor
//
// ======================================

template <class NUMTYPE>
QR<NUMTYPE>::QR( const QR<NUMTYPE> &aqr ):
     Qmat( aqr.Qmat ), Rmat( aqr.Rmat )
{
    ;
};

// ======================================
//
// Constructor - Calculate QR Decomposition of a Matrix
//
// ======================================

template <class NUMTYPE>
QR<NUMTYPE>::QR( Matrix<NUMTYPE> &matA )
{
    int first_row, first_col, size;
    register int row, col, k;
    QR<NUMTYPE> *temp;
	NUMTYPE sum;

    if (( !matA.is_symmetric() )||( matA.is_empty() )){
        return;
    }

// Get MatA's stored Decomposition if available
    temp = (QR<NUMTYPE> *)( matA.getDecompose( QR_DECOMP ) );
    if ( temp != (QR<NUMTYPE> *)0 ) {
        *this = *temp;
        return;
    }

//
// QR decomposition routine
//


    size =  matA.Rows();
    first_row = matA.FirstRow();
    first_col = matA.FirstCol();

    qr_mat = Matrix<NUMTYPE>( size, size, first_row, first_col, 0, "QR_Factor");  

    for( row = first_row; row < first_row + size; row++ ){
		for( col = row-first_row+first_col; col < first_col + size; col++ ){
		    for( sum = matA[row][col], k = row -1; k>=first_row; k-- ){
			    sum -= matA[row][k-first_row+first_col] * matA[col][k-first_row+first_col];
			}
			if( row == col ){
			    if( sum <= NUMTYPE(0) ){  // Matrix is NOT POSITIVE DEFINITE
				    qr_mat = Matrix<NUMTYPE>();
					return;
				} else {
				    qr_mat[row][row-first_row+first_col] = sqrt( sum );
				}
			} else {
				qr_mat[col][row] = sum / matA[row][row-first_row+first_col];
			}
		}
	}

//
// End QR routine
// 
	matA.setDecompose( new QR<NUMTYPE>( *this ), QR_DECOMP );
};

// ======================================
//
// Destructor
//
// ======================================

template <class NUMTYPE>
QR<NUMTYPE>::~QR()
{
    ;
};


// ======================================
//
// Assignment Operator
//
// ======================================

template <class NUMTYPE>
QR<NUMTYPE>&  QR<NUMTYPE>::operator= (const QR<NUMTYPE> & chol)
{
    if ( ! chol.is_empty() ){
		qr_mat = chol.qr_mat;
    }
	return *this;
};


// ======================================
//
// Matrix Solution via QR Decomposition
//
// ======================================

template <class NUMTYPE>
Matrix<NUMTYPE> QR<NUMTYPE>::solve_for( const Matrix<NUMTYPE> &matA )
{
    register int row, col, k;
    int first_row, first_col, size;
    NUMTYPE sum;
    Matrix<NUMTYPE> result;

    if( matA.Rows () != qr_mat.Rows () ){
        Abort("QR<NUMTYPE>::solve_for(const Matrix<NUMTYPE>&): Incompatible Row Dimensions!\n");
    }
	
    if((matA.FirstRow() != qr_mat.FirstRow())
           ||(matA.FirstCol() != qr_mat.FirstCol())){
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
			    sum -= qr_mat[row][k-first_row+first_col] * result[k][col];
			}
			result[row][col] = sum / qr_mat[row][row-first_row+first_col];
		}
//
// backsubstitution
//
		for( row = first_row + size - 1; row >= first_row; row-- ){
			for( sum = result[row][col], k = row + 1; k >= first_row; k++ ){
			    sum -= qr_mat[k][row-first_row+first_col] * result[k][col];
			}
			result[row][col] = sum / qr_mat[row][row-first_row+first_col];
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
NUMTYPE  QR<NUMTYPE>:: determinant() 
{
    register int i, j;
    NUMTYPE result;

    result = NUMTYPE( 1 );
    for( i = qr_mat.FirstRow(), j = qr_mat.FirstCol(); i < qr_mat.FirstRow() + qr_mat.Rows(); i++, j++ )
        result *= qr_mat[i][j];

    return result;
};

// ======================================
//
// Get a DeComposition Sub Matrix
//
// ======================================

template <class NUMTYPE>
Matrix<NUMTYPE> QR<NUMTYPE>:: operator[] ( int  key ) const
{
    register int i,j;
    Matrix<NUMTYPE> rtn( qr_mat );
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


#endif // _QR_h

//
//  History:
//    $Log: QR.h,v $
//    Revision 1.1  1997/09/14 04:20:36  jak
//    Skeletons of the QR and SVD now built from Cholesky - lot of work left. -jak
//
//
