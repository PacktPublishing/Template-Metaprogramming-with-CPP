#include <iostream>
#include <array>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <type_traits>
#include <array>
#include <numeric>
#include <algorithm>
#include <functional>

#include "wrapper.h"




namespace n201
{
   template <typename T>
   T add(T const a, T const b)
   {
      return a + b;
   }

   class foo
   {
      int value;
   public:
      explicit foo(int const i) :value(i)
      {
      }

      explicit operator int() const { return value; }
   };

   foo operator+(foo const a, foo const b)
   {
      return foo((int)a + (int)b);
   }

   template <typename Input, typename Predicate>
   int count_if(Input start, Input end, Predicate p)
   {
      int total = 0;
      for (Input i = start; i != end; i++)
      {
         if (p(*i))
            total++;
      }
      return total;
   }
}

namespace n202
{
   template <typename T>
   class wrapper
   {
   public:
      wrapper(T const v):value(v)
      { }

      T const& get() const { return value; }
   private:
      T value;
   };
}

namespace n203
{
   template <typename T>
   class wrapper;

   void use_wrapper(wrapper<int>* ptr);
}

namespace n203
{
   template <typename T>
   class wrapper
   {
   public:
      wrapper(T const v) :value(v)
      {
      }

      T const& get() const { return value; }
   private:
      T value;
   };

   void use_wrapper(wrapper<int>* ptr)
   {
      std::cout << ptr->get() << '\n';
   }
}

namespace n204
{
   template <typename T>
   class composition
   {
   public:
      T add(T const a, T const b)
      {
         return a + b;
      }
   };
}

namespace n205
{
   class composition
   {
   public:
      template <typename T>
      T add(T const a, T const b)
      {
         return a + b;
      }
   };
}

namespace n206
{
   template <typename T>
   class wrapper
   {
   public:
      wrapper(T const v) :value(v)
      {
      }

      T const& get() const { return value; }

      template <typename U>
      U as() const
      {
         return static_cast<U>(value);
      }
   private:
      T value;
   };
}

namespace n207
{
   template <typename T>
   class wrapper1 { /* ... */ };

   template <typename T = int>
   class wrapper2 { /* ... */ };

   template <typename>
   class wrapper3;

   template <typename = int>
   class wrapper4;

   template <typename... T>
   class wrapper5 { /* ... */ };

   template <typename T>
   concept WrappableType = std::is_trivial_v<T>;

   template <WrappableType T>
   class wrapper6 { /* ... */ };

   template <WrappableType T = int>
   class wrapper7 { /* ... */ };

   template <WrappableType... T>
   class wrapper8 { /* ... */ };
}

namespace n208
{
   template <int V>
   class foo1 { /*...*/ };

   template <int V = 42>
   class foo2 { /*...*/ };

   template <int... V>
   class foo3 { /*...*/ };
}

namespace n209
{
   template <typename T, size_t S>
   class buffer
   {
      T data_[S];
   public:
      constexpr T const * data() const { return data_; }

      constexpr T& operator[](size_t const index)
      {
         return data_[index];
      }

      constexpr T const & operator[](size_t const index) const
      {
         return data_[index];
      }
   };

   template <typename T, size_t S>
   buffer<T, S> make_buffer()
   {
      return {};
   }
}

namespace n210
{
   struct device
   {
      virtual void output() = 0;
      virtual ~device() {}
   };

   template <typename Command, void (Command::*action)()>
   struct smart_device : device
   {
      smart_device(Command* command) : cmd(command) {}

      void output() override
      {
         (cmd->*action)();
      }
   private:
      Command* cmd;
   };

   struct hello_command
   {
      void say_hello_in_english()
      {
         std::cout << "Hello, world!\n";
      }

      void say_hello_in_spanish()
      {
         std::cout << "Hola mundo!\n";
      }
   };
}

namespace n211
{
   struct device
   {
      virtual void output() = 0;
      virtual ~device() {}
   };

   template <typename Command, void (Command::*action)()>
   struct smart_device : device
   {
      smart_device(Command& command) : cmd(command) {}

      void output() override
      {
         (cmd.*action)();
      }
   private:
      Command& cmd;
   };

   struct hello_command
   {
      void say_hello_in_english()
      {
         std::cout << "Hello, world!\n";
      }

      void say_hello_in_spanish()
      {
         std::cout << "Hola mundo!\n";
      }
   };
}

namespace n212
{
   struct device
   {
      virtual void output() = 0;
      virtual ~device() {}
   };

   template <void (*action)()>
   struct smart_device : device
   {
      void output() override
      {
         (*action)();
      }
   };

   void say_hello_in_english()
   {
      std::cout << "Hello, world!\n";
   }

   void say_hello_in_spanish()
   {
      std::cout << "Hola mundo!\n";
   }
}

namespace n213
{
   template <auto x>
   struct foo
   {
   };
}

namespace n214
{
   template<size_t N>
   struct string_literal
   {
      constexpr string_literal(const char(&str)[N])
      {
         std::copy_n(str, N, value);
      }

      char value[N];
   };

   template <string_literal x>
   struct foo
   {
   };
}

namespace n215
{
   template<auto... x>
   struct foo
   { /* ... */ };
}

namespace n216
{
   template <typename T>
   class simple_wrapper
   {
   public:
      T value;
   };

   template <typename T>
   class fancy_wrapper
   {
   public:
      fancy_wrapper(T const v) :value(v)
      {
      }

      T const& get() const { return value; }

      template <typename U>
      U as() const
      {
         return static_cast<U>(value);
      }
   private:
      T value;
   };

   template <typename T, typename U, template<typename> typename W = fancy_wrapper>
   class wrapping_pair
   {
   public:
      wrapping_pair(T const a, U const b) :
         item1(a), item2(b)
      {
      }

      W<T> item1;
      W<U> item2;
   };
}

namespace n217
{
   template <typename T = int>
   class foo { /*...*/ };

   template <typename T = int, typename U = double>
   class bar { /*...*/ };
}

namespace n218
{
   // template <typename T = int, typename U>
   // class bar { };

   template <typename T = int, typename U>
   void func() {}
}

namespace n219
{
   template <typename T, typename U = double>
   struct foo;

   template <typename T = int, typename U>
   struct foo;

   template <typename T, typename U>
   struct foo
   {
      T a;
      U b;
   };
}

namespace n220
{
   template <typename T = int>
   struct foo;

   //template <typename T = int> // error
   //struct foo {};
}

namespace n221
{
   template <typename T>
   struct foo
   {
   protected:
      using value_type = T;
   };

   template <typename T, typename U = typename T::value_type>
   struct bar
   {
      using value_type = U;
   };
}

namespace n222
{
   template <typename T>
   struct foo
   {
      void f() {}
      void g() { int a = "42"; }
   };
}

namespace n223
{
   template <typename T>
   struct foo
   {
      void f() {}
      void g() {}
   };
}

namespace n224
{
   template <typename T>
   struct control
   {
   };

   template <typename T>
   struct button : public control<T>
   {
   };

   void show(button<int>* ptr)
   {
      control<int>* c = ptr;
   }
}

namespace n225
{
   template <typename T>
   struct foo
   {
      static T data;
   };

   template <typename T> T foo<T>::data = 0;
}

namespace n226
{
   template <typename T>
   struct wrapper
   {
      T value;
   };

   template struct wrapper<int>;        // [1]
}

template struct n226::wrapper<double>;  // [2]

namespace n226
{
   template <typename T>
   T add(T const a, T const b)
   {
      return a + b;
   }

   template int add(int, int);         // [1]
}

template int n226::add(int, int);       // [2]

namespace n227
{
   template <typename T>
   class foo
   {
      struct bar {};

      T f(bar const arg)
      {
         return {};
      }
   };

   template int foo<int>::f(foo<int>::bar);
}

namespace n228
{
   template <typename T>
   struct is_floating_point
   {
      constexpr static bool value = false;
   };

   template <>
   struct is_floating_point<float>
   {
      constexpr static bool value = true;
   };

   template <>
   struct is_floating_point<double>
   {
      constexpr static bool value = true;
   };

   template <>
   struct is_floating_point<long double>
   {
      constexpr static bool value = true;
   };

   template <typename T>
   inline constexpr bool is_floating_point_v = is_floating_point<T>::value;
}

namespace n229
{
   template <typename T>
   struct is_floating_point;

   template <>
   struct is_floating_point<float>
   {
      constexpr static bool value = true;
   };

   template <typename T>
   struct is_floating_point
   {
      constexpr static bool value = false;
   };
}

namespace n230
{
   template <typename>
   struct foo {};    // primary template

   template <>
   struct foo<int>;  // explicit specialization declaration
}

namespace n231
{
   template <typename T>
   struct foo {};

   template <typename T>
   void func(foo<T>)
   {
      std::cout << "primary template\n";
   }

   template<>
   void func(foo<int>)
   {
      std::cout << "int specialization\n";
   }
}

namespace n232
{
   template <typename T>
   void func(T a)
   {
      std::cout << "primary template\n";
   }

   template <>
   void func(int a /*= 0*/) // error: default argument
   {
      std::cout << "int specialization\n";
   }
}

namespace n233
{
   template <typename T>
   struct foo
   {
      static T value;
   };

   template <typename T> T foo<T>::value = 0;

   template <> int foo<int>::value = 42;
}

namespace n234
{
   template <typename T, typename U>
   void func(T a, U b)
   {
      std::cout << "primary template\n";
   }

   template <>
   void func(int a, int b)
   {
      std::cout << "int-int specialization\n";
   }

   template <>
   void func(int a, double b)
   {
      std::cout << "int-double specialization\n";
   }
}

namespace n235
{
   template <typename T, int S>
   struct collection
   {
      void operator()()
      { 
         std::cout << "primary template\n";
      }
   };

   template <typename T>
   struct collection<T, 10>
   {
      void operator()()
      {
         std::cout << "partial specialization <T, 10>\n";
      }
   };

   template <int S>
   struct collection<int, S>
   { 
      void operator()()
      {
         std::cout << "partial specialization <int, S>\n";
      }
   };

   template <typename T, int S>
   struct collection<T*, S>
   { 
      void operator()()
      {
         std::cout << "partial specialization <T*, S>\n";
      }
   };
}

namespace n236
{
   template <int A, int B>
   struct foo {};

   template <int A>
   struct foo<A, A> {};

   //template <int A>
   //struct foo<A, A + 1> {}; // error
}

namespace n237
{
   template <typename T, size_t S>
   std::ostream& pretty_print(std::ostream& os, std::array<T, S> const& arr)
   {
      os << '[';
      if (S > 0)
      {
         size_t i = 0;
         for (; i < S - 1; ++i)
            os << arr[i] << ',';
         os << arr[S - 1];
      }
      os << ']';

      return os;
   }
}

namespace n238
{
   template <size_t S>
   std::ostream& pretty_print(std::ostream& os, std::array<char, S> const& arr)
   {
      os << '[';
      for (auto const& e : arr)
         os << e;
      os << ']';

      return os;
   }
}

namespace n239
{
   constexpr double PI = 3.1415926535897932385L;

   template <typename T>
   T sphere_volume(T const r)
   {
      return static_cast<T>(4 * PI * r * r * r / 3);
   }
}

namespace n240
{
   template <typename T>
   struct PI
   {
      static const T value;
   };

   template <typename T> 
   const T PI<T>::value = T(3.1415926535897932385L);

   template <typename T>
   T sphere_volume(T const r)
   {
      return 4 * PI<T>::value * r * r * r / 3;
   }
}

namespace n241
{
   template<class T>
   constexpr T PI = T(3.1415926535897932385L);

   template <typename T>
   T sphere_volume(T const r)
   {
      return 4 * PI<T> * r * r * r / 3;
   }
}

namespace n242
{
   struct math_constants
   {
      template<class T>
      static constexpr T PI = T(3.1415926535897932385L);
   };

   template <typename T>
   T sphere_volume(T const r)
   {
      return 4 * math_constants::PI<T> *r * r * r / 3;
   }
}

namespace n243
{
   struct math_constants
   {
      template<class T>
      static const T PI;
   };

   template<class T>
   const T math_constants::PI = T(3.1415926535897932385L);

   template <typename T>
   T sphere_volume(T const r)
   {
      return 4 * math_constants::PI<T> *r * r * r / 3;
   }
}

namespace n244
{
   template<typename T> 
   constexpr T SEPARATOR = '\n';

   template<> 
   constexpr wchar_t SEPARATOR<wchar_t> = L'\n';

   template <typename T>
   std::basic_ostream<T>& show_parts(std::basic_ostream<T>& s, 
                                     std::basic_string_view<T> const& str)
   {
      using size_type = typename std::basic_string_view<T>::size_type;
      size_type start = 0;
      size_type end;
      do
      {
         end = str.find(SEPARATOR<T>, start);
         s << '[' << str.substr(start, end - start) << ']' << SEPARATOR<T>;
         start = end+1;
      } while (end != std::string::npos);

      return s;
   }
}

namespace n245
{
   template <typename T>
   struct foo
   {
      typedef T value_type;
   };
}

namespace n246
{
   template <typename T>
   struct foo
   {
      using value_type = T;
   };
}

namespace n247
{
   template <typename T>
   using customer_addresses_t = std::map<int, std::vector<T>>;

   struct delivery_address_t {};
   struct invoice_address_t {};

   using customer_delivery_addresses_t = customer_addresses_t<delivery_address_t>;
   using customer_invoice_addresses_t = customer_addresses_t<invoice_address_t>;
}

namespace n247
{
   template <typename T, size_t S>
   struct list
   {
      using type = std::vector<T>;
   };

   template <typename T>
   struct list<T, 1>
   {
      using type = T;
   };

   template <typename T, size_t S>
   using list_t = typename list<T, S>::type;
}

namespace n248
{

}

int main()
{
   {
      using namespace n201;

      auto a1 = add(42, 21);
      auto a2 = add<int>(42, 21);
      auto a3 = add<>(42, 21);

      auto b = add<short>(42, 21);

      //auto d1 = add(41.0, 21); // error
      auto d2 = add<double>(41.0, 21);

      auto f = add(foo(42), foo(41));

      int arr[]{ 1,1,2,3,5,8,11 };
      int odds = count_if(std::begin(arr), std::end(arr),
         [](int const n) { return n % 2 == 1; });
      std::cout << odds << '\n';
   }

   {
      using namespace n202;

      wrapper a(42);           // wraps an int
      wrapper<int> b(42);      // wraps an int
      wrapper<short> c(42);    // wraps a short
      wrapper<double> d(42.0); // wraps a double
      wrapper e("42");         // wraps a char const *
   }

   {
      using namespace n203;

      wrapper<int> a(42);          // wraps an int
      use_wrapper(&a);
   }

   {
      using namespace n204;

      composition<int> c;
      c.add(41, 21);
   }

   {
      using namespace n205;

      composition c;
      c.add<int>(41, 21);
   }

   {
      using namespace n206;

      wrapper<double> a(42.0);
      auto d = a.get();       // double
      auto n = a.as<int>();   // int
   }

   {
      using namespace n209;

      buffer<int, 10> b1;
      b1[0] = 42;
      std::cout << b1[0] << '\n';

      auto b2 = make_buffer<int, 10>();      
   }

   {
      using namespace n209;

      buffer<int, 10> b1;
      buffer<int, 2*5> b2;

      std::cout << std::is_same_v<buffer<int, 10>, buffer<int, 2*5>> << '\n';
      std::cout << std::is_same_v<decltype(b1), decltype(b2)> << '\n';

      static_assert(std::is_same_v<decltype(b1), decltype(b2)>);

      buffer<int, 3*5> b3;
      static_assert(!std::is_same_v<decltype(b1), decltype(b3)>);
   }

   {
      using namespace n210;

      hello_command cmd;

      auto w1 = std::make_unique<smart_device<hello_command, &hello_command::say_hello_in_english>>(&cmd);
      w1->output();

      auto w2 = std::make_unique<smart_device<hello_command, &hello_command::say_hello_in_spanish>>(&cmd);
      w2->output();
   }

   {
      using namespace n211;

      hello_command cmd;

      auto w1 = std::make_unique<smart_device<hello_command, &hello_command::say_hello_in_english>>(cmd);
      w1->output();

      auto w2 = std::make_unique<smart_device<hello_command, &hello_command::say_hello_in_spanish>>(cmd);
      w2->output();
   }

   {
      using namespace n212;

      auto w1 = std::make_unique<smart_device<&say_hello_in_english>>();
      w1->output();

      auto w2 = std::make_unique<smart_device<&say_hello_in_spanish>>();
      w2->output();

      static_assert(!std::is_same_v<decltype(w1), decltype(w2)>);
   }

   {
      using namespace n212;

      std::unique_ptr<device> w1 = std::make_unique<smart_device<&say_hello_in_english>>();
      w1->output();

      std::unique_ptr<device> w2 = std::make_unique<smart_device<&say_hello_in_spanish>>();
      w2->output();

      static_assert(std::is_same_v<decltype(w1), decltype(w2)>);
   }

   {
      using namespace n213;
      [[maybe_unused]] foo<42> f1;    // foo<int>
      [[maybe_unused]] foo<42.0> f2;  // foo<double>
      //foo<"42"> f3;  // error
   }

   {
      using namespace n214;
      [[maybe_unused]] foo<"42"> f;
   }

   {
      using namespace n215;
      [[maybe_unused]] foo<42, 42.0, false, 'x'> f;
   }

   {
      using namespace n216;
      wrapping_pair<int, double> p1(42, 42.0);
      std::cout << p1.item1.get() << ' '
                << p1.item2.get() << '\n';

      wrapping_pair<int, double, simple_wrapper> p2(42, 42.0);
      std::cout << p2.item1.value << ' '
                << p2.item2.value << '\n';
   }

   {
      using namespace n219;
      foo f{ 42, 42.0 };
   }

   {
      using namespace n221;
      //bar<foo<int>> x;
   }

   {
      using namespace n222;
      foo<int> a;
      a.f();
      //a.g();
   }

   {
      using namespace n223;

      [[maybe_unused]] foo<int>* p;
      [[maybe_unused]] foo<int> x;
      [[maybe_unused]] foo<double>* q;
   }

   {
      using namespace n223;

      [[maybe_unused]] foo<int>* p;
      foo<int> x;
      foo<double>* q = nullptr;

      x.f();
      q->g();
   }

   {
      using namespace n225;

      foo<int> a;
      foo<double> b;
      foo<double> c;

      std::cout << a.data << '\n'; // 0
      std::cout << b.data << '\n'; // 0
      std::cout << c.data << '\n'; // 0

      b.data = 42;
      std::cout << b.data << '\n'; // 42
      std::cout << c.data << '\n'; // 42
   }

   {
      using namespace ext;

      wrapper<int> a{ 0 };

      std::cout << a.data << '\n';
      f();
      g();
   }

   {
      using namespace n228;

      std::cout << is_floating_point<int>::value << '\n';
      std::cout << is_floating_point<float>::value << '\n';
      std::cout << is_floating_point<double>::value << '\n';
      std::cout << is_floating_point<long double>::value << '\n';
      std::cout << is_floating_point<std::string>::value << '\n';
   }

   {
      using namespace n228;

      std::cout << is_floating_point_v<int> << '\n';
      std::cout << is_floating_point_v<float> << '\n';
      std::cout << is_floating_point_v<double> << '\n';
      std::cout << is_floating_point_v<long double> << '\n';
      std::cout << is_floating_point_v<std::string> << '\n';
   }

   {
      using namespace n229;

      std::cout << is_floating_point<int>::value << '\n';
      std::cout << is_floating_point<float>::value << '\n';
   }

   {
      using namespace n230;

      [[maybe_unused]] foo<double> a; // OK
      [[maybe_unused]] foo<int>* b;   // OK
      //foo<int> c;    // error, foo<int> incomplete type
   }

   {
      using namespace n231;

      func(foo<int>{});
      func(foo<double>{});
   }

   {
      using namespace n232;

      func(42.0);
      func(42);
   }

   {
      using namespace n233;

      foo<double> a, b;
      std::cout << a.value << '\n';
      std::cout << b.value << '\n';

      foo<int> c;
      std::cout << c.value << '\n';

      a.value = 100;
      std::cout << a.value << '\n';
      std::cout << b.value << '\n';
      std::cout << c.value << '\n';
   }

   {
      using namespace n234;

      func(1, 2);
      func(1, 2.0);
      func(1.0, 2.0);
   }

   {
      using namespace n235;

      collection<char, 42>{}();  // primary template
      collection<int, 42>{}();   // partial specialization <int, S>
      collection<char, 10>{}();  // partial specialization <T, 10>
      collection<int*, 20>{}();  // partial specialization <T*, S>

      //collection<int, 10>{}();      // error: collection<T,10> or collection<int,S>
      //collection<char*, 10>{}();    // error: collection<T,10> or collection<T*,S>
   }

   {
      using namespace n237;

      std::array<int, 9> arr {1, 1, 2, 3, 5, 8, 13, 21};
      pretty_print(std::cout, arr);

      std::array<char, 9> str;
      std::strcpy(str.data(), "template");
      pretty_print(std::cout, str);
   }

   {
      using namespace n238;

      std::array<char, 9> str;
      std::strcpy(str.data(), "template");
      pretty_print(std::cout, str);
   }

   {
      using namespace n239;

      float v1 = sphere_volume(42.0f);
      double v2 = sphere_volume(42.0);
   }

   {
      using namespace n240;

      float v1 = sphere_volume(42.0f);
      double v2 = sphere_volume(42.0);
   }

   {
      using namespace n241;

      float v1 = sphere_volume(42.0f);
      double v2 = sphere_volume(42.0);
   }

   {
      using namespace n242;

      float v1 = sphere_volume(42.0f);
      double v2 = sphere_volume(42.0);
   }

   {
      using namespace n244;
      show_parts<char>(std::cout, "one\ntwo\nthree");
      show_parts<wchar_t>(std::wcout, L"one line");
   }

   {
      typedef int index_t;
      typedef std::vector<std::pair<int, std::string>> NameValueList;
      typedef int (*fn_ptr)(int, char);

   }

   {
      using index_t = int;
      using NameValueList = std::vector<std::pair<int, std::string>>;
      using fn_ptr = int(int, char);
   }

   {
      using namespace n247;
      static_assert(std::is_same_v<list_t<int, 1>, int>);
      static_assert(std::is_same_v<list_t<int, 2>, std::vector<int>>);
   }

   {
      int arr[] = { 1,6,3,8,4,2,9 };
      std::sort(
         std::begin(arr), std::end(arr),
         [](int const a, int const b) {return a > b; });

      int pivot = 5;
      auto count = std::count_if(
         std::begin(arr), std::end(arr),
         [pivot](int const a) {return a > pivot; });

      std::cout << count << '\n';
   }

   {
      auto l1 = [](int a) {return a + a; };           // C++11, regular lambda
      auto l2 = [](auto a) {return a + a; };          // C++14, generic lambda
      auto l3 = []<typename T>(T a) { return a + a; };// C++20, template lambda

      auto v1 = l1(42);	                  // OK
      auto v2 = l1(42.0);	               // warning
      //auto v3 = l1(std::string{ "42" });   // error

      auto v5 = l2(42);                    // OK
      auto v6 = l2(42.0);                  // OK
      auto v7 = l2(std::string{"42"});     // OK

      auto v8 = l3(42);                    // OK
      auto v9 = l3(42.0);                  // OK
      auto v10 = l3(std::string{ "42" });   // OK
   }

   {
      auto l1 = [](int a, int b) {return a + b; };
      auto l2 = [](auto a, auto b) {return a + b; };
      auto l3 = []<typename T, typename U>(T a, U b) { return a + b; };
      auto l4 = [](auto a, decltype(a) b) {return a + b; };

      auto v1 = l1(42, 1);	                  // OK
      auto v2 = l1(42.0, 1.0);	               // warning
      //auto v3 = l1(std::string{ "42" }, '1'); // error

      auto v4 = l2(42, 1);                    // OK
      auto v5 = l2(42.0, 1);                  // OK
      auto v6 = l2(std::string{ "42" }, '1'); // OK
      auto v7 = l2(std::string{ "42" }, std::string{ "1" }); // OK

      auto v8 = l3(42, 1);                    // OK
      auto v9 = l3(42.0, 1);                  // OK
      auto v10 = l3(std::string{ "42" }, '1'); // OK
      auto v11 = l3(std::string{ "42" }, std::string{ "42" }); // OK

      auto v12 = l4(42.0, 1); // OK
      auto v13 = l4(42, 1.0); // warning
      //auto v14 = l4(std::string{ "42" }, '1'); // error
   }

   {
      auto l = []<typename T, size_t N>(std::array<T, N> const& arr) { return std::accumulate(arr.begin(), arr.end(), static_cast<T>(0)); };

      //auto v1 = l(1); // error
      auto v2 = l(std::array<int, 3>{1, 2, 3});
   }

   {
      auto l = []<typename T>(T a, T b) { return a + b; };

      auto v1 = l(42, 1);        // OK
      //auto v2 = l(42.0, 1);    // error

      auto v4 = l(42.0, 1.0);    // OK
      //auto v5 = l(42, false);    // error

      auto v6 = l(std::string{ "42" }, std::string{ "1" }); // OK
      //auto v6 = l(std::string{ "42" }, '1');                // error
   }

   {
      std::function<int(int)> factorial;
      factorial = [&factorial](int const n) {
         if (n < 2) return 1;
         else return n * factorial(n - 1);
      };

      std::cout << factorial(5) << '\n';
   }

   {
      auto factorial = [](auto f, int const n) {
         if (n < 2) return 1;
         else return n * f(f, n - 1);
      };

      std::cout << factorial(factorial, 5) << '\n';
   }
}

