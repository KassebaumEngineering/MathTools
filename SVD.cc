
//  SVD.cc
//  C++ File to Instantiate SVD Template
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
//    $Id: SVD.cc,v 1.3 1997/09/14 04:20:38 jak Exp $
//
//
// =====================================
//

static char rcsid_SVD_cc[] = "$Id: SVD.cc,v 1.3 1997/09/14 04:20:38 jak Exp $";

#pragma implementation
#include "SVD.h"

template               SVD<float>:: SVD( Matrix< float > & );
template              SVD<double>:: SVD( Matrix< double > & );
template          SVD<longdouble>:: SVD( Matrix< longdouble > & );
template            SVD<MP_Float>:: SVD( Matrix< MP_Float > & );
template            SVD<MP_Ratio>:: SVD( Matrix< MP_Ratio > & );

template               SVD<float>:: ~SVD();
template              SVD<double>:: ~SVD();
template          SVD<longdouble>:: ~SVD();
template            SVD<MP_Float>:: ~SVD();
template            SVD<MP_Ratio>:: ~SVD();

template               SVD<float>:: is_empty() const;
template              SVD<double>:: is_empty() const;
template          SVD<longdouble>:: is_empty() const;
template            SVD<MP_Float>:: is_empty() const;
template            SVD<MP_Ratio>:: is_empty() const;


SVD<float_complex>::SVD( Matrix<float_complex> &matA ):
    svdmat()
{
    ;
};

SVD<double_complex>::SVD( Matrix<double_complex> &matA ):
    svdmat()
{
    ;
};

SVD<long_double_complex>::SVD( Matrix<long_double_complex> &matA ):
    svdmat()
{
    ;
};

SVD<mp_float_complex>::SVD( Matrix<mp_float_complex> &matA ):
    svdmat()
{
    ;
};

SVD<mp_ratio_complex>::SVD( Matrix<mp_ratio_complex> &matA ):
    svdmat()
{
    ;
};


//  History:
//    $Log: SVD.cc,v $
//    Revision 1.3  1997/09/14 04:20:38  jak
//    Skeletons of the QR and SVD now built from Cholesky - lot of work left. -jak
//
//
