#include <iostream>
#include <string>
#include <sstream>
#include <array>
#include <cassert>
#include <vector>
#include <ranges>
#include <string>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <format>

namespace n901
{
   int sum_proper_divisors(int const number)
   {
      int result = 1;

      for (int i = 2; i <= std::sqrt(number); ++i)
      {
         if (number % i == 0)
         {
            result += (i == (number / i)) ? i : (i + number / i);
         }
      }

      return result;
   }

   bool is_abundant(int const number)
   {
      return sum_proper_divisors(number) > number;
   }
}

namespace n902
{
   template <typename R>
   struct step_iterator;

   template <typename R>
   struct step_sentinel
   {
      using base = std::ranges::iterator_t<R>;
      using size_type = std::ranges::range_difference_t<R>;

      step_sentinel() = default;

      constexpr step_sentinel(base end) : end_{ end } {}
      constexpr bool is_at_end(step_iterator<R> it) const;

   private:
      base      end_;
   };

   template <typename R>
   struct step_iterator : std::ranges::iterator_t<R>
   {
      using base = std::ranges::iterator_t<R>;
      using value_type = typename std::ranges::range_value_t<R>;
      using reference_type = typename std::ranges::range_reference_t<R>;

      constexpr step_iterator(base start, base end, std::ranges::range_difference_t<R> step) :
         pos_{ start }, end_{ end }, step_{ step }
      {
      }

      constexpr step_iterator operator++(int)
      {
         auto ret = *this;
         pos_ = std::ranges::next(pos_, step_, end_);
         return ret;
      }

      constexpr step_iterator& operator++()
      {
         pos_ = std::ranges::next(pos_, step_, end_);
         return *this;
      }

      constexpr reference_type operator*() const
      {
         return *pos_;
      }

      constexpr bool operator==(step_sentinel<R> s) const
      {
         return s.is_at_end(*this);
      }

      constexpr base const value() const { return pos_; }

   private:
      base                                pos_;
      base                                end_;
      std::ranges::range_difference_t<R>  step_;
   };

   template <typename R>
   constexpr bool step_sentinel<R>::is_at_end(step_iterator<R> it) const
   {
      return end_ == it.value();
   }

   template<std::ranges::view R>
   struct step_view : public std::ranges::view_interface<step_view<R>>
   {
   private:
      R                                   base_;
      std::ranges::range_difference_t<R>  step_;

   public:
      step_view() = default;

      constexpr step_view(R base, std::ranges::range_difference_t<R> step)
         : base_(std::move(base))
         , step_(step)
      {
      }

      constexpr R base() const&
         requires std::copy_constructible<R>
      { return base_; }
      constexpr R base()&& { return std::move(base_); }

      constexpr std::ranges::range_difference_t<R> const& increment() const { return step_; }

      constexpr auto begin()
      {
         return step_iterator<R const>(std::ranges::begin(base_), std::ranges::end(base_), step_);
      }

      constexpr auto begin() const requires std::ranges::range<R const>
      {
         return step_iterator<R const>(std::ranges::begin(base_), std::ranges::end(base_), step_);
      }

      constexpr auto end()
      {
         return step_sentinel<R const>{std::ranges::end(base_)};
      }

      constexpr auto end() const requires std::ranges::range<R const>
      {
         return step_sentinel<R const>{std::ranges::end(base_)};
      }

      constexpr auto size() const requires std::ranges::sized_range<R const>
      { auto d = std::ranges::size(base_); return step_ == 1 ? d : static_cast<int>((d + 1) / step_); }

      constexpr auto size() requires std::ranges::sized_range<R>
      { auto d = std::ranges::size(base_); return step_ == 1 ? d : static_cast<int>((d + 1) / step_); }
   };

   template<class R>
   step_view(R&& base, std::ranges::range_difference_t<R> step)
      ->step_view<std::ranges::views::all_t<R>>;

   namespace details
   {
      using test_range_t = std::ranges::views::all_t<std::vector<int>>;
      static_assert(std::input_iterator<step_iterator<test_range_t>>);
      static_assert(std::sentinel_for<step_sentinel<test_range_t>, step_iterator<test_range_t>>);

      struct step_view_fn_closure
      {
         std::size_t step_;
         constexpr step_view_fn_closure(std::size_t step)
            : step_(step)
         {
         }

         template <std::ranges::range R>
         constexpr auto operator()(R&& r) const
         {
            return step_view(std::forward<R>(r), step_);
         }
      };

      struct step_view_fn
      {
         template<std::ranges::range R>
         constexpr auto operator () (R&& r, std::size_t step) const
         {
            return step_view(std::forward<R>(r), step);
         }

         constexpr auto operator () (std::size_t step) const
         {
            return step_view_fn_closure(step);
         }
      };

      template <std::ranges::range R>
      constexpr auto operator | (R&& r, step_view_fn_closure&& a)
      {
         return std::forward<step_view_fn_closure>(a)(std::forward<R>(r));
      }
   }

   namespace views
   {
      inline constexpr details::step_view_fn step;
   }
}

namespace n903
{
   template <typename R>
   struct replicate_iterator;

   template <typename R>
   struct replicate_sentinel
   {
      using base = std::ranges::iterator_t<R>;
      using size_type = std::ranges::range_difference_t<R>;

      replicate_sentinel() = default;

      constexpr replicate_sentinel(base end) : end_{ end } {}
      constexpr bool is_at_end(replicate_iterator<R> it) const;

   private:
      base      end_;
   };

   template <typename R>
   struct replicate_iterator : std::ranges::iterator_t<R>
   {
      using base = std::ranges::iterator_t<R>;
      using value_type = typename std::ranges::range_value_t<R>;
      using reference_type = typename std::ranges::range_reference_t<R>;

      constexpr replicate_iterator(base start, std::ranges::range_difference_t<R> count) :
         pos_{ start }, count_{ count }
      {
      }

      constexpr replicate_iterator operator++(int)
      {
         if (step_ == count_)
         {
            step_ = 1;
            pos_++;
         }
         else
         {
            step_++;
         }

         return pos_;
      }

      constexpr replicate_iterator& operator++()
      {
         if (step_ == count_)
         {
            step_ = 1;
            pos_++;
         }
         else
         {
            step_++;
         }

         return (*this);
      }

      constexpr reference_type operator*() const
      {
         return *pos_;
      }

      constexpr bool operator==(replicate_sentinel<R> s) const
      {
         return s.is_at_end(*this);
      }

      constexpr base const value() const { return pos_; }

   private:
      base                                pos_;
      std::ranges::range_difference_t<R>  count_;
      std::ranges::range_difference_t<R>  step_ = 1;
   };

   template <typename R>
   constexpr bool replicate_sentinel<R>::is_at_end(replicate_iterator<R> it) const
   {
      return end_ == it.value();
   }

   template<std::ranges::view R>
   struct replicate_view : public std::ranges::view_interface<replicate_view<R>>
   {
   private:
      R                                   base_;
      std::ranges::range_difference_t<R>  count_;

   public:
      replicate_view() = default;

      constexpr replicate_view(R base, std::ranges::range_difference_t<R> count)
         : base_(std::move(base))
         , count_(count)
      {
      }

      constexpr R base() const&
         requires std::copy_constructible<R>
      { return base_; }
      constexpr R base()&& { return std::move(base_); }

      constexpr std::ranges::range_difference_t<R> const& increment() const { return count_; }

      constexpr auto begin()
      {
         return replicate_iterator<R const>(std::ranges::begin(base_), count_);
      }

      constexpr auto begin() const requires std::ranges::range<R const>
      {
         return replicate_iterator<R const>(std::ranges::begin(base_), count_);
      }

      constexpr auto end()
      {
         return replicate_sentinel<R const>{std::ranges::end(base_)};
      }

      constexpr auto end() const requires std::ranges::range<R const>
      {
         return replicate_sentinel<R const>{std::ranges::end(base_)};
      }

      constexpr auto size() const requires std::ranges::sized_range<R const>
      { return count_ * std::ranges::size(base_); }

      constexpr auto size() requires std::ranges::sized_range<R>
      { return count_ * std::ranges::size(base_); }
   };

   template<class R>
   replicate_view(R&& base, std::ranges::range_difference_t<R> count)
      ->replicate_view<std::ranges::views::all_t<R>>;

   namespace details
   {
      using test_range_t = std::ranges::views::all_t<std::vector<int>>;
      static_assert(std::input_iterator<replicate_iterator<test_range_t>>);
      static_assert(std::sentinel_for<replicate_sentinel<test_range_t>, replicate_iterator<test_range_t>>);

      struct replicate_view_fn_closure
      {
         std::size_t step_;
         constexpr replicate_view_fn_closure(std::size_t step)
            : step_(step)
         {
         }

         template <std::ranges::range R>
         constexpr auto operator()(R&& r) const
         {
            return replicate_view(std::forward<R>(r), step_);
         }
      };

      struct replicate_view_fn
      {
         template<std::ranges::range R>
         constexpr auto operator () (R&& r, std::size_t step) const
         {
            return replicate_view(std::forward<R>(r), step);
         }

         constexpr auto operator () (std::size_t step) const
         {
            return replicate_view_fn_closure(step);
         }
      };

      template <std::ranges::range R>
      constexpr auto operator | (R&& r, replicate_view_fn_closure&& a)
      {
         return std::forward<replicate_view_fn_closure>(a)(std::forward<R>(r));
      }
   }

   namespace views
   {
      inline constexpr details::replicate_view_fn replicate;
   }
}

struct Item
{
   int         id;
   std::string name;
   double      price;
};

int main()
{
   // all abundant numbers from 1 to 100
   {
      using namespace n901;

      std::cout << "abundant numbers: \n";

      for (int i = 1; i <= 100; i++)
      {
         if (is_abundant(i))
            std::cout << i << '\n';
      }
   }

   {
      using namespace n901;

      std::cout << "abundant numbers: \n";

      for (int i : std::views::iota(1, 101) | 
                   std::views::filter(is_abundant))
      {
         std::cout << i << '\n';
      }
   }

   {
      using namespace n901;

      std::cout << "abundant numbers: \n";

      for (int i : std::ranges::filter_view(
                     std::ranges::iota_view(1, 101), 
                     is_abundant))
      {
         std::cout << i << '\n';
      }
   }

   {
      using namespace n901;

      std::vector<int> nums{ 10, 12, 14, 16, 18, 20 };
      for (int i : nums | std::views::filter(is_abundant))
      {
         std::cout << i << '\n';
      }
   }

   // transform - get halves
   {
      using namespace n901;

      for (auto i : std::views::iota(1, 101) |
                    std::views::filter(is_abundant) |
                    std::views::transform([](int const n) {return n / 2;}))
      {
         std::cout << i << '\n';
      }
   }

   // take first N
   {
      using namespace n901;

      for (auto i : 
         std::views::iota(1, 101) |
         std::views::filter(is_abundant) |
         std::views::take(5))
      {
         std::cout << i << '\n';
      }
   }

   // take last N in reverse order
   {
      using namespace n901;

      for (auto i :
         std::views::iota(1, 101) |
         std::views::reverse |
         std::views::filter(is_abundant) |
         std::views::take(5))
      {
         std::cout << i << '\n';
      }
   }

   // take last N in ascending order
#ifndef WIN32
   {
      using namespace n902;

      for (auto i :
         std::views::iota(1, 101) |
         std::views::reverse |
         std::views::filter(is_abundant) |
         std::views::take(5) |
         std::views::reverse)
      {
         std::cout << i << '\n';
      }
   }
#endif

   // all except for first N and last N
   {
      using namespace n901;

      std::cout << "trim N\n";

      for (auto i :
         std::views::iota(1, 101) |
         std::views::filter(is_abundant) |
         std::views::drop(5) |
         std::views::reverse |
         std::views::drop(5) | 
         std::views::reverse)
      {
         std::cout << i << '\n';
      }
   }

   {
      std::vector<int> v{ 1, 5, 3, 2, 4 };
      std::sort(v.begin(), v.end());
   }

   {
      std::vector<int> v{ 1, 5, 3, 2, 4 };
      std::ranges::sort(v);
   }

   {
      std::array<int, 5> a{ 1, 5, 3, 2, 4 };
      auto even = std::count_if(a.begin(), a.end(), [](int const n) {return n % 2 == 0; });
      assert(even == 2);
   }

   {
      std::array<int, 5> a{ 1, 5, 3, 2, 4 };
      auto even = std::ranges::count_if(a, [](int const n) {return n % 2 == 0; });
      assert(even == 2);
   }

   {
      std::cout << "square of even numbers (1):\n";

      std::vector<int> v{ 1, 5, 3, 2, 8, 7, 6, 4 };

      // copy only the even elements
      std::vector<int> temp;
      std::copy_if(v.begin(), v.end(), std::back_inserter(temp), [](int const n) {return n % 2 == 0; });

      // sort the sequence
      std::sort(temp.begin(), temp.end(), [](int const a, int const b) {return a > b; });

      // remove the first two
      temp.erase(temp.begin() + temp.size() - 2, temp.end());

      // transform the elements
      std::transform(temp.begin(), temp.end(), temp.begin(), [](int const n) {return n * n; });

      // print each element
      std::for_each(temp.begin(), temp.end(), [](int const n) {std::cout << n << '\n'; });
   }

   {
      std::cout << "square of even numbers (2):\n";

      std::vector<int> v{ 1, 5, 3, 2, 8, 7, 6, 4 };

      std::ranges::sort(v);
      auto r =
         v
         | std::ranges::views::filter([](int const n) {return n % 2 == 0; })
         | std::ranges::views::drop(2)
         | std::ranges::views::reverse
         | std::ranges::views::transform([](int const n) {return n * n; });

      std::ranges::for_each(r, [](int const n) {std::cout << n << '\n'; });
   }

   {
      std::cout << "square of even numbers (3):\n";

      namespace rv = std::ranges::views;

      std::vector<int> v{ 1, 5, 3, 2, 8, 7, 6, 4 };
      
      std::ranges::sort(v);
      auto r =
         rv::transform(
            rv::reverse(
               rv::drop(
                  rv::filter(v,
                             [](int const n) {return n % 2 == 0; }),
                  2)),
            [](int const n) {return n * n; });

      std::ranges::for_each(r, [](int const n) {std::cout << n << '\n'; });
   }

   {      
      namespace rv = std::ranges::views;

      std::vector<int> v{ 1, 5, 3, 2, 4, 7, 6, 8 };
      auto r1 = rv::reverse(v);
      auto r2 = v | rv::reverse;
   }

   {
      namespace rv = std::ranges::views;

      std::vector<int> v{ 1, 5, 3, 2, 4, 7, 6, 8 };

      auto r1 = rv::take(v, 2);
      auto r2 = rv::take(2)(v);
      auto r3 = v | rv::take(2);
   }

   {
      namespace rv = std::ranges::views;

      std::vector<int> v{ 1, 5, 3, 2, 4, 7, 6, 8 };

      for (auto i :
         v |
         rv::reverse |
         rv::filter([](int const n) {return n % 2 == 1; }) |
         rv::take(2))
      {
         std::cout << i << '\n';
      }
   }

   {
      namespace rv = std::ranges::views;

      std::vector<int> v{ 1, 5, 3, 2, 4, 7, 16, 8 };
      for (auto i : v |
         rv::take_while([](int const n) {return n < 10; }) |
         rv::drop_while([](int const n) {return n % 2 == 1; })
         )
      {
         std::cout << i << '\n';
      }
   }

   {
      namespace rv = std::ranges::views;

      std::vector<std::tuple<int, double, std::string>> v = 
      { 
         {1, 1.1, "one"}, 
         {2, 2.2, "two"}, 
         {3, 3.3, "three"}
      };

      std::cout << "keys:\n";
      for (auto i : v | rv::keys)
         std::cout << i << '\n';

      std::cout << "values:\n";
      for (auto i : v | rv::values)
         std::cout << i << '\n';

      std::cout << "elements:\n";
      for (auto i : v | rv::elements<2>)
         std::cout << i << '\n';
   }

   {
      namespace rv = std::ranges::views;

      std::cout << "join:\n";

      std::vector<std::string> words{ "a", "join", "example" };
      for (auto s : words | rv::join)
         std::cout << s;

      std::cout << '\n';
   }

   {
      namespace rv = std::ranges::views;

      std::cout << "join:\n";

      std::vector<std::vector<int>> v{ {1,2,3}, {4}, {5, 6} };

      for (int const i : v | rv::join)
         std::cout << i << ' ';  // prints 1 2 3 4 5 6

      // for(int const i : v | rv::join_with(0))
      //   std::cout << i << ' ';  // print 1 2 3 0 4 0 5 6

      std::cout << '\n';
   }

   {
#ifndef WIN32
      std::string text{ "this is a demo!" };
      constexpr std::string_view delim{ " " };
      for (auto const word : text | rv::split(delim))
      {
         std::cout << std::string_view(word.begin(), word.end()) << '\n';
      }
#endif
   }

   /*
   {
      std::array<int, 4> a{ 1, 2, 3, 4 };
      std::vector<double> v{ 10.0, 20.0, 30.0 };

      auto z = rv::zip(a, v) // { {1, 10.0}, {2, 20.0}, {3, 30.0} }
   }

   {
      std::array<int, 4> a{ 1, 2, 3, 4 };
      std::vector<double> v{ 10.0, 20.0, 30.0 };

      auto z = rv::zip_transform(std::multiplies<double>(), a, v) // { {1, 10.0}, {2, 20.0}, {3, 30.0} }
   }
   */

   {
      std::vector<int> v{ 8, 5, 3, 2, 4, 7, 6, 1 };
      auto r = std::views::iota(1, 10);

      std::cout << "size(v)=" << std::ranges::size(v) << '\n';
      std::cout << "size(r)=" << std::ranges::size(r) << '\n';

      std::cout << "empty(v)=" << std::ranges::empty(v) << '\n';
      std::cout << "empty(r)=" << std::ranges::empty(r) << '\n';

      std::cout << "first(v)=" << *std::ranges::begin(v) << '\n';
      std::cout << "first(r)=" << *std::ranges::begin(r) << '\n';

      std::cout << "rbegin(v)=" << *std::ranges::rbegin(v) << '\n';
      std::cout << "rbegin(r)=" << *std::ranges::rbegin(r) << '\n';

      std::cout << "data(v)=" << *std::ranges::data(v) << '\n';      
   }

   {
      for (auto i : std::ranges::iota_view(1, 10))
         std::cout << i << '\n';

      for (auto i : std::views::iota(1, 10))
         std::cout << i << '\n';
   }

   {
      constexpr std::ranges::empty_view<int> ev;
      static_assert(std::ranges::empty(ev));
      static_assert(std::ranges::size(ev) == 0);
      static_assert(std::ranges::data(ev) == nullptr);
   }

   {
      constexpr std::ranges::single_view<int> sv{42};
      static_assert(!std::ranges::empty(sv));
      static_assert(std::ranges::size(sv) == 1);
      static_assert(*std::ranges::data(sv) == 42);
   }

   {
      auto v1 = std::ranges::views::iota(1, 10);
      std::ranges::for_each(v1, [](int const n) {std::cout << n << '\n'; });

      auto v2 = std::ranges::views::iota(1) |
                std::ranges::views::take(9);
      std::ranges::for_each(v2, [](int const n) {std::cout << n << '\n'; });
   }

   {
      auto text = "19.99 7.50 49.19 20 12.34";
      auto stream = std::istringstream{ text };
      std::vector<double> prices;
      double price;
      while (stream >> price)
      {
         prices.push_back(price);
      }

      auto total = std::accumulate(prices.begin(), prices.end(), 0.0);
      std::cout << std::format("total: {}\n", total);
   }

   {
      auto text = "19.99 7.50 49.19 20 12.34";
      auto stream = std::istringstream{ text };
      std::vector<double> prices;
      for (double const price : std::ranges::istream_view<double>(stream))
      {
         prices.push_back(price);
      }
      auto total = std::accumulate(prices.begin(), prices.end(), 0.0);
      std::cout << std::format("total: {}\n", total);
   }

   {
      auto text = "19.99 7.50 49.19 20 12.34";
      auto stream = std::istringstream{ text };
      std::vector<double> prices;
      std::ranges::for_each(
         std::ranges::istream_view<double>(stream),
         [&prices](double const price) {prices.push_back(price); });
      auto total = std::accumulate(prices.begin(), prices.end(), 0.0);
      std::cout << std::format("total: {}\n", total);
   }

   {
      auto l_odd = [](int const n) {return n % 2 == 1; };

      std::vector<int> v{ 1, 1, 2, 3, 5, 8, 13 };
      std::vector<int> o;
      auto e1 = std::copy_if(v.begin(), v.end(), std::back_inserter(o), l_odd);

      int arr[] = { 1, 1, 2, 3, 5, 8, 13 };
      auto e2 = std::copy_if(std::begin(arr), std::end(arr), std::back_inserter(o), l_odd);
   }

   {
      auto l_odd = [](int const n) {return n % 2 == 1; };

      std::vector<int> v{ 1, 1, 2, 3, 5, 8, 13 };
      std::vector<int> o;
      auto e1 = std::ranges::copy_if(v, std::back_inserter(o), l_odd);

      int arr[] = { 1, 1, 2, 3, 5, 8, 13 };
      auto e2 = std::ranges::copy_if(arr, std::back_inserter(o), l_odd);

      auto r = std::ranges::views::iota(1, 10);
      auto e3 = std::ranges::copy_if(r, std::back_inserter(o), l_odd);
   }

   {
      namespace rv = std::ranges::views;

      std::vector<Item> items{
         {1, "pen", 5.49},
         {2, "ruler", 3.99},
         {3, "pensil case", 12.50}
      };

      std::vector<Item> copies;

      std::ranges::copy_if(
         items, 
         std::back_inserter(copies),
         [](Item const& i) {return i.name[0] == 'p'; });

      copies.clear();

      std::ranges::copy_if(
         items, 
         std::back_inserter(copies),
         [](std::string const& name) {return name[0] == 'p'; },
         &Item::name);

      copies.clear();

      std::vector<std::string> names;
      std::ranges::copy_if(
         items | rv::transform(&Item::name),
         std::back_inserter(names),
         [](std::string const& name) {return name[0] == 'p'; });
   }
   
   {
      using namespace n902;

      std::cout << "step(1)\n";
      for (auto i : std::views::iota(1, 10) | n902::views::step(1))
         std::cout << i << '\n';

      std::cout << "step(2)\n";
      for (auto i : std::views::iota(1, 10) | n902::views::step(2))
         std::cout << i << '\n';

      std::cout << "step(3)\n";
      for (auto i : std::views::iota(1, 10) | n902::views::step(3))
         std::cout << i << '\n';

      std::cout << "step(4)\n";
      for (auto i : std::views::iota(1, 10) | n902::views::step(4))
         std::cout << i << '\n';

      std::cout << "step(5)\n";
      for (auto i : std::views::iota(1, 10) | n902::views::step(5))
         std::cout << i << '\n';
      
      std::cout << "step(2) | take(3)\n";
      for (auto i : std::views::iota(1, 10) | 
                    n902::views::step(2) |
                    std::views::take(3))
         std::cout << i << '\n';
      
      std::cout << "step(2) | take(3)\n";
      auto r = n902::views::step(std::views::iota(1, 10), 2);
      auto t = std::ranges::take_view(r, 3);
      for (auto i : t)
         std::cout << i << '\n';
      
   }

   {
      using namespace n902;

      auto v = std::views::iota(1, 10) | n902::views::step(1);
      auto it = v.begin();
      it++; // post-increment
      ++it; // pre-increment
   }
   
   {
      using namespace n903;

      std::cout << "replicate(2)\n";
      for (auto i : std::views::iota(1, 5) | n903::views::replicate(2))
         std::cout << i << '\n';

      std::cout << "replicate(2)\n";
      for (auto i : std::views::iota(1, 5) | n903::views::replicate(2) | std::views::take(5))
         std::cout << i << '\n';
   }
}