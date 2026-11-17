CXX = g++
CXXFLAGS = -std=c++11 -O3 -Wall

all: hhl_project hhl_verifier

hhl_project: src/hhl_project.cpp
	$(CXX) $(CXXFLAGS) src/hhl_project.cpp -o hhl_project

hhl_verifier: src/hhl_numerical_verifier.cpp
	$(CXX) $(CXXFLAGS) src/hhl_numerical_verifier.cpp -o hhl_verifier

clean:
	rm -f hhl_project hhl_verifier
