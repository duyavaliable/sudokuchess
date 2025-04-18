all:
	g++ -I src\include\SDL2 -L src\lib -o main main.cpp interface_map.cpp logic_game.cpp images.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
clean:
	del main.exe
