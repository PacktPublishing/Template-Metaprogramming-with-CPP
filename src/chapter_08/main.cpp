#include <iostream>
#include <string>
#include <array>
#include <cassert>
#include <vector>
#include <string>

namespace n801
{
   template <typename T, std::size_t N>
   requires(N > 0)
   class circular_buffer_iterator;

   template <typename T, std::size_t N>
   requires(N > 0)
   class circular_buffer
   {
   public:
      using value_type = T;
      using size_type = std::size_t;
      using difference_type = std::ptrdiff_t;
      using reference = value_type&;
      using const_reference = value_type const&;
      using pointer = value_type*;
      using const_pointer = value_type const*;
      using iterator = circular_buffer_iterator<T, N>;
      using const_iterator = circular_buffer_iterator<T const, N>;

   public:
      constexpr circular_buffer() = default;

      constexpr circular_buffer(value_type const (&values)[N]) :
         size_(N), tail_(N-1)
      {
         std::copy(std::begin(values), std::end(values), data_.begin());
      }

      constexpr circular_buffer(const_reference v):
         size_(N), tail_(N-1)
      {
         std::fill(data_.begin(), data_.end(), v);
      }

      constexpr size_type size() const noexcept { return size_; }
      constexpr size_type capacity() const noexcept { return N; }
      constexpr bool empty() const noexcept { return size_ == 0; }
      constexpr bool full() const noexcept { return size_ == N; }
      constexpr void clear() noexcept { size_ = 0; head_ = 0; tail_ = 0; };

      constexpr reference operator[](size_type const pos)
      {
         return data_[(head_ + pos) % N];
      }

      constexpr const_reference operator[](size_type const pos) const
      {
         return data_[(head_ + pos) % N];
      }

      constexpr reference at(size_type const pos)
      {
         if (pos < size_)
            return data_[(head_ + pos) % N];

         throw std::out_of_range("Index is out of range");
      }

      constexpr const_reference at(size_type const pos) const
      {
         if (pos < size_)
            return data_[(head_ + pos) % N];

         throw std::out_of_range("Index is out of range");
      }

      constexpr reference front()
      {
         if (size_ > 0) return data_[head_];
         throw std::logic_error("Buffer is empty");
      }

      constexpr const_reference front() const
      {
         if (size_ > 0) return data_[head_];
         throw std::logic_error("Buffer is empty");
      }

      constexpr reference back()
      {
         if (size_ > 0) return data_[tail_];
         throw std::logic_error("Buffer is empty");
      }

      constexpr const_reference back() const
      {
         if (size_ > 0) return data_[tail_];
         throw std::logic_error("Buffer is empty");
      }

      constexpr void push_back(T const& value)
      {
         if (empty())
         {
            data_[tail_] = value;
            size_++;
         }
         else if (!full())
         {
            data_[++tail_] = value;
            size_++;
         }
         else
         {
            head_ = (head_ + 1) % N;
            tail_ = (tail_ + 1) % N;
            data_[tail_] = value;
         }
      }

      constexpr void push_back(T&& value)
      {
         if (empty())
         {
            data_[tail_] = value;
            size_++;
         }
         else if (!full())
         {
            data_[++tail_] = std::move(value);
            size_++;
         }
         else
         {
            head_ = (head_ + 1) % N;
            tail_ = (tail_ + 1) % N;
            data_[tail_] = std::move(value);
         }
      }

      constexpr T pop_front()
      {
         if (empty()) throw std::logic_error("Buffer is empty");

         size_type index = head_;

         head_ = (head_ + 1) % N;
         size_--;

         return data_[index];
      }

      iterator begin()
      {
         return iterator(*this, 0);
      }

      iterator end()
      {
         return iterator(*this, size_);
      }

      const_iterator begin() const
      {
         return const_iterator(*this, 0);
      }

      const_iterator end() const
      {
         return const_iterator(*this, size_);
      }

   private:
      std::array<value_type, N> data_;
      size_type                 head_ = 0;
      size_type                 tail_ = 0;
      size_type                 size_ = 0;

      friend circular_buffer_iterator<T, N>;
   };

   template <typename T, std::size_t N>
   requires(N > 0)
   class circular_buffer_iterator
   {
   public:
      using self_type = circular_buffer_iterator<T, N>;
      using value_type = T;
      using reference = value_type&;
      using const_reference = value_type const &;
      using pointer = value_type*;
      using const_pointer = value_type const*;
      using iterator_category = std::random_access_iterator_tag;
      using size_type = std::size_t;
      using difference_type = std::ptrdiff_t;

   public:
      explicit circular_buffer_iterator(circular_buffer<T, N>& buffer, size_type const index):
         buffer_(buffer), index_(index)
      { }

      self_type& operator++()
      {         
         if(index_ >= buffer_.get().size())
            throw std::out_of_range("Iterator cannot be incremented past the end of the range");

         index_++;
         return *this;
      }

      self_type operator++(int)
      {
         self_type temp = *this;
         ++*this;
         return temp;
      }

      // input iterator requirements

      bool operator==(self_type const& other) const
      {
         return compatible(other) && index_ == other.index_;
      }

      bool operator!=(self_type const& other) const
      {
         return !(*this == other);
      }

      const_reference operator*() const
      {
         if (buffer_.get().empty() || !in_bounds())
            throw std::logic_error("Cannot dereferentiate the iterator");
         return buffer_.get().data_[(buffer_.get().head_ + index_) % buffer_.get().capacity()];
      }

      const_reference operator->() const
      {
         if (buffer_.get().empty() || !in_bounds())
            throw std::logic_error("Cannot dereferentiate the iterator");
         return buffer_.get().data_[(buffer_.get().head_ + index_) % buffer_.get().capacity()];
      }

      // output iterator requirements

      reference operator*()
      {
         if (buffer_.get().empty() || !in_bounds())
            throw std::logic_error("Cannot dereferentiate the iterator");
         return buffer_.get().data_[(buffer_.get().head_ + index_) % buffer_.get().capacity()];
      }

      reference operator->()
      {
         if (buffer_.get().empty() || !in_bounds())
            throw std::logic_error("Cannot dereferentiate the iterator");
         return buffer_.get().data_[(buffer_.get().head_ + index_) % buffer_.get().capacity()];
      }

      // forward iterator requirements

      // bidirectional iterator requirements

      self_type& operator--()
      {
         if(index_ <= 0)
            throw std::out_of_range("Iterator cannot be decremented before the beginning of the range");

         index_--;
         return *this;
      }

      self_type operator--(int)
      {
         self_type temp = *this;
         --*this;
         return temp;
      }

      // random access iterator requirements

      self_type operator+(difference_type offset) const
      {
         self_type temp = *this;
         return temp += offset;
      }

      self_type operator-(difference_type offset) const
      {
         self_type temp = *this;
         return temp -= offset;
      }

      difference_type operator-(self_type const& other) const
      {
         return index_ - other.index_;
      }

      self_type& operator +=(difference_type const offset)
      {
         difference_type next = (index_ + next) % buffer_.get().capacity();
         if (next >= buffer_.get().size())
            throw std::out_of_range("Iterator cannot be incremented past the bounds of the range");

         index_ = next;
         return *this;
      }

      self_type& operator -=(difference_type const offset)
      {
         return *this += -offset;
      }

      value_type& operator[](difference_type const offset)
      {
         return *((*this + offset));
      }

      value_type const & operator[](difference_type const offset) const
      {
         return *((*this + offset));
      }

      bool operator<(self_type const& other) const
      {
         return index_ < other.index_;
      }

      bool operator>(self_type const& other) const
      {
         return other < *this;
      }

      bool operator<=(self_type const& other) const
      {
         return !(other < *this);
      }

      bool operator>=(self_type const& other) const
      {
         return !(*this < other);
      }
   private:
      bool compatible(self_type const& other) const
      {
         return buffer_.get().data_.data() == other.buffer_.get().data_.data();
      }

      bool in_bounds() const
      {
         return
            !buffer_.get().empty() &&
            (buffer_.get().head_ + index_) % buffer_.get().capacity() <= buffer_.get().tail_;
      }

   private:
      std::reference_wrapper<circular_buffer<T, N>> buffer_;
      size_type              index_ = 0;
   };   

   static_assert(std::is_swappable_v<circular_buffer_iterator<int, 10>>);
}

namespace n802
{
   template <typename InputIt1, typename InputIt2, typename OutputIt>
   OutputIt flatzip(
      InputIt1 first1, InputIt1 last1,
      InputIt2 first2, InputIt2 last2,
      OutputIt dest)
   {
      auto it1 = first1;
      auto it2 = first2;

      while (it1 != last1 && it2 != last2)
      {
         *dest++ = *it1++;
         *dest++ = *it2++;
      }

      return dest;
   }
}

int main()
{
   {
      using namespace n801;

      {
         circular_buffer<int, 3> a({ 1, 2, 3 });
         assert(a[0] == 1);
         assert(a[1] == 2);
         assert(a[2] == 3);

         circular_buffer<int, 3> b = a;
         assert(b[0] == 1);
         assert(b[1] == 2);
         assert(b[2] == 3);

         circular_buffer<int, 3> c = std::move(b);
         assert(c[0] == 1);
         assert(c[1] == 2);
         assert(c[2] == 3);

         circular_buffer<int, 3> d(c);
         assert(d[0] == 1);
         assert(d[1] == 2);
         assert(d[2] == 3);
      }

      {
         circular_buffer<int, 1> b;

         assert(b.size() == 0);
         assert(b.capacity() == 1);
         assert(!b.full());
         assert(b.empty());

         try
         {
            b.front();

            assert(false);
         }
         catch (std::logic_error const& e)
         {
            assert(strcmp(e.what(), "Buffer is empty") == 0);
         }

         try
         {
            b.back();

            assert(false);
         }
         catch (std::logic_error const& e)
         {
            assert(strcmp(e.what(), "Buffer is empty") == 0);
         }
      }

      {
         circular_buffer<int, 3> b({ 1, 2, 3 });

         assert(b.size() == 3);
         assert(b.capacity() == 3);
         assert(b.full());
         assert(!b.empty());

         assert(b.front() == 1);
         assert(b.back() == 3);
      }

      {
         circular_buffer<int, 3> b(42);

         assert(b.size() == 3);
         assert(b.capacity() == 3);
         assert(b.full());
         assert(!b.empty());

         assert(b.front() == 42);
         assert(b.back() == 42);
      }

      {
         circular_buffer<int, 3> b({ 1, 2, 3 });

         assert(b[0] == 1);
         assert(b[1] == 2);
         assert(b[2] == 3);

         b[0] = 11;
         b[1] = 12;
         b[2] = 13;

         assert(b[0] == 11);
         assert(b[1] == 12);
         assert(b[2] == 13);
      }

      {
         circular_buffer<int, 3> const b({ 1, 2, 3 });

         assert(b[0] == 1);
         assert(b[1] == 2);
         assert(b[2] == 3);

         assert(b.at(0) == 1);
         assert(b.at(1) == 2);
         assert(b.at(2) == 3);

         try
         {
            b.at(3);

            assert(false);
         }
         catch (std::out_of_range const& e)
         {
            assert(strcmp(e.what(), "Index is out of range") == 0);
         }
      }


      {
         circular_buffer<int, 3> b({ 1, 2, 3 });

         assert(b[0] == 1);
         assert(b[1] == 2);
         assert(b[2] == 3);

         b[0] = 11;
         b[1] = 12;
         b[2] = 13;

         assert(b[0] == 11);
         assert(b[1] == 12);
         assert(b[2] == 13);
      }

      {
         circular_buffer<int, 1> b;

         b.push_back(1);
         assert(b.front() == 1);

         b.push_back(2);
         assert(b.front() == 2);
      }

      {
         circular_buffer<int, 3> b({ 1, 2, 3 });

         b.push_back(4);
         assert(b[0] == 2);
         assert(b[1] == 3);
         assert(b[2] == 4);

         b.push_back(5);
         assert(b[0] == 3);
         assert(b[1] == 4);
         assert(b[2] == 5);

         b.push_back(6);
         assert(b[0] == 4);
         assert(b[1] == 5);
         assert(b[2] == 6);

         b.push_back(7);
         assert(b[0] == 5);
         assert(b[1] == 6);
         assert(b[2] == 7);
      }

      {
         circular_buffer<int, 1> b;

         try
         {
            b.pop_front();

            assert(false);
         }
         catch (std::logic_error const& e)
         {
            assert(strcmp(e.what(), "Buffer is empty") == 0);
         }
      }

      {
         circular_buffer<int, 3> b({ 1, 2, 3 });

         assert(b.size() == 3);
         assert(b[0] == 1);
         assert(b[1] == 2);
         assert(b[2] == 3);

         b.pop_front();
         assert(b.size() == 2);
         assert(b[0] == 2);
         assert(b[1] == 3);

         b.pop_front();
         assert(b.size() == 1);
         assert(b[0] == 3);

         b.pop_front();
         assert(b.size() == 0);
      }

      {
         circular_buffer<int, 4> b({ 1, 2, 3, 4 });

         assert(b.size() == 4);
         assert(b[0] == 1);
         assert(b[1] == 2);
         assert(b[2] == 3);
         assert(b[3] == 4);

         b.push_back(5);
         assert(b.size() == 4);
         assert(b[0] == 2);
         assert(b[1] == 3);
         assert(b[2] == 4);
         assert(b[3] == 5);

         b.push_back(6);
         assert(b.size() == 4);
         assert(b[0] == 3);
         assert(b[1] == 4);
         assert(b[2] == 5);
         assert(b[3] == 6);

         b.pop_front();
         assert(b.size() == 3);
         assert(b[0] == 4);
         assert(b[1] == 5);
         assert(b[2] == 6);
      }

      {
         circular_buffer<int, 3> b;
         auto s = b.begin();
         try
         {
            *s;
            assert(false);
         }
         catch (std::logic_error const& e)
         {
            assert(strcmp(e.what(), "Cannot dereferentiate the iterator") == 0);
         }
      }

      {
         circular_buffer<int, 3> b;
         b.push_back(1);

         auto s = b.begin();
         assert(*s == 1);

         s++;
         try
         {
            *s;
            assert(false);
         }
         catch (std::logic_error const& e)
         {
            assert(strcmp(e.what(), "Cannot dereferentiate the iterator") == 0);
         }
      }

      {
         circular_buffer<int, 3> b({1, 2, 3});
         std::vector<int> v;
         for (auto it = b.begin(); it != b.end(); ++it)
         {
            v.push_back(*it);
         }
         assert(v == std::vector<int>({ 1, 2, 3 }));
      }

      {
         circular_buffer<int, 4> b;
         b.push_back(1);
         b.push_back(2);
         b.push_back(3);

         std::vector<int> v;
         for (auto it = b.begin(); it != b.end(); ++it)
         {
            v.push_back(*it);
         }
         assert(v == std::vector<int>({ 1, 2, 3 }));
      }

      {
         circular_buffer<int, 3> b({ 1, 2, 3 });
         std::vector<int> v;
         for (auto const e : b)
         {
            v.push_back(e);
         }
         assert(v == std::vector<int>({ 1, 2, 3 }));
      }

      {
         circular_buffer<int, 3> b({ 1,2,3 });
         *b.begin() = 0;

         assert(b.front() == 0);
      }

      {
         circular_buffer<int, 3> b({ 1, 2, 3 });
         b.push_back(4);

         assert(b[0] == 2);
         assert(b[1] == 3);
         assert(b[2] == 4);
      }
   }

   {
      using namespace n802;

      {
         std::vector<int> v1;
         std::vector<int> v2;
         std::vector<int> v3;

         flatzip(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
         assert(v3.empty());
      }

      {
         std::vector<int> v1 {1,2,3};
         std::vector<int> v2;
         std::vector<int> v3;

         flatzip(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v3));
         assert(v3.empty());
      }

      {
         std::vector<int> v1;
         std::vector<int> v2{ 1,2,3 };
         std::vector<int> v3;

         flatzip(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v3));
         assert(v3.empty());
      }

      {
         std::vector<int> v1 {1, 2, 3};
         std::vector<int> v2 { 4, 5 };
         std::vector<int> v3;

         flatzip(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v3));
         assert(v3 == std::vector<int>({ 1, 4, 2, 5 }));
      }

      {
         std::vector<int> v1{ 1, 2 };
         std::vector<int> v2{ 3, 4, 5 };
         std::vector<int> v3;

         flatzip(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v3));
         assert(v3 == std::vector<int>({ 1, 3, 2, 4 }));
      }

      {
         std::vector<int> v1{ 1, 2, 3 };
         std::vector<int> v2{ 4, 5, 6 };
         std::vector<int> v3;

         flatzip(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v3));
         assert(v3 == std::vector<int>({ 1, 4, 2, 5, 3, 6 }));
      }
   }

   {
      using namespace n801;
      using namespace n802;

      circular_buffer<int, 4> a({1, 2, 3, 4});
      circular_buffer<int, 3> b({5, 6, 7});
      circular_buffer<int, 8> c(0);

      flatzip(a.begin(), a.end(), b.begin(), b.end(), c.begin());

      std::vector<int> v;
      for (auto e : c)
      {
         v.push_back(e);
      }
      assert(v == std::vector<int>({ 1, 5, 2, 6, 3, 7, 0, 0 }));
   }
}