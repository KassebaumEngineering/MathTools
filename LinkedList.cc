
//  LinkedList.cc
//  C++ File to Instantiate LinkedList Template
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
//    $Id: LinkedList.cc,v 1.1.1.1 1997/09/13 23:38:40 jak Exp $
//
// =====================================
//

static char rcsid_LinkedList_cc[] = "$Id: LinkedList.cc,v 1.1.1.1 1997/09/13 23:38:40 jak Exp $";

#pragma implementation
#include "LinkedList.h"

#include "MatrixComposition.h"

template class LinkedList< MatrixComposition< float > >;
template class LinkedList< MatrixComposition< double > >;
template class LinkedList< MatrixComposition< longdouble > >;
template class LinkedList< MatrixComposition< float_complex > >;
template class LinkedList< MatrixComposition< double_complex > >;
template class LinkedList< MatrixComposition< double_complex > >;
template class LinkedList< MatrixComposition< long_double_complex > >;

//
//  History:
//    $Log: LinkedList.cc,v $
//    Revision 1.1.1.1  1997/09/13 23:38:40  jak
//    Restored MathTools to new CVS repository. -jak
//
//

