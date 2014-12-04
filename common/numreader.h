#ifndef __NUMREADER_
#define __NUMREADER_


namespace mgo {

template <class Num>
bool in (Num val, Num ub)
{
   return  val >= 0  && val <= ub;
}


template <class Num>
bool in (Num val, Num lb, Num ub)
{
   return  val >= lb  && val <= ub;
}


enum  NumeralSystem
{
   Decimal,
   Binary,
   Hexadecimal
};



template <class Range, class T>
bool  readInteger (Range &range, T &value)
{
   if (range.empty())  return false; 
   Range::Iterator  begin = range.begin();

   if (range.front() == '-'  ||  range.front() == '+')   range.pop_front();  // sign character
   
   if (range.empty()  ||  !in (range.front(), '0', '9'))   // first character is not a digit
   {
      range.begin (begin);
      return false;
   }
   value = range.front() - '0';
   range.pop_front();

   while (!range.empty()  &&  in (range.front(), '0', '9'))
   {
      value = value*10 + range.front() - '0';
      range.pop_front();
   }

   if (*begin == '-')  value = -value;
   return true;
}



template <class iterT, class typeT>
bool  ReadIntegerBinary (iterT &iter, iterT end, typeT &value)
{
   iterT begin = iter;

   for (value = 0;  iter != end;  ++iter)
   {
      if (*iter == '0')  value <<= 1;
      else if (*iter == '1')  value = (value << 1) + 1;
      else return  begin != iter;
   }
   return  iter != begin;
}



template <class iterT, class typeT>
bool  ReadIntegerHex (iterT &iter, iterT end, typeT &value)
{
   iterT begin = iter;

   for (value = 0; iter != end; ++iter)
   {
      if (iter == end)  return  begin != iter;
      else if (in(*iter, '0', '9'))  value = (value << 4) + *iter - '0';
      else if (in(*iter, 'a', 'f'))  value = (value << 4) + *iter - 87;
      else if (in(*iter, 'A', 'F'))  value = (value << 4) + *iter - 55;
      else return  begin != iter;
   }
   return  iter != begin;
}



template <class iterT, class typeT>
bool  ReadInteger (iterT &iter, const iterT &end, typeT &value, int basis = 10)
{
   if (*iter == '0')
   {
      ++iter;
      if (*iter == 'x' || *iter == 'X')
      {
         if (ReadIntegerHex (++iter, end, value))  return  true;
         --iter;
         return  false;
      }
      value = 0;
   }

   if (basis == 10)  return ReadIntegerDecimal (iter, end, value);
   if (basis == 2)   return ReadIntegerBinary  (iter, end, value);
   if (basis == 16)  return ReadIntegerHex     (iter, end, value);

   return  false;
}


inline
double Pow10 (int pow)
{
        if (pow ==  0)  return  1;
   else if (pow ==  1)  return  10;
   else if (pow >= 64)  return  1e64 * Pow10(pow-64);
   else if (pow >= 32)  return  1e32 * Pow10(pow-32);
   else if (pow >= 16)  return  1e16 * Pow10(pow-16);
   else if (pow >=  8)  return  1e8  * Pow10(pow-8);
   else if (pow >=  4)  return  1e4  * Pow10(pow-4);
   else if (pow >=  2)  return  100  * Pow10(pow-2);
   else                 return  1. / Pow10(-pow);
}



template <class Range>
bool  readDouble (Range& range, double &value)
{
   int   mantise  = 0,
         exponent = 0;
   bool  negative = false;

   // read the integer part
   bool  integer_found = readInteger (range, mantise);
   if (mantise < 0)
   {
      mantise  = -mantise;
      negative = true;
   }

   // read the decimals 
   Range::Iterator  pos = range.begin(); 
   if (!range.empty()  &&  range.front() == '.')
   {
      range.pop_front();

      while (!range.empty()  &&  in (range.front(),'0','9'))
      {
         mantise = mantise*10 + (range.front() - '0');
         --exponent;
         range.pop_front();
      }

      if (!integer_found  &&  exponent == 0)
      {
         range.begin (pos);
         return false;
      }
   }
   else if (!integer_found)  return false;


   // read the exponent part
   pos = range.begin();
   if (!range.empty()  &&  
      (range.front() == 'E' || range.front() == 'e'))
   {
      range.pop_front();
      int val;
      if (readInteger (range, val))
         exponent += val; 
      else
         range.begin (pos);
   }

   // calculate double value;
   value = double(mantise) * Pow10 (exponent);
   if (negative)  value = -value;
   return  true;
}


} // namespace pi
#endif 


