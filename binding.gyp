{
  "targets": [
    {
      "target_name": "SegfaultHandler",
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "sources": ["src/segfault-handler.cc"],
      "cflags": [ "-Wno-misleading-indentation" ],
      "cflags_cc!": [ "-fno-exceptions" ],
    }
  ]
}
