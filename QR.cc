
//  QR.cc
//  C++ File to Instantiate QR Template
//
//  Copyright (C) 1993, John A. Kassebaum
//
//  This software was written by John Kassebaum as a part of his graduate
//  course work at Purdue.  He retains all rights to this software.  
//
//  Known Bugs:
//	  NOT YET READY FOR USE! 
//
//  Revision:
//    $Id: QR.cc,v 1.1 1997/09/14 04:20:32 jak Exp $
//
//
// =====================================
//

static char rcsid_QR_cc[] = "$Id: QR.cc,v 1.1 1997/09/14 04:20:32 jak Exp $";

#pragma implementation
#include "QR.h"

template               QR<float>:: QR( Matrix< float > & );
template              QR<double>:: QR( Matrix< double > & );
template          QR<longdouble>:: QR( Matrix< longdouble > & );
template            QR<MP_Float>:: QR( Matrix< MP_Float > & );
template            QR<MP_Ratio>:: QR( Matrix< MP_Ratio > & );

template               QR<float>:: ~QR();
template              QR<double>:: ~QR();
template          QR<longdouble>:: ~QR();
template            QR<MP_Float>:: ~QR();
template            QR<MP_Ratio>:: ~QR();

template               QR<float>:: is_empty() const;
template              QR<double>:: is_empty() const;
template          QR<longdouble>:: is_empty() const;
template            QR<MP_Float>:: is_empty() const;
template            QR<MP_Ratio>:: is_empty() const;


QR<float_complex>::QR( Matrix<float_complex> &matA ):
    qr_mat()
{
    ;
};

QR<double_complex>::QR( Matrix<double_complex> &matA ):
    qr_mat()
{
    ;
};

QR<long_double_complex>::QR( Matrix<long_double_complex> &matA ):
    qr_mat()
{
    ;
};

QR<mp_float_complex>::QR( Matrix<mp_float_complex> &matA ):
    qr_mat()
{
    ;
};

QR<mp_ratio_complex>::QR( Matrix<mp_ratio_complex> &matA ):
    qr_mat()
{
    ;
};


//  History:
//    $Log: QR.cc,v $
//    Revision 1.1  1997/09/14 04:20:32  jak
//    Skeletons of the QR and SVD now built from Cholesky - lot of work left. -jak
//
//
