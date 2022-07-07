#pragma once 

namespace ext
{
   template <typename T>
   struct wrapper
   {
      T data;
   }; 

   extern template wrapper<int>;

   void f();
   void g();
}