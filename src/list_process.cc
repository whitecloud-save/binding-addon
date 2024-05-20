#include <napi.h>
#include <windows.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <direct.h>
#include <io.h>
#include <tlhelp32.h>
#include <processthreadsapi.h>
#include <fstream>
#include <tlhelp32.h>
#include <tchar.h>
#include <psapi.h>

using namespace Napi;

std::string wstring_TO_UTF8(const std::wstring& str) {
  int nLen = ::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, NULL, NULL, NULL);

  char* pBuf = new char[nLen + 1];
  ZeroMemory(pBuf, nLen + 1);

  ::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.length(), pBuf, nLen, NULL, NULL);

  std::string retStr(pBuf);
  delete[]pBuf;
  pBuf = NULL;

  return retStr;
}

std::string GetPathByProcessId(DWORD dwPid)
{
  HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);
  if (hProcess == NULL)
    return "";

  TCHAR processPath[MAX_PATH] = { 0 };
  GetModuleFileNameEx(hProcess, NULL, processPath, MAX_PATH);
  CloseHandle(hProcess);
  return wstring_TO_UTF8(processPath);
}


Napi::Value ListProcesses(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  HANDLE hProcessSnap;
  HANDLE hProcess;
  PROCESSENTRY32 pe32;
  DWORD dwPriorityClass;

  hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hProcessSnap == INVALID_HANDLE_VALUE)
  {
    return Napi::Array::New(info.Env(), 0);
  }

  pe32.dwSize = sizeof(PROCESSENTRY32);

  if (!Process32First(hProcessSnap, &pe32))
  {
    CloseHandle(hProcessSnap);
    return Napi::Array::New(info.Env(), 0);
  }

  Napi::Array result = Napi::Array::New(info.Env(), 0);
  int i = 0;
  do
  {
      auto path = GetPathByProcessId(pe32.th32ProcessID);
      if (path.size() > 0) {
          result[i] = Napi::String::New(info.Env(), path);
          i++;
      }

  } while (Process32Next(hProcessSnap, &pe32));

  CloseHandle(hProcessSnap);

  return result;
}
