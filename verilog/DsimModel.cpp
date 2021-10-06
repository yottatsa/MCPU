//#include "stdafx.h"
#include "DsimModel.h"

INT DsimModel::isdigital(CHAR *pinname) {
	return TRUE;											// Indicates all the pins are digital
}

VOID DsimModel::setup(IINSTANCE *instance, IDSIMCKT *dsimckt) {

	int n;
	char s[8];

	inst = instance;
	ckt = dsimckt;

	// verilator instance
	//Verilated::debug(0);
	//Verilated::randReset(2);
	//Verilated::traceEverOn(true);
	//Verilated::mkdir("logs");
	// Verilated::commandArgs(argc, argv);
	cpu = new VMCPU_0;

	CREATEPOPUPSTRUCT *cps = new CREATEPOPUPSTRUCT;
	cps->caption = "CPU Simulator Debugger Log";	// WIN Header
	cps->flags = PWF_VISIBLE | PWF_SIZEABLE;	// Show + Size
	cps->type = PWT_DEBUG;						// WIN DEBUG
	cps->height = 500;
	cps->width = 400;
	cps->id = 1;

	myPopup = (IDEBUGPOPUP *)instance->createpopup(cps);

	//InfoLog("Connecting control pins...");

	pin_OE = inst->getdsimpin("$OE$", true);				// Connects memory read pin
	pin_WE = inst->getdsimpin("$WE$", true);				// Connects memory write pin
	pin_RESET = inst->getdsimpin("$RESET$", true);			// Connects reset pin
	pin_CLOCK = inst->getdsimpin("CLOCK", true);			// Connects Clock pin

	//InfoLog("Connecting data pins...");
	for (n = 0; n < 8; n++) {								// Connects Data pins
		s[0] = 'D';
		_itoa_s(n, &s[1], 7, 10);
		pin_D[n] = inst->getdsimpin(s, true);
	}

	//InfoLog("Connecting address pins...");
	for (n = 0; n < 16; n++) {								// Connects Address pins
		s[0] = 'A';
		_itoa_s(n, &s[1], 7, 10);
		pin_A[n] = inst->getdsimpin(s, true);
	}

	// Connects function to handle Clock steps (instead of using "simulate")
	pin_CLOCK->sethandler(this, (PINHANDLERFN)&DsimModel::clockstep);
}

VOID DsimModel::runctrl(RUNMODES mode) {
}

VOID DsimModel::actuate(REALTIME time, ACTIVESTATE newstate) {
}

BOOL DsimModel::indicate(REALTIME time, ACTIVEDATA *data) {
	return FALSE;
}


VOID DsimModel::clockstep(ABSTIME time, DSIMMODES mode) {
	if (!Verilated::gotFinish() && (pin_CLOCK->isposedge() || pin_CLOCK->isnegedge())) {
		#define SyncIn(vsm_pin, v_pin) v_pin = ishigh(vsm_pin->istate());
		SyncIn(pin_CLOCK, cpu->clk);
		SyncIn(pin_RESET, cpu->rst);

		#define SyncOut(v_pin, vsm_pin) if (cpu->oe) vsm_pin->SetLow; else vsm_pin->SetHigh;
		SyncOut(cpu->oe, pin_OE);
		SyncOut(cpu->we, pin_WE);

		cpu->eval();
	}
}

VOID DsimModel::simulate(ABSTIME time, DSIMMODES mode) {
}

VOID DsimModel::callback(ABSTIME time, EVENTID eventid) {
}

ISIBLING* DsimModel::queryinterface(UINT iid)
{
	return nullptr;
}
