#include <iostream>
#include <vector>
#include <mutex>
#include <string>
#include <map>

namespace n401
{
   template <typename T>
   struct parser;             // [1] template declaration

   void handle(double value)  // [2] handle(double) definition
   {
      std::cout << "processing a double: " << value << '\n';
   }

   template <typename T>
   struct parser              // [3] template definition
   {
      void parse()
      {
         handle(42);          // [4] non-dependent name
      }
   };

   void handle(int value)     // [5] handle(int) definition
   {
      std::cout << "processing an int: " << value << '\n';
   }
}

namespace n402
{
   template <typename T>
   struct handler          // [1] template definition
   {
      void handle(T value)
      {
         std::cout << "handler<T>: " << value << '\n';
      }
   };

   template <typename T>
   struct parser           // [2] template definition
   {
      void parse(T arg)
      {
         arg.handle(42);   // [3] dependent name
      }
   };
   
   template <>
   struct handler<int>     // [4] template specialization
   {
      void handle(int value)
      {
         std::cout << "handler<int>: " << value << '\n';
      }
   };
}

namespace n403
{
   template <typename T>
   struct base_parser
   {
      void init()
      {
         std::cout << "init\n";
      }
   };

   template <typename T>
   struct parser : base_parser<T>
   {
      void parse()
      {
         //init();        // error: identifier not found
         this->init();

         std::cout << "parse\n";
      }
   };
}

namespace n404
{
   template <typename T>
   struct base_parser
   {
      void init()
      {
         std::cout << "init\n";
      }
   };

   template <typename T>
   struct parser : base_parser<T>
   {
      void parse()
      {
         this->init();

         std::cout << "parse\n";
      }
   };

   template <>
   struct base_parser<int>
   {
      void init()
      {
         std::cout << "specialized init\n";
      }
   };
}

namespace n405
{
   template <typename T>
   struct base_parser
   {
      using value_type = T;
   };

   template <typename T>
   struct parser : base_parser<T>
   {
      void parse()
      {
         //value_type v{}; //syntax error: unexpected token 'identifier', expected ';'
         //base_parser<T>::value_type v{};
         typename base_parser<T>::value_type v{};

         std::cout << "parse\n";
      }
   };
}

namespace n406
{
   template <typename T>
   struct base_parser
   {
      template <typename U>
      void init()
      {
         std::cout << "init\n";
      }

      template <typename U>
      struct token {};
   };

   template <typename T>
   struct parser : base_parser<T>
   {
      void parse()
      {
         //base_parser<T>::init<int>(); // error

         base_parser<T>::template init<int>();

         using token_type = base_parser<T>::template token<int>;
         token_type t1{};

         typename base_parser<T>::template token<int> t2{};

         std::cout << "parse\n";
      }
   };
}

namespace n407
{
   template <typename T>
   struct parser
   {
      parser* p1;          // parser is the CI
      parser<T>* p2;       // parser<T> is the CI
      n407::parser<T>* p3;  // ::parser<T> is the CI
      //parser<T*> p4;     // parser<T*> is not the CI

      struct token
      {
         token* t1;              // token is the CI
         parser<T>::token* t2;   // parser<T>::token is the CI
         //typename parser<T*>::token* t3; // parser<T*>::token is not the CI
      };
   };

   template <typename T>
   struct parser<T*>
   {
      parser<T*>* p1;   // parser<T*> is the CI
      parser<T>*  p2;   // parser<T> is not the CI
   };
}

namespace n408
{
   template <unsigned int N>
   struct factorial
   {
      static constexpr unsigned int value = N * factorial<N - 1>::value;
   };

   template <>
   struct factorial<0>
   {
      static constexpr unsigned int value = 1;
   };
}

namespace n409
{
   template <unsigned int N>
   inline constexpr unsigned int factorial = N * factorial<N - 1>;

   template <>
   inline constexpr unsigned int factorial<0> = 1;
}

namespace n409b
{
   template <unsigned int n>
   constexpr unsigned int factorial()
   {
      return n * factorial<n - 1>();
   }

   template<> constexpr unsigned int factorial<1>() { return 1; }
   template<> constexpr unsigned int factorial<0>() { return 1; }
}

namespace n410
{
   constexpr unsigned int factorial(unsigned int const n)
   {
      return n > 1 ? n * factorial(n - 1) : 1;
   }

   template <typename T>
   struct wrapper {};

   template <int N>
   struct manyfold_wrapper
   {
      using value_type = wrapper<typename manyfold_wrapper<N - 1>::value_type>;
   };

   template <>
   struct manyfold_wrapper<0>
   {
      using value_type = unsigned int;
   };
}

namespace n411
{
   template<unsigned int N>
   inline constexpr unsigned int sum = N + sum<N - 1>;

   template<>
   inline constexpr unsigned int sum<0> = 0;
}

namespace n412
{
   template <typename T>
   void process(T arg)
   {
      std::cout << "processing " << arg << '\n';
   }

   struct account_t
   {
      int         number;

      int get_number() { return number; }
      int from_string(std::string text) { return std::atoi(text.c_str()); }
   };

   struct transaction_t
   {
      double amount;
   };

   struct balance_report_t {};

   struct balance_t
   {
      account_t account;
      double    amount;

      account_t get_account() { return account; }
      int get_account_number() { return account.number; }
      bool can_withdraw(double const value) { return amount >= value; };
      transaction_t withdraw(double const value) { amount -= value; return transaction_t{ -value }; }
      balance_report_t make_report(int const type) { return {}; }
   };

   template <typename T>
   void process01(T) 
   { std::cout << "T" << '\n'; }

   template <typename T>
   void process02(T const) 
   { std::cout << "T const" << '\n'; }

   template <typename T>
   void process03(T volatile) 
   { std::cout << "T volatile" << '\n'; }

   template <typename T>
   void process04(T*) 
   { std::cout << "T*" << '\n'; }

   template <typename T>
   void process04(T&) 
   { std::cout << "T&" << '\n'; }

   template <typename T>
   void process05(T&&) 
   { std::cout << "T&&" << '\n'; }

   template <typename T>
   void process06(T[5]) 
   { std::cout << "T[5]" << '\n'; }

   template <size_t n>
   void process07(account_t[5][n]) 
   { std::cout << "C[5][n]" << '\n'; }

   template<typename T>
   void process08(T(*)()) 
   { std::cout << "T (*)()" << '\n'; }

   template<typename T>
   void process08(account_t(*)(T)) 
   { std::cout << "C (*)(T)" << '\n'; }

   template<typename T, typename U>
   void process08(T(*)(U)) 
   { std::cout << "T (*)(U)" << '\n'; }

   template<typename T>
   void process09(T(account_t::*)())
   { std::cout << "T (C::*)()" << '\n'; }

   template<typename T, typename U>
   void process09(T(account_t::*)(U))
   { std::cout << "T (C::*)(U)" << '\n'; }

   template<typename T, typename U>
   void process09(T(U::*)())
   { std::cout << "T (U::*)()" << '\n'; }

   template<typename T, typename U, typename V>
   void process09(T(U::*)(V))
   { std::cout << "T (U::*)(V)" << '\n'; }

   template<typename T>
   void process09(account_t(T::*)())
   { std::cout << "C (T::*)()" << '\n'; }

   template<typename T, typename U>
   void process09(transaction_t(T::*)(U))
   { std::cout << "C (T::*)(U)" << '\n'; }

   template<typename T>
   void process09(balance_report_t(balance_t::*)(T))
   { std::cout << "D (C::*)(T)" << '\n'; }

   template<typename T>
   void process10(T account_t::*)
   { std::cout << "T C::*" << '\n'; }

   template<typename T>
   void process10(account_t T::*)
   { std::cout << "C T::*" << '\n'; }

   template<typename T, typename U>
   void process10(T U::*)
   { std::cout << "T U::*" << '\n'; }

   template <typename T>
   struct wrapper
   {
      T data;
   };

   template <size_t i>
   struct int_array
   {
      int data[i];
   };

   template<typename T>
   void process11(wrapper<T>)
   { std::cout << "C<T>" << '\n'; }

   template<size_t i>
   void process12(int_array<i>)
   { std::cout << "C<i>" << '\n'; }

   template<template<typename> class TT, typename T>
   void process13(TT<T>) { std::cout << "TT<T>" << '\n'; }

   template<template<size_t> typename TT, size_t i>
   void process14(TT<i>) { std::cout << "TT<i>" << '\n'; }

   template<template<typename> typename TT>
   void process15(TT<account_t>) { std::cout << "TT<C>" << '\n'; }
}

namespace n413
{
   template <typename T, T i>
   void process(double arr[i])
   {
      using index_type = T;
      std::cout << "processing " << i << " doubles" << '\n';
      std::cout << "index type is " << typeid(T).name() << '\n';
   }
}

namespace n414
{
   template <typename T>
   void process(T a = 0, T b = 42)
   {
      std::cout << a << "," << b << '\n';
   }
}

namespace n415
{
   template <typename T>
   void invoke(void(*pfun)(T, int))
   {
      pfun(T{}, 42);
   }

   template <typename T>
   void alpha(T, int) { std::cout << "alpha(T,int)" << '\n'; }

   void beta(int, int) { std::cout << "beta(int,int)" << '\n'; }
   void beta(short, int) { std::cout << "beta(short,int)" << '\n'; }

   void gamma(short, int, long long) { std::cout << "gamma(short,int,long long)" << '\n'; }
   void gamma(double, int) { std::cout << "gamma(double,int)" << '\n'; }
}

namespace n416
{
   template <size_t Size>
   void process1(int a[Size]) { std::cout << "process(int[Size])" << '\n'; }

   template <size_t Size>
   void process2(int a[5][Size]) { std::cout << "process(int[5][Size])" << '\n'; }

   template <size_t Size>
   void process3(int(&a)[Size]) { std::cout << "process(int[Size]&)" << '\n'; }

   template <size_t Size>
   void process4(int(*a)[Size]) { std::cout << "process(int[Size]*)" << '\n'; }
}

namespace n417
{
   template <size_t N>
   struct ncube 
   {
      static constexpr size_t dimensions = N;
   };

   template <size_t N>
   void process(ncube<N - 1> cube)
   {
      std::cout << cube.dimensions << '\n';
   }
}

namespace n418
{
   template <typename T>
   struct wrapper
   {
      T data;
   };

   template <typename T>
   constexpr wrapper<T> make_wrapper(T&& data)
   {
      return wrapper{ data };
   }

   template <typename T>
   wrapper(T) -> wrapper<T>;

   template <typename T, typename... Ts, typename Allocator = std::allocator<T>>
   auto make_vector(T&& first, Ts&&... args)
   {
      return std::vector<std::decay_t<T>, Allocator> {
         std::forward<T>(first),
         std::forward<Ts>(args)... 
      };
   }
}

namespace n419
{
   template <typename T>
   struct point_t
   {
      point_t(T vx, T vy) : x(vx), y(vy) {}

   private:
      T x;
      T y;
   };
}

namespace n420
{
   template <typename T>
   struct range_t
   {
      template <typename Iter>
      range_t(Iter first, Iter last)
      {
         std::copy(first, last, std::back_inserter(data));
      }
   private:
      std::vector<T> data;
   };

   template <typename Iter>
   range_t(Iter first, Iter last) -> range_t<typename std::iterator_traits<Iter>::value_type>;
}

namespace n421
{
   struct foo
   {
      int data;
   };

   void f(foo& v)
   {
      std::cout << "f(foo&)\n";
   }

   void g(foo& v)
   {
      std::cout << "g(foo&)\n";
   }

   void g(foo&& v)
   {
      std::cout << "g(foo&&)\n";
   }

   void h(foo&& v)
   {
      std::cout << "h(foo&&)\n";
   }
}

namespace n422
{
   struct foo
   {
      int data;
   };

   void g(foo& v)
   {
      std::cout << "g(foo&)\n";
   }

   void g(foo&& v)
   {
      std::cout << "g(foo&&)\n";
   }

   void h(foo& v)
   {
      g(v);
   }

   void h(foo&& v)
   {
      g(v);
   }
}

namespace n423
{
   struct foo
   {
      int data;
   };

   void g(foo& v)
   {
      std::cout << "g(foo&)\n";
   }

   void g(foo&& v)
   {
      std::cout << "g(foo&&)\n";
   }

   void h(foo& v)
   {
      g(std::forward<foo&>(v));
   }

   void h(foo&& v)
   {
      g(std::forward<foo&&>(v));
   }
}

namespace n424
{
   template <typename T>
   void f(T&& arg)               // forwarding reference
   { std::cout << "f(T&&)\n"; }

   template <typename T>
   void f(T const&& arg)         // rvalue reference
   { std::cout << "f(T const&&)\n"; }

   template <typename T>
   void f(std::vector<T>&& arg)  // rvalue reference
   { std::cout << "f(vector<T>&&)\n"; }

   template <typename T>
   struct S
   {
      void f(T&& arg)            // rvalue reference
      { std::cout << "S.f(T&&)\n"; }
   };
}

namespace n425
{
   struct foo
   {
      int data;
   };

   void g(foo& v)
   {
      std::cout << "g(foo&)\n";
   }

   void g(foo&& v)
   {
      std::cout << "g(foo&&)\n";
   }

   template <typename T>
   void h(T& v)
   {
      g(v);
   }

   template <typename T>
   void h(T&& v)
   {
      g(v);
   }
}

namespace n426
{
   struct foo
   {
      int data;
   };

   void g(foo& v)
   {
      std::cout << "g(foo&)\n";
   }

   void g(foo&& v)
   {
      std::cout << "g(foo&&)\n";
   }

   template <typename T>
   void h(T&& v)
   {
      g(std::forward<T>(v));
   }
}

namespace n428
{
   int f() { return 42; }

   int g() { return 0; }
   int g(int a) { return a; }
   int g(int a, int b) { return a + b; }

   struct wrapper
   {
      int val;

      int get() const { return val; }
   };
}

namespace n429
{
   struct foo
   {
      int          a = 0;
      volatile int b = 0;
      const int    c = 42;
   };
}

namespace n430
{
   template <typename T>
   T minimum(T&& a, T&& b)
   {
      return a < b ? a : b;
   }
}

namespace n431
{
   template <typename T, typename U>
   auto minimum(T&& a, U&& b) -> decltype(a < b ? a : b)
   {
      return a < b ? a : b;
   }
}

namespace n432
{
   template <typename T, typename U>
   decltype(auto) minimum(T&& a, U&& b)
   {
      return a < b ? a : b;
   }
}

namespace n433
{
   template <typename T, typename U>
   auto minimum(T&& a, U&& b)
   {
      return a < b ? a : b;
   }
}

namespace n434
{
   template <typename T>
   T const& func(T const& ref)
   {
      return ref;
   }

   template <typename T>
   auto func_caller(T&& ref)
   {
      return func(std::forward<T>(ref));
   }
}

namespace n435
{
   template <typename T>
   T const& func(T const& ref)
   {
      return ref;
   }

   template <typename T>
   decltype(auto) func_caller(T&& ref)
   {
      return func(std::forward<T>(ref));
   }
}

namespace n436
{
   template <typename T, typename U>
   struct composition
   {
      using result_type = decltype(T{} + U{});
   };

   struct wrapper
   {
      wrapper(int const v) : value(v){}

      int value;

      friend wrapper operator+(int const a, wrapper const& w)
      {
         return wrapper(a + w.value);
      }

      friend wrapper operator+(wrapper const& w, int const a)
      {
         return wrapper(a + w.value);
      }
   };
}

namespace n437
{
   using n436::wrapper;

   template <typename T, typename U>
   struct composition
   {
      using result_type = decltype(std::declval<T>() + std::declval<U>());
   };
}

namespace n438
{
   struct wrapper
   {   
      wrapper(int const v) :value(v)
      {}
   private:
      int value;

      friend void print(wrapper const & w);
   };

   void print(wrapper const& w)
   {
      std::cout << w.value << '\n';
   }
}

namespace n439
{
   struct wrapper
   {
      wrapper(int const v) :value(v)
      {
      }
   private:
      int value;

      template <typename T>
      friend void print(wrapper const&);

      template <typename T>
      friend struct printer;
   };   

   template <typename T>
   void print(wrapper const& w)
   {
      std::cout << w.value << '\n';
   }

   template <typename T>
   struct printer
   {
      void operator()(wrapper const& w)
      {
         std::cout << w.value << '\n';
      }
   };
}

namespace n440
{
   struct wrapper;

   template <typename T>
   void print(wrapper const& w);

   template <typename T>
   struct printer;

   struct wrapper
   {
      wrapper(int const v) :value(v)
      {
      }
   private:
      int value;

      friend void print<int>(wrapper const&);
      friend struct printer<int>;
   };

   template <typename T>
   void print(wrapper const& w)
   {
      // std::cout << w.value << '\n'; // error
   }

   template <>
   void print<int>(wrapper const& w)
   {
      std::cout << w.value << '\n';
   }

   template <typename T>
   struct printer
   {
      void operator()(wrapper const& w)
      {
         // std::cout << w.value << '\n'; // error
      }
   };

   template <>
   struct printer<int>
   {
      void operator()(wrapper const& w)
      {
         std::cout << w.value << '\n';
      }
   };
}

namespace n441
{
   template <typename T>
   struct wrapper
   {
      wrapper(T const v) :value(v)
      {
      }
   private:
      T value;

      friend void print(wrapper<int> const&);
   };

   void print(wrapper<int> const& w)
   {
      std::cout << w.value << '\n';
   }

   void print(wrapper<char> const& w)
   {
      // std::cout << w.value << '\n'; // error
   }
}

namespace n442
{
   template <typename T>
   struct printer;

   template <typename T>
   struct wrapper
   {
      wrapper(T const v) :value(v)
      {
      }
   private:
      T value;

      friend void print<int>(wrapper<int> const&);
      friend struct printer<int>;
   };

   template <typename T>
   void print(wrapper<T> const& w)
   {
      // std::cout << w.value << '\n'; // error
   }

   template<>
   void print(wrapper<int> const& w)
   {
      std::cout << w.value << '\n';
   }

   template <typename T>
   struct printer
   {
      void operator()(wrapper<T> const& w)
      {
         //std::cout << w.value << '\n'; // error
      }
   };

   template <>
   struct printer<int>
   {
      void operator()(wrapper<int> const& w)
      {
         std::cout << w.value << '\n';
      }
   };
}

namespace n443
{
   template <typename T>
   struct printer;

   template <typename T>
   struct wrapper
   {
      wrapper(T const v) :value(v) {}
   private:
      T value;

      template <typename U>
      friend void print(wrapper<U> const&);

      template <typename U>
      friend struct printer;
   };

   template <typename T>
   void print(wrapper<T> const& w)
   {
      std::cout << w.value << '\n';
   }

   template <typename T>
   struct printer
   {
      void operator()(wrapper<T> const& w)
      {
         std::cout << w.value << '\n';
      }
   };
}

namespace n444
{
   template <typename T>
   struct printer;

   template <typename T>
   struct wrapper
   {
      wrapper(T const v) :value(v) {}
   private:
      T value;

      friend void print<T>(wrapper<T> const&);
      friend struct printer<T>;
   };

   template <typename T>
   void print(wrapper<T> const& w)
   {
      std::cout << w.value << '\n';
   }

   template <typename T>
   struct printer
   {
      void operator()(wrapper<T> const& w)
      {
         std::cout << w.value << '\n';
      }
   };
}

namespace n445
{
   template <typename T>
   struct connection
   {
      connection(std::string const& host, int const port) 
         :ConnectionString(host + ":" + std::to_string(port)) 
      {}
   private:
      std::string ConnectionString;
      friend T;
   };

   struct executor
   {
      void run()
      {
         connection<executor> c("localhost", 1234);
         
         std::cout << c.ConnectionString << '\n';
      }
   };
}

namespace n446
{
   struct dictionary_traits
   {
      using key_type = int;
      using map_type = std::map<key_type, std::string>;
      static constexpr int identity = 1;
   };

   template <typename T>
   struct dictionary : T::map_type
   {
      int start_key{ T::identity };
      T::key_type next_key;

      using value_type = T::map_type::mapped_type;

      void add(T::key_type const&, value_type const&) {}
   };
}

namespace std
{
   template <typename T>
   pair(T&&, char const*) -> pair<T, std::string>;

   template <typename T>
   pair(char const*, T&&) -> pair<std::string, T>;

   pair(char const*, char const*) -> pair<std::string, std::string>;
}

int main()
{
   {
      using namespace n401;

      parser<int> p;       // [6] template instantiation
      p.parse();
   }

   {
      using namespace n402;

      handler<int> h;         // [5] template instantiation
      parser<handler<int>> p; // [6] template instantiation
      p.parse(h);
   }

   {
      using namespace n403;

      parser<int> p;
      p.parse();
   }

   {
      using namespace n404;

      parser<int> p1;
      p1.parse();

      parser<double> p2;
      p2.parse();
   }

   {
      using namespace n405;

      parser<int> p;
      p.parse();
   }

   {
      using namespace n406;

      parser<int> p;
      p.parse();
   }

   {
      using namespace n407;

      [[maybe_unused]]
      parser<int> p;
   }

   {
      using namespace n408;

      std::cout << factorial<0>::value << '\n';
      std::cout << factorial<1>::value << '\n';
      std::cout << factorial<2>::value << '\n';
      std::cout << factorial<3>::value << '\n';
      std::cout << factorial<4>::value << '\n';
      std::cout << factorial<5>::value << '\n';
      std::cout << factorial<12>::value << '\n';
   }

   {
      using namespace n409;

      std::cout << factorial<0> << '\n';
      std::cout << factorial<1> << '\n';
      std::cout << factorial<2> << '\n';
      std::cout << factorial<3> << '\n';
      std::cout << factorial<4> << '\n';
      std::cout << factorial<5> << '\n';
      std::cout << factorial<12> << '\n';
   }

   {
      using namespace n409b;

      std::cout << factorial<1>() << '\n';
      std::cout << factorial<2>() << '\n';
      std::cout << factorial<3>() << '\n';
      std::cout << factorial<4>() << '\n';
      std::cout << factorial<5>() << '\n';
   }

   {
      using namespace n410;

      std::cout << factorial(0) << '\n';
      std::cout << factorial(1) << '\n';
      std::cout << factorial(2) << '\n';
      std::cout << factorial(3) << '\n';
      std::cout << factorial(4) << '\n';
      std::cout << factorial(5) << '\n';

      std::cout << typeid(manyfold_wrapper<0>::value_type).name() << '\n';
      std::cout << typeid(manyfold_wrapper<1>::value_type).name() << '\n';
      std::cout << typeid(manyfold_wrapper<2>::value_type).name() << '\n';
      std::cout << typeid(manyfold_wrapper<3>::value_type).name() << '\n';
   }

   {
      using namespace n411;

      std::cout << sum<256> << '\n';
   }

   {
      using namespace n412;

      process(42);          // T is int
      process<int>(42);     // T is int, redundant
      process<short>(42);   // T is short
   }

   {
      using namespace n412;

      account_t ac{ 42 };
      process01(ac);   // T
      process02(ac);   // T const
      process03(ac);   // T volatile

      process04(&ac);  // T*
      process04(ac);   // T&
      process05(ac);   // T&&

      account_t arr1[5]{};
      process06(arr1); // T[5]

      process06(&ac);  // T[5]

      account_t arr2[5][3];
      process07(arr2); // C[5][n]

      account_t(*pf1)() = nullptr;
      account_t(*pf2)(int) = nullptr;
      double (*pf3)(int) = nullptr;
      process08(pf1);    // T (*)()
      process08(pf2);    // C (*)(T)
      process08(pf3);    // T (*)(U)

      int (account_t:: * pfm1)() = &account_t::get_number;
      int (account_t:: * pfm2)(std::string) = &account_t::from_string;
      int (balance_t:: * pfm3)() = &balance_t::get_account_number;
      bool (balance_t:: * pfm4)(double) = &balance_t::can_withdraw;
      account_t(balance_t:: * pfm5)() = &balance_t::get_account;
      transaction_t(balance_t:: * pfm6)(double) = &balance_t::withdraw;
      balance_report_t(balance_t:: * pfm7)(int) = &balance_t::make_report;


      process09(pfm1);    // T (C::*)()
      process09(pfm2);    // T (C::*)(U)
      process09(pfm3);    // T (U::*)()
      process09(pfm4);    // T (U::*)(V)
      process09(pfm5);    // C (T::*)()
      process09(pfm6);    // C (T::*)(U)
      process09(pfm7);    // D (C::*)(T)

      // process10(&account_t::number);   // T C::*
      process10(&balance_t::account);  // C T::*
      process10(&balance_t::amount);   // T U::*

      wrapper<double> wd{ 42.0 };
      process11(wd); // C<T>

      int_array<5> ia{};
      process12(ia); // C<i>

      process13(wd);    // TT<U>
      process14(ia);    // TT<i>

      wrapper<account_t> wa{ {42} };
      process15(wa);    // TT<C>
   }

   {
      using namespace n413;

      double arr[5]{};
      //process(arr);           // error
      process<int, 5>(arr);   // OK
   }

   {
      using namespace n414;
      //process();        // [1] error
      process<int>();   // [2] OK
      process(10);      // [3] OK
   }

   {
      using namespace n415;

      //invoke(&alpha);
      //invoke(&beta);
      invoke(&gamma);
   }

   {
      using namespace n416;

      int arr1[10];
      int arr2[5][10];

      //process1(arr1);   // error
      process2(arr2);   // OK
      process3(arr1);   // OK
      process4(&arr1);  // OK
   }

   {
      using namespace n417;

      ncube<5> cube;
      // process(cube); // error
      process<6>(cube); // OK
   }

   {
      using namespace n418;

      std::pair<int, double> p{ 42, 42.0 };
      std::vector<int> v{ 1,2,3,4,5 };
      wrapper<int> w{ 42 };
   }

   {
      using namespace n418;

      auto p = std::make_pair(42, 42.0);
      auto v = make_vector(1, 2, 3, 4, 5);
      auto w = make_wrapper(42);
   }

   {
      using namespace n418;

      std::pair   p{ 42, 42.0 };
      std::vector v{ 1,2,3,4,5 };
      wrapper     w{ 42 };
   }

   {
      using namespace n419;

      auto p = new point_t(1, 2);

      std::mutex mt;
      auto l = std::lock_guard(mt);

      // stream_t<42> s; // C++20
   }

   {
      // without custom deduction guides
      std::pair  p1{ 1, "one" };       // std::pair<int, const char*>
      std::pair  p2{ "two", 2 };       // std::pair<const char*, int>
      std::pair  p3{ "3", "three" };   // std::pair<const char*, const char*>
   }

   {
      // with custom deduction guides
      std::pair  p1{ 1, "one" };       // std::pair<int, std::string>
      std::pair  p2{ "two", 2 };       // std::pair<std::string, int>
      std::pair  p3{ "3", "three" };   // std::pair<std::string, std::string>
   }

   {
      using namespace n420;

      int arr[] = { 1,2,3,4,5 };
      range_t r(std::begin(arr), std::end(arr));
   }

   {
      std::pair<int, std::string> p1{ 1, "one" };  // [1] OK
      std::pair p2{ 2, "two" };                    // [2] OK
      //std::pair<> p3{ 3, "three" };                // [3] error
      //std::pair<int> p4{ 4, "four" };              // [4] error
   }

   {
      std::vector v1{ 42 };      // vector<int>
      std::vector v2{ v1, v1 };  // vector<vector<int>>
      std::vector v3{ v1 };      // vector<int>

      std::cout << typeid(decltype(v1)).name() << '\n';
      std::cout << typeid(decltype(v2)).name() << '\n';
      std::cout << typeid(decltype(v3)).name() << '\n';
   }

   {
      using namespace n421;

      foo x = { 42 };   // x is l-value
      foo& rx = x;      // rx is l-value

      f(x);             // f(foo&)
      f(rx);            // f(foo&)
      //f(foo{ 42 });     // error, a non-const reference may only be bound to an lvalue
   }

   {
      using namespace n421;

      foo x = { 42 };   // x is l-value
      foo& rx = x;      // rx is l-value

      g(x);             // g(foo&)
      g(rx);            // g(foo&)
      g(foo{ 42 });     // g(foo&&)
   }

   {
      using namespace n421;

      foo x = { 42 };   // x is l-value
      foo& rx = x;      // rx is l-value

      //h(x);             // error, cannot bind an lvalue to an rvalue reference
      //h(rx);         // error
      h(foo{ 42 });  // h(foo&&)
      h(std::move(x));  // h(foo&7)
   }

   {
      using namespace n422;

      foo x{ 42 };

      h(x);          // g(foo&)
      h(foo{ 42 });  // g(foo&)
   }

   {
      using namespace n423;

      foo x{ 42 };

      h(x);          // g(foo&)
      h(foo{ 42 });  // g(foo&&)
   }

   {
      using namespace n424;

      int x = 42;
      f(x);                   // f(T&&)
      f(42);                  // f(T&&)

      int const cx = 100;
      f(cx);                  // f(T&&)
      f(std::move(cx));       // f(T const&&)

      std::vector<int> v{ 42 };
      f(v);                   // f(T&&)
      f(std::vector<int>{42});// f(vector<T>&&)

      S<int> s;
      //s.f(x);                 // error
      s.f(42);                // S.f(T&&)
   }

   {
      using lrefint = int&;
      using rrefint = int&&;
      int x = 42;
      lrefint& r1 = x; // type of r1 is int&
      lrefint&& r2 = x; // type of r2 is int&
      rrefint& r3 = x; // type of r3 is int&
      rrefint&& r4 = 1; // type of r4 is int&&
   }

   {
      int x = 42;
      auto&& rx = x;          // int&
      auto&& rc = 42;         // int&&
      //auto const&& rcx = x;   // error

      std::vector<int> v{ 42 };
      auto&& rv = v[0];       // int&
   }

   {
      using namespace n425;

      foo x{ 42 };

      h(x);          // g(foo&)
      h(foo{ 42 });  // g(foo&)
   }

   {
      using namespace n426;

      foo x{ 42 };

      h(x);          // g(foo&)
      h(foo{ 42 });  // g(foo&&)
   }

   {
      using namespace n428;

      int a = 42;
      int& ra = a;
      const double d = 42.99;
      long arr[10];
      long l = 0;
      char* p = nullptr;
      char c = 'x';
      wrapper w1{ 1 };
      wrapper* w2 = new wrapper{ 2 };

      [[maybe_unused]] decltype(a) e1;             // R1, int
      [[maybe_unused]] decltype(ra) e2 = a;        // R1, int&
      [[maybe_unused]] decltype(f) e3;             // R1, int()
      [[maybe_unused]] decltype(f()) e4;           // R2, int
      //[[maybe_unused]] decltype(g) e5;           // R1, error
      [[maybe_unused]] decltype(g(1)) e6;          // R2, int
      [[maybe_unused]] decltype(&f) e7 = nullptr;  // R4, int(*)()
      [[maybe_unused]] decltype(d) e8 = 1;         // R1, const double
      [[maybe_unused]] decltype(arr) e9;           // R1, long[10]
      [[maybe_unused]] decltype(arr[1]) e10 = l;   // R3, long&
      [[maybe_unused]] decltype(w1.val) e11;       // R1, int
      [[maybe_unused]] decltype(w1.get()) e12;     // R1, int
      [[maybe_unused]] decltype(w2->val) e13;      // R1, int
      [[maybe_unused]] decltype(w2->get()) e14;    // R1, int
      [[maybe_unused]] decltype(42) e15 = 1;       // R4, int
      [[maybe_unused]] decltype(1 + 2) e16;        // R4, int
      [[maybe_unused]] decltype(a + 1) e17;        // R4, int
      [[maybe_unused]] decltype(a = 0) e18 = a;    // R3, int&
      [[maybe_unused]] decltype(p) e19 = nullptr;  // R1, char*
      [[maybe_unused]] decltype(*p) e20 = c;       // R3, char&
      [[maybe_unused]] decltype(p[0]) e21 = c;     // R3, char&

      delete w2;
   }

   {
      int a = 42;
      decltype(a = 1) e = a;
      std::cout << a << '\n';
   }

   {
      using namespace n412;
      [[maybe_unused]] decltype(wrapper<double>::data) e1;  // double

      int a = 42;
      [[maybe_unused]] decltype(wrapper<char>::data, a) e2; // int&
   }

   {
      using namespace n429;

      foo f;
      foo const cf;
      volatile foo* pf = &f;

      [[maybe_unused]] decltype(f.a) e1 = 0;       // int
      [[maybe_unused]] decltype(f.b) e2 = 0;       // int volatile
      [[maybe_unused]] decltype(f.c) e3 = 0;       // int const

      [[maybe_unused]] decltype(cf.a) e4 = 0;      // int
      [[maybe_unused]] decltype(cf.b) e5 = 0;      // int volatile
      [[maybe_unused]] decltype(cf.c) e6 = 0;      // int const

      [[maybe_unused]] decltype(pf->a) e7 = 0;     // int
      [[maybe_unused]] decltype(pf->b) e8 = 0;     // int volatile
      [[maybe_unused]] decltype(pf->c) e9 = 0;     // int const

      [[maybe_unused]] decltype(foo{}.a) e10 = 0;  // int
      [[maybe_unused]] decltype(foo{}.b) e11 = 0;  // int volatile
      [[maybe_unused]] decltype(foo{}.c) e12 = 0;  // int const
   }

   {
      using namespace n429;

      foo f;
      foo const cf;
      volatile foo* pf = &f;

      int x = 1;
      int volatile y = 2;
      int const z = 3;

      [[maybe_unused]] decltype((f.a)) e1 = x;       // int&
      [[maybe_unused]] decltype((f.b)) e2 = y;       // int volatile&
      [[maybe_unused]] decltype((f.c)) e3 = z;       // int const&

      [[maybe_unused]] decltype((cf.a)) e4 = x;      // int const&
      [[maybe_unused]] decltype((cf.b)) e5 = y;      // int const volatile&
      [[maybe_unused]] decltype((cf.c)) e6 = z;      // int const&

      [[maybe_unused]] decltype((pf->a)) e7 = x;     // int volatile&
      [[maybe_unused]] decltype((pf->b)) e8 = y;     // int volatile&
      [[maybe_unused]] decltype((pf->c)) e9 = z;     // int const volatile&

      [[maybe_unused]] decltype((foo{}.a)) e10 = 0;  // int&&
      [[maybe_unused]] decltype((foo{}.b)) e11 = 0;  // int volatile&&
      [[maybe_unused]] decltype((foo{}.c)) e12 = 0;  // int const&&
   }

   {
      int a = 0;
      int& ra = a;
      int const c = 42;
      int volatile d = 99;

      [[maybe_unused]] decltype(ra)& e1 = a;          // int&
      [[maybe_unused]] decltype(c) const e2 = 1;      // int const
      [[maybe_unused]] decltype(d) volatile e3 = 1;   // int volatile
   }

   {
      using namespace n430;
      auto m1 = minimum(1, 5);         // OK
      auto m2 = minimum(18.49, 9.99);  // OK
      //auto m3 = minimum(1, 9.99);      // error, arguments of different type
   }

   {
      using namespace n431;
      auto m1 = minimum(1, 5);         // OK
      auto m2 = minimum(18.49, 9.99);  // OK
      auto m3 = minimum(1, 9.99);      // OK
   }

   {
      using namespace n432;
      auto m1 = minimum(1, 5);         // OK
      auto m2 = minimum(18.49, 9.99);  // OK
      auto m3 = minimum(1, 9.99);      // OK
   }

   {
      using namespace n433;
      auto m1 = minimum(1, 5);         // OK
      auto m2 = minimum(18.49, 9.99);  // OK
      auto m3 = minimum(1, 9.99);      // OK
   }

   {
      using namespace n434;

      int a = 42;

      decltype(func(a)) r1 = func(a);                 // int const&
      decltype(func_caller(a)) r2 = func_caller(a);   // int
   }

   {
      using namespace n435;

      int a = 42;

      decltype(func(a)) r1 = func(a);                 // int const&
      decltype(func_caller(a)) r2 = func_caller(a);   // int const&
   }

   {
      using namespace n436;

      static_assert(
         std::is_same_v<
         double,
         composition<int, double>::result_type>);

      // error, no appropriate default constructor available
      /*
      static_assert(
         std::is_same_v<
         wrapper,
         composition<int, wrapper>::result_type>);
         */
   }

   {
      using namespace n437;

      static_assert(
         std::is_same_v<
         double,
         composition<int, double>::result_type>);

      static_assert(
         std::is_same_v<
         wrapper,
         composition<int, wrapper>::result_type>);
   }

   {
      using namespace n438;

      wrapper w{ 42 };
      print(w);
   }

   {
      using namespace n439;

      wrapper w{ 42 };
      print<int>(w);
      print<char>(w);
      printer<int>()(w);
      printer<double>()(w);
   }

   {
      using namespace n440;

      wrapper w{ 43 };
      print<int>(w);
      print<char>(w);
      printer<int>()(w);
      printer<double>()(w);
   }

   {
      using namespace n442;

      wrapper w1{ 43 };
      print(w1);
      printer<int>()(w1);

      wrapper w2{ 'a' };
      print(w2);
      printer<char>()(w2);
   }

   {
      using namespace n443;

      wrapper w1{ 43 };
      print(w1);
      printer<int>()(w1);

      wrapper w2{ 'a' };
      print(w2);
      printer<char>()(w2);
   }

   {
      using namespace n445;

      executor e;
      e.run();
   }

   {
      using namespace n446;
      dictionary<dictionary_traits> d;
      d.add(1, "2");
   }
}
