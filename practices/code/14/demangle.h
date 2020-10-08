//------------------------------------------------------------------------
// demangle.h - (C) Gustavo Romero López 2015 - GPLv3
//------------------------------------------------------------------------

#ifndef __DEMANGLE_H__
#define __DEMANGLE_H__

//------------------------------------------------------------------------

#include <cxxabi.h>  // abi::__cxa_demangle
#include <cstdlib>   // free
#include <memory>    // unique_ptr
#include <stdexcept> // runtime_error
#include <string>    // std::string
#include <typeinfo>  // typeid

//------------------------------------------------------------------------

#if __cplusplus < 201103L
#error C++11 compiler required.
#endif

//------------------------------------------------------------------------

namespace ec
{
	std::string demangle(const char* name)
	{
		int status = 0;

		// enable c++11 by passing the flag -std=c++11 to g++
		std::unique_ptr<char, void(*)(void*)> result 
		{
			abi::__cxa_demangle(name, nullptr, nullptr, &status), 
			std::free
		};

		switch(status)
		{
			case  0: return result.get(); break;
			case -1: throw std::runtime_error("a memory allocation failure occurred"); break;
			case -2: throw std::runtime_error("mangled name is not a valid name under the C++ ABI mangling rules"); break;
			case -3: throw std::runtime_error("invalid argument"); break;
		}

		return name;
	}

	std::string demangle(const std::string& name)
	{
		return demangle(name.c_str());
	}

	template<class _T> std::string demangle()
	{
		return demangle(typeid(_T).name());
	}

	template<class _T> std::string demangle(const _T& __t)
	{
		return demangle(typeid(__t).name());
	}

} // namespace ec

//------------------------------------------------------------------------

#endif // __DEMANGLE_H__
