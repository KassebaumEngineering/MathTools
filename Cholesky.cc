
//  Cholesky.cc
//  C++ File to Instantiate Cholesky Template
//
//  Copyright (C) 1993, John A. Kassebaum
//
//  This software was written by John Kassebaum as a part of his graduate
//  course work at Purdue.  He retains all rights to this software.  
//
//  Known Bugs:
//
//  Revision:
//    $Id: Cholesky.cc,v 1.2 1997/09/14 01:01:28 jak Exp $
//
//
// =====================================
//

static char rcsid_Cholesky_cc[] = "$Id: Cholesky.cc,v 1.2 1997/09/14 01:01:28 jak Exp $";

#pragma implementation
#include "Cholesky.h"

template               Cholesky<float>:: Cholesky( Matrix< float > & );
template              Cholesky<double>:: Cholesky( Matrix< double > & );
template          Cholesky<longdouble>:: Cholesky( Matrix< longdouble > & );
template            Cholesky<MP_Float>:: Cholesky( Matrix< MP_Float > & );
template            Cholesky<MP_Ratio>:: Cholesky( Matrix< MP_Ratio > & );

template               Cholesky<float>:: ~Cholesky();
template              Cholesky<double>:: ~Cholesky();
template          Cholesky<longdouble>:: ~Cholesky();
template            Cholesky<MP_Float>:: ~Cholesky();
template            Cholesky<MP_Ratio>:: ~Cholesky();

template               Cholesky<float>:: is_empty() const;
template              Cholesky<double>:: is_empty() const;
template          Cholesky<longdouble>:: is_empty() const;
template            Cholesky<MP_Float>:: is_empty() const;
template            Cholesky<MP_Ratio>:: is_empty() const;


Cholesky<float_complex>::Cholesky( Matrix<float_complex> &matA ):
    cholmat()
{
    ;
};

Cholesky<double_complex>::Cholesky( Matrix<double_complex> &matA ):
    cholmat()
{
    ;
};

Cholesky<long_double_complex>::Cholesky( Matrix<long_double_complex> &matA ):
    cholmat()
{
    ;
};

Cholesky<mp_float_complex>::Cholesky( Matrix<mp_float_complex> &matA ):
    cholmat()
{
    ;
};

Cholesky<mp_ratio_complex>::Cholesky( Matrix<mp_ratio_complex> &matA ):
    cholmat()
{
    ;
};


//  History:
//    $Log: Cholesky.cc,v $
//    Revision 1.2  1997/09/14 01:01:28  jak
//    Some purely cosmetic changes. -jak
//
//    Revision 1.1.1.1  1997/09/13 23:38:37  jak
//    Restored MathTools to new CVS repository. -jak
//
//
