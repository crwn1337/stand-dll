#include "defs.hpp"

HRESULT __stdcall D3DReflectStub(LPCVOID pSrcData, SIZE_T SrcDataSize, const IID *const pInterface, void **ppReflector) {
    static auto D3DReflect_real = (D3DReflect_typedef)(GetProcAddress(d3dcompiler_dll, "D3DReflect"));
    return D3DReflect_real(pSrcData, SrcDataSize, pInterface, ppReflector);
}
