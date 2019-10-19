CXXFLAGS=-O0 -g

TARGETS=min_sound editor editor_btc

all: $(TARGETS)

clean:
	$(RM) $(TARGETS:%=%.exe) $(TARGETS:%=%.o)
