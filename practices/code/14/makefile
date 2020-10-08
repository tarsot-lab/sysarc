SHELL = /bin/bash
.ONESHELL:

SRC = $(sort $(wildcard *.c *.cc))
EXE = $(basename $(SRC))
ATT = $(EXE:=.att)
STK = $(findstring stack,$(EXE))
LOG = $(STK:=.log)
PNG = $(STK:=.png)

CFLAGS = -fgnu-tm -ggdb -march=native -O3 -pthread -Wall -Wl,--no-as-needed
CXXFLAGS = $(CFLAGS) --std=c++11

all: $(PNG)

clean:
	$(RM) $(ATT) $(EXE) $(LOG) $(PNG) core.* perf.* *.log *~
	for d in `find -mindepth 1 -maxdepth 1 -type d`; do $(MAKE) -C $$d $@; done

stat: $(STK)
	@for i in $^; do
		perf stat ./$$i > /dev/null
	done

stk: $(STK)
	@printf "%80s\n" | tr ' ' '-'
	for i in $^; do
		./$$i
		printf "%80s\n" | tr ' ' '-'
	done

%.att:	%
	objdump -C -d -S $< > $@

%.k: %
	valgrind --branch-sim=yes --callgrind-out-file=./$*.log --cache-sim=yes --collect-bus=yes --collect-jumps=yes --demangle=yes --dump-instr=yes -q --tool=callgrind --trace-jump=yes ./$<
	kcachegrind ./$*.log

%.log: %
	./$< | tee >(sed -e 's/  */ /g' -e 's/^ //' -e 's/\.//g' > $@)

%.perf: %
	perf record --call-graph dwarf ./$<
	perf report

%.png: %.log Makefile
	columns=`head -n 1 $< | wc -w`
	for (( i=1; i<$$columns; ++i )); do
		lines="$$lines'$<' u 1:$$((i+1)) w lp lw 2 pt $$((2 * i + 2)), "
	done
	gnuplot <<< "set grid; set key top center horizontal autotitle columnheader; set output '$@'; set terminal pngcairo transparent enhanced; set xlabel 'threads'; set ylabel 'throughput'; plot $${lines%, }"

.PRECIOUS: $(LOG)
.PHONY: all clean stat stk
