all: benchmark conversions

benchmark: ../convert.cpp benchmark.cpp
	g++ -std=c++20 -O2 -I../include -ltbb -o benchmark ../convert.cpp benchmark.cpp

conversions: ../convert.cpp conversions.cpp
	g++ -std=c++20 -O2 -I../include -ltbb -o conversions ../convert.cpp conversions.cpp

clean:
	rm -f benchmark conversions
