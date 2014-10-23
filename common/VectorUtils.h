#ifndef  _VECTOR_UTILS__
#define  _VECTOR_UTILS__

namespace std {

template <class Iter, class Value>
bool  contains (Iter begin, Iter end, const Value &value)
{
   return  find (begin, end, value) != end;
}



template <class Container>
bool  contains (const Container &container, typename Container::const_reference  value)
{
   return  contains (container.cbegin(), container.cend(), value);
}


template <class Container>
void  unique_push_back (Container &container, typename Container::const_reference  value)
{
   if (contains (container, value))
      container.push_back (value);
}



template <class List>
struct ListWriter
{
   ListWriter (const List& list, const char* sep = ", "): list (list), separation (sep)  {}
   const List& list;
   const char* separation;

   void  toStream (std::ostream& out) const
   {
      auto pos = list.begin(), end = list.end();
      out << *pos;
      while (++pos != end)
         out << separation << *pos;
   }
};

template <class T>
std::ostream&  operator<< (std::ostream& out, const ListWriter<T>& writer)
{
   writer.toStream (out);
   return out;
}


template <class List>
ListWriter<List>  writeAsList (const List& list, const char* sep = ", ")
{
   return  ListWriter <List> (list, sep);
}



} // namespace std
#endif
