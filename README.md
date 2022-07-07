# Template Metaprogramming with C++
Template Metaprogramming with C++, published by Packt

# Structure

The source code for the book is organized per chapters. Each chapter has its own subfolder in the `src` folder. These subfolders are called `chapter_01`, `chapter_02`, etc. For most chapters, the code is found in a single source file, `main.cpp`. This file is organized in multiple namespaces, such as `n101`, `n102`, `n103` etc. for the first chapter, `n201`, `n202`, `n203` etc. for the second chapter and so on.

# Supported compilers

All the code provided in the book is cross-platform. You can use any of the major compilers, MSVC, Clang, or GCC to compile the sources.

You can also run snippets of code using compilers available online:

- [Compiler Explorer](https://godbolt.org/)
- [Wandbox](https://wandbox.org/)
- [C++ Insights](https://cppinsights.io/)

# How to build

You must have CMake to build the code.

## Example for creating a Visual Studio solution

Run the following commands to create a solution for Visual Studio 2019:

```
mkdir build
cd build
cmake -G "Visual Studio 16 2019" -A x64 ..
```

Run the following commands to create a solution for Visual Studio 2022:

```
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
```