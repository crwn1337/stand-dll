#pragma once

#include <unknwnbase.h>

static auto d3dcompiler_dll = LoadLibraryW(LR"(C:\Windows\System32\d3dcompiler_43.dll)");

typedef HRESULT(__stdcall *D3DReflect_typedef)(LPCVOID pSrcData, SIZE_T SrcDataSize, const IID *const pInterface, void **ppReflector);
extern "C" HRESULT __stdcall D3DReflectStub(LPCVOID pSrcData, SIZE_T SrcDataSize, const IID *const pInterface, void **ppReflector);
