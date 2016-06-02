GCC="D:\Program Files\TDM_GCC_64\bin\g++"
KEYS=-Wall -std=c++11

all: program

program: "o\fourier.o" "o\lint.o" "o\merkle_hellman.o" "o\test.o"
	$(GCC) "o\fourier.o" "o\lint.o" "o\test.o" "o\merkle_hellman.o" -o program

fourier.o: "..\Fourier\fourier.cpp"
	$(GCC) $(KEYS) -c -o "o\fourier.o" "..\Fourier\fourier.cpp"

lint.o: "..\Long\lint.cpp"
	$(GCC) $(KEYS) -c -o "o\lint.o" "..\Long\lint.cpp"

merkle_hellman.o: "..\Merkle Hellman\merkle_hellman.cpp"
	$(GCC) $(KEYS) -c -o "o\merkle_hellman.o" "..\Merkle Hellman\merkle_hellman.cpp"

test.o: test.cpp
	$(GCC) $(KEYS) -c -o "o\test.o" test.cpp

clear:
	del *.o