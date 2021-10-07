//#include "stdafx.h"
#include "DsimModel.h"

INT DsimModel::isdigital(CHAR *pinname) {
  return TRUE; // Indicates all the pins are digital
}

VOID DsimModel::setup(IINSTANCE *instance, IDSIMCKT *dsimckt) {

  unsigned char i;
  char s[8];
  ABSTIME time = 0;

  inst = instance;
  ckt = dsimckt;

  // verilator instance
  // Verilated::debug(0);
  // Verilated::randReset(2);
  // Verilated::traceEverOn(true);
  // Verilated::mkdir("logs");
  // Verilated::commandArgs(argc, argv);
  cpu = new VMCPU_0;

  CREATEPOPUPSTRUCT *cps = new CREATEPOPUPSTRUCT;
  cps->caption = "CPU Simulator Debugger Log"; // WIN Header
  cps->flags = PWF_VISIBLE | PWF_SIZEABLE;     // Show + Size
  cps->type = PWT_DEBUG;                       // WIN DEBUG
  cps->height = 500;
  cps->width = 400;
  cps->id = 1;

  myPopup = (IDEBUGPOPUP *)instance->createpopup(cps);

  InfoLog("Connecting control pins...");

  pin_OE = inst->getdsimpin("$OE$", true);   // Connects memory read pin
  pin_WE = inst->getdsimpin("$WE$", true);   // Connects memory write pin
  pin_RST = inst->getdsimpin("$RST$", true); // Connects reset pin
  pin_CLK = inst->getdsimpin("CLK", true);   // Connects Clock pin

  InfoLog("Connecting data pins...");
  for (i = 0; i < DATABUS; i++) { // Connects Data pins
    s[0] = 'D';
    _itoa_s(i, &s[1], 7, 10);
    pin_D[i] = inst->getdsimpin(s, true);
  }

  InfoLog("Connecting address pins...");
  for (i = 0; i < ADDRESSBUS; i++) { // Connects Address pins
    s[0] = 'A';
    _itoa_s(i, &s[1], 7, 10);
    pin_A[i] = inst->getdsimpin(s, true);
  }

  for (i = 0; i < DATABUS; i++) {
    pin_D[i]->SetFloat;
  }
  SyncIn(pin_CLK, cpu->clk);
  SyncIn(pin_RST, cpu->rst);

  SyncOut(cpu->oe, pin_OE);
  SyncOut(cpu->we, pin_WE);

  // Connects function to handle Clock steps (instead of using "simulate")
  pin_CLK->sethandler(this, (PINHANDLERFN)&DsimModel::clockstep);
}

VOID DsimModel::runctrl(RUNMODES mode) {}

VOID DsimModel::actuate(REALTIME time, ACTIVESTATE newstate) {}

BOOL DsimModel::indicate(REALTIME time, ACTIVEDATA *data) { return FALSE; }

VOID DsimModel::clockstep(ABSTIME time, DSIMMODES mode) {
  unsigned char i, a;

  if (!Verilated::gotFinish() &&
      (pin_CLK->isposedge() || pin_CLK->isnegedge())) {
    sprintf_s(LogMessage, (pin_CLK->isposedge()) ? "Pos" : "Neg");
    InfoLog(LogMessage);
  }

  if (!Verilated::gotFinish() && pin_CLK->isnegedge()) {
    SyncIn(pin_CLK, cpu->clk);
    SyncIn(pin_RST, cpu->rst);
    pin_OE->SetHigh;
    pin_WE->SetHigh;

    for (i = 0; i < DATABUS; i++) {
      pin_D[i]->SetFloat;
    }

    cpu->eval();

    if (cpu->oe == 0) {
      a = cpu->adress;
      sprintf_s(LogMessage, "Address: %02x", a);
      InfoLog(LogMessage);

      for (i = 0; i < ADDRESSBUS; i++) {
        SyncOut(a & 1, pin_A[i]);
        a = a >> 1;
      }

      if (cpu->we == 0) {
        a = cpu->data;

        for (i = 0; i < DATABUS; i++) {
          SyncOut(a & 1, pin_D[i]);
          a = a >> 1;
        }
        sprintf_s(LogMessage, "Write %02x to %02x", cpu->data, cpu->adress);
        InfoLog(LogMessage);
      }
    }

    SyncOut(cpu->oe, pin_OE);
    SyncOut(cpu->we, pin_WE);
  }

  if (!Verilated::gotFinish() && pin_CLK->isposedge()) {
    if (cpu->oe == 0 && !(cpu->we == 0)) {
      a = 0;
      for (i = 0; i < DATABUS; i++) {
        if (ishigh(pin_D[i]->istate())) {
          a = a + 1;
        } else {
        }
        a = a * 2;
      }
      cpu->data = a;

      sprintf_s(LogMessage, "Read %02x from %02x", cpu->data, cpu->adress);
      InfoLog(LogMessage);
    }
    cpu->eval();
    SyncIn(pin_CLK, cpu->clk);
    SyncIn(pin_RST, cpu->rst);
    cpu->eval();
  }
}

VOID DsimModel::simulate(ABSTIME time, DSIMMODES mode) {}

VOID DsimModel::callback(ABSTIME time, EVENTID eventid) {}

ISIBLING *DsimModel::queryinterface(UINT iid) { return nullptr; }
