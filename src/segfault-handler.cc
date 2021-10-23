#include <nan.h>

#define __V8__
#include "backtrace.hpp"

// V8 Isolated context
static v8::Isolate *isolate = nullptr;

void segfault_handler(int sig) {

  // Retrieve process ID
  pid_t pid = getpid();

  // Start printing frames
  fprintf(stderr, "=========== Caught a Segmentation Fault [pid=%d] ===========\n", pid);

  // Print native stacktraces
  fprintf(stderr, "-----[ Native Stacktraces ]-----\n");
  Backtrace::PrintNative();
  
  if (isolate != nullptr) {
    fprintf(stderr, "\n---[ V8 JavaScript Stacktraces ]---\n");
    // Print V8 stacktraces
    Backtrace::PrintV8(isolate);
  }
  fprintf(stderr, "============================================================\n");

  exit(1);
}

/**
 * Print native stacktrace
 */
void PrintNativeStacktraces(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Backtrace::PrintNative();
}

/**
 * Initialize the segfault handler.
 */
void RegisterHandler(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  // Setup the signal handler
  signal(SIGSEGV, segfault_handler);
}

/**
 * Causes the program to segfault
 */
void Segfault(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int *bad_ptr = (int*)-1; // make a bad pointer
  printf("%d\n", *bad_ptr); // causes segfault
}

/**
 * When we initialize the module, we register the functions we want to exports
 */
void Init(v8::Local<v8::Object> exports) {
  // Create an export context
  v8::Local<v8::Context> context = exports->CreationContext();
  isolate = context->GetIsolate();
  // Register the functions
  exports->Set(context,
               Nan::New("registerHandler").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(RegisterHandler)->GetFunction(context).ToLocalChecked()
              );

  exports->Set(context,
              Nan::New("segfault").ToLocalChecked(),
              Nan::New<v8::FunctionTemplate>(Segfault)->GetFunction(context).ToLocalChecked()
            );

  exports->Set(context,
               Nan::New("printNativeStacktraces").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(PrintNativeStacktraces)->GetFunction(context).ToLocalChecked()
              );
}

NODE_MODULE(segfault_handler, Init)