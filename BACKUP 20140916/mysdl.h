#ifndef MYSDL_H
#define MYSDL_H

#include <stdlib.h>
#include <SDL.h>
#include <iostream>
#define show_width 640
#define show_height 480
using namespace std;
class MySDL
{
public:
	MySDL(int screen_width,int screen_height,unsigned char *inbuffer);
	~MySDL();
	int InitSDL();
	int SDLshow();

private:
	int screen_w;
	int screen_h;
	SDL_Surface *screen;
	SDL_Overlay *overlay;
 	SDL_Rect rect;
	unsigned char *buffer;
};


#endif
