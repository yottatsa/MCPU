all: mcpu

mcpu: MCPU_0.1a.v
	verilator --cc $< --Mdir $@

clean:
	rm -rf mcpu

.PHONY: all clean
