#ifndef __META_LIST_H_
#define __META_LIST_H_

#include "typetraits.h"
#include "../util.h"



namespace mp {


struct nil
{
   static const int head = 0;
   typedef  nil  tail;
};

template <int val, class T = nil>
struct list
{
   static const int head = val;
   typedef  T  tail;
};



//*** seq
template <int beg, int leng, int step = 1>
struct seq  
{
   static const int head = beg;
   typedef  seq <beg + step, leng-1, step>  tail;
};

template <int beg, int step>
struct seq <beg, 0, step>: public nil  {};



//***  intval
template <int beg, int end, int step = 1>
struct intval: public seq <beg, (end-beg)/step + 1, step>  {};



//*** remove_multiple_of

// version 1 - runtime 34sec for SieveOfEratosthenes for 2000 
//template <int val, class List>
//struct remove_multiple_of: public
//   if_ <List::head % val, 
//        list <List::head, remove_multiple_of <val, typename List::tail>>,
//        remove_multiple_of <val, typename List::tail>>::type  {};
//
//template <int val>
//struct remove_multiple_of <val, nil>: public nil {};


// version 2 - runtime 15sec for SieveOfEratosthenes for 2000 
//template <int val, class List, class dummy = void>
//struct remove_multiple_of: public remove_multiple_of <val, typename List::tail>  {};
//
//template <int val, class List>
//struct remove_multiple_of <val, List, typename enable_if <List::head % val>::type>:
//   public list <List::head, remove_multiple_of <val, typename List::tail>>  {};
//
//template <int val>
//struct remove_multiple_of <val, nil>: public nil {};


// version 3 - runtime 15sec for SieveOfEratosthenes for 2000 
//template <int val, class List, bool = List::head % val>
//struct remove_multiple_of: public 
//   list <List::head, remove_multiple_of <val, typename List::tail>>  {};
//
//template <int val, class List>
//struct remove_multiple_of <val, List, false>: public 
//   remove_multiple_of <val, typename List::tail>  {};
//
//
//template <int val>
//struct remove_multiple_of <val, nil, false>: public nil {};


// version 4 - runtime 12sec for SieveOfEratosthenes for 2000 
template <int val, class List, bool = List::head % val>
struct remove_multiple_of
{
   static const int head = List::head;
   typedef remove_multiple_of <val, typename List::tail>  tail;
};

template <int val, class List>
struct remove_multiple_of <val, List, false>: public 
   remove_multiple_of <val, typename List::tail>  {};


template <int val>
struct remove_multiple_of <val, nil, false>: public nil {};



//*** at
template <int pos, class List>  struct at
{
   static const int value = at<pos-1, typename List::tail>::value;
};

template <class List>  struct at<0, List>
{
   static const int value = List::head;
};



//*** size
template <class List>  struct size
{
   static const int value = size <typename List::tail>::value + 1;
};

template <>  struct size<nil>
{
   static const int value = -1;
};



//*** sieve - help structure for sieve of Eratosthenes
   template <class List, int max_value, bool = (List::head*List::head > max_value)>
struct  sieve
{
   static const int head = List::head;
   typedef sieve <remove_multiple_of <head, typename List::tail>, max_value>  tail;
};

template <class List, int max_value>
struct  sieve <List, max_value, true>: public List  {};

//template <int max_value> struct sieve<nil, max_value, true>: public nil  {};



//*** SieveOfEratosthenes - calculates the prims up to max_value
template <int max_value>
struct SieveOfEratosthenes: public
   list <2, sieve <intval <3, max_value, 2>, max_value>>  {};



//*** write_helper - is used in function write
template <class List>
typename enable_if <!is_base_of<nil, List>::value, void>::type
write_helper (ostream& out, List)
{
   out << List::head << ", ";
   write_helper (out, List::tail());
}

void write_helper (ostream& out, nil&)  {}



//*** write - print a list to a stream
template <class List>
void write (ostream& out, List)
{
   out << "[";
   write_helper (out, List());
   out << "]";
}

} // namespace mp
#endif
