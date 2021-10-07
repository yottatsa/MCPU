#include "ActiveModel.h"
#include "DsimModel.h"
#include "StdAfx.h"
#include "sdk/auth.h"
#include "sdk/vsm.hpp"

#ifdef _WIN32
extern "C" {
IDSIMMODEL __declspec(dllexport) *
    createdsimmodel(CHAR *device, ILICENCESERVER *ils) {
  if (ils->authorize(PRODUCT_ID, API_VER))
    return new DsimModel;
  else
    return NULL;
}
}

extern "C" VOID __declspec(dllexport) deletedsimmodel(IDSIMMODEL *model) {
  delete (DsimModel *)model;
}
#endif