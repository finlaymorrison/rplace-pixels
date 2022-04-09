CXXFLAGS = -Ofast

prog.out: main.cpp load.h
	g++ main.cpp $(CXXFLAGS) -o prog.out

clean:
	rm prog