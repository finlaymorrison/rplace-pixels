CXXFLAGS = -Ofast

prog: main.cpp parse.h
	g++ $^ $(CXXFLAGS) -o prog

clean:
	rm prog