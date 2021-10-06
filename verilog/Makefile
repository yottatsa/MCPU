VERILATOR_ROOT = $(shell verilator --getenv VERILATOR_ROOT)
VERILATOR_INCLUDE = $(VERILATOR_ROOT)/include
CXXFLAGS = -I$(VERILATOR_INCLUDE)

all: mcpu.dll

model/verilated.o model/VMCPU_0__ALLcls.o  model/VMCPU_0__ALLsup.o: MCPU_0.1a.v
	verilator --Mdir model --cc $^
	cd model; make -f VMCPU_0.mk; make -f VMCPU_0.mk verilated.o

#%.o: %.cpp
#	cd model; make -f VMCPU_0.mk ../$@

mcpu.dll: model/verilated.o model/VMCPU_0__ALLcls.o  model/VMCPU_0__ALLsup.o ActiveModel.o dllmain.o DsimModel.o mcpu.o StdAfx.o
	g++ -shared -o mcpu.dll $^

clean:
	rm -rf model mcpu.dll *.o

.PHONY: all clean
