build:
	g++ ./main.cpp ./src/lib/*.cpp ./src/lib/*.hpp -o szachy -lallegro_primitives -lallegro -lallegro_ttf -lallegro_font -lallegro_image -lallegro_audio -lallegro_acodec
clear:
	rm ./szachy
	rm ./src/lib/*gch
run:
	./szachy
runz:
	clear
	./szachy
debug:
	g++ -g ./main.cpp ./src/lib/*.cpp ./src/lib/*.hpp -o szachy -lallegro_primitives -lallegro -lallegro_ttf -lallegro_font -lallegro_image -lallegro_audio -lallegro_acodec
	gdb ./szachy
