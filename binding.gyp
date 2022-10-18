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
            "-Bstatic <!(pkg-config --libs libunwind 2> /dev/null || echo '')"
          ],
          "defines": [
            "__V8__",
            "<!(pkg-config --libs libunwind 1>/dev/null 2>/dev/null && echo 'USE_LIBUNWIND=1' || echo 'USE_LIBUNWIND=0')"
          ]
        }],
        [ 'OS!="linux"', {
          "defines": [
            "__V8__",
            "USE_LIBUNWIND=0"
          ]
        }]
      ]
    }
  ]
}
