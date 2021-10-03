FILES=main.cpp imgui/backends/imgui_impl_sdl.cpp imgui/backends/imgui_impl_opengl3.cpp imgui/imgui*.cpp

main: $(FILES)
	c++ `sdl2-config --cflags` -I imgui -I imgui/backends `sdl2-config --libs` $(FILES) -framework OpenGl -framework CoreFoundation -o main

.PHONY: run

run:
	./main
