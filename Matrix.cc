
//  Matrix.cc
//  C++ File to Instantiate Matrix Template
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
//  Known Bugs:
//
//  Revision:
//    $Id: Matrix.cc,v 1.1.1.1 1997/09/13 23:38:41 jak Exp $
//
// =====================================
//

static char rcsid_Matrix_cc[] = "$Id: Matrix.cc,v 1.1.1.1 1997/09/13 23:38:41 jak Exp $";

#pragma implementation
#include "Matrix.h"

template class Matrix< float >;
template class Matrix< double >;
template class Matrix< longdouble >;
template class Matrix< float_complex >;
template class Matrix< double_complex >;
template class Matrix< long_double_complex >;
template class Matrix< MP_Float >;
template class Matrix< mp_float_complex >;
template class Matrix< MP_Ratio >;
template class Matrix< mp_ratio_complex >;

//template Matrix<float> conjugate_transpose( const Matrix<float> & );
//template Matrix<double> conjugate_transpose( const Matrix<double> & );
//template Matrix<longdouble> conjugate_transpose( const Matrix<longdouble> & );
//template Matrix<float_complex> conjugate_transpose( const Matrix<float_complex> & );
//template Matrix<double_complex> conjugate_transpose( const Matrix<double_complex> & );
//template Matrix<long_double_complex> conjugate_transpose( const Matrix<long_double_complex> & );
//template Matrix<MP_Float> conjugate_transpose( const Matrix<MP_Float> & );
//template Matrix<mp_float_complex> conjugate_transpose( const Matrix<mp_float_complex> & );
//template Matrix<MP_Ratio> conjugate_transpose( const Matrix<MP_Ratio> & );
//template Matrix<mp_ratio_complex> conjugate_transpose( const Matrix<mp_ratio_complex> & );

template operator << ( ostream &, Matrix< float > const &);
template operator << ( ostream &, Matrix< double > const &);
template operator << ( ostream &, Matrix< longdouble > const &);
template operator << ( ostream &, Matrix< float_complex > const &);
template operator << ( ostream &, Matrix< double_complex > const &);
template operator << ( ostream &, Matrix< long_double_complex > const &);
template operator << ( ostream &, Matrix< MP_Float > const &);
template operator << ( ostream &, Matrix< mp_float_complex > const &);
template operator << ( ostream &, Matrix< MP_Ratio > const &);
template operator << ( ostream &, Matrix< mp_ratio_complex > const &);

template operator  + ( Matrix< float > const &, float );
template operator  + ( Matrix< double > const &, double );
template operator  + ( Matrix< longdouble > const &, longdouble );
template operator  + ( Matrix< float_complex > const &, float_complex );
template operator  + ( Matrix< double_complex > const &, double_complex );
template operator  + ( Matrix< long_double_complex > const &, long_double_complex );
template operator  + ( Matrix< MP_Float > const &, MP_Float );
template operator  + ( Matrix< mp_float_complex > const &, mp_float_complex );
template operator  + ( Matrix< MP_Ratio > const &, MP_Ratio );
template operator  + ( Matrix< mp_ratio_complex > const &, mp_ratio_complex );

//  History:
//    $Log: Matrix.cc,v $
//    Revision 1.1.1.1  1997/09/13 23:38:41  jak
//    Restored MathTools to new CVS repository. -jak
//
//

