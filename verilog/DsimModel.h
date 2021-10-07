#pragma once
#include "StdAfx.h"
#include "sdk/vsm.hpp"

// verilator
#include "model/VMCPU_0.h"
#include "verilated.h"

#define DATABUS 8
#define ADDRESSBUS 6

#define InfoLog(__s__)                                                         \
  sprintf_s(LogLineT, "%05d: ", LogLine++);                                    \
  myPopup->print(LogLineT);                                                    \
  myPopup->print(__s__);                                                       \
  myPopup->print("\n")

#define SyncIn(vsm_pin, v_pin) v_pin = ishigh(vsm_pin->istate());
#define SyncOut(v_pin, vsm_pin)                                                \
  if (v_pin)                                                                   \
    vsm_pin->SetHigh;                                                          \
  else                                                                         \
    vsm_pin->SetLow;

#define SetHigh setstate(time, 1, SHI)
#define SetLow setstate(time, 1, SLO)
#define SetFloat setstate(time, 1, FLT)

class DsimModel : public IDSIMMODEL {
public:
  INT isdigital(CHAR *pinname);
  VOID setup(IINSTANCE *inst, IDSIMCKT *dsim);
  VOID runctrl(RUNMODES mode);
  VOID actuate(REALTIME time, ACTIVESTATE newstate);
  BOOL indicate(REALTIME time, ACTIVEDATA *data);
  VOID clockstep(ABSTIME time, DSIMMODES mode);
  VOID reset(ABSTIME time, DSIMMODES mode);
  VOID simulate(ABSTIME time, DSIMMODES mode);
  VOID callback(ABSTIME time, EVENTID eventid);

  ISIBLING *queryinterface(UINT iid);

private:
  IINSTANCE *inst;
  IDSIMCKT *ckt;
  VMCPU_0 *cpu;

  IDSIMPIN *pin_OE, *pin_WE;
  IDSIMPIN *pin_RST;
  IDSIMPIN *pin_CLK;
  IDSIMPIN *pin_A[ADDRESSBUS];
  IDSIMPIN *pin_D[DATABUS];

  IDEBUGPOPUP *myPopup;

  // Global variables

  int LogLine = 1;
  char LogLineT[10];
  char LogMessage[256];
};
