#include "NumberGenerator.h"


#include <iostream>

namespace mathX {



int  NumberGenerator::operator() (const Range& range) const
{
   return  range.lb + device() % range.width();
}



void  NumberGenerator::operator() (double& value) const
{
   value = operator()();
   value *= pow (10, operator()() % 100);
}


double Accuracy::generate (const NumberGenerator& gen)
{
   assert0 (mantisse < 11);
   int bound = (int) std::pow (10, mantisse);
   double value = gen ({ -bound, bound });

   return  value / std::pow (10, decimals);
}


} // namespace mathX


