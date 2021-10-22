#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <nan.h>

#define MAX_STACK_FRAMES 255

void segfault_handler(int sig) {
  void *stack_frames[MAX_STACK_FRAMES];
  pid_t pid = getpid();

  size_t frames_count = backtrace(stack_frames, MAX_STACK_FRAMES);

  if (frames_count == MAX_STACK_FRAMES) {
      fprintf(stderr, "Stack trace too large, truncated to %llu frames.", frames_count);
  }

  // print out all the frames to stderr
  fprintf(stderr, "PID %d received a SIGSEGV", pid);
  backtrace_symbols_fd(stack_frames, frames_count, STDERR_FILENO);
  exit(1);
}

void RegisterHandler(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  signal(SIGSEGV, segfault_handler);
}

void Segfault(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int *bad_ptr = (int*)-1; // make a bad pointer
  printf("%d\n", *bad_ptr); // causes segfault
}

void Init(v8::Local<v8::Object> exports) {
  v8::Local<v8::Context> context = exports->CreationContext();
  exports->Set(context,
               Nan::New("registerHandler").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(RegisterHandler)->GetFunction(context).ToLocalChecked()
              );

  exports->Set(context,
              Nan::New("segfault").ToLocalChecked(),
              Nan::New<v8::FunctionTemplate>(Segfault)->GetFunction(context).ToLocalChecked()
            );
}

NODE_MODULE(segfault_handler, Init)