//  doubletest.cc
//  C++ Program to test double precision Matrix math
//
//  Copyright (C) 1993, John A. Kassebaum
//
//  This software was written by John Kassebaum as a part of his graduate
//  course work at Purdue.  He retains all rights to this software.  
//
//  Revision:
//    $Id: doubletest.cc,v 1.2 1997/09/14 01:02:05 jak Exp $
//
// =====================================
//

static char rcsid_doubletest_cc[] = "$Id: doubletest.cc,v 1.2 1997/09/14 01:02:05 jak Exp $";

//
// ANSI-C Library Functions
//
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <strings.h>
#include <stdlib.h>
#include <math.h>
#ifdef HPPA
#include <values.h>
#endif

//
// Libg++ Library Functions
//
#include <iostream.h>

//
// stdc++ Library Functions
//
#include <string>
#include <complex>

#include "Matrix.h"

main()
{
    double a[3][3] = { {1.0, 2.0, 3.0},
	                   {4.0, 5.0, 6.0},
					   {7.0, 8.0, 9.0}
				     };
					 
	MP_Ratio aratio( 10 );
	
	Matrix< double >   A( 3, 3, 0, 0,  &(a[0][0]), "A"     );
	
	Matrix< float >                 Hf(               Matrix< float >::Hilbert( 7 ) );
	Matrix< double >                Hd(              Matrix< double >::Hilbert( 7 ) );
 	Matrix< longdouble >           Hld(          Matrix< longdouble >::Hilbert( 7 ) );
 	Matrix< float_complex >        HfC(       Matrix< float_complex >::Hilbert( 7 ) );
 	Matrix< double_complex >       HdC(      Matrix< double_complex >::Hilbert( 7 ) );
 	Matrix< long_double_complex > HldC( Matrix< long_double_complex >::Hilbert( 7 ) );
	
  	Matrix< MP_Float >            Hmpf(            Matrix< MP_Float >::Hilbert( 7 ) );
 	Matrix< mp_float_complex >   HmpfC(    Matrix< mp_float_complex >::Hilbert( 7 ) );

  	Matrix< MP_Ratio >            Hmpq(            Matrix< MP_Ratio >::Hilbert( 7 ) );
 	Matrix< mp_ratio_complex >   HmpqC(    Matrix< mp_ratio_complex >::Hilbert( 7 ) );

    cout << A;

    cout << ~A;

    cout << conjugate_transpose(A);
	
    cout << Hf;
    cout << inverse( Hf );
	
    cout << Hd;
    cout << inverse( Hd );
	
    cout << Hld;
    cout << inverse( Hld );
	
    cout << HfC;
    cout << inverse( HfC );
	
    cout << HdC;
    cout << inverse( HdC );
	
    cout << HldC;
    cout << inverse( HldC );
	
    cout << Hmpf;
    cout << inverse( Hmpf );
	
    cout << aratio << "\n";
    cout << (aratio / 0) << "\n" ;
    cout << ( - aratio / 0) << "\n\n" ;

    cout << MP_Ratio::Epsilon() << "\n\n";
	
//    cout << MP_Ratio( 10, 20 ) << "\n";
    cout << MP_Ratio( 1,2 ) << " = "<< MP_Ratio( 0.5 ) << " = "<< MP_Float( MP_Ratio( 0.5 ) ) << "\n";
    cout << MP_Ratio( 1,4 ) << " = "<< MP_Ratio( 0.25 ) << " = "<< MP_Float( MP_Ratio( 0.25 ) ) << "\n";
    cout << MP_Ratio( 1,10 ) << " = "<< MP_Ratio( 0.1 ) << " = "<< MP_Float( MP_Ratio( 0.1 ) ) << "\n";
    cout << MP_Ratio( 1,100 ) << " = "<< MP_Ratio( 0.01 ) << " = "<< MP_Float( MP_Ratio( 0.01 ) ) << "\n";
    cout << MP_Ratio( 5,1000 ) << " = "<< MP_Ratio( 0.005 ) << " = "<< MP_Float( MP_Ratio( 0.005 ) ) << "\n";
    cout << MP_Ratio( 10,1 ) << " = "<< MP_Ratio( 10.0 ) << " = "<< MP_Float( MP_Ratio( 10.0 ) ) << "\n";
    cout << MP_Ratio( 100,1 ) << " = "<< MP_Ratio( 100.0 ) << " = "<< MP_Float( MP_Ratio( 100.0 ) ) << "\n";
    cout << MP_Ratio( 500,1 ) << " = "<< MP_Ratio( 500.0 ) << " = "<< MP_Float( MP_Ratio( 500.0 ) ) << "\n";
//    cout << sqrt( MP_Ratio( 2 ) ) << "\n\n";

    cout << MP_Float( M_E ) << "\n";
    cout << exp( MP_Float( 1.0 ) ) << "\n\n";

    cout << MP_Ratio( M_E ) << " = " << MP_Float( MP_Ratio( M_E ) ) << "\n";
    cout << exp( MP_Ratio( 1 ) ) << " = " << MP_Float( exp( MP_Ratio( 1 ) )) << "\n\n";
	
    cout << Hmpq;
    cout << inverse( Hmpq );
	
    cout << HmpfC;
    cout << inverse( HmpfC );

    cout << HmpqC;
    cout << inverse( HmpqC );

    cout << (A^2);
}

//
//  History:
//    $Log: doubletest.cc,v $
//    Revision 1.2  1997/09/14 01:02:05  jak
//    Some purely cosmetic changes. -jak
//
//    Revision 1.1.1.1  1997/09/13 23:38:42  jak
//    Restored MathTools to new CVS repository. -jak
//
//
