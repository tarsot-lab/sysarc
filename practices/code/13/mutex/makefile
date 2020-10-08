SHELL = /bin/bash
.ONESHELL:

DIR  = $(sort $(shell find -mindepth 1 -maxdepth 1 -type d))
SRC  = $(sort $(wildcard *.cc))
EXE  = $(basename $(SRC))
ATT  = $(EXE:=.att)
DAT  = $(EXE:=.dat)
LOG  = $(EXE:=.log)
PERF = $(EXE:=.perf)
TMP  = $(EXE:=.tmp)

CXXFLAGS = -fgnu-tm -latomic -march=native -O3 -pthread --std=c++11 -Wall

default: $(ATT)
	-find -mindepth 2 -iname 'makefile' -execdir make \;

comp: $(DIR)
	@function lpad { printf "%$$(( 9 - $${#1} ))s$$1"; }; \
	function pretty_print { lpad "$$1"; shift; (( $$# > 0 )) && pretty_print "$$@" || echo; }; \
	function f { \
	printf '%81s\n' | tr ' ' '-'; \
	pretty_print "$$1". push pop sum real user system u+s kop/sec; \
	printf '%81s\n' | tr ' ' '-'; \
	for i in $^; do \
		msg=$$(make -C $$i -s test | grep " $$1 "); \
		echo "$$(lpad $$i)$${msg:9}"; \
	done 2> /dev/null; \
	printf '%81s\n' | tr ' ' '-'; \
	}; \
	f sec; \
	f par

check: $(DIR)
	@declare -A bug=(); \
	for i in $^; do \
		$(MAKE) -C $$i -s par; \
		bug[$$i]=0; \
	done; \
	for (( j=1; j<=100; ++j)); do \
		for i in $^; do \
			$$i/par || (( ++bug[$$i] )); \
		done &> /dev/null ; \
		echo -n "bugs[$$j]:"; \
		for i in $^; do \
			echo -n " $$i:$${bug[$$i]}"; \
		done; \
		echo; \
	done

clean:
	-rm -fv $(ATT) $(DAT) $(EXE) $(LOG) $(PERF) $(TMP) core.* *~
	-find -mindepth 2 -iname 'makefile' -execdir make $@ \;

test: $(filter-out visual,$(EXE))
	@function lpad { printf "%$$(( 9 - $${#1} ))s$$1"; }; \
	function pretty_print { lpad "$$1"; shift; (( $$# > 0 )) && pretty_print "$$@" || echo; }; \
	printf '%81s\n' | tr ' ' '-'; \
	pretty_print $$(lpad $$(basename $$PWD)) push pop sum real user system u+s kop/sec; \
	printf '%81s\n' | tr ' ' '-'; \
	for i in $^; do \
		make -s ./$$i.log ./$$i.tmp; \
		p1=$$(cat ./$$i.log | cut -d ' ' -f 1); \
		p2=$$(cat ./$$i.log | cut -d ' ' -f 2); \
		t=$$(bc -lq <<< "$$p1 + $$p2"); \
		r=$$(cat ./$$i.tmp | cut -d ' ' -f 1); \
		u=$$(cat ./$$i.tmp | cut -d ' ' -f 2); \
		s=$$(cat ./$$i.tmp | cut -d ' ' -f 3); \
		us=$$(bc -lq <<< "$$u + $$s"); \
		e=$$(bc -lq <<< "$$t / $$us / 1000.0"); \
		pretty_print $$i $$p1 $$p2 $$t $$r $$u $$s $$us $${e%%.*}; \
	done; \
	printf '%81s\n' | tr ' ' '-'; \

tt: $(DIR)
	@for i in $^; do \
		$(MAKE) -C $$i -s test | sed "s/   program/`printf '%10s\n' $$i`/g"; \
	done

%.att: %
	objdump -C -d $< > $@

%.dat: %
	perf record --call-graph dwarf -g -o $@ -- ./$<

%.log %.tmp: %
	$$(/usr/bin/time -f "%e %U %S" -o $*.tmp -- ./$< > $*.log 2> /dev/null) &> /dev/null
	grep -v '[[:alpha:]]' $*.log > $*.log2 && mv $*.log2 $*.log
	grep -v '[[:alpha:]]' $*.tmp > $*.tmp2 && mv $*.tmp2 $*.tmp
	if [ ! -s $*.log ]; then
		echo '0 0' > $*.log
	fi
	if [ ! -s $*.tmp ]; then
		echo '0 0 0' > $*.tmp
	fi

%.perf: %.dat
	perf report -g -i $<

.PRECIOUS: $(DAT) $(EXE)
.PHONY: all check clean comp default test $(PERF)

#	function pretty_print \
#	{ \
#		for j in "$$@"; do \
#			if [ "$$j" == "$$1" ]; then \
#				lpad "$$j" 10; \
#			else \
#				lpad "$$j"  8; \
#			fi; \
#		done; \
#		echo; \
#	}; \

