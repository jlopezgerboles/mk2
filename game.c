#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <glad/glad.h>
#include <SDL2/SDL.h>

static int resolution_width = 512;
static int resolution_height = 448;
static int canvas_width = 1024;
static int canvas_height = 896;

const char *vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

int main (int argc, char **argv) {

////////////////	
// GAME SETUP //
	// GAME WINDOW
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("Mk2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, canvas_width, canvas_height, SDL_WINDOW_OPENGL);
	SDL_GLContext graphic_context = SDL_GL_CreateContext(window);
	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

	// GAME INPUT
	int game_input_x = 0;
	int game_input_y = 0;

	// OPENGL CONTEXT CREATION
	glViewport(0, 0, resolution_width, resolution_height);

	float vertex_input[] = {
		+0.5, +0.5, 0, 0, 0,
		+0.5, -0.5, 0, 0, 1,
		-0.5, -0.5, 0, 1, 1,
		-0.5, +0.5, 0, 1, 0
	};

	int indices [] = {
		0, 1, 3,
		1, 2, 3
	};

	// VERTEX BUFFER OBJECT
	int vertex_buffer_object;
	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_input), vertex_input, GL_STATIC_DRAW);
	
	// VERTEX SHADER
	int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	bool running = true;
	SDL_Event event;

///////////////
// GAME LOOP //
	while(running)
	{
		while(SDL_PollEvent(&event))
		{
			////////// INPUT LOOP //////////
			switch (event.type) {
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_LEFT:
							printf("KEY LEFT\n");
							game_input_x = -1;
							break;
						case SDLK_RIGHT:
							printf("KEY RIGHT\n");
							game_input_x = +1;
							break;
						case SDLK_UP:
							printf("KEY UP\n");
							game_input_y = +1;
							break;
						case SDLK_DOWN:
							game_input_x = -1;
							printf("KEY DOWN\n");
							break;
						case SDLK_ESCAPE:
							running = false;
							break; 
						default:
							break;
					}
				case SDL_KEYUP:
					switch(event.key.keysym.sym) {
						case SDLK_LEFT:
							printf("KEY LEFT RELEASED\n");
							if(game_input_x < 0) game_input_x = 0;
							break;
						case SDLK_RIGHT:
							printf("KEY RIGHT RELEASED\n");
							if(game_input_x > 0) game_input_x = 0;
							game_input_x = +1;
							break;
						case SDLK_UP:
							printf("KEY UP RELEASED\n");
							if(game_input_y < 0) game_input_y = 0;
							game_input_y = +1;
							break;
						case SDLK_DOWN:
							printf("KEY DOWN RELEASED\n");
							if(game_input_y > 0) game_input_y = 0;
							break;
						default:
							break;
					}
			}
			////////// END INPUT LOOP //////////

			if(event.type == SDL_QUIT) running = false;

		}
		glClearColor(0, 0, 255, 255);
		glClear(GL_COLOR_BUFFER_BIT);
		SDL_GL_SwapWindow(window);
		//SDL_RenderClear(renderer);
		//SDL_RenderPresent(renderer);
	}

	//SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
