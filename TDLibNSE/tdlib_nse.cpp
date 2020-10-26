#include "tdlib_nse.h"

extern "C" {

#ifndef WINDOWS
__attribute__((visibility("default")))
#endif
BOOL SCAPI SciterLibraryInit(ISciterAPI *psapi, SCITER_VALUE *plibobject) {
#ifdef WINDOWS
#pragma comment(linker, "/EXPORT:" __FUNCTION__"=" __FUNCDNAME__)
#endif
	_SAPI(psapi);
	static sciter::om::hasset<tdlibnse::TDLib> tdlib_root = new tdlibnse::TDLib();
	*plibobject = sciter::value::wrap_asset(tdlib_root);
	return TRUE;
}

}