CXXFLAGS=-O0 -g

TARGETS=min_sound editor editor_btc misc_tests

all: $(TARGETS)

clean:
	$(RM) $(TARGETS:%=%.exe) $(TARGETS:%=%.o)
