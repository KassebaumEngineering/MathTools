
//  LinkedList.h
//  C++ Template Class Definition
//
//  Copyright (C) 1993, John A. Kassebaum
//
//  This software was written by John Kassebaum as a part of his graduate
//  course work at Purdue.  He retains all rights to this software.  
//
//  *************************  Description  **************************
//  LinkedList Object:
//    The LinkedList Object is a simple list of objects with a few 
//    access methods: add(), del(), next(), and reset(). 
//
// ------------------------------------------------------------------
//  Public Data:
//  None.
// ------------------------------------------------------------------
//  Responding Messages:
//
//  LinkedList( void )                                 CONSTRUCTOR
//    This constructor creates an empty list.
//
//  ~LinkedList( void )                                DESTRUCTOR
//    The destructor deletes all of the list as well as calling delete on
//    all members in it's "datum" fields.  
//
//  void add( T *object, int ident )
//    The add() method places your object in the list with the identifier 
//    "ident".
//
//  void del( void )
//    The del() method will remove the current object from the list, 
//    calling delete upon it as described above in the DESTRUCTOR.
//
//  void del( int  )
//    The del(int ) method will remove the labeled object from the list, 
//    calling delete upon it as described above in the DESTRUCTOR.
//
//  T & find( int  )
//    The find() method returns a reference to the labelled object in the
//    list.
//
//  void *next( void )
//    The next() method returns a pointer to the next object in the list. If 
//    the list is empty, or if you have reached the end of the list, next()
//    will return the Null pointer, (void *)0.  The first call to next() always 
//    returns the element at the head of the list.
//
//  void reset( void )
//    The reset() method returns you to the head of the list.  The next call to 
//    next() will obtain the first element of the list.
//
// ------------------------------------------------------------------
//  Original Messages:
//    None.
//
// ------------------------------------------------------------------
//  Side Effects:
//    None.
//
// ------------------------------------------------------------------
//  Known Bugs:
//    None. 
//
//  ---------------------------------------------------------------------------
//  Author(s) and Credits:
//	  This Software was originally written by John Kassebaum as a part of 
//    his graduate classwork at Purdue University.  
//
// ------------------------------------------------------------------
//  Revision:
//    $Id: LinkedList.h,v 1.1.1.1 1997/09/13 23:38:40 jak Exp $
//
//  ******************************************************************
#ifndef _LinkedList_h
#define _LinkedList_h

static char rcsid_LinkedList_h[] = "$Id: LinkedList.h,v 1.1.1.1 1997/09/13 23:38:40 jak Exp $";

#pragma interface

template <class T> class LinkedList;

template <class T> class LL_Node {
    friend class LinkedList<T>;
private:
    int          id;
     T*          datum;
    LL_Node<T>  *next;

public:
    LL_Node(T* something, LL_Node<T> *nxt, int name):
        datum(something), next(nxt), id(name)
    {};
   ~LL_Node( void ){
        delete datum;
    };

    inline int getId( void ) { return id; };
};


template <class T> class LinkedList {
    friend class LL_Node<T>;
private:
    int Ref_count;
    LL_Node<T> *head;
    LL_Node<T> *current;

public:
// Constructor and Destructor
    LinkedList( void );
    ~LinkedList( void );

// Other Methods
    void  add ( T* , int  );
    void  del ( void );
    void  del ( int );
    T*    next ( void );
    T*    find ( int  ) const;
    void  relabel ( int , int  );
	
	inline void inc_refcount( void ){
	    Ref_count++;
	};

	inline void dec_refcount( void ){
	    if( Ref_count-- <= 0 )
		    delete this;
	};

    inline void reset( void ){
        current = head;
    };
};

//  ************************************************************
//
//  C++ Template Implementation for a LinkedList Class
//
//  ************************************************************

// --------------------------------------------------
//  CONSTRUCTOR
//
template <class T>
LinkedList<T>::LinkedList( void ) : head(0), current(0), Ref_count(0)
{
};

// --------------------------------------------------
//  Destructor
//
template <class T>
LinkedList<T>::~LinkedList( void ) 
{
    LL_Node<T> *index_ptr, *next_index_ptr;
    index_ptr = head;
    while ( index_ptr != (LL_Node<T> *)0 ){
        next_index_ptr = index_ptr->next;
        delete index_ptr;
        index_ptr = next_index_ptr;
    }
}

// --------------------------------------------------
//  Other instance methods
//
template <class T>
void LinkedList<T>::add(T *thing, int name ){
    LL_Node<T> *index_ptr;

    index_ptr = new LL_Node<T>( thing, head, name );
    head = index_ptr;
};

template <class T>
void LinkedList<T>::del(void){
    LL_Node<T> *index_ptr, *prev_index_ptr;
    
    index_ptr = current;
    if (index_ptr == (LL_Node<T> *)0){   // Empty List
        return;
    } else if ( index_ptr == head ){  // 1st element
        current = index_ptr->next;
        head = index_ptr->next;
        delete index_ptr;
    } else {                          // Middle of list
        prev_index_ptr = head;
        while(prev_index_ptr->next != current) 
             prev_index_ptr = prev_index_ptr->next;
        current = index_ptr->next;
        prev_index_ptr->next = current;
        delete index_ptr;
    }
};

template <class T>
void LinkedList<T>::del( int  name  ){
    LL_Node<T> *index_ptr, *prev_index_ptr;
    index_ptr = head;
    if (index_ptr == (LL_Node<T> *)0){       // Empty List
        return;
    } else if (index_ptr->id == name ){  // Matches 1st element
        head = index_ptr->next;
        delete index_ptr;
    } else {                // Matches Any element or not in list
        while( index_ptr != (LL_Node<T> *)0 ){
            prev_index_ptr = index_ptr;
            index_ptr = index_ptr->next;
            if ((index_ptr != (LL_Node<T> *)0) && index_ptr->id == name ){
                prev_index_ptr->next = index_ptr->next;
                delete index_ptr;
            }
        }
    }
};

template <class T>
T* LinkedList<T>::find( int name ) const
{
    LL_Node<T> *index_ptr, *prev_index_ptr;
    T *rtn;

    rtn = (T *)0;
    index_ptr = head;
    if (index_ptr == (LL_Node<T> *)0){   // Empty List
        rtn = (T *)0;
    } else if (index_ptr->id == name ){  // Matches 1st element
        rtn = index_ptr->datum;
    } else {                // Matches Any element or not in list
        while( index_ptr != (LL_Node<T> *)0 ){
            prev_index_ptr = index_ptr;
            index_ptr = index_ptr->next;
            if ((index_ptr != (LL_Node<T> *)0) && index_ptr->id == name ){
                rtn = index_ptr->datum;
            }
        }
    }

    return rtn;
};

template <class T>
void LinkedList<T>::relabel( int prevname, int newname)
{
    LL_Node<T> *index_ptr, *prev_index_ptr;

    index_ptr = head;
    if (index_ptr == (LL_Node<T> *)0){       // Empty List
        ;
    } else if (index_ptr->id == prevname ){  // Matches 1st element
        index_ptr->id = newname ;
    } else {                // Matches Any element or not in list
        while( index_ptr != (LL_Node<T> *)0 ){
            prev_index_ptr = index_ptr;
            index_ptr = index_ptr->next;
            if ((index_ptr != (LL_Node<T> *)0) && index_ptr->id == prevname ){
                index_ptr->id = newname ;
            }
        }
    }
};

template <class T>
T* LinkedList<T>::next( void ) {
    T *rtn;

    if ( current == (LL_Node<T> *)0)
        rtn = (T *)0;
    else  {
        rtn = current->datum;
        current = current->next;
    }

    return rtn;
};

#endif

//
//  History:
//    $Log: LinkedList.h,v $
//    Revision 1.1.1.1  1997/09/13 23:38:40  jak
//    Restored MathTools to new CVS repository. -jak
//
//
