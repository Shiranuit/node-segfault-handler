#ifndef __BACKTRACE_H__
#define __BACKTRACE_H__

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <cxxabi.h>

#define UNW_LOCAL_ONLY
#include <libunwind.h>

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