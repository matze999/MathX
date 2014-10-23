#ifndef IEEE754_H
#define IEEE754_H


namespace IEEE {


struct Representation
{
   static const unsigned BIAS = 1023;

   unsigned long long mantisse: 52;
   unsigned long long exponent: 11;
   unsigned long long sign: 1;
};


class Double
{
public:
   Double () = default;
   Double (double value): value (value)  {}


   operator double&() 
   {
      return value;
   }

   operator double()  const
   {
      return value;
   }

   operator Representation&()
   {
      return  representation;
   }

   operator Representation() const
   {
      return  representation;
   }

private:
   union {
      double  value;
      Representation  representation;
   };
};


Representation&  numeric_cast (double& value)
{
   return  reinterpret_cast <Representation&> (value);
}

} // namespace IEEE
#endif
