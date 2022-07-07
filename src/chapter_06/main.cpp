#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <string_view>
#include <type_traits>
#include <memory>

namespace n601
{
   template <typename T>
   T add(T const a, T const b)
   {
      return a + b;
   }
}

namespace n602
{
   template <typename T,
             typename = typename std::enable_if_t<std::is_arithmetic_v<T>>>
   T add(T const a, T const b)
   {
      return a + b;
   }
}

namespace n603
{
   template <typename T>
   T add(T const a, T const b)
   {
      static_assert(std::is_arithmetic_v<T>, "Arithmetic type required");
      return a + b;
   }
}

namespace n604
{
   template <typename T>
   requires std::is_arithmetic_v<T>
   T add(T const a, T const b)
   {
      return a + b;
   }
}

namespace n605
{
   template <typename T>      
   T add(T const a, T const b) requires std::is_arithmetic_v<T>
   {
      return a + b;
   }
}

namespace n606
{
   template<typename T>
   concept arithmetic = requires { std::is_arithmetic_v<T>; };

   template <arithmetic T>
   T add(T const a, T const b) 
   {
      return a + b;
   }
}

namespace n607
{
   template<typename T>
   concept arithmetic = std::is_arithmetic_v<T>;

   template <arithmetic T>
   T add(T const a, T const b)
   {
      return a + b;
   }
}

namespace n608
{
   template <typename T>
      requires std::is_arithmetic_v<T>
   T add(T const a, T const b)
   {
      return a + b;
   }

   template <typename T>
      requires std::is_arithmetic_v<T>
   T mul(T const a, T const b)
   {
      return a * b;
   }
}

namespace n610
{
   template <typename T, typename U = void>
   struct is_container : std::false_type {};

   template <typename T>
   struct is_container<T,
      std::void_t<typename T::value_type,
                  typename T::size_type,
                  typename T::allocator_type,
                  typename T::iterator,
                  typename T::const_iterator,
                  decltype(std::declval<T>().size()),
                  decltype(std::declval<T>().begin()),
                  decltype(std::declval<T>().end()),
                  decltype(std::declval<T>().cbegin()),
                  decltype(std::declval<T>().cend())>> 
      : std::true_type{};

   template <typename T, typename U = void>
   constexpr bool is_container_v = is_container<T, U>::value;

   struct foo {};

   static_assert(!is_container_v<foo>);
   static_assert(is_container_v<std::vector<foo>>);
}

namespace n611
{
   template <typename T>
   concept container = requires(T t)
   {
      typename T::value_type;
      typename T::size_type;
      typename T::allocator_type;
      typename T::iterator;
      typename T::const_iterator;
      t.size();
      t.begin();
      t.end();
      t.cbegin();
      t.cend();
   };

   struct foo{};

   static_assert(!container<foo>);
   static_assert(container<std::vector<foo>>);

   template <container C>
   void process(C&& c) {}
}

namespace n612
{
   template<typename T>
   concept arithmetic = requires 
   {
      std::is_arithmetic_v<T>; 
   };

   template <typename T>
   concept addable = requires(T a, T b) 
   { 
      a + b; 
   };

   template <typename T>
   concept logger = requires(T t)
   {
      t.error("just");
      t.warning("a");
      t.info("demo");
   };

   template <logger T>
   void log_error(T& logger)
   {
   }

   struct console_logger
   {
      void error(std::string_view text){}
      void warning(std::string_view text) {}
      void info(std::string_view text) {}
   };

   struct stream_logger
   {
      void error(std::string_view text, bool = false) {}
      void warning(std::string_view text, bool = false) {}
      void info(std::string_view text, bool) {}
   };
}

namespace n613
{
   template <typename T>
   concept KVP = requires {
      typename T::key_type;
      typename T::value_type;
   };

   template <typename T, typename V>
   struct key_value_pair
   {
      using key_type = T;
      using value_type = V;

      key_type    key;
      value_type  value;
   };

   static_assert(KVP<key_value_pair<int, std::string>>);
   static_assert(!KVP<std::pair<int, std::string>>);

   template <typename T>
   requires std::is_arithmetic_v<T>
   struct container
   { /* ... */ };

   template <typename T>
   concept containerizeable = requires {
      typename container<T>;
   };

   static_assert(containerizeable<int>);
   static_assert(!containerizeable<std::string>);
}

namespace n614
{
   template <typename T>
   void f(T) noexcept {}

   template <typename T>
   void g(T) {}

   template <typename F, typename ... T>
   concept NonThrowing = requires(F && func, T ... t)
   {
      {func(t...)} noexcept;
   };

   template <typename F, typename ... T>
      requires NonThrowing<F, T...>
   void invoke(F&& func, T... t)
   {
      func(t...);
   }   
}

namespace n615
{
   template <typename T>
   concept timer = requires(T t)
   {
      {t.start()} -> std::same_as<void>;
      {t.stop()} -> std::convertible_to<long long>;
   };

   struct timerA
   {
      void start() {}
      long long stop() { return 0; }
   };

   struct timerB
   {
      void start() {}
      int stop() { return 0; }
   };

   struct timerC
   {
      void start() {}
      void stop() {}
      long long getTicks() { return 0; }
   };

   static_assert(timer<timerA>);
   static_assert(timer<timerB>);
   static_assert(!timer<timerC>);
}

namespace n616
{
   template<typename T, typename... Ts>
   inline constexpr bool are_same_v = std::conjunction_v<std::is_same<T, Ts>...>;

   template <typename ... T>
   concept HomogenousRange = requires(T... t)
   {
      (... + t);
      requires are_same_v<T...>;
      requires sizeof...(T) > 1;
   };

   template <typename ... T>
   requires HomogenousRange<T...>
   auto add(T&&... t)
   {
      return (... + t);
   }

   static_assert(HomogenousRange<int, int>);
   static_assert(!HomogenousRange<int>);
   static_assert(!HomogenousRange<int, double>);
}

namespace n617
{
   template <typename T>
   requires std::is_integral_v<T> && std::is_signed_v<T>
   T decrement(T value) 
   {
      return value--;
   }
}

namespace n618
{
   template <typename T>
   concept Integral = std::is_integral_v<T>;

   template <typename T>
   concept Signed = std::is_signed_v<T>;

   template <typename T>
   concept SignedIntegral = Integral<T> && Signed<T>;

   template <SignedIntegral T>
   T decrement(T value)
   {
      return value--;
   }
}

namespace n619
{
   template <typename T>
   requires std::is_integral_v<T> || std::is_floating_point_v<T>
   T add(T a, T b)
   {
      return a + b;
   }
}

namespace n620
{
   template <typename T>
   concept Integral = std::is_integral_v<T>;

   template <typename T>
   concept FloatingPoint = std::is_floating_point_v<T>;

   template <typename T>
   concept Number = Integral<T> || FloatingPoint<T>;

   template <Number T>
   T add(T a, T b)
   {
      return a + b;
   }
}

namespace n621
{   
   template <typename T>
   concept A = std::is_integral_v<T>;

   template <typename T>
   concept B = std::is_floating_point_v<T>;

   // disjunctions
   template <typename T>
   requires A<T> || B<T>
   void f() {}

   template <typename T>
   requires (A<T> || B<T>)
   void f() {}

   template <typename T>
   requires A<T> && (!A<T> || B<T>)
   void f() {}

   // logical operators
   template <typename T>
   requires (!(A<T> || B<T>))
   void f() {}

   template <typename T>
   requires (static_cast<bool>(A<T> || B<T>))
   void f() {}
}

namespace n622
{
   template <typename ... T>
   requires (std::is_integral_v<T> && ...)
   auto add(T ... args)
   {
      return (args + ...);
   }
}

namespace n623
{
   template <typename T>
   concept Integral = std::is_integral_v<T>;

   template <typename ... T>
   requires (Integral<T> && ...)
   auto add(T ... args)
   {
      return (args + ...);
   }
}

namespace n624
{
   int add(int a, int b)
   {
      return a + b;
   }

   template <typename T>
   T add(T a, T b)
   {
      return a + b;
   }
}

namespace n625
{
   template <typename T>
   T add(T a, T b)
   {
      return a + b;
   }

   template <typename T>
   requires std::is_integral_v<T>
   T add(T a, T b)
   {
      return a + b;
   }
}

namespace n626
{
   template <typename T>
      requires (sizeof(T) == 4)
   T add(T a, T b)
   {
      return a + b;
   }

   template <typename T>
      requires std::is_integral_v<T>
   T add(T a, T b)
   {
      return a + b;
   }
}

namespace n627
{
   template <typename T>
      requires std::is_integral_v<T>
   T add(T a, T b)
   {
      return a + b;
   }

   template <typename T>
      requires std::is_integral_v<T> && (sizeof(T) == 4)
   T add(T a, T b)
   {
      return a + b;
   }
}

namespace n628
{
   template <typename T>
   concept Integral = std::is_integral_v<T>;

   template <typename T>
      requires Integral<T>
   T add(T a, T b)
   {
      return a + b;
   }

   template <typename T>
      requires Integral<T> && (sizeof(T) == 4)
   T add(T a, T b)
   {
      return a + b;
   }
}

namespace n629
{
   template <typename T>
   concept Integral = std::is_integral_v<T>;

   template <Integral T>
   T add(T a, T b)
   {
      return a + b;
   }

   template <Integral T>
   requires (sizeof(T) == 4)
   T add(T a, T b)
   {
      return a + b;
   }
}

namespace n630
{
   template <typename T>
   struct wrapper
   {
      T value;

      bool operator==(std::string_view str)
      requires std::is_convertible_v<T, std::string_view>
      {
         return value == str;
      }
   };
}

namespace n631a
{
   template <typename T>
   struct wrapper
   {
      T value;

      wrapper(T const & v) :value(v) {}
   };
}

namespace n631b
{
   template <typename T>
   struct wrapper
   {
      T value;

      template <typename U,
                typename = std::enable_if_t<std::is_copy_constructible_v<U> &&
                                            std::is_convertible_v<U, T>>>
      wrapper(U const& v) :value(v) {}
   };
}

namespace n631c
{
   template <typename T>
   struct wrapper
   {
      T value;

      wrapper(T const& v) 
         requires std::is_copy_constructible_v<T> 
      :value(v) {}
   };
}

namespace n632a
{
   void handle(int v)
   { /* do something else */ }

   //void handle(long v)
   //   requires (sizeof(long) > sizeof(int))
   //{ /* do something else */ }
}

namespace n632b
{
   void handle(long v)
   {
      if constexpr (sizeof(long) > sizeof(int))
      {
         /* do something else */
      }
      else
      {
         /* do something */
      }
   }
}

namespace n633a
{
   template <std::integral T>
   struct wrapper
   {
      T value;
   };
}

namespace n633b
{
   template <std::integral T>
   struct wrapper
   {
      T value;
   };

   template <std::integral T>
      requires (sizeof(T) == 4)
   struct wrapper<T>
   {
      union
      {
         T value;
         struct
         {
            uint8_t byte4;
            uint8_t byte3;
            uint8_t byte2;
            uint8_t byte1;
         };
      };
   };
}

namespace n634
{
   template <std::floating_point T>
   constexpr T PI = T(3.1415926535897932385L);
}

namespace n635a
{
   template <std::integral T>
   using integral_vector = std::vector<T>;
}

namespace n635b
{
   template <typename T>
      requires std::integral<T>
   using integral_vector = std::vector<T>;
}

namespace n636a
{
   auto add(auto a, auto b)
   {
      return a + b;
   }

   template<>
   auto add(char const* a, char const* b)
   {
      return std::string(a) + std::string(b);
   }
}

namespace n636b
{
   auto add(std::integral auto a, std::integral auto b)
   {
      return a + b;
   }
}

namespace n636c
{
   auto add(std::integral auto ... args)
   {
      return (args + ...);
   }
}

namespace n637a
{
   template <typename T>
   concept addable = requires(T a, T b) { a + b; }; // requires expression

   template <typename T>
      requires addable<T>  // requires clause with concept
   auto add(T a, T b)
   {
      return a + b;
   }
}

namespace n637b
{
   template <typename T>
      requires requires(T a, T b) { a + b; }  // requires clause with requires expression
   auto add(T a, T b)
   {
      return a + b;
   }
}

int main()
{
   {
      using namespace n601;
      using namespace std::string_literals;

      add(42, 1);
      add(42.0, 1.0);
      add("42"s, "1"s);
      //add("42", "1");   // error: cannot add two pointers
   }

   {
      using namespace n602;
      using namespace std::string_literals;

      add(42, 1);
      add(42.0, 1.0);
      //add("42"s, "1"s); // error: no matching overloaded function found
      //add("42", "1");   // error: no matching overloaded function found
   }

   {
      using namespace n603;
      using namespace std::string_literals;

      add(42, 1);
      add(42.0, 1.0);
      //add("42"s, "1"s); // error: Arithmetic type required
      //add("42", "1");   // error: Arithmetic type required
   }

   {
      using namespace n604;
      using namespace std::string_literals;

      add(42, 1);
      add(42.0, 1.0);
      //add("42"s, "1"s); // error: the associated constraints are not satisfied
      //add("42", "1");   // error: the associated constraints are not satisfied
   }

   {
      using namespace n605;
      using namespace std::string_literals;

      add(42, 1);
      add(42.0, 1.0);
      //add("42"s, "1"s); // error: the associated constraints are not satisfied
      //add("42", "1");   // error: the associated constraints are not satisfied
   }

   {
      using namespace n606;
      using namespace std::string_literals;

      add(42, 1);
      add(42.0, 1.0);
      //add("42"s, "1"s); // error: the associated constraints are not satisfied
      //add("42", "1");   // error: the associated constraints are not satisfied
   }

   {
      using namespace n607;
      using namespace std::string_literals;

      add(42, 1);
      add(42.0, 1.0);
      //add("42"s, "1"s); // error: the associated constraints are not satisfied
      //add("42", "1");   // error: the associated constraints are not satisfied
   }

   {
      using namespace n612;

      console_logger cl;
      log_error(cl);

      [[maybe_unused]] stream_logger sl;
      // log_error(sl); // error: the associated constraints are not satisfied
   }

   {
      using namespace n614;

      invoke(f<int>, 42);
      //invoke(g<int>, 42); // error
   }

   {
      using namespace n616;

      add(1, 2);
      //add(1);
      //add(1, 2.0);
   }

   {
      using namespace n622;

      add(1, 2, 3);
      //add(1, 42.0);
   }

   {
      using namespace n623;

      add(1, 2, 3);
      //add(1, 42.0);
   }

   {
      using namespace n624;

      add(1.0, 2.0);
      add(1, 2);
   }

   {
      using namespace n625;

      add(1.0, 2.0);
      add(1, 2);
   }

   {
      using namespace n626;

      add((short)1, (short)2);
      //add(1, 2);
   }

   {
      using namespace n627;

      add((short)1, (short)2);
      //add(1, 2);
   }

   {
      using namespace n628;

      add((short)1, (short)2);
      add(1, 2);
   }

   {
      using namespace n629;

      add((short)1, (short)2);
      add(1, 2);
   }

   {
      using namespace n630;

      wrapper<int> a{ 42 };
      wrapper<char const*> b{ "42" };

      //if(a == 42) {} // error
      if(b == "42") {}
   }

   {
      using namespace n631a;

      wrapper<int> a = 42;

      //wrapper<std::unique_ptr<int>> p = std::make_unique<int>(42); // error
   }

   {
      using namespace n631b;

      wrapper<int> a = 42;

      //wrapper<std::unique_ptr<int>> p = std::make_unique<int>(42); //error
   }

   {
      using namespace n631c;

      wrapper<int> a = 42;

      //wrapper<std::unique_ptr<int>> p = std::make_unique<int>(42); //error
   }

   {
      using namespace n633a;

      wrapper<int> a{ 42 };
      // wrapper<double> b{ 42.0 }; // error
   }

   {
      using namespace n633b;

      wrapper<short> a{ 42 };
      std::cout << a.value << '\n';

      wrapper<int> b{ 0x11223344 };
      std::cout << std::hex << b.value << '\n';
      std::cout << std::hex << (int)b.byte1 << '\n';
      std::cout << std::hex << (int)b.byte2 << '\n';
      std::cout << std::hex << (int)b.byte3 << '\n';
      std::cout << std::hex << (int)b.byte4 << '\n';
   }

   {
      using namespace n634;

      std::cout << PI<double> << '\n';  // OK
      //std::cout << PI<int> << '\n';     // error
   }

   {
      using namespace n635a;

      integral_vector<int> v1 { 1,2,3 };
      //integral_vector<double> v2 {1.0, 2.0, 3.0}; // error
   }

   {
      using namespace n635b;

      integral_vector<int> v1{ 1,2,3 };
      //integral_vector<double> v2 {1.0, 2.0, 3.0}; // error
   }

   {
      using namespace n636a;

      add(4, 2);
      add(4.0, 2);
      add("4", "2");
   }

   {
      using namespace n636b;

      add(4, 2);
      //add(4.2, 0); // error
   }

   {
      using namespace n636c;

      add(1, 2, 3);
   }
}