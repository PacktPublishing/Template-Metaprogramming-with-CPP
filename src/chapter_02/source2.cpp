#include "wrapper.h"
#include <iostream>

namespace ext
{
   void g()
   {
      wrapper<int> a{ 100 };

      std::cout << a.data << '\n';
   }
}