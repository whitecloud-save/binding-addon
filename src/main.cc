#include <napi.h>
#include "list_process.h"

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "listProcesses"),
              Napi::Function::New(env, ListProcesses));
  return exports;
}

NODE_API_MODULE(addon, Init)
