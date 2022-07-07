#include <iostream>

namespace n101
{
   int max(int const a, int const b)
   {
      return a > b ? a : b;
   }

   double max(double const a, double const b)
   {
      return a > b ? a : b;
   }

   using swap_fn = void(*)(void*, int const, int const);
   using compare_fn = bool(*)(void*, int const, int const);

   int partition(void* arr, int const low, int const high,
      compare_fn fcomp, swap_fn fswap)
   {
      int i = low - 1;

      for (int j = low; j <= high - 1; j++)
      {
         if (fcomp(arr, j, high))
         {
            i++;
            fswap(arr, i, j);
         }
      }

      fswap(arr, i + 1, high);

      return i + 1;
   }

   void quicksort(void* arr, int const low, int const high,
      compare_fn fcomp, swap_fn fswap)
   {
      if (low < high)
      {
         int const pi = partition(arr, low, high, fcomp, fswap);
         quicksort(arr, low, pi - 1, fcomp, fswap);
         quicksort(arr, pi + 1, high, fcomp, fswap);
      }
   }

   void swap_int(void* arr, int const i, int const j)
   {
      int* iarr = (int*)arr;
      int t = iarr[i];
      iarr[i] = iarr[j];
      iarr[j] = t;
   }

   bool less_int(void* arr, int const i, int const j)
   {
      int* iarr = (int*)arr;
      return iarr[i] <= iarr[j];
   }

   struct int_vector
   {
      int_vector();

      size_t size() const;
      size_t capacity() const;
      bool empty() const;

      void clear();
      void resize(size_t const size);

      void push_back(int value);
      void pop_back();

      int at(size_t const index) const;
      int operator[](size_t const index) const;
   private:
      int* data_;
      size_t size_;
      size_t capacity_;
   };

   constexpr char NewLine = '\n';
   constexpr wchar_t NewLineW = L'\n';
   constexpr char8_t NewLineU8 = u8'\n';
   constexpr char16_t NewLineU16 = u'\n';
   constexpr char32_t NewLineU32 = U'\n';
}

namespace n102
{
   template <typename T>
   T max(T const a, T const b)
   {
      return a > b ? a : b;
   }

   struct foo {};

   template <typename T>
   void swap(T* a, T* b)
   {
      T t = *a;
      *a = *b;
      *b = t;
   }

   template <typename T>
   int partition(T arr[], int const low, int const high)
   {
      T pivot = arr[high];
      int i = (low - 1);

      for (int j = low; j <= high - 1; j++)
      {
         if (arr[j] < pivot)
         {
            i++;
            swap(&arr[i], &arr[j]);
         }
      }

      swap(&arr[i + 1], &arr[high]);

      return i + 1;
   }

   template <typename T>
   void quicksort(T arr[], int const low, int const high)
   {
      if (low < high)
      {
         int const pi = partition(arr, low, high);
         quicksort(arr, low, pi - 1);
         quicksort(arr, pi + 1, high);
      }
   }

   template <typename T>
   struct vector
   {
      vector();

      size_t size() const;
      size_t capacity() const;
      bool empty() const;

      void clear();
      void resize(size_t const size);

      void push_back(T value);
      void pop_back();

      T at(size_t const index) const;
      T operator[](size_t const index) const;
   private:
      T* data_;
      size_t size_;
      size_t capacity_;
   };

   template<typename T>
   constexpr T NewLine = T('\n');
}

int main()
{
   {
      using namespace n101;

      int arr[] = { 13, 1, 8, 3, 5, 2, 1 };
      int n = sizeof(arr) / sizeof(arr[0]);
      quicksort(arr, 0, n - 1, less_int, swap_int);
   }

   {
      using namespace n102;

      max(1, 2);        // OK, compares ints
      max(1.0, 2.0);    // OK, compares doubles

      //foo f1, f2;
      //max(f1, f2);      // Error, operator> not overloaded for foo

      max<int>(1, 2);
      max<double>(1.0, 2.0);
      //max<foo>(f1, f2);
   }

   {
      using namespace n102;

      int arr[] = { 13, 1, 8, 3, 5, 2, 1 };
      int n = sizeof(arr) / sizeof(arr[0]);
      quicksort(arr, 0, n - 1);
   }

   {
      using namespace n102;

      std::wstring test = L"demo";
      test += NewLine<wchar_t>;
      std::wcout << test;
   }
}