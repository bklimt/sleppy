
LIBS=-lstdc++ -lasound

all: bin/sleppy

clean:
	rm obj/*.o || true
	rm bin/sleppy || true

bin/sleppy: obj/sleppy.o
	mkdir -p bin && gcc -o $@ $^ $(LIBS)

obj/sleppy.o: src/sleppy.cc
	mkdir -p obj && gcc -o $@ -c $<

