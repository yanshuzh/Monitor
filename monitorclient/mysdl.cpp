#include "mysdl.h"

MySDL::MySDL(int screen_width,int screen_height,unsigned char*inbuffer)
{
	this->screen_w = screen_width;
	this->screen_h = screen_height;
	this->buffer = inbuffer;
}
MySDL::~MySDL()
{
	SDL_Quit();
}

int MySDL::InitSDL()
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER))
	{
		cout<<"Could not initialize SDL"<<SDL_GetError()<<endl;
		return -1;
	}
//	screen = SDL_SetVideoMode(screen_w, screen_h, 0,0);
	screen = SDL_SetVideoMode(show_width, show_height, 0,0);
	if(!screen) 
	{    
        	cout<<"SDL: could not set video mode"<<SDL_GetError()<<endl;    
        	return -1;  
    	}  
	overlay=SDL_CreateYUVOverlay(screen_w, screen_h,SDL_YV12_OVERLAY, screen);
	return 0;
}

int MySDL::SDLshow()
{
	SDL_LockYUVOverlay(overlay);
        memcpy(overlay->pixels[0], buffer, screen_w*screen_h);
        memcpy(overlay->pixels[2], buffer+screen_w*screen_h, screen_w*screen_h/4);
        memcpy(overlay->pixels[1], buffer+screen_w*screen_h*5/4, screen_w*screen_h/4);
	SDL_UnlockYUVOverlay(overlay); 
	rect.x = 0;      
        rect.y = 0;      
 //       rect.w = screen_w;      
 //       rect.h = screen_h;  
       rect.w = show_width; ///用于把352*288的原始图像显示成其他大小的 比如640*480     
       rect.h = show_height;
//测试自己填充数据----------------  
        SDL_DisplayYUVOverlay(overlay, &rect);   
//延时40ms  
        SDL_Delay(40);   
		
}

