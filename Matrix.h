
//  Matrix.h
//  C++ Template Class Definitions related to the Matrix math
//
//  Copyright (C) 1993, John A. Kassebaum
//
//  This software was written by John Kassebaum as a part of his graduate
//  course work at Purdue.  He retains all rights to this software.  
//
//  *************************  Description  *******************************
//
//  This file declares the following templates
//    class Matrix< NUMTYPE >;
//
//  Matrix Class Template:
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
//  ---------------------------------------------------------------------------
//  Author(s) and Credits:
//	  This Software was originally written by John Kassebaum as a part of 
//    his graduate classwork at Purdue University.  
//
//  Known Bugs:
//
//  Revision:
//    $Id: Matrix.h,v 1.1 1997/09/13 23:38:41 jak Exp $
//
//
// ==========================================================================
//
#ifndef _Matrix_h
#define _Matrix_h

static char rcsid_Matrix_h[] = "$Id: Matrix.h,v 1.1 1997/09/13 23:38:41 jak Exp $";

#pragma interface

//
// ANSI-C Library Functions
//
#include <iostream.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//
// Libg++ Library Functions
//

//
// stdc++ Library Functions
//
#include <string>
#include <complex>

//
// special preprocessor functions
//
#define ABS(x) ((x)>=0?(x):-(x))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// NOTE -> Should use Throw!
inline void Abort( const char *s ){
    cerr <<  s << "\n";
    abort();
};

//
// Use the following template classes
//
#include "LinkedList.h"
#include "Range.h"

//
// Use the following Template Classes
// 
template <class NUMTYPE> class Matrix;
template <class NUMTYPE> class MatrixComposition;
template <class NUMTYPE> class LU_Decomposition;
template <class NUMTYPE> class Cholesky;

//
// Use the following Classes
// 
#include "MP_Float.h"
#include "MP_Ratio.h"

// ---------------------------------------------------------------------
// Basic Matrix Types:
// Matrix<NUMTYPE>  : Template Class
// ---------------------------------------------------------------------
typedef long double longdouble;

typedef Matrix< float >                  Float_Matrix;
typedef Matrix< double >                 Double_Matrix;
typedef Matrix< longdouble >             Long_Double_Matrix;
typedef Matrix< float_complex >          Float_Complex_Matrix;
typedef Matrix< double_complex >         Double_Complex_Matrix;
typedef Matrix< double_complex >                Complex_Matrix;
typedef Matrix< long_double_complex >    Long_Double_Complex_Matrix;
typedef Matrix< MP_Float >               MP_Float_Matrix;
typedef Matrix< complex< MP_Float > >    MP_Float_Complex_Matrix;
typedef Matrix< MP_Ratio >               MP_Ratio_Matrix;
typedef Matrix< complex< MP_Ratio > >    MP_Ratio_Complex_Matrix;

// ---------------------------------------------------------------------
// MatrixStorage<NUMTYPE>  : Template Class
// A Private MatrixStorage Representation Class
// ---------------------------------------------------------------------

template <class NUMTYPE>
class MatrixStorage
{
public:
    friend class Matrix<NUMTYPE>;
	
	//
	//  Storage for 2D Matrix data
	//
	NUMTYPE       **matrix;           // matrix data in row major order -> matrix[row][col]
	unsigned int    number_of_rows;   // size in rows 
	unsigned int    number_of_cols;   // and collumns
	int     		first_row;		  // The offset location of the upper left corner
	int     		first_col;		  // of the 2D square matrix.
	
	unsigned int    reference_count;  // number of references to current storage

//
//  Default Constructor with initialization from C-standard matrix
//
 	MatrixStorage( unsigned int rows, unsigned int cols, int firstrow = 0, int firstcol = 0, NUMTYPE *mat = 0 ):
 		number_of_rows(rows), number_of_cols(cols),
 		first_row( firstrow ), first_col( firstcol ), reference_count(1)
 	{
 		allocate();
		
		if( mat != 0 ){
	        register int r, c;
			register NUMTYPE *ptr;
			ptr = mat;
			for( r = first_row; r < number_of_rows; r++ ) {
				for( c = first_col; c < number_of_cols; c++ ) {
					matrix[r][c] = *ptr++ ;
				}
			}
		}
 	};
	
//
// Copy Constructor
//
	MatrixStorage( const MatrixStorage& aMat  ):
		number_of_rows( aMat.number_of_rows), number_of_cols( aMat.number_of_cols ),
		first_row( aMat.first_row ), first_col( aMat.first_col ), reference_count(1)
	{
		register int r,c;
		register NUMTYPE *ptrA, *ptrB;
	
		allocate();
	
		if( &(matrix[first_row]) != (NUMTYPE **)0) {
			for( r=first_row; r < first_row+number_of_rows; r++) {
				ptrA = &(      matrix[r][first_col] );
				ptrB = &( aMat.matrix[r][first_col] );
				for( c=0; c< number_of_cols; c++) {
					*ptrA++ = *ptrB++;
				}
			}
		}
	}

//
// Destuctor
//
	~MatrixStorage()
	{
		if( reference_count == 1 ) deallocate();
	}
	
//
// allocation & deallocation functions
//
	void deallocate()
	{
		if ( &(matrix[first_row]) != (NUMTYPE **)0 ){
			delete [] &(matrix[first_row][first_col]);
			delete [] &(matrix[first_row]);
		}
	};
	
	void allocate()
	{
		register int r, size;
		register NUMTYPE *newmat, *ptr;
	
		if( (size = number_of_rows * number_of_cols) > 0 ){
	
			newmat = (NUMTYPE  *) new NUMTYPE  [ size ];
			matrix = (NUMTYPE **) new NUMTYPE* [ number_of_rows ];
			if ((newmat == (NUMTYPE *)0 )||(matrix == (NUMTYPE **)0 )) {
				cerr << "Matrix<" << "NUMTYPE" << ">::allocate(): couln't allocate space for a ";
				cerr << "Matrix<" << "NUMTYPE" << ">( " << first_row << ","  ;
				cerr << number_of_rows << "," << first_col << "," ;
				cerr << number_of_cols << " )\n" ;
				abort();
			}
	
			for( ptr = newmat, r=0; r < size; r++){
				*ptr++ = NUMTYPE( 0.0 );
			}
		
			for (ptr = newmat, r = 0; r < number_of_rows; r++){
				matrix[r] = ptr - first_col;
				ptr += number_of_cols;
			}
			matrix = matrix - first_row;
	
		} else {
	
			newmat = (NUMTYPE *)0;
			matrix = (NUMTYPE **)0;
	
		}
	};

//
// accessor functions
//
	void inc_refcount()
	{
		reference_count++;
	};
	
	void dec_refcount()
	{
		if( reference_count-- <= 1)
			delete this;
	};
	
	inline NUMTYPE **getMatrix( void ) { return matrix; }
};


// ---------------------------------------------------------------------
// Matrix<NUMTYPE>  : Template Class
// ---------------------------------------------------------------------

template <class NUMTYPE>
class Matrix
{

public:

	MatrixStorage<NUMTYPE>  *matrix_p;   // Internal matrix storage with reference count.
	 
private:
    string   label;      // An optional name or label for the matrix
	                     // possibly useful for keeping track of
						 // the matrix symbolicly or for formatted output
	
  //
  //  List of decompositions of the current matrix
  //
    LinkedList< MatrixComposition<NUMTYPE> > *decomp_list;

  //
  // special accessors
  //
    inline unsigned int &  number_of_rows( void ) const { return matrix_p->number_of_rows; };
    inline int          &       first_row( void ) const { return matrix_p->first_row;      };
    inline unsigned int &  number_of_cols( void ) const { return matrix_p->number_of_cols; };
    inline int          &       first_col( void ) const { return matrix_p->first_col;      };

public:

//
// Constructors and Destructors
//
    Matrix( unsigned int = 0, unsigned int = 0, int = 0, int = 0, NUMTYPE *imat = 0, const string & = "");
    Matrix( const Matrix<NUMTYPE>& );
    ~Matrix( void );

// Assignment Operations
    Matrix<NUMTYPE>&  operator = (const Matrix<NUMTYPE> &);

//  Submatrix Access Methods
    Matrix<NUMTYPE> operator() ( Range<int>, Range<int> ) const ;
    Matrix<NUMTYPE> operator() ( int, int, int, int ) const;
    Matrix<NUMTYPE>&  shift_to ( int, int );

// Matrix Information Methods
    inline string getLabel( void ) const { return label; };
    inline void setLabel( string aname ) {
        label = aname; 
    };
	
    inline unsigned int     Rows( void ) const { return matrix_p->number_of_rows; };
    inline int          FirstRow( void ) const { return matrix_p->first_row;      };
    inline unsigned int     Cols( void ) const { return matrix_p->number_of_cols; };
    inline int          FirstCol( void ) const { return matrix_p->first_col;      };
	
    inline NUMTYPE* & operator [] ( int i ) const { return matrix_p->matrix[ i ]; };
    inline int has_element( int i, int j) const {
        if (     (i <  matrix_p->first_row) 
              || (i >= matrix_p->first_row + matrix_p->number_of_rows ) 
              || (j <  matrix_p->first_col) 
              || (j >= matrix_p->first_col + matrix_p->number_of_cols ))
            return 0;
        else
            return 1;
    };
    inline int is_empty( void ) const {
        return &(matrix_p->matrix[FirstRow()])?0:1;
    };
	int is_symmetric( void ) const;
	int is_positive_definite( void );
	int is_negative_definite( void );

// Matrix Operator Methods
    Matrix<NUMTYPE> & operator += ( const Matrix<NUMTYPE>& );
    Matrix<NUMTYPE> & operator += ( NUMTYPE );
    Matrix<NUMTYPE> & operator -= ( const Matrix<NUMTYPE>& );
    Matrix<NUMTYPE> & operator -= ( NUMTYPE );
    Matrix<NUMTYPE> & operator *= ( const Matrix<NUMTYPE>& );
    Matrix<NUMTYPE> & operator *= ( NUMTYPE );
    Matrix<NUMTYPE> & operator /= ( Matrix<NUMTYPE>& );
    inline Matrix<NUMTYPE> & operator /= ( NUMTYPE scalar ){
        return ( (*this) *= ( NUMTYPE( 1.0 ) / scalar ));
    };
    Matrix<NUMTYPE> & operator &= ( const Matrix<NUMTYPE> & );  // elementwise multiply
    Matrix<NUMTYPE> & operator |= ( const Matrix<NUMTYPE> & );  // elementwise divide

    Matrix<NUMTYPE> & apply ( NUMTYPE(*)( NUMTYPE ) );
    Matrix<NUMTYPE> & apply ( NUMTYPE(*)( NUMTYPE, NUMTYPE ), const Matrix<NUMTYPE>& );

// Matrix Norm Function
// default Euclidean norm p = 2
// p = 1 -> 1-norm, p=0 -> infinity norm
    friend NUMTYPE norm ( const Matrix<NUMTYPE>& , NUMTYPE = 2.0 ); 

// Matrix Determinant Function
    friend NUMTYPE determinant ( Matrix<NUMTYPE>& ); 

// Comparison Operator Function
    friend int operator == ( const Matrix<NUMTYPE>&, const Matrix<NUMTYPE>& );

// Matrix - Matrix Operator Functions
    friend Matrix<NUMTYPE> operator + ( const Matrix<NUMTYPE>&, const Matrix<NUMTYPE>& );
    friend Matrix<NUMTYPE> operator - ( const Matrix<NUMTYPE>&, const Matrix<NUMTYPE>& );
    friend Matrix<NUMTYPE> operator * ( const Matrix<NUMTYPE>&, const Matrix<NUMTYPE>& );
    friend Matrix<NUMTYPE> operator / ( Matrix<NUMTYPE>&, Matrix<NUMTYPE>& );
		// ___________ NOTE ________^
		// solve X = matA / matB
		// means :
		//  matB * X = matA
		//         X = inverse( matB ) * matA
		//  NOT
		//  X * matB = matA
		//         X = matA * inverse( matB );
		// if the second case occurs, then the alg cannot
		// recognize it. But you can transpose the input to fix it.
		// --------------------------------

// Scalar - Matrix Operator Functions 
    friend Matrix<NUMTYPE> operator * (NUMTYPE, const Matrix<NUMTYPE>&);
    friend inline Matrix<NUMTYPE> operator * (const Matrix<NUMTYPE> &matA, NUMTYPE scalar){
        return (scalar * matA);
    };

    friend Matrix<NUMTYPE> operator + ( const Matrix<NUMTYPE> &, NUMTYPE );
    friend inline Matrix<NUMTYPE> operator + (NUMTYPE scalar, const Matrix<NUMTYPE> &matA){
        return ( matA + scalar );
    };
    friend inline Matrix<NUMTYPE> operator - (const Matrix<NUMTYPE> &matA, NUMTYPE scalar){
        return ( matA + ( - scalar ) );
    };
    friend inline Matrix<NUMTYPE> operator - (NUMTYPE scalar, const Matrix<NUMTYPE> &matA){
        return -( matA + scalar );
    };
    friend inline Matrix<NUMTYPE> operator / (Matrix<NUMTYPE> &matA, NUMTYPE scalar){
        return ( ( NUMTYPE(1.0) / scalar ) * matA );
    };
    friend inline Matrix<NUMTYPE> operator / (NUMTYPE scalar, Matrix<NUMTYPE> &matA){
        return ( scalar * inverse( matA ) );
    };

// Unitary Matrix Operator Methods
    Matrix<NUMTYPE> operator! (void) const;
	Matrix<NUMTYPE> operator~ (void) const;
    inline Matrix<NUMTYPE> operator - ( void ) const 
	{
        return ( NUMTYPE(-1) * (*this) );
    };

// Transformations
    inline friend Matrix<NUMTYPE> conjugate_transpose( const Matrix<NUMTYPE> &aMat ) { return !aMat; }; 
    inline friend Matrix<NUMTYPE>           transpose( const Matrix<NUMTYPE> &aMat ) { return ~aMat; }; 
    friend Matrix<NUMTYPE>                  conjugate( const Matrix<NUMTYPE> &     ); 
    friend Matrix<NUMTYPE>                    inverse(       Matrix<NUMTYPE> &     );
	
// Matrix Exponentiation
	Matrix<NUMTYPE>  operator ^ ( int p ) const;    
	friend inline Matrix<NUMTYPE>  power( const Matrix<NUMTYPE> &aMat , int p ) {
	    return ( aMat^p );
	};

// Matrix elementwise functions (for signal processing)
    friend Matrix<NUMTYPE> operator & ( const Matrix<NUMTYPE> &, const Matrix<NUMTYPE> & ); // elementwise multiply
    friend Matrix<NUMTYPE> operator | ( const Matrix<NUMTYPE> &, const Matrix<NUMTYPE> & ); // elementwise divide

    friend Matrix<NUMTYPE> map ( NUMTYPE(*)( NUMTYPE ), const Matrix<NUMTYPE>& );
    friend Matrix<NUMTYPE> map ( NUMTYPE(*)( NUMTYPE, NUMTYPE ), const Matrix<NUMTYPE>&, const Matrix<NUMTYPE>& );

// Input Output
    friend ostream & operator << (ostream &, const Matrix<NUMTYPE>&);
    friend istream & operator >> (istream &, const Matrix<NUMTYPE>&);

// Special Matrices
    static Matrix<NUMTYPE>          Ones( unsigned int , unsigned int );
    static Matrix<NUMTYPE>      Identity( unsigned int );
    static Matrix<NUMTYPE> UpperTriangle( unsigned int );
    static Matrix<NUMTYPE> LowerTriangle( unsigned int );
    static Matrix<NUMTYPE>       Hilbert( unsigned int );
	
// Decompositions
    void                        setDecompose ( MatrixComposition<NUMTYPE> *, int );
    MatrixComposition<NUMTYPE> *getDecompose ( int ) const ;
    void                        delDecompose ( int );
};

//  *********************************************************************************
//
//  C++ Template Implementation for a Matrix Class
//
//  *********************************************************************************

//
// Use the following Template Classes
// 
#include "MatrixComposition.h"
#include "LU_Decomposition.h"
#include "Cholesky.h"

// ------------------------------------------------------------------------------------
//  CONSTRUCTORS
//

// ===================================
//
//  Default Constructor
//
// ===================================

template <class NUMTYPE>
Matrix<NUMTYPE>::Matrix(unsigned int r, unsigned int c, int fr, int fc, NUMTYPE *imat, const string &aLabel): 
	matrix_p( new MatrixStorage<NUMTYPE>(r,c,fr,fc,imat) ),
	label( aLabel ), decomp_list( new LinkedList< MatrixComposition<NUMTYPE> >)
{
    ; 
};

// ===================================
//
//  Copy Constructor
//
// ===================================

template <class NUMTYPE>
Matrix<NUMTYPE>::Matrix(const Matrix<NUMTYPE> &matA):
    label( matA.label ), decomp_list( matA.decomp_list ), matrix_p( matA.matrix_p )
{
	decomp_list->inc_refcount();
	matrix_p->inc_refcount();
};

// ------------------------------------------------------------------------------------
//
//  Destructor
//
//

template <class NUMTYPE>
Matrix<NUMTYPE>::~Matrix( void )
{
    decomp_list->dec_refcount();
	matrix_p->dec_refcount();
};


// ------------------------------------------------------------------------------------
//
//  Matrix Methods 
//
// 

// ===================================
//
//  Copy/Assignment Operator
//
// ===================================

template <class NUMTYPE>
Matrix<NUMTYPE>& Matrix<NUMTYPE>::operator=(const Matrix<NUMTYPE> &matB)   // performs a copy
{
	label = matB.label;
	
	decomp_list->dec_refcount();
	decomp_list = matB.decomp_list;
	decomp_list->inc_refcount();

    matrix_p->dec_refcount();
	matrix_p = matB.matrix_p;
    matrix_p->inc_refcount();

    return (*this);
};

// ===================================
//
//  Submatrix Access Operators
//
// ===================================

template <class NUMTYPE>
Matrix<NUMTYPE> Matrix<NUMTYPE>:: operator() ( Range<int> Row_Range, Range<int> Col_Range ) const
{ 
    register int r, c, nr, nc;
    Matrix<NUMTYPE> result( Row_Range.RangeCount(), Col_Range.RangeCount() );

    for( r = Row_Range.reset(), nr = 0; !Row_Range.done(); Row_Range.next(), nr++ ){
	    for( c = Col_Range.reset(), nc = 0; !Col_Range.done(); Col_Range.next(), nc++ ){
		    result[nr][nc] = (*this)[r][c];
		}
	}
    
	return result;
};

template <class NUMTYPE>
Matrix<NUMTYPE> Matrix<NUMTYPE>:: operator() ( int fr, int fc, int lr, int lc ) const 
{
    Matrix<NUMTYPE> result;

    result = Ones( ABS(lr-fr), ABS(lc-fc) );
    result.shift_to(fr,fc);

    return (result & *this);
};

// ===================================
//
//  Shift Matrix Upper Left Corner
//
// ===================================

template <class NUMTYPE>
Matrix<NUMTYPE>& Matrix<NUMTYPE>:: shift_to ( int new_fr, int new_fc )
{
    register int r;

//  check reference count - copy if necessary
    if( matrix_p->reference_count > 1 ) {
	    MatrixStorage<NUMTYPE> *oldmatrix_p;
		oldmatrix_p = matrix_p;
		
	    matrix_p = new MatrixStorage<NUMTYPE>( *oldmatrix_p );
		oldmatrix_p->dec_refcount();
	}

// adjust collumns
	for(r = first_row(); r< first_row() + number_of_rows(); r++)
			(*this)[r] += first_col() - new_fc;
    first_col() = new_fc;

// adjust rows
    matrix_p->matrix  +=  first_row() - new_fr;
    first_row() = new_fr;

    return *this;
};

// ===================================
//
//  Test Matrix for Symmetric Property
//
// ===================================

template <class NUMTYPE>
int Matrix<NUMTYPE>::is_symmetric( void ) const
{
    register int r,c;
	int flag;
	
	if( is_empty() )
		return (1);  // symmetric, but stupid
		
	if( number_of_rows() != number_of_cols())
	    return (0);  // obviously non-symmetric
		
	flag = 1;
	for ( r = first_row() + 1; r < first_row() + number_of_rows() - 1; r++)
	    for ( c = first_col(); c < r-first_row()+first_col(); c++)
		    if( (*this)[r][c] != (*this)[c-first_col()+first_row()][r-first_row()+first_col()] )
			    flag = 0;
			
	return (flag);
};

// ===================================
//
//  Test Matrix for Positive Definite Property
//
// ===================================

template <class NUMTYPE>
int Matrix<NUMTYPE>::is_positive_definite( void )
{
    Cholesky<NUMTYPE> cholesky_decomposition( *this );
	return ( cholesky_decomposition.is_empty() == 0 );
};

// ===================================
//
//  Test Matrix for Negative Definite Property
//
// ===================================

template <class NUMTYPE>
int Matrix<NUMTYPE>::is_negative_definite( void )
{
    Matrix<NUMTYPE> aMat = -(*this);
    Cholesky<NUMTYPE> cholesky_decomposition( aMat );
	return ( cholesky_decomposition.is_empty() == 0 );
};

// ===================================
//
//  Matrix Accumulation Operation
//
// ===================================

template <class NUMTYPE>
Matrix<NUMTYPE> & Matrix<NUMTYPE>::operator += ( const Matrix<NUMTYPE>& matA )
{
    int fr,fc,rows,cols;
    register int r,c;
    register NUMTYPE *ptr_r, *ptr_t, *ptr_a;
    MatrixStorage<NUMTYPE> *result_p;

  //  remove decomposition list - about to be invalidated
	decomp_list->dec_refcount();
    decomp_list = new LinkedList< MatrixComposition<NUMTYPE> >;
	
  //  calculate size of new matrix
    fr = MIN( first_row(), matA.first_row() );
    fc = MIN( first_col(), matA.first_col() );
    rows = MAX( first_row() + number_of_rows(), matA.first_row() + matA.number_of_rows() );
    cols = MAX( first_col() + number_of_cols(), matA.first_col() + matA.number_of_cols() );
    rows -= fr;
    cols -= fc;

 	result_p = new MatrixStorage<NUMTYPE>(rows, cols, fr, fc);
	
 //  add matrices
    for( r = first_row(); r < first_row() + number_of_rows(); r++ ){  
        ptr_r = &(result_p->matrix[r][ first_col() ]);
        ptr_t = &((*this)[r][ first_col() ]);
        for( c = 0; c < number_of_cols(); c++ )
            *ptr_r++ = *ptr_t++;
    }
    for( r = matA.first_row(); r < matA.first_row() + matA.number_of_rows(); r++ ){  
        ptr_r = &(result_p->matrix[r][ matA.first_col() ]);
        ptr_a = &(matA[r][ matA.first_col() ]);
        for( c = 0; c < matA.number_of_cols(); c++ )
            *ptr_r++ += *ptr_a++;
    }
    
 //  replace matrix storage with result
	matrix_p->dec_refcount();
    matrix_p = result_p;

    return (*this);
};

// ===================================
//
//  Add Scalar to Matrix Elements
//
// ===================================

template <class NUMTYPE>
Matrix<NUMTYPE> & Matrix<NUMTYPE>::operator += ( NUMTYPE scalar )
{
    register int r,c;

  //  check reference count - copy if necessary
    if( matrix_p->reference_count > 1 ) {
	    MatrixStorage<NUMTYPE> *oldmatrix_p;
		oldmatrix_p = matrix_p;
		
	    matrix_p = new MatrixStorage<NUMTYPE>( *oldmatrix_p );
		oldmatrix_p->dec_refcount();
	}

  //  remove decomposition list - about to be invalidated
	decomp_list->dec_refcount();
    decomp_list = new LinkedList< MatrixComposition<NUMTYPE> >;

  // Add Scalar to Matrix Elements
    for( r=first_row(); r<number_of_rows(); r++)
        for( c=first_col(); c<number_of_cols(); c++)
            (*this)[r][c] += scalar;

    return *this;
};

// ===================================
//
//  Matrix Accumulation (subtraction) Operation
//
// ===================================

template <class NUMTYPE>
Matrix<NUMTYPE> & Matrix<NUMTYPE>::operator -= ( const Matrix<NUMTYPE>& matA)
{
    int fr,fc,rows,cols;
    register int r,c;
    register NUMTYPE *ptr_r, *ptr_t, *ptr_a;
    MatrixStorage<NUMTYPE> *result_p;

  //  remove decomposition list - about to be invalidated
	decomp_list->dec_refcount();
    decomp_list = new LinkedList< MatrixComposition<NUMTYPE> >;
	
  //  calculate size of new matrix
    fr = MIN( first_row(), matA.first_row() );
    fc = MIN( first_col(), matA.first_col() );
    rows = MAX( first_row() + number_of_rows(), matA.first_row() + matA.number_of_rows() );
    cols = MAX( first_col() + number_of_cols(), matA.first_col() + matA.number_of_cols() );
    rows -= fr;
    cols -= fc;

 	result_p = new MatrixStorage<NUMTYPE>(rows, cols, fr, fc);
	
 //  add matrices
    for( r = first_row(); r < first_row() + number_of_rows(); r++ ){  
        ptr_r = &(result_p->matrix[r][ first_col() ]);
        ptr_t = &((*this)[r][ first_col() ]);
        for( c = 0; c < number_of_cols(); c++ )
            *ptr_r++ = *ptr_t++;
    }
    for( r = matA.first_row(); r < matA.first_row() + matA.number_of_rows(); r++ ){  
        ptr_r = &(result_p->matrix[r][ matA.first_col() ]);
        ptr_a = &(matA[r][ matA.first_col() ]);
        for( c = 0; c < matA.number_of_cols(); c++ )
            *ptr_r++ -= *ptr_a++;
    }
    
 //  replace matrix storage with result
	matrix_p->dec_refcount();
    matrix_p = result_p;

    return (*this);
};

// ===================================
//
//  Subtract Scalar to Matrix Elements
//
// ===================================

template <class NUMTYPE>
Matrix<NUMTYPE> & Matrix<NUMTYPE>::operator -= ( NUMTYPE scalar )
{
    register int r,c;

  //  check reference count - copy if necessary
    if( matrix_p->reference_count > 1 ) {
	    MatrixStorage<NUMTYPE> *oldmatrix_p;
		oldmatrix_p = matrix_p;
		
	    matrix_p = new MatrixStorage<NUMTYPE>( *oldmatrix_p );
		oldmatrix_p->dec_refcount();
	}

  //  remove decomposition list - about to be invalidated
	decomp_list->dec_refcount();
    decomp_list = new LinkedList< MatrixComposition<NUMTYPE> >;

  // Add Scalar to Matrix Elements
    for( r=first_row(); r<number_of_rows(); r++)
        for( c=first_col(); c<number_of_cols(); c++)
            (*this)[r][c] -= scalar;

    return *this;
};

// ===================================
//
//  Matrix Multiply Operation
//
// ===================================

template <class NUMTYPE>
Matrix<NUMTYPE> & Matrix<NUMTYPE>::operator *= ( const Matrix<NUMTYPE>& matA )
{
    register int r,c,q;
    register NUMTYPE *ptr_r, *ptr_a, *ptr_b;

	decomp_list->dec_refcount();
    decomp_list = new LinkedList< MatrixComposition<NUMTYPE> >;
	
  // MxQ times QxN
    if ( number_of_cols() == matA.number_of_rows() ){
        Matrix<NUMTYPE> A_bycols( transpose( matA ) );
		MatrixStorage<NUMTYPE> *result_p;
		
 	    result_p = new MatrixStorage<NUMTYPE>(number_of_rows(), matA.number_of_cols(), first_row(), matA.first_col());
			
		for( r = first_row(); r < first_row() + number_of_rows(); r++ ){  
			ptr_r = &( result_p->matrix[r][first_col()] );
            for ( c = first_col(); c < first_col() + number_of_cols(); c++, ptr_r++){
                ptr_a = &( (*this)[r][ first_col() ] );
                ptr_b = &( A_bycols[c][ matA.first_row() ] );
                *ptr_r = NUMTYPE( 0.0 );
                for ( q = 0; q < number_of_cols(); q++)
                    *ptr_r += *ptr_a++ * *ptr_b++; 
            }
		}
		
		matrix_p->dec_refcount();
		matrix_p=result_p;
 
  // 1x1 times NxM
    } else if ((number_of_rows() == 1) && (number_of_cols() == 1)){
        *this = (*this)[first_row()][first_col()] * matA;

    } else if ((matA.number_of_rows() == 1) && (matA.number_of_cols() == 1)){
         *this *= matA[matA.first_row()][matA.first_col()];

  // Error 
    } else {
        Abort("operator*=(const Matrix<NUMTYPE>& ):Incompatible Matrix Dimensions");
    }

    return *this;
};

// ===================================
//
//  Matrix Multiply by Scalar Operation
//
// ===================================

template <class NUMTYPE>
Matrix<NUMTYPE> & Matrix<NUMTYPE>::operator *= ( NUMTYPE scalar )
{
    register int r,c;

  //  check reference count - copy if necessary
    if( matrix_p->reference_count > 1 ) {
	    MatrixStorage<NUMTYPE> *oldmatrix_p;
		oldmatrix_p = matrix_p;
		
	    matrix_p = new MatrixStorage<NUMTYPE>( *oldmatrix_p );
		oldmatrix_p->dec_refcount();
	}

  //  remove decomposition list - about to be invalidated
	decomp_list->dec_refcount();
    decomp_list = new LinkedList< MatrixComposition<NUMTYPE> >;

    for( r = first_row(); r < number_of_rows(); r++)
        for( c = first_col(); c < number_of_cols(); c++)
            (*this)[r][c] *= scalar;

    return *this;
};

// ===================================
//
//  Matrix Divide (Solve) Operation
//
// ===================================

template <class NUMTYPE>
Matrix<NUMTYPE> & Matrix<NUMTYPE>::operator /= ( Matrix<NUMTYPE>& matA)
{

  //  check reference count - copy if necessary
    if( matrix_p->reference_count > 1 ) {
	    MatrixStorage<NUMTYPE> *oldmatrix_p;
		oldmatrix_p = matrix_p;
		
	    matrix_p = new MatrixStorage<NUMTYPE>( *oldmatrix_p );
		oldmatrix_p->dec_refcount();
	}

  //  remove decomposition list - about to be invalidated
	decomp_list->dec_refcount();
    decomp_list = new LinkedList< MatrixComposition<NUMTYPE> >;

    *this = *this / matA ;
	
	return *this;
};

// ===================================
//
//  Matrix MUltiply Element by Element Operation
//
// ===================================

template <class NUMTYPE>
Matrix<NUMTYPE> & Matrix<NUMTYPE>::operator &= ( const Matrix<NUMTYPE>& matA )
{
    int fr, fc, rows, cols;
    register int r,c;
    register NUMTYPE *ptr_r, *ptr_a, *ptr_b;
    MatrixStorage<NUMTYPE> *result_p;

  //  check reference count - copy if necessary
    if( matrix_p->reference_count > 1 ) {
	    MatrixStorage<NUMTYPE> *oldmatrix_p;
		oldmatrix_p = matrix_p;
		
	    matrix_p = new MatrixStorage<NUMTYPE>( *oldmatrix_p );
		oldmatrix_p->dec_refcount();
	}

  //  remove decomposition list - about to be invalidated
	decomp_list->dec_refcount();
    decomp_list = new LinkedList< MatrixComposition<NUMTYPE> >;

  //  calculate size of new matrix
    fr = MIN( first_row(), matA.first_row() );
    fc = MIN( first_col(), matA.first_col() );
    rows = MAX( first_row() + number_of_rows(), matA.first_row() + matA.number_of_rows() );
    cols = MAX( first_col() + number_of_cols(), matA.first_col() + matA.number_of_cols() );
    rows = rows - fr;
    cols = cols - fc;

    if((rows > 0) && (cols > 0)){
	
 	    result_p = new MatrixStorage<NUMTYPE>(rows, cols, fr, fc);
		
		for( r=fr; r < fr + rows; r++){
			ptr_r = &( result_p->matrix[r][fc] );
			ptr_a = &(          (*this)[r][fc] );
			ptr_b = &(             matA[r][fc] );
			for( c=0; c < cols; c++)
				*ptr_r++ = *ptr_a++ * *ptr_b++ ;
		}
		
        matrix_p->dec_refcount();
        matrix_p = result_p;
		
    } else {
		ptr_r = &((*this)[first_row()][first_col()]);
        for( r = 0; r < number_of_rows(); r++)
            for( c = 0; c < number_of_cols(); c++)
                *ptr_r++ = NUMTYPE( 0.0 );
    }
	
    return *this;
};

// ===================================
//
//  Matrix Division Element by Element Operation
//
// ===================================

template <class NUMTYPE>
Matrix<NUMTYPE> & Matrix<NUMTYPE>::operator |= ( const Matrix<NUMTYPE> & matA)
{
    int fr, fc, rows, cols;
    register int r,c;
    register NUMTYPE *ptr_r, *ptr_a, *ptr_b;
    MatrixStorage<NUMTYPE> *result_p;

  //  check reference count - copy if necessary
    if( matrix_p->reference_count > 1 ) {
	    MatrixStorage<NUMTYPE> *oldmatrix_p;
		oldmatrix_p = matrix_p;
		
	    matrix_p = new MatrixStorage<NUMTYPE>( *oldmatrix_p );
		oldmatrix_p->dec_refcount();
	}

  //  remove decomposition list - about to be invalidated
	decomp_list->dec_refcount();
    decomp_list = new LinkedList< MatrixComposition<NUMTYPE> >;

  //  calculate size of new matrix
    fr = MIN( first_row(), matA.first_row() );
    fc = MIN( first_col(), matA.first_col() );
    rows = MAX( first_row() + number_of_rows(), matA.first_row() + matA.number_of_rows() );
    cols = MAX( first_col() + number_of_cols(), matA.first_col() + matA.number_of_cols() );
    rows = rows - fr;
    cols = cols - fc;

    if((rows > 0) && (cols > 0)){
	
 	    result_p = new MatrixStorage<NUMTYPE>(rows, cols, fr, fc);
		
		for( r=fr; r < fr + rows; r++){
			ptr_r = &( result_p->matrix[r][fc] );
			ptr_a = &(          (*this)[r][fc] );
			ptr_b = &(             matA[r][fc] );
			for( c=0; c < cols; c++)
				*ptr_r++ = *ptr_a++ / *ptr_b++ ;
		}
		
        matrix_p->dec_refcount();
        matrix_p = result_p;
		
    } else {
		ptr_r = &((*this)[first_row()][first_col()]);
        for( r = 0; r < number_of_rows(); r++)
            for( c = 0; c < number_of_cols(); c++)
                *ptr_r++ = NUMTYPE( 0.0 );
    }
	
    return *this;
};

// ===================================
//
//  Apply a function to a Matrix Element by Element
//
// ===================================

template <class NUMTYPE>
Matrix<NUMTYPE> & Matrix<NUMTYPE>::apply ( NUMTYPE(*f)( NUMTYPE ) )
{
    *this = map( f, *this );
	 
	decomp_list->dec_refcount();
    decomp_list = new LinkedList< MatrixComposition<NUMTYPE> >;

    return *this;
};

// ===================================
//
//  Apply a function to two Matrices Element by Element
//
// ===================================

template <class NUMTYPE>
Matrix<NUMTYPE> & Matrix<NUMTYPE>::apply ( NUMTYPE(*f)( NUMTYPE, NUMTYPE ), const Matrix<NUMTYPE>& matA)
{
    *this = map( f, *this , matA);

	decomp_list->dec_refcount();
    decomp_list = new LinkedList< MatrixComposition<NUMTYPE> >;

    return *this;
};

// ------------------------------------------------------------------------------------
//
//  Matrix Operators and Functions of Matrices
//
// 

// ==============================================
//
//  Matrix Add
//
// ==============================================

template <class NUMTYPE>
Matrix<NUMTYPE> operator+( const Matrix<NUMTYPE> &matA, const Matrix<NUMTYPE> &matB )
{
    Matrix<NUMTYPE>   result;
    int fr, fc, rows, cols;
    register int r,c;
    register NUMTYPE *ptr_r, *ptr_a, *ptr_b;

    fr = MIN( matB.first_row(), matA.first_row() );
    fc = MIN( matB.first_col(), matA.first_col() );
    rows = MAX( matB.first_row() + matB.number_of_rows(), matA.first_row() + matA.number_of_rows() );
    cols = MAX( matB.first_col() + matB.number_of_cols(), matA.first_col() + matA.number_of_cols() );
    rows -= fr;
    cols -= fc;

    result = Matrix<NUMTYPE>( rows, cols, fr, fc );
    
    if ( !result.is_empty() ){
		for( r = matA.first_row(); r < matA.first_row() + matA.number_of_rows(); r++ ){  
			ptr_r = &( result[r][matA.first_col()] );
			ptr_a = &(   matA[r][matA.first_col()] );
			for( c = 0; c < matA.number_of_cols(); c++)
				*ptr_r++ = *ptr_a++;
		}
		for( r = matB.first_row(); r < matB.first_row() + matB.number_of_rows(); r++ ){  
			ptr_r = &( result[r][matB.first_col()] );
			ptr_b = &(   matB[r][matB.first_col()] );
			for( c = 0; c < matB.number_of_cols(); c++)
				*ptr_r++ += *ptr_b++;
		}
    }

    return result;
};

// ==============================================
//
//  Matrix Subtract
//
// ==============================================

template <class NUMTYPE>
Matrix<NUMTYPE> operator-( const Matrix<NUMTYPE> &matA, const Matrix<NUMTYPE> &matB )
{
    Matrix<NUMTYPE>   result;
    int fr, fc, rows, cols;
    register int r,c;
    register NUMTYPE *ptr_r, *ptr_a, *ptr_b;

    fr = MIN( matB.first_row(), matA.first_row() );
    fc = MIN( matB.first_col(), matA.first_col() );
    rows = MAX( matB.first_row() + matB.number_of_rows(), matA.first_row() + matA.number_of_rows() );
    cols = MAX( matB.first_col() + matB.number_of_cols(), matA.first_col() + matA.number_of_cols() );
    rows -= fr;
    cols -= fc;

    result = Matrix<NUMTYPE>( rows, cols, fr, fc );
    
    if ( !result.is_empty() ){
		for( r = matA.first_row(); r < matA.first_row() + matA.number_of_rows(); r++ ){  
			ptr_r = &( result[r][matA.first_col()] );
			ptr_a = &(   matA[r][matA.first_col()] );
			for( c = 0; c < matA.number_of_cols(); c++)
				*ptr_r++ = *ptr_a++;
		}
		for( r = matB.first_row(); r < matB.first_row() + matB.number_of_rows(); r++ ){  
			ptr_r = &( result[r][matB.first_col()] );
			ptr_b = &(   matB[r][matB.first_col()] );
			for( c = 0; c < matB.number_of_cols(); c++)
				*ptr_r++ -= *ptr_b++;
		}
    }

    return result;
};

// ==============================================
//
//  Matrix Multiply
//
// ==============================================

template <class NUMTYPE>
Matrix<NUMTYPE> operator*( const Matrix<NUMTYPE> &matA, const Matrix<NUMTYPE> &matB )
{    
    Matrix<NUMTYPE>   result;
    int fr, fc, rows, cols;
    register int r,c,q;
    register NUMTYPE *ptr_r, *ptr_a, *ptr_b;

// NUll Matrix in multiply
    if( matA.is_empty() || matB.is_empty()) {
        result = Matrix<NUMTYPE>(0, 0, 0, 0);

// MxQ times QxN
    } else if ( matA.number_of_cols() == matB.number_of_rows() ){
        Matrix<NUMTYPE> B_bycols( transpose( matB ) );

        fr = matA.first_row();
        rows = matA.number_of_rows();
        fc = matB.first_col();
        cols = matB.number_of_cols();
        result = Matrix<NUMTYPE>( rows,  cols, fr, fc );

		for (r=fr; r< fr + rows; r++){
			ptr_r = &(result[r][fc]);
			for ( c = fc; c < fc + cols; c++, ptr_r++){
				ptr_a = &(     matA[r][matA.first_col()] );
				ptr_b = &( B_bycols[c][matB.first_row()] );
				*ptr_r = NUMTYPE( 0.0 );
				for ( q = 0; q < matA.number_of_cols(); q++, ptr_a++, ptr_b++ )
					*ptr_r += (*ptr_a * *ptr_b); 
			}
		}

// 1x1 times NxM
    } else if ((matA.number_of_rows() == 1) && (matA.number_of_cols() == 1)){
        result = matA[ matA.first_row() ][ matA.first_col() ] * matB;

    } else if ((matB.number_of_rows() == 1) && (matB.number_of_cols() == 1)){
        result = matA * matB[ matB.first_row() ][ matB.first_col() ];

// Error 
    } else {
        Abort("operator*(const Matrix<NUMTYPE>&, const Matrix<NUMTYPE>& ):Incompatible Matrix Dimensions");
    }

    return result;
};

// ==============================================
//
//  Multiply -  Multiply a Matrix by scalar - Element by Element
//
// ==============================================

template <class NUMTYPE>
Matrix<NUMTYPE> operator*( NUMTYPE scalar, const Matrix<NUMTYPE> &matA )
{
    Matrix<NUMTYPE> result( matA );
    register int r,c;

	for( r = matA.first_row(); r < matA.first_row() + matA.number_of_rows(); r++)
		for( c = matA.first_col(); c < matA.first_col() + matA.number_of_cols(); c++)
			result[r][c] = matA[r][c] * scalar;

    return result;
};

// ==============================================
//
//  Add -  Add a scalar to a Matrix Element by Element
//
// ==============================================

template <class NUMTYPE>
Matrix<NUMTYPE> operator+( const Matrix<NUMTYPE> &matA, NUMTYPE scalar)
{
    Matrix<NUMTYPE> result( matA );
    register int r,c;

	for( r = matA.first_row(); r < matA.first_row() + matA.number_of_rows(); r++)
		for( c = matA.first_col(); c < matA.first_col() + matA.number_of_cols(); c++)
			result[r][c] = matA[r][c] + scalar;

    return result;
};

// ==============================================
//
//  Map -  Apply a function to a Matrix Element by Element
//
// ==============================================

template <class NUMTYPE>
Matrix<NUMTYPE> map( NUMTYPE (*f)(NUMTYPE), const Matrix<NUMTYPE> &matA){
    Matrix<NUMTYPE> result( matA );
    register int r,c;

	for( r = matA.first_row(); r < matA.first_row() + matA.number_of_rows(); r++)
		for( c = matA.first_col(); c < matA.first_col() + matA.number_of_cols(); c++)
			result[r][c] = (*f)(matA[r][c]);
	
    return result;
};

// ==============================================
//
//  Map -  Apply a function to two Matrices Element by Element
//
// ==============================================

template <class NUMTYPE>
Matrix<NUMTYPE> map( NUMTYPE (*f)(NUMTYPE, NUMTYPE), const Matrix<NUMTYPE> &matA, const Matrix<NUMTYPE> &matB){
    Matrix<NUMTYPE> result( matA );
    register int r,c;

    if ( matA.number_of_rows != matB.number_of_rows ) 
        Abort("map(NUMTYPE(*)(NUMTYPE, NUMTYPE),const Matrix<NUMTYPE>&,const Matrix<NUMTYPE>&):Incompatible Row Dimensions. \n");
    if ( matA.first_row != matB.first_row ) 
        Abort("map(NUMTYPE(*)(NUMTYPE, NUMTYPE),const Matrix<NUMTYPE>&,const Matrix<NUMTYPE>&):Incompatible Row Indices. \n");
    if ( matA.number_of_cols != matB.number_of_cols ) 
        Abort("map(NUMTYPE(*)(NUMTYPE, NUMTYPE),const Matrix<NUMTYPE>&,const Matrix<NUMTYPE>&):Incompatible Column Dimensions.\n");
    if ( matA.first_col != matB.first_col ) 
        Abort("map(NUMTYPE(*)(NUMTYPE, NUMTYPE),const Matrix<NUMTYPE>&,const Matrix<NUMTYPE>&):Incompatible Column Indices. \n");

	for( r = matA.first_row(); r < matA.first_row() + matA.number_of_rows(); r++)
		for( c = matA.first_col(); c < matA.first_col() + matA.number_of_cols(); c++)
			result[r][c] = (*f)( matA[r][c], matB[r][c] );
	
    return result;
};

// ==============================================
//
//  Multiplication -  Matrix Element by Element
//
// ==============================================

template <class NUMTYPE>
Matrix<NUMTYPE> operator & ( const Matrix<NUMTYPE> &matA, const Matrix<NUMTYPE> &matB){
    Matrix<NUMTYPE> result;
    int fr, fc, rows, cols;
    register int r,c;
    register NUMTYPE *ptr_r, *ptr_a, *ptr_b;

    fr = MAX( matB.first_row(), matA.first_row() );
    fc = MAX( matB.first_col(), matA.first_col() );
    rows = MIN( matB.first_row() + matB.number_of_rows(), matA.first_row() + matA.number_of_rows() );
    cols = MIN( matB.first_col() + matB.number_of_cols(), matA.first_col() + matA.number_of_cols() );
    rows = rows - fr;
    cols = cols - fc;

    if((rows > 0) && (cols > 0)){
		result = Matrix<NUMTYPE>( rows,  cols, fr, fc  );
		for( r=fr; r < fr + rows; r++){
			ptr_r = &( result[r][fc] );
			ptr_a = &(   matA[r][fc] );
			ptr_b = &(   matB[r][fc] );
			for( c=0; c < cols; c++)
				*ptr_r++ = *ptr_a++ * *ptr_b++ ;
		}
    } else {
		result = Matrix<NUMTYPE>( matA.number_of_rows(), matA.number_of_cols() );
    }
    return result;
};

// ==============================================
//
//  Division -  Matrix Element by Element
//
// ==============================================

template <class NUMTYPE>
Matrix<NUMTYPE> operator | ( const Matrix<NUMTYPE> &matA, const Matrix<NUMTYPE> &matB){
    Matrix<NUMTYPE> result;
    int fr, fc, rows, cols;
    register int r,c;
    register NUMTYPE *ptr_r, *ptr_a, *ptr_b;

    fr = MAX( matB.first_row(), matA.first_row() );
    fc = MAX( matB.first_col(), matA.first_col() );
    rows = MIN( matB.first_row() + matB.number_of_rows(), matA.first_row() + matA.number_of_rows() );
    cols = MIN( matB.first_col() + matB.number_of_cols(), matA.first_col() + matA.number_of_cols() );
    rows = rows - fr;
    cols = cols - fc;

    if((rows > 0) && (cols > 0)){
		result = Matrix<NUMTYPE>( rows, cols, fr, fc );
		for( r=fr; r < fr + rows; r++){
			ptr_r = &( result[r][fc] );
			ptr_a = &(   matA[r][fc] );
			ptr_b = &(   matB[r][fc] );
			for( c=0; c < cols; c++)
				*ptr_r++ = *ptr_a++ / *ptr_b++ ;
		}
    } else {
		result = Matrix<NUMTYPE>( matA.number_of_rows(), matA.number_of_cols(), "Empty");
    }
    return result;
};

// ==============================================
//
//  Matrix Division
//
// ==============================================

template <class NUMTYPE>
Matrix<NUMTYPE> operator / (Matrix<NUMTYPE> &matA, Matrix<NUMTYPE> &matB)
{
  // solve X = matA / matB
  // means :
  //  matB * X = matA
  //         X = inverse( matB ) * matA
  //  NOT
  //  X * matB = matA
  //         X = matA * inverse( matB );
  // if the second case occurs, then the alg cannot
  // recognize it. But you can transpose the input to fix it.
  //
    if ((matB.number_of_rows() == 1)&&(matB.number_of_cols() == 1)){
        return (matA * ( NUMTYPE( 1.0 ) / matB[matB.first_row()][matB.first_col()] ));
    } else {
        LU_Decomposition<NUMTYPE> LU( matB );
        return  LU.solve_for( matA );
    }
};

// ==============================================
//
//  Matrix Inverse
//
// ==============================================

template <class NUMTYPE>
Matrix<NUMTYPE> inverse( Matrix<NUMTYPE> &matA )
{
    Matrix<NUMTYPE> soln;
    unsigned int size;

    size = matA.number_of_rows();
	
    if ( size != matA.number_of_cols() )
        Abort("inverse( const Matrix<NUMTYPE>& ):Singular! - Matrix Argument is not square!");

    if ((matA.number_of_rows() == 1) && (matA.number_of_cols() == 1)){
        soln = matA;
        soln[matA.first_col()][matA.first_row()] = NUMTYPE( 1 ) / matA[matA.first_col()][matA.first_row()];
    } else {
        LU_Decomposition<NUMTYPE> lu = matA;
		Matrix<NUMTYPE> b = Matrix<NUMTYPE>::Identity( size );
		b.shift_to( matA.first_col(), matA.first_row() );
		soln = ( lu.solve_for( b ) );
    }
	
	soln.setLabel( matA.getLabel() + string("_inverse") );
	
	return soln;
};

// ==============================================
//
//  Matrix Power
//
// ==============================================

template <class NUMTYPE>
Matrix<NUMTYPE> Matrix<NUMTYPE>::operator ^ ( int p ) const
{
	Matrix<NUMTYPE> result( *this );
	static int isTop = 1;
	
	if ( isTop ) {
	    char buff[50];
		
		if( Rows() != Cols() ){
			char buff[100];
			sprintf( buff, "power( Matrix: %d x %d, %d ): Cannot compute! Matrix is not square!", Rows(), Cols(), p );
			Abort( buff );
		}
		
	    isTop = 0;
		result = (*this)^p;
		isTop = 1;
		
		sprintf( buff,"^%d", p );
	    result.setLabel( getLabel() + string(buff) );
	} else {
		if ( p < 0 ){ // negative exponent
			Matrix<NUMTYPE> temp( (*this)^(-p) );
			result = inverse( temp );
		} else if ( p == 0 ) { // zero exponent
			result = Matrix<NUMTYPE>::Identity( Rows() );
		} else if ( (p % 2) == 1 ) { // odd
			result = ( (*this) * ( (*this)^(p-1) ));
		} else { // even
			Matrix<NUMTYPE> temp( (*this)^(p/2) );
			result = ( temp * temp );
		}
	}
	
	return result;
};

// ==============================================
//
//  Matrix Norm
//
// ==============================================
//
//   default Euclidean norm p = 2
//   p = 1 -> 1-norm, p=0 -> infinity norm
//

template <class NUMTYPE>
NUMTYPE norm ( const Matrix<NUMTYPE>& matA, double p )
{
    register int r,c;
    register NUMTYPE temp, max_val;

    if ( p == 0.0 ){        // infinity norm (max row-sum norm)
        for( max_val = 0.0, r = matA.first_row();
             r < matA.first_row() + matA.number_of_rows();
			 r++
		    )
		{
			for( temp = 0.0, c = matA.first_col();
                 c < matA.first_col() + matA.number_of_cols();
				 c++
				)   temp += fabs( matA[r][c] );
             max_val = MAX( max_val, temp );
        }
    } else if ( p == 1.0 ){ // 1 - norm      (max col-sum norm)
		for( max_val = 0.0, c = matA.first_col();
			 c < matA.first_col() + matA.number_of_cols(); 
			 c++
			)
		{
			for( temp = 0.0, r = matA.first_row();
				 r < matA.first_row() + matA.number_of_rows(); 
				 r++
				)   temp += fabs( matA[r][c] );
            max_val = MAX( max_val, temp );
        }
    } else {              // p - norm => p=2 euclidean norm
		for( max_val = 0.0, c = matA.first_col();
			 c < matA.first_col() + matA.number_of_cols(); 
			 c++
			)
		{
			for( temp = 0.0, r = matA.first_row();
				 r < matA.first_row() + matA.number_of_rows(); 
				 r++
				)   temp += fabs( pow(  matA[r][c], p ) );
            temp = pow( temp , (1.0 / p) );
            max_val = MAX( max_val, temp );
        }
    }

    return (NUMTYPE)max_val;
};

// ==============================================
//
//  Matrix Determinant
//
// ==============================================

template <class NUMTYPE>
NUMTYPE determinant ( Matrix<NUMTYPE>& matA )
{
    LU_Decomposition<NUMTYPE> lu;
    unsigned int size;

    if ((size = matA.number_of_rows()) != matA.number_of_cols())
        Abort("determinant( const Matrix<NUMTYPE>& ):Singular! - Matrix Argument is not square!");

    lu = LU_Decomposition<NUMTYPE>( matA );
    
    return lu.determinant();
}; 

// ==============================================
//
// Comparison Operation
//
// ==============================================

template <class NUMTYPE>
int operator == ( const Matrix<NUMTYPE>& matA, const Matrix<NUMTYPE>& matB )
{
    register int r,c;
    register NUMTYPE result;

    if ( matA.number_of_rows() != matB.number_of_rows() )  return 0;
    if (      matA.first_row() != matB.first_row()      )  return 0;
    if ( matA.number_of_cols() != matB.number_of_cols() )  return 0;
    if (      matA.first_col() != matB.first_col()      )  return 0;

    result = 0.0;
    for( r = matA.first_row(); r < matA.first_row() + matA.number_of_rows(); r++)
        for( c = matA.first_col(); c < matA.first_col() + matA.number_of_cols(); c++)
            result += fabs( matA[r][c] - matB[r][c] );

    return (result == 0.0);
};

// ==============================================
//
//  Matrix Transpose
//
// ==============================================

template <class NUMTYPE>
Matrix<NUMTYPE> Matrix<NUMTYPE>:: operator~ () const
{
    register NUMTYPE *ptr_dest;
    register int r,c;
    Matrix<NUMTYPE> result( number_of_cols(), number_of_rows(), first_col(), first_row() );

    if ( !result.is_empty() ){
		for( r = result.first_row(); r < result.first_row() + result.number_of_rows(); r++ ){
			ptr_dest = &( result[r][result.first_col()] );
			for( c = result.first_col(); c < result.first_col() + result.number_of_cols(); c++ ){
				*ptr_dest++ = (*this)[c][r];
			}
		}
    }
	
	result.setLabel( getLabel() + string("_transpose") );

    return result;
};

// ==============================================
//
//  Default conj() function
//
//  conj() is really only defined for Complex 
//  quantities.
//
// ==============================================

template <class T>
inline T conj( T val ){ return val; };

// ==============================================
//
//  Matrix Conjugate
//
// ==============================================

template <class NUMTYPE>
Matrix<NUMTYPE> conjugate( const Matrix<NUMTYPE> &matA )
{
    register NUMTYPE *ptr_dest;
    register int r,c;
    Matrix<NUMTYPE>   result( matA.number_of_rows(), matA.number_of_cols(), matA.first_row(), matA.first_col() );

    if ( !result.is_empty() ){
		for( r = result.first_row(); r < result.first_row() + result.number_of_rows(); r++ ){
			ptr_dest = &( result[r][result.first_col()] );
			for( c = result.first_col(); c < result.first_col() + result.number_of_cols(); c++ ){
				*ptr_dest++ = conj( matA[r][c] );
			}
		}
    }
	
	result.setLabel( matA.getLabel() + string("_conjugate") );
    return result;
};

// ==============================================
//
//  Matrix Conjugate Transpose
//
// ==============================================

template <class NUMTYPE>
Matrix<NUMTYPE> Matrix<NUMTYPE>:: operator! () const
{
    register NUMTYPE *ptr_dest;
    register int r,c;
    Matrix<NUMTYPE>   result( number_of_cols(), number_of_rows(), first_col(), first_row() );

    if ( !result.is_empty() ){
		for( r = result.first_row(); r < result.first_row() + result.number_of_rows(); r++ ){
			ptr_dest = &( result[r][result.first_col()] );
			for( c = result.first_col(); c < result.first_col() + result.number_of_cols(); c++ ){
				*ptr_dest++ = conj( (*this)[c][r] );
			}
		}
    }
	result.setLabel( getLabel() + string("_conjugate_transpose") );
    return result;
};

// ------------------------------------------------------------
//
//  Matrix Stream Operators
//
//

// ==============================================
//
//  Matrix Stream Output
//
// ==============================================

template <class NUMTYPE>
ostream & operator << (ostream &cbuf, const Matrix<NUMTYPE> &matA)
{
    register NUMTYPE *ptr;
    register unsigned r,c;

    if ( (matA.getLabel()).length() > 0)
        cbuf << matA.getLabel() << " ";
		
    cbuf << "at ( " << matA.first_row() ;
    cbuf << " , " << matA.first_col() << " )\n";
	
    if( matA.is_empty() )
        cbuf << "Null Matrix\n";
    else {
		for ( r = matA.first_row(); r < matA.first_row() + matA.number_of_rows(); r++){
			ptr = &( matA[r][matA.first_col() ]);
			for ( c = 0; c < matA.number_of_cols(); c++){
				cbuf << *ptr++ << "\t";
			}
			cbuf << "\n";
		}
		cbuf << "\n";
    }
	
    return cbuf;
};

// ==============================================
//
//  Matrix Stream Input  (insufficient - needs work)
//
// ==============================================

template <class NUMTYPE>
istream & operator >> (istream &cbuf, Matrix<NUMTYPE> &matA)
{
    register NUMTYPE *ptr;
    register unsigned short r,c;

    for ( r = matA.first_row(); r < matA.first_row() + matA.number_of_rows(); r++){
        ptr = &( matA[r][matA.first_col()] );
        for ( c = 0; c < matA.number_of_cols(); c++){
            cbuf >> *ptr++;
        }
    }
    return cbuf;
};


// ------------------------------------------------------------
//
// Special Matrices
//
// 

// ==============================================
//
//  Matrix containing all 1's 
//
// ==============================================

template <class NUMTYPE>
Matrix<NUMTYPE> Matrix<NUMTYPE>::Ones( unsigned int rows, unsigned int cols )
{
	register int i,j;
	Matrix<NUMTYPE> id( rows, cols, 0, 0, 0, string("Ones") );
	for(i=0;i<rows;i++)
		for(j=0;j<cols;j++)
			id[i][j] = NUMTYPE( 1 );
	return id;
};

// ==============================================
//
//  Identity Matrix 
//
// ==============================================

template <class NUMTYPE>
Matrix<NUMTYPE> Matrix<NUMTYPE>::Identity( unsigned int size )
{
	register int i;
	Matrix<NUMTYPE> id( size, size, 0, 0, 0, string("Identity") );
	for(i=0;i<size;i++)
		id[i][i] = NUMTYPE( 1 );
	return id;
};

// ==============================================
//
//  Upper Triangular Matrix of 1's 
//
// ==============================================

template <class NUMTYPE>
Matrix<NUMTYPE> Matrix<NUMTYPE>::UpperTriangle( unsigned int size )
{
	register int i,j;
	Matrix<NUMTYPE> id( size, size, 0, 0, 0, string("UpperTriangular") );
	for(i=0;i<size;i++)
		for(j=i;j<size;j++)
			id[i][j] = NUMTYPE( 1 );
	return id;
};

// ==============================================
//
//  Lower Triangular Matrix of 1's 
//
// ==============================================

template <class NUMTYPE>
Matrix<NUMTYPE> Matrix<NUMTYPE>::LowerTriangle( unsigned int size )
{
	register int i,j;
	Matrix<NUMTYPE> id( size, size, 0, 0, 0, string("LowerTriangular") );
	for(i=0;i<size;i++)
		for(j=0;j<=i;j++)
			id[i][j] = NUMTYPE( 1 );
	return id;
};

// ==============================================
//
//  Hilbert Matrix 
//
// ==============================================

template <class NUMTYPE>
Matrix<NUMTYPE> Matrix<NUMTYPE>::Hilbert( unsigned int size ) 
{
	register int i,j;
	Matrix<NUMTYPE> hilbertm( size, size, 0, 0, 0, string("Hilbert") );
	
	for(i=0;i<size;i++)
		for(j=0;j<size;j++)
			hilbertm[i][j] = NUMTYPE(1)/( NUMTYPE(i) + NUMTYPE(j) + NUMTYPE(1));
			
	return hilbertm;
};

//
// ---------------------------------------------------------------------
//

// ---------------------------------------------------------------------
//
// Decomposition Methods
//
// 

// ==============================================
//
//  Add a Decomposition to Matrix's Decomposition List
//
// ==============================================

template <class NUMTYPE>
void Matrix<NUMTYPE>:: setDecompose ( MatrixComposition<NUMTYPE> *comA, int  key )
{
    decomp_list->add( comA, key );
};

// ==============================================
//
//  Get a Decomposition from a Matrix's Decomposition List
//
// ==============================================

template <class NUMTYPE>
MatrixComposition<NUMTYPE>* Matrix<NUMTYPE>:: getDecompose ( int key ) const
{
    return decomp_list->find( key );
};

// ==============================================
//
//  Remove a Decomposition from a Matrix's Decomposition List
//
// ==============================================

template <class NUMTYPE>
void Matrix<NUMTYPE>:: delDecompose ( int  key )
{
    decomp_list->del( key );
};

//
// ---------------------------------------------------------------------
//

// ---------------------------------------------------------------------
#endif // _Matrix_h

//
//  History:
//    $Log: Matrix.h,v $
//    Revision 1.1  1997/09/13 23:38:41  jak
//    Initial revision
//
//
