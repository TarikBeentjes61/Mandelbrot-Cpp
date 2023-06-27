#include <SDL.h>
#include <iostream>
#include <windows.h>
#include <string>

float width = 1920;
float height = 1080;
float xCoordinate = -0.9930165830362011;
float yCoordinate = 0.25026840977042286;
float zoom = 3;
int maxIterations = 1000;
bool saveImage = false;

using namespace std;

//struct for saving rgb values
struct rgb {
	int red;
	int green;
	int blue;
};
//palette generation
rgb* generatePalette() {
	rgb* palette = new rgb[255];
	int redOffset = 29;
	int greenOffset = 24;
	int blueOffset = 4;
	for (int i = 0; i < 255; i++)
	{
		palette[i].red = redOffset;
		palette[i].green = greenOffset;
		palette[i].blue = blueOffset;
		if (i < 64) {
			redOffset += 3;
		}
		else if (i < 128) {
			greenOffset += 3;
		}
		else if (i < 192) {
			blueOffset += 3;
		}
	}
	return palette;
}
//generating mandelbrotset
void generateSet() {

	//iniate SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);

	//get the palette
	rgb* generatedPalette = generatePalette();
	int count = 0;

	//infinite loop
	while (1) {
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
		//zoom in
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				int iterations = 0;
				float ny = ((static_cast <float>(y) / height) * 2.f - 1.f - 0.5f) / zoom + xCoordinate;
				float nx = ((static_cast <float>(x) / width) * 2.f - 1.f) / zoom + yCoordinate;

				float zx = 0.f;
				float zy = 0.f;

				while (zx * zx + zy * zy < 4.f && iterations < maxIterations)
				{
					float temp = zx * zx - zy * zy + ny;
					zy = 2.f * zx * zy + nx;
					zx = temp;
					iterations++;
				}

				//check if its in or out
				if (iterations < maxIterations)
				{
					int index = (iterations) * (255) / (maxIterations);
					rgb colors = generatedPalette[index];
					SDL_SetRenderDrawColor(renderer, colors.red, colors.green, colors.blue, 255);
					SDL_RenderDrawPoint(renderer, x, y);
				}
				else {
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					SDL_RenderDrawPoint(renderer, x, y);
				}
			}
		}
		//save the frames to ur directory
		if (saveImage) {
			SDL_Surface* frame = SDL_GetWindowSurface(window);
			SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ABGR8888, frame->pixels, frame->pitch);
			std::string file = std::to_string(count) + ".bmp";
			SDL_SaveBMP(frame, file.c_str());
			SDL_FreeSurface(frame);
			count++;
		}
	}
	delete[] generatedPalette;
}


int main(int argc, char* argv[]) {
	generateSet();
	return 0;
}

