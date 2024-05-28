build:
	g++ ./main.cpp ./src/lib/*.cpp ./src/lib/*.hpp
clear:
	rm ./a.out
	rm ./src/lib/*gch
run:
	./a.out
runz:
	clear
	./a.out
debug:
	g++ -g ./main.cpp ./src/lib/*.cpp ./src/lib/*.hpp
	gdb ./a.out