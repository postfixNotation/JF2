#ifndef CORE_HPP_
#define CORE_HPP_

#ifdef JF2_PLATFORM_WINDOWS
#ifdef JF2_BUILD_DLL
#define JF2_API __declspec(dllexport)
#else
#define JF2_API __declspec(dllimport)
#endif
#else
#error JF2 only supports Windows!
#endif

#endif // CORE_HPP_
