{
  "targets": [
    {
      "target_name": "SegfaultHandler",
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "sources": ["src/segfault-handler.cc"],
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
