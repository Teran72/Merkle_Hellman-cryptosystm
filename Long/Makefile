GCC="D:\Program Files\TDM_GCC_64\bin\g++"
KEYS=-Wall -std=c++11

all: program clear

program: fourier.o lint.o test.o
	$(GCC) fourier.o lint.o test.o -o program

fourier.o: "..\Fourier\fourier.cpp"
	$(GCC) $(KEYS) -c "..\Fourier\fourier.cpp"

lint.o: lint.cpp
	$(GCC) $(KEYS) -c lint.cpp

test.o: test.cpp
	$(GCC) $(KEYS) -c test.cpp

clear:
	del *.o