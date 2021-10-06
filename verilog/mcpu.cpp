#include "StdAfx.h"
#include "sdk/vsm.hpp"
#include "sdk/auth.h"
#include "ActiveModel.h"
#include "DsimModel.h"

#ifdef _WIN32
extern "C" {
	IDSIMMODEL __declspec (dllexport) *createdsimmodel(CHAR *device, ILICENCESERVER *ils) {
		if (ils->authorize(VSMModel_key, VSMModel_API))
			return new DsimModel;
		else
			return NULL;
	}
}

extern "C" VOID __declspec (dllexport) deletedsimmodel(IDSIMMODEL *model) {
	delete (DsimModel *)model;
}
#endif