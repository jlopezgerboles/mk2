#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

static int resolution_width = 512;
static int resolution_height = 488;
static int canvas_width = 1024;
static int canvas_height = 896;

const char *vertex_shader_source = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"layout (location = 2) in vec2 aTexCoord;\n"
	"out vec3 ourColor;\n"
	"out vec2 TexCoord;\n"
	"void main()\n"
	"{\n"
	"gl_Position = vec4(aPos, 1.0);\n"
	"ourColor = aColor;\n"
	"TexCoord = aTexCoord;\n"
	"}";

const char *fragment_shader_source = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 ourColor;\n"
	"in vec2 TexCoord;\n"
	"uniform sampler2D ourTexture;\n"
	"void main()\n"
	"{\n"
	"FragColor = texture(ourTexture, TexCoord);\n"
	"}";

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

	// GAME AUDIO
	SDL_Init(SDL_INIT_AUDIO);
	Mix_Music *music;
	Mix_Chunk *sound;
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	music = Mix_LoadMUS("wav/low.wav");
	sound = Mix_LoadWAV("wav/sound1.wav");
	
	// FONT IMPLEMENTATION
	FT_Library font;
	FT_Init_FreeType(&font);
	FT_Face font_face;
	FT_New_Face(font, "fonts/pixel.ttf", 0, &font_fame);
	FT_Set_Pixel_Sizes(font_face, 0, 48);
	FT_Load_Char(font_face, 'X', FT_LOAD_RENDER);



	// OPENGL CONTEXT CREATION
	glViewport(0, 0, canvas_width, canvas_height);

	//	X____, Y____, Z___, R___, G___, B___, U___, V___,
	float vertex_input[] = {
		+0.5f, +1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 2.0f, 1.0f,
		+0.5f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 2.0f, 0.0f,
		-0.5f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, +1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	int indices [] = {
		0, 1, 3,
		1, 2, 3
	};

	int vertex_buffer_object, vertex_array_object, element_buffer_object;
	glGenVertexArrays(1, &vertex_array_object);
	glGenBuffers(1, &vertex_buffer_object);
	glGenBuffers(1, &element_buffer_object);
	
	glBindVertexArray(vertex_array_object);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_input), vertex_input, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
    	
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	int texture;
	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	char *data = stbi_load("bmp/m7a_path_wall2.png", &width, &height, &nrChannels, 0);
	printf("The image has a width of %d, a height of %d and %d channels.\n", width, height, nrChannels);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	
	// VERTEX SHADER
	int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	// FRAGMENT SHADER
	int fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	// SHADER PROGRAM
	int shader_program;
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	glUseProgram(shader_program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	
	bool running = true;
	SDL_Event event;

	////////// GAME LOOP //////////
	while(running) {
		//Mix_PlayMusic(music, -1);
		////////// INPUT LOOP //////////
		while(SDL_PollEvent(&event)) {
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
							Mix_PlayChannel(-1, sound, 0);
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
			if(event.type == SDL_QUIT) running = false;
		}
		////////// END OF INPUT LOOP //////////
		
		////////// RENDER LOOP //////////
		glClearColor(0, 255, 255, 255);
		glClear(GL_COLOR_BUFFER_BIT);	
		glBindTexture(GL_TEXTURE_2D, texture);
		glUseProgram(shader_program);
		glBindVertexArray(vertex_array_object);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		SDL_GL_SwapWindow(window);
		///////// END RENDER LOOP //////////

		////////// RENDER TEXT //////////
		RnderText(shader, "This is sample text!", 25.0f, 25.0f, 1.0f,vec(0.5, 0.8f, 0.2f));
		////////// END OF RENDER TEXT //////////

		////////// AUDIO LOOP //////////
		////////// END OF AUDIO LOOP //////////
	}

	// Destroy audios and sounds
	Mix_FreeMusic(music);
	music = NULL;
	Mix_FreeChunk(sound);
	sound = NULL;

	// Destroy OpenGL functionalities
	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteBuffers(1, &vertex_buffer_object);
	glDeleteProgram(shader_program);
	
	// Destroy window
	SDL_DestroyWindow(window);
	
	// Quit SDL subsystems
	Mix_Quit();
	SDL_Quit();
	return 0;
}
