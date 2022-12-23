#include <nan.h>

#define __V8__
#include "backtrace.hpp"

// V8 Isolated context
static v8::Isolate *isolate = nullptr;
static FILE *errOutputFile = stderr;
static bool contextAlreadySet = false;

void segfault_handler(int sig) {

  // Retrieve process ID
  pid_t pid = getpid();

  // Start printing frames
  fprintf(errOutputFile, "=========== Caught a Segmentation Fault [pid=%d] ===========\n", pid);

  // Print native stacktraces
  fprintf(errOutputFile, "-----[ Native Stacktraces ]-----\n");
  Backtrace::PrintNative(errOutputFile);
  
  if (isolate != nullptr) {
    fprintf(errOutputFile, "\n---[ V8 JavaScript Stacktraces ]---\n");
    // Print V8 stacktraces
    Backtrace::PrintV8(isolate, errOutputFile);
  }
  fprintf(errOutputFile, "============================================================\n");
  fclose(errOutputFile);

  exit(1);
}

/**
 * Print native stacktrace
 */
void PrintNativeStacktraces(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Backtrace::PrintNative(errOutputFile);
}

/**
 * Initialize the segfault handler.
 */
void RegisterHandler(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (contextAlreadySet) {
    Nan::ThrowTypeError("Cannot register handler twice");
    return;
  }

  if (info.Length() > 0) {
    if (info[0]->IsString()) {
      auto path = info[0]->ToString(info.GetIsolate()->GetCurrentContext());
      if (!path.IsEmpty()) {
        Nan::Utf8String utfPath(info[0]);
        std::string strUtfPath(*utfPath, utfPath.length());
        errOutputFile = fopen(strUtfPath.c_str(), "w");
        if (errOutputFile == nullptr) {
          Nan::ThrowTypeError("Cannot open file");
          return;
        }
      } else {
        Nan::ThrowTypeError("Path must not be empty");
        return;
      }
    } else if (!info[0]->IsUndefined()) {
      Nan::ThrowTypeError("Expected a string");
      return;
    }
  }

  contextAlreadySet = true;
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
  #if NODE_MODULE_VERSION >=93 // Node >=16
    v8::Local<v8::Context> context = exports->GetCreationContext().FromMaybe(v8::Local<v8::Context>())
  #else
    v8::Local<v8::Context> context = exports->CreationContext();
  #endif

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