#include "wrapper.h"
#include <iostream>

namespace ext
{
   template wrapper<int>;

   void f()
   {
      ext::wrapper<int> a{ 42 };

      std::cout << a.data << '\n';
   }
}