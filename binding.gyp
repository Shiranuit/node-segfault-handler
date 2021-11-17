{
  "targets": [
    {
      "target_name": "NodeSegfaultHandler",
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "includes"
      ],
      "sources": ["src/segfault-handler.cc", "src/backtrace.cc"],
      "cflags": [ "-O0", "-funwind-tables" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "conditions": [
        [ 'OS=="linux"', {
          "libraries": [
            "<!(python3 -c 'from ctypes.util import find_library;print(find_library(\"unwind\")!=None and \"-lunwind\" or \"\")')"
          ],
          "defines": [
            "__V8__",
            "<!(python3 -c 'from ctypes.util import find_library;print(find_library(\"unwind\")!=None and \"USE_LIBUNWIND=1\" or \"USE_LIBUNWIND=0\")')"
          ]
        }],
        [ 'OS!="linux"', {
          "defines": [
            "__V8__"
          ]
        }]
      ]
    }
  ]
}
