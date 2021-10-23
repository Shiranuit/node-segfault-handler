{
  "targets": [
    {
      "target_name": "SegfaultHandler",
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
            "-lunwind"
          ],
        }]
      ]
    }
  ]
}
