{
  'targets': [
    {
      'target_name': 'binding-addon-native',
      'sources': [
        'src/list_process.cc',
        'src/main.cc'
      ],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
      },
      "defines": [
        "_UNICODE",
        "UNICODE"
      ],
    }
  ]
}
