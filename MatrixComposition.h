
//  MatrixComposition.h
//  C++ Class Interface Definition of the MatrixComposition Class
//
//  Copyright (C) 1993, John A. Kassebaum
//
//  This software was written by John Kassebaum as a part of his graduate
//  course work at Purdue.  He retains all rights to this software.  
//
//  *************************  Description  **************************
//  MatrixComposition Class:
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
//  Revision:
//    $Id: MatrixComposition.h,v 1.1 1997/09/13 23:38:42 jak Exp $
//
//  ---------------------------------------------------------------------------
//  Author(s) and Credits:
//	  This Software was originally written by John Kassebaum as a part of 
//    his graduate classwork at Purdue University.  
//
//  ******************************************************************
//
#ifndef _MatrixComposition_h
#define _MatrixComposition_h

static char rcsid_MatrixComposition_h[] = "$Id: MatrixComposition.h,v 1.1 1997/09/13 23:38:42 jak Exp $";

#pragma interface

//
// Use the following Template Classes
// 
template <class NUMTYPE> class Matrix;

//---------------------------------------------------------------------
// MatrixComposition : Template Class
// ---------------------------------------------------------------------

template <class NUMTYPE>
class MatrixComposition 
{
public:
    MatrixComposition( void ){};
    virtual ~MatrixComposition( void ){};

    virtual Matrix<NUMTYPE> operator[]( int  key ) const          = 0 ;
    virtual Matrix<NUMTYPE> solve_for ( const Matrix<NUMTYPE> & ) = 0 ;
};

//
// ---------------------------------------------------------------------
//
#include "Matrix.h"


//---------------------------------------------------------------------
// Global Definitions for All Matrix Decompositions
// ---------------------------------------------------------------------

//
// Decomposition Components (keys for linked list)
//
#define    LU_DECOMP   0
#define   SVD_DECOMP   1
#define    QR_DECOMP   2
#define  CHOL_DECOMP   3
// etc


#endif

//
//  History:
//    $Log: MatrixComposition.h,v $
//    Revision 1.1  1997/09/13 23:38:42  jak
//    Initial revision
//
//
