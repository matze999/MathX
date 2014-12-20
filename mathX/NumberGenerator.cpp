#include "NumberGenerator.h"


#include <iostream>

namespace mathX {



int  NumberGenerator::operator() (const Intval& intval) const
{
   return  intval.lb + device() % intval.width();
}



void  NumberGenerator::operator() (double& value) const
{
   value = operator()();
   value *= pow (10, operator()() % 100);
}


double Accuracy::generate (const NumberGenerator& gen)
{
   if (mantisse > 10)  mantisse = 10;

   int bound = (int) std::pow (10, mantisse);
   double value = gen ({ -bound, bound }, exclude_zero);

   return  value / std::pow (10, decimals);
}


} // namespace mathX


