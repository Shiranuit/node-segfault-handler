#ifndef __BACKTRACE_H__
#define __BACKTRACE_H__

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <cxxabi.h>

#define UNW_LOCAL_ONLY
#if USE_LIBUNWIND==1 && __has_include(<libunwind.h>)
#define NATIVE_STACKTRACE
#endif

#ifdef NATIVE_STACKTRACE
#include <libunwind.h>
#endif

#ifdef __V8__
  #include <nan.h>
#endif // __V8__

class Backtrace {
  public:
    static void PrintNative();
    
    #ifdef __V8__
      static void PrintV8(v8::Isolate *isolate);
    #endif // __V8__
};

#endif // __BACKTRACE_H__