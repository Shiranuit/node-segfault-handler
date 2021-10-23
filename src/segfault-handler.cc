#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <cxxabi.h>
#include <nan.h>

#define UNW_LOCAL_ONLY
#include <libunwind.h>

void segfault_handler(int sig) {
  unw_cursor_t cursor;
  unw_context_t context;

  // Retrieve process ID
  pid_t pid = getpid();

  // Start printing frames
  fprintf(stderr, "PID %d received a SIGSEGV\n", pid);
  
  // print out all the frames to stderr
  // Initialize cursor to current frame for local unwinding.
  unw_getcontext(&context);
  unw_init_local(&cursor, &context);
  
  // Unwind frames one by one, going up the frame stack.
  size_t count = 0;
  while (unw_step(&cursor)) {
    unw_word_t offset, instruction_pointer, stack_pointer;
    
    unw_get_reg(&cursor, UNW_REG_IP, &instruction_pointer);
    unw_get_reg(&cursor, UNW_REG_SP, &stack_pointer);

    fprintf(stderr, "#%03llu [pc=0x%016lx, sp=0x%016lx] ", count, instruction_pointer, stack_pointer);

    char symbol_name[256] = {"<unknown symbol>"};
    char *name = symbol_name;
    if (unw_get_proc_name(&cursor, symbol_name, sizeof(symbol_name), &offset) == 0) {
      int status = 0;
      // If cannot demangle name, use the symbol name
      if ((name = abi::__cxa_demangle(symbol_name, NULL, NULL, &status)) == 0) { // Failed to demangle symbol_name
        name = symbol_name;
      }
    }

    fprintf(stderr, "in %s (+0x%lx)\n", name, offset);
    count += 1;
  }


  exit(1);
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

  // Register the functions
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