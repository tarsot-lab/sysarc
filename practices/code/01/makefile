SRC = $(wildcard *.s *.c *.cc)
EXE = $(basename $(SRC))
ATT = $(EXE:=.att)
DATA = $(EXE:=.data)
PERF = $(DATA:=.perf)

ASFLAGS = -g -nostartfiles
CFLAGS = -g -march=native -Ofast -Wall
CXXFLAGS = $(CFLAGS)

all: $(EXE) $(ATT)

clean:
	$(RM) $(ATT) $(DATA) $(EXE) $(OBJ) core.* *.data *.data.old *~

%.att: %
	objdump -C -D $< > $@

fib-: CFLAGS+=-fno-optimize-sibling-calls
fib+: CFLAGS+=-foptimize-sibling-calls

for0: CFLAGS+=-O0
for3: CFLAGS+=-O3

sum0: CFLAGS+=-O0
sum3: CFLAGS+=-O3
sums: CFLAGS+=-Os

%.data: %
	perf record --call-graph dwarf -o $@ -- ./$< || true

%.perf: %.data
	perf report -g graph --no-children -i $<

.PHONY: all clean $(PERF)
.PRECIOUS: $(DATA)

