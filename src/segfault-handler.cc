#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <nan.h>

#define UNW_LOCAL_ONLY
#include <libunwind.h>

void segfault_handler(int sig) {
  unw_cursor_t cursor;
  unw_context_t context;

  pid_t pid = getpid();
  // print out all the frames to stderr
  fprintf(stderr, "PID %d received a SIGSEGV\n", pid);
   // Initialize cursor to current frame for local unwinding.
  unw_getcontext(&context);
  unw_init_local(&cursor, &context);

  // Unwind frames one by one, going up the frame stack.
  while (unw_step(&cursor) > 0) {
    unw_word_t offset, pc;
    unw_get_reg(&cursor, UNW_REG_IP, &pc);
    if (pc == 0) {
      break;
    }
    fprintf(stderr, "0x%lx:", pc);

    char sym[256];
    if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0) {
      fprintf(stderr, " (%s+0x%lx)\n", sym, offset);
    } else {
      fprintf(stderr," -- error: unable to obtain symbol name for this frame\n");
    }
  }
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