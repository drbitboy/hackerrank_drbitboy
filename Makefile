CXXFLAGS=-O0 -g

TARGETS=min_sound

all: $(TARGETS)

clean:
	$(RM) $(TARGETS:%=%.exe) $(TARGETS:%=%.o)
