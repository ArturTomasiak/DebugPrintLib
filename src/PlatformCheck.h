#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define windows 1
#elif defined(__linux__) || defined(__gnu_linux__)
#define linux 1
// the below define exposes syscalls
#define _GNU_SOURCE 
#endif