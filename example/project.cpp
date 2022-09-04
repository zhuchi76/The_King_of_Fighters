/* 2020.06.16  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL.h> // Using SDL
#include "SDL2_gfxPrimitives.h" // Using SDL2_gfx
#include "SDL_image.h"  // Using SDL2 image extension library 
#include <math.h>
//#include "SDL_ttf.h" // Using truetype font
#include "SDL_mixer.h"

//Screen dimension constants
const int WIDTH = 800;
const int HEIGHT = 600;

// Button constants
const int BUTTON_X = 0 ;
const int BUTTON_Y = 0 ;
const int BUTTON_WIDTH = WIDTH ;
const int BUTTON_HEIGHT = HEIGHT ;

const int restartwidth = 50;
const int restartheight = 50;

const int HITDISTANCE = 100;

const int SOLID = 100;
const int SHADED = 101;
const int BLENDED = 102;

enum MouseState
{
	NONE = 0,
	OUT = 1, // Mouse out
	IN_LB_SC = 2,  // Inside, Left Button, Single Click
	IN_RB_SC = 3,  // Inside, RIGHT Button, Single Click
	IN_LB_DC = 4,  // Inside, Left Button, Double Click
	IN_RB_DC = 5,  // Inside, RIGHT Button, Double Click
	IN_LB_PR = 6,  // Inside, Left Button, Press
	IN_RB_PR = 7,  // Inside, Left Button, Press
	IN_WU = 8,  // Inside, Wheel UP
	IN_WD = 9,  // Inside, Wheel DOWN
	HOVER = 10 // Mouse hover
};


//F1FRAME
const int RESTARTFRAME = 3;
const int TAPFRAME = 14;
const  int F1HPFRAME = 22;
const  int F1BUFFFRAME = 6;
const  int F1STANDINGRIGHTFRAME = 2;
const  int F1STANDINGLEFTFRAME = 2;
const  int F1JUMPINGLEFTFRAME = 6;
const  int F1JUMPINGRIGHTFRAME = 6;
const  int F1FRONTPUNCHHITLEFTFRAME = 3;
const  int F1FRONTPUNCHHITRIGHTFRAME = 3;
const  int F1WALKINGRIGHTFRAME = 2;
const  int F1WALKINGLEFTFRAME = 2;
const  int F1BEINGHITRIGHTFRAME = 1;
const  int F1BEINGHITLEFTFRAME = 1;
const  int F1BEINGPUNCHAWAYRIGHTFRAME = 4;
const  int F1BEINGPUNCHAWAYLEFTFRAME = 4;
const  int F1DEFENDRIGHTFRAME = 2;
const  int F1DEFENDLEFTFRAME = 2;
const  int F1FARPUNCHHITRIGHTFRAME = 10;
const  int F1FARPUNCHHITLEFTFRAME = 10;
const  int F1FUCKINGKILLEXRIGHTFRAME = 10;
const  int F1FUCKINGKILLEXLEFTFRAME = 10;
const  int F1FUCKINGKILLWEAPONFRAME = 2;
//const  int F1WINFRAME = 1;



//F2FRAME
const  int F2HPFRAME = 22;
const  int F2BUFFFRAME = 6;
const  int F2STANDINGRIGHTFRAME = 2;
const  int F2STANDINGLEFTFRAME = 2;
const  int F2JUMPINGLEFTFRAME = 8;
const  int F2JUMPINGRIGHTFRAME = 8;
const  int F2FRONTPUNCHHITLEFTFRAME = 7;
const  int F2FRONTPUNCHHITRIGHTFRAME = 7;
const  int F2WALKINGRIGHTFRAME = 6;
const  int F2WALKINGLEFTFRAME = 6;
const  int F2BEINGHITRIGHTFRAME = 1;
const  int F2BEINGHITLEFTFRAME = 1;
const  int F2BEINGPUNCHAWAYRIGHTFRAME = 7;
const  int F2BEINGPUNCHAWAYLEFTFRAME = 7;
const  int F2DEFENDRIGHTFRAME = 3;
const  int F2DEFENDLEFTFRAME = 3;
const  int F2FARPUNCHHITRIGHTFRAME = 12;
const  int F2FARPUNCHHITLEFTFRAME = 12;
const  int F2FUCKINGKILLEXRIGHTFRAME = 4;
const  int F2FUCKINGKILLEXLEFTFRAME = 4;
const  int F2FUCKINGKILLWEAPONFRAME = 2;
//const  int F2WINFRAME = 1;


static const int VELOCITY = 10 / 2;
static const int F1FARPUNCHHITWEAPONVELOCITY = 10;
static const int F2FARPUNCHHITWEAPONVELOCITY = 10;


//Initialize the offsets
int f1PosX = 0;

//int f2PosX = WIDTH - 80;
int f2PosX = WIDTH - 150;
int f1farpunchhitweaponPosX;
int f2farpunchhitweaponPosX;
int f1fuckingkillweaponPosX;
int f2fuckingkillweaponPosX;


 
int restartPosX = 400;
int restartPosY = 50;



int f1PosY = 400;
int f2PosY = 400;
int f1orY = 400;
int f2orY = 400;
int f1farpunchhitweaponPosY = f1orY + 50;
int f2farpunchhitweaponPosY = f1orY + 50;
int f1fuckingkillweaponPosY = f1orY + 50;
int f2fuckingkillweaponPosY = f1orY + 50;

int f1dirY = -1; //-1上1下
int f2dirY = -1;
int f1farpunchhitweapondir; //-1 left 1 right
int f2farpunchhitweapondir; //-1 left 1 right
int f1fuckingkillweapondir; //-1 left 1 right
int f2fuckingkillweapondir; //-1 left 1 right

//Initialize the velocity
int f1VelX = 0;
int f1VelY = 0;
int f2VelX = 0;
int f2VelY = 0;
//Initialize the health point
//int f1HP = 500;
//int f2HP = 500;

//是否滑鼠點擊
bool menu_initial = true;
bool menu_finish = false;
bool clickmenu = false;
bool hoverrestart_initial = true;
bool hoverrestart_finish = false;

//fucking_operating f1或f2正在執行大招動畫 正在執行誰都不可按按鍵
bool fucking_operating = false;

//f1 沒死嗎
bool f1HPempty = false;
//f1 魔沒有滿
bool f1BUFFfull = false;
//f1finishf2 f1贏f2
bool f1finishf2 = false;
//jumpingleft 跳到左邊牆壁上
bool f1jumping_left = false;
//jumpingright 跳到右邊牆邊上
bool f1jumping_right = false;
//fronthitoperating 正在進行近攻動畫
bool f1front_hit_operating = false;
//walkingoperating 正在進行走路動畫
bool f1walking_operating = false;
//f1beinghitoperating 正在進行被攻擊動畫
bool f1being_hit_operating = false;
//f1defendoperating 正在進行防禦動畫
bool f1defend_operating = false;
//f1farpunchhitoperating 正在進行遠攻動畫
bool f1far_punch_hit_operating = false;
//f1farpunchhitweaponoperating 正在進行發射遠攻武器動畫
bool f1far_punch_hit_weapon_operating = false;
//f1fucking_kill_triller_operating 正在進行大招前置動畫
bool f1fucking_kill_triller_operating = false;


//standingright 往右看
bool f1standing_right = false;
//standingleft 往左看
bool f1standing_left = false;
//initial 初始狀態
bool initial = true;
//frontpunchhit 要近攻嗎
bool f1front_punch_hit = false;
//f1beinghit 有被打嗎
bool f1being_hit = false;
//f1being_hit_start_counting_time 被攻擊剛開始的時間
bool f1being_hit_start_counting_time = false;
//f1being_hit_different_way 被換不同進攻方式 
bool f1being_hit_different_way = false;
//f1defend_要防禦嗎
bool f1defend_ = false;
//f1defend_effective 防禦有用嗎
bool f1defend_effective = false;
//f1front_punch_hit_count A近攻B，A的攻擊動畫一次，只會扣一次B的血量
bool f1front_punch_hit_count = false;
//f1being_punch_hit_count 計算被打幾次
int f1being_punch_hit_count = 0;
//f1being_punch_away 要被擊飛嗎
bool f1being_punch_away = false;
//jump 要跳嗎
bool f1jump = false;
//walking 要走嗎
bool f1walking = false;
//f1farpunchhit 要遠攻嗎
bool f1far_punch_hit = false;
//f1farpunchhitweapon 要發射遠攻武器嗎
bool f1far_punch_hit_weapon = false;
//f1far_punch_hit_weapon_effective 遠攻會不會傷害對方
bool f1far_punch_hit_weapon_effective = false;
//f1fucking_kill_on 可不可以啟用大招
bool f1fucking_kill_on = false;
//f1fucking_kill_triller 啟用大招前置嗎
bool f1fucking_kill_triller = false;
//f1fucking_kill_ex 啟用大招施行嗎
bool f1fucking_kill_ex = false;
//f1fucking_kill_weapon 啟用大招武器施行嗎
bool f1fucking_kill_weapon = false;

//pressedwalking 按下左右或AD鍵
bool f1pressed_walking = false;
//presseddefend 按下S或下鍵
bool f1pressed_defend = false;



//f2 沒死嗎
bool f2HPempty = false;
//f2 魔沒有滿
bool f2BUFFfull = false;
//f2finishf1 f2贏f1
bool f2finishf1 = false;
//jumpingleft 跳到左邊牆壁上
bool f2jumping_left = false;
//jumpingright 跳到右邊牆邊上
bool f2jumping_right = false;
//fronthitoperating 正在進行近攻動畫
bool f2front_hit_operating = false;
//walkingoperating 正在進行走路動畫
bool f2walking_operating = false;
//f2behitoperating 正在進行被攻擊動畫
bool f2being_hit_operating = false;
//f2defendoperating 正在進行防禦動畫
bool f2defend_operating = false;
//f1farpunchhitoperating 正在進行遠攻動畫
bool f2far_punch_hit_operating = false;
//f1farpunchhitweaponoperating 正在進行發射遠攻武器動畫
bool f2far_punch_hit_weapon_operating = false;
//f2fucking_kill_triller_operating 正在進行大招前置動畫
bool f2fucking_kill_triller_operating = false;

//standingright 往右看
bool f2standing_right = false;
//standingleft 往左看
bool f2standing_left = false;
//initial 初始狀態
//bool initial = true;
//frontpunchhit 要打嗎
bool f2front_punch_hit = false;
//f2beinghit 有被打嗎
bool f2being_hit = false;
//f2being_hit_start_counting_time 被攻擊剛開始的時間
bool f2being_hit_start_counting_time = false;
//f2being_hit_different_way 被換不同進攻方式 
bool f2being_hit_different_way = false;
//f2defend_要防禦嗎
bool f2defend_ = false;
//f2defend_effective 防禦有用嗎
bool f2defend_effective = false;
//f2front_punch_hit_count A打B，A的攻擊動畫一次，只會扣一次B的血量
bool f2front_punch_hit_count = false;
//f2being_punch_hit_count 計算被打幾次
int f2being_punch_hit_count = 0;
//f2being_punch_away 要被擊飛嗎
bool f2being_punch_away = false;
//jump 要跳嗎
bool f2jump = false;
//walking 要走嗎
bool f2walking = false;
//f2farpunchhit 要遠攻嗎
bool f2far_punch_hit = false;
//f2farpunchhitweapon 要遠攻嗎
bool f2far_punch_hit_weapon = false;
//f2far_punch_hit_weapon_effective 遠攻會不會傷害對方
bool f2far_punch_hit_weapon_effective = false;
//f2fucking_kill_on 可不可以啟用大招
bool f2fucking_kill_on = false;
//f2fucking_kill_triller 啟用大招前置嗎
bool f2fucking_kill_triller = false;
//f2fucking_kill_ex 啟用大招施行嗎
bool f2fucking_kill_ex = false;
//f2fucking_kill_weapon 啟用大招武器施行嗎
bool f2fucking_kill_weapon = false;

//pressedwalking 按下左右或AD鍵
bool f2pressed_walking = false;
//presseddefend 按下S或下鍵
bool f2pressed_defend = false;

/*struct TextData
{
	char path[100];
	SDL_Texture *texture;
	int width;
	int height;
};*/

struct ImageData
{

	char path[100];
	SDL_Texture *texture;
	int width;
	int height;
};


int initSDL(); // Starts up SDL and creates window
void closeSDL(); // Frees media and shuts down SDL
ImageData loadTexture(char *path, bool ckEnable, Uint8 r, Uint8 g, Uint8 b);
//TextData loadTextTexture(const char *str, const char *fontPath, int fontSize, Uint8 fr, Uint8 fg, Uint8 fb, int textType, Uint8 br, Uint8 bg, Uint8 bb);
//void textRender(SDL_Renderer *renderer, TextData text, int posX, int posY, int cx, int cy, double angle, SDL_RendererFlip flip);
void imgRender(SDL_Renderer *renderer, ImageData img, int posX, int posY);
bool loadAudio();

SDL_Window *window = NULL; // The window we'll be rendering to
SDL_Renderer *renderer = NULL; // The window renderer

// Current displayed texture
SDL_Texture *bkTexture = NULL;
SDL_Texture *pikaTexture = NULL;
SDL_Texture *spTexture = NULL;

// The SDL_mixer data type for music is Mix_Music
// https://www.libsdl.org/projects/SDL_mixer/docs/SDL_mixer_86.html
// The music that will be played

// backgroundmusic
Mix_Music *backgroundmusic = NULL;

// f1 effect
Mix_Chunk *f1beinghiteffect = NULL;
Mix_Chunk *f1farpunchhiteffect = NULL;
Mix_Chunk *f1frontpunchhiteffect = NULL;
Mix_Chunk *f1fuckingkilltrillereffect = NULL;
Mix_Chunk *f1fuckingkillexeffect = NULL;
Mix_Chunk *f1fuckingkillweaponeffect = NULL;
Mix_Chunk *f1jumpeffect = NULL;
Mix_Chunk *f1walkingeffect = NULL;
Mix_Chunk *f1defeateffect = NULL;
Mix_Chunk *f1deadeffect = NULL;

// f2 effect
Mix_Chunk *f2beinghiteffect = NULL;
Mix_Chunk *f2farpunchhiteffect = NULL;
Mix_Chunk *f2frontpunchhiteffect = NULL;
Mix_Chunk *f2fuckingkilltrillereffect = NULL;
Mix_Chunk *f2fuckingkillexeffect = NULL;
Mix_Chunk *f2fuckingkillweaponeffect = NULL;
Mix_Chunk *f2jumpeffect = NULL;
Mix_Chunk *f2walkingeffect = NULL;
Mix_Chunk *f2defeateffect = NULL;
Mix_Chunk *f2deadeffect = NULL;

int initSDL()
{
	// Initialize SDL	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0)
	{
		// Error Handling		
		printf("SDL_Init failed: %s\n", SDL_GetError());
		return 1;
	}

	// Create window	
	// SDL_WINDOWPOS_UNDEFINED: Used to indicate that you don't care what the window position is.
	window = SDL_CreateWindow("OOP SDL Tutorial", 50, 50, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 2;
	}
	// Initialize SDL_ttf	
	/*if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return 4;
	}*/

	// Initialize PNG loading	
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image failed: %s\n", IMG_GetError());
		return 3;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return 5;
	}

	// Create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		SDL_DestroyWindow(window);
		printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 5;
	}

	return 0;
}


void closeSDL()
{
	// Free the sound effects	

	Mix_FreeChunk(f1beinghiteffect);
	Mix_FreeChunk(f1farpunchhiteffect);
	Mix_FreeChunk(f1frontpunchhiteffect);
	Mix_FreeChunk(f1fuckingkilltrillereffect);
	Mix_FreeChunk(f1fuckingkillexeffect);
	Mix_FreeChunk(f1fuckingkillweaponeffect);
	Mix_FreeChunk(f1jumpeffect);
	Mix_FreeChunk(f1walkingeffect);
	Mix_FreeChunk(f1defeateffect);
	Mix_FreeChunk(f1deadeffect);

	Mix_FreeChunk(f2beinghiteffect);
	Mix_FreeChunk(f2farpunchhiteffect);
	Mix_FreeChunk(f2frontpunchhiteffect);
	Mix_FreeChunk(f2fuckingkilltrillereffect);
	Mix_FreeChunk(f2fuckingkillexeffect);
	Mix_FreeChunk(f2fuckingkillweaponeffect);
	Mix_FreeChunk(f2jumpeffect);
	Mix_FreeChunk(f2walkingeffect);
	Mix_FreeChunk(f2defeateffect);
	Mix_FreeChunk(f2deadeffect);

	// Free the music
	Mix_FreeMusic(backgroundmusic);
	// Quit Mixer subsystem
	Mix_Quit();
	// Free loaded image
	SDL_DestroyTexture(bkTexture);
	SDL_DestroyTexture(pikaTexture);


	// Destroy renderer	
	// Destroy window	
	// Quit Image subsystem
	// Quit SDL subsystems
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	// Shutdown and cleanup the truetype font API.
	//TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

/*
TextData loadTextTexture(const char *str, const char *fontPath, int fontSize, Uint8 fr, Uint8 fg, Uint8 fb, int textType, Uint8 br, Uint8 bg, Uint8 bb)
{
	TextData text;

	// TTF Font sturct
	TTF_Font *ttfFont = NULL;

	// Open the font
	ttfFont = TTF_OpenFont(fontPath, fontSize);
	if (ttfFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
	}

	// A structure that represents a color.
	SDL_Color textFgColor = { fr, fg, fb }, textBgColor = { br, bg, bb };

	// Render text surface

	SDL_Surface *textSurface = NULL;

	// Creates a solid/shaded/blended color surface from the font, text, and color given.
	if (textType == SOLID)
	{
		// Quick and Dirty
		textSurface = TTF_RenderText_Solid(ttfFont, str, textFgColor);
	}
	else if (textType == SHADED)
	{
		// Slow and Nice, but with a Solid Box
		textSurface = TTF_RenderText_Shaded(ttfFont, str, textFgColor, textBgColor);
	}
	else if (textType == BLENDED)
	{
		// Slow Slow Slow, but Ultra Nice over another image
		textSurface = TTF_RenderText_Blended(ttfFont, str, textFgColor);
	}

	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		// Create texture from surface pixels
		text.texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (text.texture == NULL)
		{
			printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
		}

		//Get text dimensions and information
		text.width = textSurface->w;
		text.height = textSurface->h;

		// Get rid of old loaded surface
		SDL_FreeSurface(textSurface);
	}

	// Free font
	TTF_CloseFont(ttfFont);

	//return font data
	return text;
}

void textRender(SDL_Renderer *renderer, TextData text, int posX, int posY, int cx, int cy, double angle, SDL_RendererFlip flip)
{
	SDL_Rect r;
	r.x = posX;
	r.y = posY;
	r.w = text.width;
	r.h = text.height;

	SDL_Point center = { cx, cy };

	SDL_RenderCopyEx(renderer, text.texture, NULL, &r, angle, &center, flip);
}

bool setTextureAlpha(SDL_Texture *texture, Uint8 alpha)
{
	// Set and enable standard alpha blending mode for a texture
	if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) == -1)
	{
		printf("SDL_SetTextureBlendMode failed: %s\n", SDL_GetError());
		return -1;
	}

	// Modulate texture alpha
	if (SDL_SetTextureAlphaMod(texture, alpha) == -1)
	{
		printf("SDL_SetTextureAlphaMod failed: %s\n", SDL_GetError());
		return -1;
	}
}*/
//SDL_Texture *loadTexture(char *path, int *width, int *height, bool ckEnable)




bool loadAudio()
{
	// Load music
	// https://www.libsdl.org/projects/SDL_mixer/docs/SDL_mixer_55.html
	// This can load WAVE, MOD, MIDI, OGG, MP3, FLAC, and any file that you use a command to play with.
	backgroundmusic = Mix_LoadMUS("../audio/background/Street Fighter BGM.wav");
	if (backgroundmusic == NULL)
	{
		printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	// Load sound effects
	// https://www.libsdl.org/projects/SDL_mixer/docs/SDL_mixer_19.html
	// This can load WAVE, AIFF, RIFF, OGG, and VOC files.

	f1beinghiteffect = Mix_LoadWAV("../audio/f1/beinghit/sp_Dmg1.wav");
	f1farpunchhiteffect = Mix_LoadWAV("../audio/f1/farpunch/hit/hlk_Dmg1.wav");
	f1frontpunchhiteffect = Mix_LoadWAV("../audio/f1/frontpunch/hit/hlk_Dmg2.wav");
	f1fuckingkilltrillereffect = Mix_LoadWAV("../audio/f1/fuckingkill/hlk_Who'sNext.wav");
	f1fuckingkillexeffect = Mix_LoadWAV("../audio/f1/fuckingkill/hlk_PowerSlam.wav");
	f1fuckingkillweaponeffect = Mix_LoadWAV("../audio/f1/fuckingkill/hulk_clash.wav");
	f1jumpeffect = Mix_LoadWAV("../audio/f1/jump/hlk_Atk2.wav");
	f1walkingeffect = Mix_LoadWAV("../audio/f1/walking/step.wav");
	f1defeateffect = Mix_LoadWAV("../audio/f1/defeat/iro_SmartBomb.wav");
	f1deadeffect = Mix_LoadWAV("../audio/f1/dead/hlk_Dead2.wav");
	if (f1beinghiteffect == NULL || f1farpunchhiteffect == NULL || f1frontpunchhiteffect == NULL || f1fuckingkilltrillereffect == NULL || f1defeateffect == NULL)
	{
		printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}
	if (f1fuckingkillexeffect == NULL || f1fuckingkillweaponeffect == NULL || f1jumpeffect == NULL || f1walkingeffect == NULL || f1deadeffect == NULL)
	{
		printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	f2beinghiteffect = Mix_LoadWAV("../audio/f2/beinghit/cap_dmg1.wav");
	f2farpunchhiteffect = Mix_LoadWAV("../audio/f2/farpunch/hit/cap_atk2.wav");
	f2frontpunchhiteffect = Mix_LoadWAV("../audio/f2/frontpunch/hit/cap_atk1.wav");
	f2fuckingkilltrillereffect = Mix_LoadWAV("../audio/f2/fuckingkill/cap_GoodWork.wav");
	f2fuckingkillexeffect = Mix_LoadWAV("../audio/f2/fuckingkill/cap_Freedom.wav");
	f2fuckingkillweaponeffect = Mix_LoadWAV("../audio/f2/fuckingkill/firebomb_expl1.wav");
	f2jumpeffect = Mix_LoadWAV("../audio/f2/jump/cap_atk3.wav");
	f2walkingeffect = Mix_LoadWAV("../audio/f2/walking/step.wav");
	f2defeateffect = Mix_LoadWAV("../audio/f2/defeat/cap_Victory.wav");
	f2deadeffect = Mix_LoadWAV("../audio/f2/dead/cap_dead.wav");
	if (f2beinghiteffect == NULL || f2farpunchhiteffect == NULL || f2frontpunchhiteffect == NULL || f2fuckingkilltrillereffect == NULL || f1defeateffect == NULL)
	{
		printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}
	if (f2fuckingkillexeffect == NULL || f2fuckingkillweaponeffect == NULL || f2jumpeffect == NULL || f2walkingeffect == NULL || f1deadeffect == NULL)
	{
		printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}
}


ImageData loadTexture(char *path, bool ckEnable, Uint8 r, Uint8 g, Uint8 b)
{
	ImageData img;


	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL)
	{
		printf("IMG_Load failed: %s\n", IMG_GetError());
	}
	else
	{
		// Set the color key (transparent pixel) in a surface.
		// https://wiki.libsdl.org/SDL_SetColorKey
		// The color key defines a pixel value that will be treated as transparent in a blit. 
		// It is a pixel of the format used by the surface, as generated by SDL_MapRGB().
		// Use SDL_MapRGB() to map an RGB triple to an opaque pixel value for a given pixel format.
		// https://wiki.libsdl.org/SDL_MapRGB
		SDL_SetColorKey(loadedSurface, ckEnable, SDL_MapRGB(loadedSurface->format, r, g, b));

		// Create texture from surface pixels
		img.texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (img.texture == NULL)
		{
			printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
		}

		//Get image dimensions
		img.width = loadedSurface->w;
		img.height = loadedSurface->h;

		// Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//return newTexture;
	return img;
}


void imgRender(SDL_Renderer *renderer, ImageData img, int posX, int posY)
{
	SDL_Rect r;
	r.x = posX;
	r.y = posY;
	r.w = img.width;
	r.h = img.height;
	SDL_RenderCopy(renderer, img.texture, NULL, &r);
}




void mouseHandleEvent(SDL_Event* e, MouseState* mouseState, int* x, int* y)
{
	// If mouse event happened
	// https://wiki.libsdl.org/SDL_Event
	// https://wiki.libsdl.org/SDL_MouseMotionEvent
	// https://wiki.libsdl.org/SDL_MouseButtonEvent
	// https://wiki.libsdl.org/SDL_MouseWheelEvent
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP || e->type == SDL_MOUSEWHEEL)
	{
		// Get mouse position
		// https://wiki.libsdl.org/SDL_GetMouseState		
		SDL_GetMouseState(x, y);
		//printf("(%d, %d)\n", *x, *y);

		// Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (*x < BUTTON_X)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (*x > BUTTON_X + BUTTON_WIDTH)
		{
			inside = false;
		}
		//Mouse above the button
		else if (*y < BUTTON_Y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (*y > BUTTON_Y + BUTTON_HEIGHT)
		{
			inside = false;
		}


		// Mouse is outside button
		if (!inside)
		{
			*mouseState = OUT;
		}
		//Mouse is inside button
		else
		{
			static int lasttime = SDL_GetTicks();
			static int curtime = SDL_GetTicks();
			int timediv;

			lasttime = curtime;
			curtime = SDL_GetTicks();
			timediv = curtime - lasttime;

			switch (e->type)
			{
			case SDL_MOUSEBUTTONDOWN:
				break;

			case SDL_MOUSEBUTTONUP:
				//*mouseState = NONE;

				/*
				if (e->button.button == SDL_BUTTON_LEFT && timediv > 800)
				{
					*mouseState = IN_LB_PR;
				}
				else if (e->button.button == SDL_BUTTON_RIGHT && timediv > 800)
				{
					*mouseState = IN_RB_PR;
				}
				else */
				if (e->button.button == SDL_BUTTON_LEFT && e->button.clicks == 2 && timediv < 150)
				{
					*mouseState = IN_LB_DC;
				}
				else if (e->button.button == SDL_BUTTON_RIGHT && e->button.clicks == 2 && timediv < 150)
				{
					*mouseState = IN_RB_DC;
				}
				else if (e->button.button == SDL_BUTTON_LEFT && e->button.clicks == 1 && timediv < 800 && timediv > 100)
				{
					*mouseState = IN_LB_SC;
				}
				else if (e->button.button == SDL_BUTTON_RIGHT && e->button.clicks == 1 && timediv < 800 && timediv > 100)
				{
					*mouseState = IN_RB_SC;
				}

				break;

			case SDL_MOUSEWHEEL:
				if (e->wheel.y > 0) // scroll up
				{
					// Put code for handling "scroll up" here!
					*y = e->wheel.y;
					*mouseState = IN_WU;
				}
				else if (e->wheel.y < 0) // scroll down
				{
					// Put code for handling "scroll down" here!
					*y = e->wheel.y;
					*mouseState = IN_WD;
				}
				break;

			case SDL_MOUSEMOTION:
				*mouseState = HOVER;
				break;
			}
		}
	}
}




void movef1()
{
	//Move the dot left or righ
	f1PosX += f1VelX;
	//走道邊界
	if ((f1PosX + 35 <= 0) || (f1PosX + 80 >= WIDTH))
	{

		//縮回來
		f1PosX -= f1VelX;

		//跳到邊界
		if (f1jump)
		{
			//左邊界
			if (f1PosX + 35 <= 0)
				f1jumping_left = true;
			//右邊界
			if (f1PosX + 80 >= WIDTH)
				f1jumping_right = true;
			/*
			//往左、右看而且沒有近攻 就換方向
			if (f1standing_right && !f1front_hit_operating)
			{
				f1standing_right = false;
				f1standing_left = true;
			}
			else if (f1standing_left && !f1front_hit_operating)
			{
				f1standing_left = false;
				f1standing_right = true;
			}

			//往左跳撞牆就往右反彈一段水平距離
			if (f1jumping_left)
			{
				if (f1orY - f1PosY >= 200)
					f1jumping_left = false;
			}
			if (f1jumping_right)
			{
				if (f1orY - f1PosY >= 200)
					f1jumping_right = false;
			}*/
		}
	}


	//如果跳躍
	if (f1jump)
	{
		//如果方向往左
		if (f1dirY == -1)
		{
			//f1VelY -= 1;
			//如果跳一段距離
			if (f1orY - f1PosY >= 200)
				f1dirY = 1;
			f1PosY += f1VelY * f1dirY;
		}
		//如果方向往右
		else if (f1dirY == 1)
		{
			//f1VelY += 1;
			//如果落地就不上下移動，也不做跳躍動畫
			if (f1orY == f1PosY)
			{
				f1dirY = -1;
				f1jump = false;
				f1walking_operating = false;
			}
			//沒落地繼續下墜
			else
				f1PosY += f1VelY * f1dirY;
		}


	}

}
void movef2()
{
	//Move the dot left or righ
	f2PosX += f2VelX;
	//走道邊界
	if ((f2PosX + 35 <= 0) || (f2PosX + 80 >= WIDTH))
	{

		//縮回來
		f2PosX -= f2VelX;

		//跳到邊界
		if (f2jump)
		{
			//左邊界
			if (f2PosX + 35 <= 0)
				f2jumping_left = true;
			//右邊界
			if (f2PosX + 80 >= WIDTH)
				f2jumping_right = true;
			/*
			//往左、右看而且沒有近攻 就換方向
			if (f1standing_right && !f1front_hit_operating)
			{
				f1standing_right = false;
				f1standing_left = true;
			}
			else if (f1standing_left && !f1front_hit_operating)
			{
				f1standing_left = false;
				f1standing_right = true;
			}

			//往左跳撞牆就往右反彈一段水平距離
			if (f1jumping_left)
			{
				if (f1orY - f1PosY >= 200)
					f1jumping_left = false;
			}
			if (f1jumping_right)
			{
				if (f1orY - f1PosY >= 200)
					f1jumping_right = false;
			}*/
		}
	}


	//如果跳躍
	if (f2jump)
	{
		//如果方向往左
		if (f2dirY == -1)
		{
			//f1VelY -= 1;
			//如果跳一段距離
			if (f2orY - f2PosY >= 200)
				f2dirY = 1;
			f2PosY += f2VelY * f2dirY;
		}
		//如果方向往右
		else if (f2dirY == 1)
		{
			//f1VelY += 1;
			//如果落地就不上下移動，也不做跳躍動畫
			if (f2orY == f2PosY)
			{
				f2dirY = -1;
				f2jump = false;
				f2walking_operating = false;
			}
			//沒落地繼續下墜
			else
				f2PosY += f2VelY * f2dirY;
		}


	}

}
//movef2 not finished


// When using SDL, you have to use "int main(int argc, char* args[])"
// int main() and void main() are not allowed
int main(int argc, char* args[])
{

	/*TextData textfinish1 = loadTextTexture("User1 win:", "../fonts/lazy.ttf", 30, 255, 0, 0, BLENDED, NULL, NULL, NULL);
	SDL_StartTextInput();
	TextData textfinish2 = loadTextTexture("User2 win:", "../fonts/lazy.ttf", 30, 255, 0, 0, BLENDED, NULL, NULL, NULL);
	SDL_StartTextInput();*/


	

//	boxRGBA(renderer, BUTTON_X, BUTTON_Y, BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT, 255, 200, 15, 0xAA);
	// The working directory is where the vcxproj file is located.
	char menuPath[100] = "../images/allfigures/menu/menu.png";
	char tapPath[100] = "../images/allfigures/menu/";
	int tapIndex = 0;
	char restart1Path[100] = "../images/allfigures/restart/1.png";
	char restart2Path[100] = "../images/allfigures/restart/2.png";
	char restartPath[100] = "../images/allfigures/restart/";
	int restartIndex = 0;
	char imgPath2[100] = "../images/allfigures/background/background1.png";
	char winPath[100] = "..images/allfigures/newgame/newgame2.png";


	//f1Path
	char f1HPPath[100] = "../images/allfigures/f1/HP/";
	int f1HPIndex = 0;
	char f1BUFFPath[100] = "../images/allfigures/f1/BUFF/";
	int f1BUFFIndex = 0;
	char f1standingrightPath[100] = "../images/allfigures/f1/f1right/standinghere/19/";
	int f1standingrightIndex = 0;
	char f1standingleftPath[100] = "../images/allfigures/f1/f1left/standinghere/19/";
	int f1standingleftIndex = 0;
	char f1jumpingrightPath[100] = "../images/allfigures/f1/f1right/jump/22/";
	int f1jumpingrightIndex = 0;
	char f1jumpingleftPath[100] = "../images/allfigures/f1/f1left/jump/23/";
	int f1jumpingleftIndex = 0;
	char f1frontpunchhitrightPath[100] = "../images/allfigures/f1/f1right/frontpunch/hit/6/";
	int f1frontpunchhitrightIndex = 0;
	char f1frontpunchhitleftPath[100] = "../images/allfigures/f1/f1left/frontpunch/hit/6/";
	int f1frontpunchhitleftIndex = 0;
	char f1walkingrightPath[100] = "../images/allfigures/f1/f1right/walking/20/";
	int f1walkingrightIndex = 0;
	char f1walkingleftPath[100] = "../images/allfigures/f1/f1left/walking/21/";
	int f1walkingleftIndex = 0;
	char f1beinghitleftPath[100] = "../images/allfigures/f1/f1left/beinghit/";
	int f1beinghitleftIndex = 0;
	char f1beinghitrightPath[100] = "../images/allfigures/f1/f1right/beinghit/";
	int f1beinghitrightIndex = 0;
	char f1defendleftPath[100] = "../images/allfigures/f1/f1left/defend/11/";
	int f1defendleftIndex = 0;
	char f1defendrightPath[100] = "../images/allfigures/f1/f1right/defend/11/";
	int f1defendrightIndex = 0;
	char f1farpunchhitrightPath[100] = "../images/allfigures/f1/f1right/farpunch/hit/5/";
	int f1farpunchhitrightIndex = 0;
	char f1farpunchhitleftPath[100] = "../images/allfigures/f1/f1left/farpunch/hit/5/";
	int f1farpunchhitleftIndex = 0;
	char f1farpunchhitweaponrightPath[100] = "../images/allfigures/f1/f1right/farpunch/hit/soccer/1.png";
	int f1farpunchhitweaponrightIndex = 0;
	char f1farpunchhitweaponleftPath[100] = "../images/allfigures/f1/f1left/farpunch/hit/soccer/1.png";
	int f1farpunchhitweaponleftIndex = 0;
	char f1fuckingkillexrightPath[100] = "../images/allfigures/f1/f1right/fuckingkill/13/";
	int f1fuckingkillexrightIndex = 0;
	char f1fuckingkillexleftPath[100] = "../images/allfigures/f1/f1left/fuckingkill/13/";
	int f1fuckingkillexleftIndex = 0;
	char f1fuckingkilltrillerPath[100] = "../images/allfigures/f1/f1right/fuckingkill/1.png";
	char f1fuckingkillweaponPath[100] = "../images/allfigures/f1/f1left/fuckingkill/boom/";
	int f1fuckingkillweaponIndex = 0;
//	char f1winPath[100] = "../images/allfigures/f1/f1win/1.png";
//	int f1winIndex = 0;
	


	//f2Path
	char f2HPPath[100] = "../images/allfigures/f2/HP/";
	int f2HPIndex = 0;
	char f2BUFFPath[100] = "../images/allfigures/f2/BUFF/";
	int f2BUFFIndex = 0;
	char f2standingrightPath[100] = "../images/allfigures/f2/f2right/standinghere/11/";
	int f2standingrightIndex = 0;
	char f2standingleftPath[100] = "../images/allfigures/f2/f2left/standinghere/11/";
	int f2standingleftIndex = 0;
	char f2jumpingrightPath[100] = "../images/allfigures/f2/f2right/jump/4/";
	int f2jumpingrightIndex = 0;
	char f2jumpingleftPath[100] = "../images/allfigures/f2/f2left/jump/4/";
	int f2jumpingleftIndex = 0;
	char f2frontpunchhitrightPath[100] = "../images/allfigures/f2/f2right/frontpunch/hit/6/";
	int f2frontpunchhitrightIndex = 0;
	char f2frontpunchhitleftPath[100] = "../images/allfigures/f2/f2left/frontpunch/hit/6/";
	int f2frontpunchhitleftIndex = 0;
	char f2walkingrightPath[100] = "../images/allfigures/f2/f2right/walking/2/";
	int f2walkingrightIndex = 0;
	char f2walkingleftPath[100] = "../images/allfigures/f2/f2left/walking/2/";
	int f2walkingleftIndex = 0;
	char f2beinghitleftPath[100] = "../images/allfigures/f2/f2left/beinghit/12/";
	int f2beinghitleftIndex = 0;
	char f2beinghitrightPath[100] = "../images/allfigures/f2/f2right/beinghit/12/";
	int f2beinghitrightIndex = 0;
	char f2defendleftPath[100] = "../images/allfigures/f2/f2left/defend/5/";
	int f2defendleftIndex = 0;
	char f2defendrightPath[100] = "../images/allfigures/f2/f2right/defend/5/";
	int f2defendrightIndex = 0;
	char f2farpunchhitrightPath[100] = "../images/allfigures/f2/f2right/farpunch/hit/16/";
	int f2farpunchhitrightIndex = 0;
	char f2farpunchhitleftPath[100] = "../images/allfigures/f2/f2left/farpunch/hit/16/";
	int f2farpunchhitleftIndex = 0;
	char f2farpunchhitweaponrightPath[100] = "../images/allfigures/f2/f2right/farpunch/hit/soccer/1.png";
	char f2farpunchhitweaponleftPath[100] = "../images/allfigures/f2/f2left/farpunch/hit/soccer/1.png";
	char f2fuckingkillexrightPath[100] = "../images/allfigures/f2/f2right/fuckingkill/19/";
	int f2fuckingkillexrightIndex = 0;
	char f2fuckingkillexleftPath[100] = "../images/allfigures/f2/f2left/fuckingkill/19/";
	int f2fuckingkillexleftIndex = 0;
	char f2fuckingkilltrillerPath[100] = "../images/allfigures/f2/f2right/fuckingkill/1.png";
	char f2fuckingkillweaponPath[100] = "../images/allfigures/f2/f2left/fuckingkill/boom/";
	int f2fuckingkillweaponIndex = 0;
//	char f2winPath[100] = "../images/allfigures/f2/f2win/1.png";
//	int f2winIndex = 0;

	ImageData rob;
	ImageData menu;
	ImageData tap[TAPFRAME];
	ImageData restart1;
	ImageData restart2;
	
	//f1imagedata
	ImageData f1HP[F1HPFRAME];
	ImageData f1BUFF[F1BUFFFRAME];
	ImageData f1standingright[F1STANDINGRIGHTFRAME];
	ImageData f1standingleft[F1STANDINGLEFTFRAME];
	ImageData f1jumpingright[F1JUMPINGRIGHTFRAME];
	ImageData f1jumpingleft[F1JUMPINGLEFTFRAME];
	ImageData f1frontpunchhitright[F1FRONTPUNCHHITRIGHTFRAME];
	ImageData f1frontpunchhitleft[F1FRONTPUNCHHITLEFTFRAME];
	ImageData f1walkingright[F1WALKINGRIGHTFRAME];
	ImageData f1walkingleft[F1WALKINGLEFTFRAME];
	ImageData f1beinghitright[F1BEINGHITRIGHTFRAME];
	ImageData f1beinghitleft[F1BEINGHITLEFTFRAME];
	ImageData f1defendright[F1DEFENDRIGHTFRAME];
	ImageData f1defendleft[F1DEFENDLEFTFRAME];
	ImageData f1farpunchhitright[F1FARPUNCHHITRIGHTFRAME];
	ImageData f1farpunchhitleft[F1FARPUNCHHITLEFTFRAME];
	ImageData f1farpunchhitweaponright;
	ImageData f1farpunchhitweaponleft;
	ImageData f1fuckingkilltriller;
	ImageData f1fuckingkillexright[F1FUCKINGKILLEXRIGHTFRAME];
	ImageData f1fuckingkillexleft[F1FUCKINGKILLEXLEFTFRAME];
	ImageData f1fuckingkillweapon[F1FUCKINGKILLWEAPONFRAME];
//	ImageData f1win;

	//f2imagedata
	ImageData f2HP[F2HPFRAME];
	ImageData f2BUFF[F2BUFFFRAME];
	ImageData f2standingright[F2STANDINGRIGHTFRAME];
	ImageData f2standingleft[F2STANDINGLEFTFRAME];
	ImageData f2jumpingright[F2JUMPINGRIGHTFRAME];
	ImageData f2jumpingleft[F2JUMPINGLEFTFRAME];
	ImageData f2frontpunchhitright[F2FRONTPUNCHHITRIGHTFRAME];
	ImageData f2frontpunchhitleft[F2FRONTPUNCHHITLEFTFRAME];
	ImageData f2walkingright[F2WALKINGRIGHTFRAME];
	ImageData f2walkingleft[F2WALKINGLEFTFRAME];
	ImageData f2beinghitright[F2BEINGHITRIGHTFRAME];
	ImageData f2beinghitleft[F2BEINGHITLEFTFRAME];
	ImageData f2defendright[F2DEFENDRIGHTFRAME];
	ImageData f2defendleft[F2DEFENDLEFTFRAME];
	ImageData f2farpunchhitright[F2FARPUNCHHITRIGHTFRAME];
	ImageData f2farpunchhitleft[F2FARPUNCHHITLEFTFRAME];
	ImageData f2farpunchhitweaponright;
	ImageData f2farpunchhitweaponleft;
	ImageData f2fuckingkilltriller;
	ImageData f2fuckingkillexright[F2FUCKINGKILLEXRIGHTFRAME];
	ImageData f2fuckingkillexleft[F2FUCKINGKILLEXLEFTFRAME];
	ImageData f2fuckingkillweapon[F2FUCKINGKILLWEAPONFRAME];
//	ImageData f2win;


	boxRGBA(renderer, BUTTON_X, BUTTON_Y, BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT, 255, 200, 15, 0xAA);

//	choosepage;
	printf("every image\n");
	// Start up SDL and create window
	if (initSDL())
	{
		printf("Failed to initialize SDL!\n");
		return -1;
	}
	
		loadAudio();
	menu = loadTexture(menuPath, false, 0xFF, 0xFF, 0xFF);
	rob = loadTexture(imgPath2, false, 0xFF, 0xFF, 0xFF);
	restart2 = loadTexture(restart2Path, false, 0xFF, 0xFF, 0xFF);
//	restart2 = loadTexture(restartPath, false, 0xFF, 0xFF, 0xFF);
//	f1win = loadTexture(f1winPath, false, 0xFF, 0xFF, 0xFF);
//	f2win = loadTexture(f2winPath, false, 0xFF, 0xFF, 0xFF);
	
	/*
	//f1 win
	for (int i = 0; i < F1WINFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s%d.png", f1HPPath, i + 1);
		f1win[i] = loadTexture(f1winPath, false, 0xFF, 0xFF, 0xFF);
	}

	//f2 win
	for (int i = 0; i < F2WINFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s%d.png", f2HPPath, i + 1);
		f2win[i] = loadTexture(f2winPath, false, 0xFF, 0xFF, 0xFF);
	}
	*/


	for (int i = 0; i < TAPFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s%d.png", tapPath, i + 1);
		tap[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}
	
//	restart1 = loadTexture(restart1Path, false, 0xFF, 0xFF, 0xFF);
	

/*	for (int i = 0; i < RESTARTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s%d.png", restart2Path, i + 1);
		restart2 = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}
*/
	
	//f1讀取照片

	
	for (int i = 0; i < F1HPFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s%d.png", f1HPPath, i + 1);
		f1HP[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}
	printf("f1hp every image\n"); 

	for (int i = 0; i < F1BUFFFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s%d.png", f1BUFFPath, i + 1);
		f1BUFF[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}
	printf("f1buff every image\n");
	for (int i = 0; i < F1STANDINGRIGHTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s19-%d.png", f1standingrightPath, i + 1);
		f1standingright[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}


	for (int i = 0; i < F1STANDINGLEFTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s19-%d.png", f1standingleftPath, i + 1);
		f1standingleft[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F1JUMPINGRIGHTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s22-%d.png", f1jumpingrightPath, i + 1);
		f1jumpingright[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F1JUMPINGLEFTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s23-%d.png", f1jumpingleftPath, i + 1);
		f1jumpingleft[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F1FRONTPUNCHHITRIGHTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s6-%d.png", f1frontpunchhitrightPath, i + 1);
		f1frontpunchhitright[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F1FRONTPUNCHHITLEFTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s6-%d.png", f1frontpunchhitleftPath, i + 1);
		f1frontpunchhitleft[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F1WALKINGRIGHTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s20-%d.png", f1walkingrightPath, i + 1);
		f1walkingright[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F1WALKINGLEFTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s21-%d.png", f1walkingleftPath, i + 1);
		f1walkingleft[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F1BEINGHITRIGHTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s4-%d.png", f1beinghitrightPath, i + 1);
		f1beinghitright[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F1BEINGHITLEFTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s4-%d.png", f1beinghitleftPath, i + 1);
		f1beinghitleft[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F1DEFENDRIGHTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s11-%d.png", f1defendrightPath, i + 1);
		f1defendright[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F1DEFENDLEFTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s11-%d.png", f1defendleftPath, i + 1);
		f1defendleft[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F1FARPUNCHHITRIGHTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s5-%d.png", f1farpunchhitrightPath, i + 1);
		f1farpunchhitright[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F1FARPUNCHHITLEFTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s5-%d.png", f1farpunchhitleftPath, i + 1);
		f1farpunchhitleft[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	f1farpunchhitweaponright = loadTexture(f1farpunchhitweaponrightPath, true, 0xFF, 0xFF, 0xFF);
	f1farpunchhitweaponleft = loadTexture(f1farpunchhitweaponleftPath, true, 0xFF, 0xFF, 0xFF);

	f1fuckingkilltriller = loadTexture(f1fuckingkilltrillerPath, true, 0xFF, 0xFF, 0xFF);
	printf("f1 fuckingkilltriller loadTexture\n");

	for (int i = 0; i < F1FUCKINGKILLEXRIGHTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s13-%d.png", f1fuckingkillexrightPath, i + 1);
		f1fuckingkillexright[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}
	printf("f1 fuckingkillexright loadTexture\n");

	for (int i = 0; i < F1FUCKINGKILLEXLEFTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s13-%d.png", f1fuckingkillexleftPath, i + 1);
		f1fuckingkillexleft[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}
	printf("f1 fuckingkillexleft loadTexture\n");


	for (int i = 0; i < F1FUCKINGKILLWEAPONFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%sboom-%d.png", f1fuckingkillweaponPath, i + 1);
		f1fuckingkillweapon[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}
	printf("f1 fuckingkillweaponleft loadTexture\n");

/*
	for (int i = 0; i < F1WINFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s%d.png", f1winPath, 1);
		f1win = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}
	printf("f1 F1HPFRAME loadTexture\n");
*/


	//f2讀照片
	for (int i = 0; i < F2HPFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s%d.png", f2HPPath, i + 1);
		f2HP[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F2BUFFFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s%d.png", f2BUFFPath, i + 1);
		f2BUFF[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F2STANDINGRIGHTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s11-%d.png", f2standingrightPath, i + 1);
		f2standingright[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}


	for (int i = 0; i < F2STANDINGLEFTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s11-%d.png", f2standingleftPath, i + 1);
		f2standingleft[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F2JUMPINGRIGHTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s4-%d.png", f2jumpingrightPath, i + 1);
		f2jumpingright[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F2JUMPINGLEFTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s4-%d.png", f2jumpingleftPath, i + 1);
		f2jumpingleft[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F2FRONTPUNCHHITRIGHTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s6-%d.png", f2frontpunchhitrightPath, i + 1);
		f2frontpunchhitright[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F2FRONTPUNCHHITLEFTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s6-%d.png", f2frontpunchhitleftPath, i + 1);
		f2frontpunchhitleft[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F2WALKINGRIGHTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s2-%d.png", f2walkingrightPath, i + 1);
		f2walkingright[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F2WALKINGLEFTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s2-%d.png", f2walkingleftPath, i + 1);
		f2walkingleft[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F2BEINGHITRIGHTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s12-%d.png", f2beinghitrightPath, i + 1);
		f2beinghitright[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F2BEINGHITLEFTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s12-%d.png", f2beinghitleftPath, i + 1);
		f2beinghitleft[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F2DEFENDRIGHTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s5-%d.png", f2defendrightPath, i + 1);
		f2defendright[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F2DEFENDLEFTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s5-%d.png", f2defendleftPath, i + 1);
		f2defendleft[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F2FARPUNCHHITRIGHTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s16-%d.png", f2farpunchhitrightPath, i + 1);
		f2farpunchhitright[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F2FARPUNCHHITLEFTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s16-%d.png", f2farpunchhitleftPath, i + 1);
		f2farpunchhitleft[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}



	f2farpunchhitweaponright = loadTexture(f2farpunchhitweaponrightPath, true, 0xFF, 0xFF, 0xFF);
	f2farpunchhitweaponleft = loadTexture(f2farpunchhitweaponleftPath, true, 0xFF, 0xFF, 0xFF);

	f2fuckingkilltriller = loadTexture(f2fuckingkilltrillerPath, true, 0xFF, 0xFF, 0xFF);

	for (int i = 0; i < F2FUCKINGKILLEXRIGHTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s19-%d.png", f2fuckingkillexrightPath, i + 1);
		f2fuckingkillexright[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}

	for (int i = 0; i < F2FUCKINGKILLEXLEFTFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s19-%d.png", f2fuckingkillexleftPath, i + 1);
		f2fuckingkillexleft[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}


	for (int i = 0; i < F2FUCKINGKILLWEAPONFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%sboom-%d.png", f2fuckingkillweaponPath, i + 1);
		f2fuckingkillweapon[i] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}
/*
	for (int i = 0; i < F2WINFRAME; i++)
	{
		char st[100];
		sprintf_s(st, 100, "%s%d.png", f2winPath, 1);
		f2win = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
	}
	printf("f2 F2HPFRAME loadTexture\n");
	*/


	//手動timer

	int tapperiod = 500,tapspeed=2;
	int tapspeedperiod = tapperiod * tapspeed;
	int tapspeedtimediv = 0;
	static int tapcurtime = SDL_GetTicks();
	static int taplasttime = tapcurtime;

	static int f1curtime = SDL_GetTicks();
	static int f1lasttime = f1curtime;
	static int f2curtime = SDL_GetTicks();
	static int f2lasttime = f2curtime;
	int f1timediv, f1period = 500;
	int f1frontpunchhitspeed = 1, f1frontpunchhitspeedtimediv = 0;
	int f1farpunchhitspeed = 2, f1farpunchhitspeedtimediv = 0;
	int f1farpunchhitweaponspeed = 20, f1farpunchhitweaponspeedtimediv = 0;
	int f1walkingspeed = 20, f1walkingspeedtimediv = 0;
	int f1frontpunchhitperiod = 500;
	int f1frontpunchhitspeedperiod = f1frontpunchhitperiod * f1frontpunchhitspeed;
	int f1farpunchhitperiod = 500;
	int f1farpunchhitspeedperiod = f1farpunchhitperiod * f1farpunchhitspeed;

	int f1fuckingkilltrillerspeed = 2, f1fuckingkilltrillerspeedtimediv = 0;
	int f1fuckingkilltrillerperiod = 500;
	int f1fuckingkilltrillerspeedperiod = f1fuckingkilltrillerperiod * f1fuckingkilltrillerspeed;

	int f1fuckingkillexspeed = 2 * 2, f1fuckingkillexspeedtimediv = 0;
	int f1fuckingkillexperiod = 500;
	int f1fuckingkillexspeedperiod = f1fuckingkillexperiod * f1fuckingkillexspeed;

	int f1fuckingkillweaponspeed = 2 * 2, f1fuckingkillweaponspeedtimediv = 0;
	int f1fuckingkillweaponperiod = 500;
	int f1fuckingkillweaponspeedperiod = f1fuckingkillweaponperiod * f1fuckingkillweaponspeed;


	int f2timediv, f2period = 500;
	int f2frontpunchhitspeed = 2, f2frontpunchhitspeedtimediv = 0;
	int f2farpunchhitspeed = 2, f2farpunchhitspeedtimediv = 0;
	int f2farpunchhitweaponspeed = 20, f2farpunchhitweaponspeedtimediv = 0;
	int f2walkingspeed = 20, f2walkingspeedtimediv = 0;
	int f2frontpunchhitperiod = 500;
	int f2frontpunchhitspeedperiod = f2frontpunchhitperiod * f2frontpunchhitspeed;
	int f2farpunchhitperiod = 500;
	int f2farpunchhitspeedperiod = f2farpunchhitperiod * f2farpunchhitspeed;


	int f2fuckingkilltrillerspeed = 2, f2fuckingkilltrillerspeedtimediv = 0;
	int f2fuckingkilltrillerperiod = 500;
	int f2fuckingkilltrillerspeedperiod = f2fuckingkilltrillerperiod * f2fuckingkilltrillerspeed;

	int f2fuckingkillexspeed = 1, f2fuckingkillexspeedtimediv = 0;
	int f2fuckingkillexperiod = 1100;
	int f2fuckingkillexspeedperiod = f2fuckingkillexperiod * f2fuckingkillexspeed;

	int f2fuckingkillweaponspeed = 2 * 2, f2fuckingkillweaponspeedtimediv = 0;
	int f2fuckingkillweaponperiod = 500;
	int f2fuckingkillweaponspeedperiod = f2fuckingkillweaponperiod * f2fuckingkillweaponspeed;


	static int f1beinghitcurtime = SDL_GetTicks();
	static int f1beinghitlasttime = f1beinghitcurtime;
	static int f2beinghitcurtime = SDL_GetTicks();
	static int f2beinghitlasttime = f2beinghitcurtime;

	int f1beinghittimediv, f2beinghittimediv;
	//記住f1上次做甚麼攻擊
	char f1last_attack = NULL;
	//記住f1這次做甚麼攻擊
	char f1this_attack = NULL;
	//記住f2上次做甚麼攻擊
	char f2last_attack = NULL;
	//記住f2這次做甚麼攻擊
	char f2this_attack = NULL;


	//Main loop flag
	bool quit = false;

	MouseState mouseState;
	int mouseX, mouseY;

	//Event handler
	SDL_Event e;
	//While application is running
	while (!quit)
	{

		mouseState = NONE;

		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			// User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true; // try to comment this line
			}

			mouseHandleEvent(&e, &mouseState, &mouseX, &mouseY);

			// If a key was pressed
			// repeat: non-zero if this is a key repeat
			// https://wiki.libsdl.org/SDL_KeyboardEvent
			// https://wiki.libsdl.org/SDL_Keycode


			if (e.type == SDL_KEYDOWN && e.key.repeat == 0 && !fucking_operating)
			{
				/*if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(inputString) > 0)
				{
					// pop out the last character
					inputString[strlen(inputString) - 1] = '\0';
					textFlag = true;
				}
				else if (e.key.keysym.sym == SDLK_c && (SDL_GetModState() & KMOD_CTRL))
				{
					SDL_SetClipboardText(inputString);
				}
				else if (e.key.keysym.sym == SDLK_v && (SDL_GetModState() & KMOD_CTRL))
				{
					strcpy_s(inputString, 100, SDL_GetClipboardText());
					textFlag = true;
				}*/

				switch (e.key.keysym.sym)
				{
					//f1
				case SDLK_w:
					Mix_PlayChannel(1, f1jumpeffect, 0);
					f1jump = true;
					f1VelY = VELOCITY;
					f1lasttime = f1curtime;
					break;
				case SDLK_a:
					Mix_PlayChannel(1, f1walkingeffect, -1);
					f1VelX -= VELOCITY;
					//walking動畫沒有進行時
					if (!f1walking_operating)
					{
						f1walking = true;
						f1lasttime = f1curtime;
						f1pressed_walking = true;
					}
					f1standing_left = true;
					f1standing_right = false;
					break;

				case SDLK_d:
					Mix_PlayChannel(1, f1walkingeffect, -1);
					f1VelX += VELOCITY;
					if (!f1walking_operating)
					{
						f1walking = true;
						f1lasttime = f1curtime;
						f1pressed_walking = true;
					}
					f1standing_right = true;
					f1standing_left = false;
					break;

				case SDLK_j:
					Mix_PlayChannel(1, f1frontpunchhiteffect, 0);
					f1last_attack = f1this_attack;
					f1this_attack = 'j';
					f1front_punch_hit_count = true;
					if (!f1front_hit_operating)
					{
						f1front_punch_hit = true;
						f1lasttime = f1curtime;
					}

					break;

				case SDLK_k:
					Mix_PlayChannel(1, f1farpunchhiteffect, 0);
					f1last_attack = f1this_attack;
					f1this_attack = 'u';
					if (!f1far_punch_hit_operating)
					{
						f1far_punch_hit = true;
						f1far_punch_hit_weapon_effective = true;
						f1lasttime = f1curtime;
					}

					break;


				case SDLK_u:
					if (f1fucking_kill_on && !f1fucking_kill_triller_operating)
					{
						Mix_PlayChannel(1, f1fuckingkilltrillereffect, 0);
						f1last_attack = f1this_attack = '0';
						f1fucking_kill_triller = true;
						f1lasttime = f1curtime;
					}

					break;

				case SDLK_s:
					if (!f1defend_operating)
					{
						f1defend_ = true;
						f1defend_effective = true;
						f1lasttime = f1curtime;
						f1pressed_defend = true;
					}

					break;
					//f2
				case SDLK_UP:
					Mix_PlayChannel(1, f2jumpeffect, 0);
					f2jump = true;
					f2VelY = VELOCITY;
					f2lasttime = f2curtime;
					break;
				case SDLK_LEFT:
					Mix_PlayChannel(1, f2walkingeffect, -1);
					f2VelX -= VELOCITY;
					//walking動畫沒有進行時
					if (!f2walking_operating)
					{
						f2walking = true;
						f2lasttime = f2curtime;
						f2pressed_walking = true;
					}
					f2standing_left = true;
					f2standing_right = false;
					break;

				case SDLK_RIGHT:
					Mix_PlayChannel(1, f2walkingeffect, -1);
					f2VelX += VELOCITY;
					if (!f2walking_operating)
					{
						f2walking = true;
						f2lasttime = f2curtime;
						f2pressed_walking = true;
					}
					f2standing_right = true;
					f2standing_left = false;
					break;
				case SDLK_KP_1:
					f2last_attack = f2this_attack;
					f2this_attack = '1';
					f2front_punch_hit_count = true;
					if (!f2front_hit_operating)
					{
						f2front_punch_hit = true;
						f2lasttime = f2curtime;
					}
					break;
				case SDLK_KP_2:
					Mix_PlayChannel(1, f2farpunchhiteffect, 0);
					f2last_attack = f2this_attack;
					f2this_attack = '4';
					if (!f2far_punch_hit_operating)
					{
						f2far_punch_hit = true;
						f2far_punch_hit_weapon_effective = true;
						f2lasttime = f2curtime;
					}
					break;

				case SDLK_KP_3:
					if (f2fucking_kill_on && !f2fucking_kill_triller_operating)
					{
						Mix_PlayChannel(1, f2fuckingkilltrillereffect, 0);
						f2last_attack = f2this_attack = '0';
						f2fucking_kill_triller = true;
						f2lasttime = f2curtime;
					}

					break;
				case SDLK_DOWN:
					if (!f2defend_operating)
					{
						f2defend_ = true;
						f2defend_effective = true;
						f2lasttime = f2curtime;
						f2pressed_defend = true;
					}

					break;
				}
			}


			//If a key was released
			//else if (e.type == SDL_KEYUP)
			else if (e.type == SDL_KEYUP && e.key.repeat == 0 && !fucking_operating)
			{
				//Adjust the velocity
				switch (e.key.keysym.sym)
				{
					//f1
				case SDLK_a:
					Mix_HaltChannel(1);
					f1VelX += VELOCITY;
					f1pressed_walking = false;
					break;
				case SDLK_d:
					Mix_HaltChannel(1);
					f1VelX -= VELOCITY;
					f1pressed_walking = false;
					break;
					/*case SDLK_j:
						f1last_attack = 'j';
						break;
					case SDLK_u:
						f1last_attack = 'u';
						break;*/
				case SDLK_s:
					if (!f1defend_operating)
					{
						f1defend_ = false;
						f1defend_effective = false;
						f1lasttime = f1curtime;
					}
					f1pressed_defend = false;
					break;
					//f2
				case SDLK_LEFT:
					Mix_HaltChannel(1);
					f2VelX += VELOCITY;
					f2pressed_walking = false;
					break;
				case SDLK_RIGHT:
					Mix_HaltChannel(1);
					f2VelX -= VELOCITY;
					f2pressed_walking = false;
					break;
					/*case SDLK_KP_1:
						f2this_attack = '1';
						break;
					case SDLK_KP_4:
						f2this_attack = '4';
						break;*/
				case SDLK_DOWN:
					if (!f2defend_operating)
					{
						f2defend_ = false;
						f2defend_effective = false;
						f2lasttime = f2curtime;
					}
					f2pressed_defend = false;
					break;
				}
			}
		}

		// Move the dot
		movef1();
		movef2();


		// Clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(renderer);

		boxRGBA(renderer, BUTTON_X, BUTTON_Y, BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT, 255, 200, 15, 0xAA);

		imgRender(renderer, rob, 0, 0);

		tapcurtime = SDL_GetTicks();
		tapspeedtimediv = (tapcurtime - taplasttime) % tapspeedperiod;
		
		f1curtime = SDL_GetTicks();
		f1timediv = (f1curtime - f1lasttime) % f1period;
		f1frontpunchhitspeedtimediv = (f1curtime - f1lasttime) % f1frontpunchhitspeedperiod;
		f1farpunchhitspeedtimediv = (f1curtime - f1lasttime) % f1farpunchhitspeedperiod;
		f1fuckingkilltrillerspeedtimediv = (f1curtime - f1lasttime) % f1fuckingkilltrillerspeedperiod;
		f1fuckingkillexspeedtimediv = (f1curtime - f1lasttime) % f1fuckingkillexspeedperiod;
		f1fuckingkillweaponspeedtimediv = (f1curtime - f1lasttime) % f1fuckingkillweaponspeedperiod;
		f1walkingspeedtimediv = f1timediv;
		//f1beingpunchhitcurtime = SDL_GetTicks();
		//f1beingpunchhittimediv = f1beingpunchhitcurtime - f1beingpunchhitlasttime;
		f1beinghitcurtime = SDL_GetTicks();
		f1beinghittimediv = (f1beinghitcurtime - f1beinghitlasttime) % f1period;

		f2curtime = SDL_GetTicks();
		f2timediv = (f2curtime - f2lasttime) % f2period;
		f2frontpunchhitspeedtimediv = (f2curtime - f2lasttime) % f2frontpunchhitspeedperiod;
		f2farpunchhitspeedtimediv = (f2curtime - f2lasttime) % f2farpunchhitspeedperiod;
		f2fuckingkilltrillerspeedtimediv = (f2curtime - f2lasttime) % f2fuckingkilltrillerspeedperiod;
		f2fuckingkillexspeedtimediv = (f2curtime - f2lasttime) % f2fuckingkillexspeedperiod;
		f2fuckingkillweaponspeedtimediv = (f2curtime - f2lasttime) % f2fuckingkillweaponspeedperiod;
		f2walkingspeedtimediv = f2timediv;
		//f2beingpunchhitcurtime = SDL_GetTicks();
		//f2beingpunchhittimediv = f2beingpunchhitcurtime - f2beingpunchhitlasttime;
		f2beinghitcurtime = SDL_GetTicks();
		f2beinghittimediv = (f2beinghitcurtime - f2beinghitlasttime) % f2period;


		if (f1HPIndex == F1HPFRAME - 1)
		{
			Mix_PlayChannel(1, f1deadeffect, 0);
			Mix_PlayChannel(1, f2defeateffect, 0);
			//return 0;
		}	
		else if (f2HPIndex == F2HPFRAME - 1)
		{
			Mix_PlayChannel(1, f2deadeffect, 0);
			Mix_PlayChannel(1, f1defeateffect, 0);
			//return 0;
		}
		else
		{
			if (Mix_PlayingMusic() == 0 && menu_finish)
				Mix_PlayMusic(backgroundmusic, 0);
		}
		imgRender(renderer, f1HP[f1HPIndex], 50, 50);
		imgRender(renderer, f2HP[f2HPIndex], WIDTH - 300, 50);

		if (f1BUFFIndex >= F1BUFFFRAME - 1)
		{
			f1fucking_kill_on = true;
			if (f1fucking_kill_triller)
				f1BUFFIndex = 0;
		}
		imgRender(renderer, f1BUFF[f1BUFFIndex], 50, 120);

		if (f2BUFFIndex >= F2BUFFFRAME - 1)
		{
			f2fucking_kill_on = true;
			if (f2fucking_kill_triller)
				f2BUFFIndex = 0;
		}
		imgRender(renderer, f2BUFF[f2BUFFIndex], WIDTH - 300, 120);

		//上一次攻擊手段
		if (f1last_attack != f1this_attack)
		{
			f2being_hit_different_way = true;
			f1last_attack = f1this_attack;
		}
		else
			f2being_hit_different_way = false;
		if (f2last_attack != f2this_attack)
		{
			f1being_hit_different_way = true;
			f2last_attack = f2this_attack;
		}
		else
			f1being_hit_different_way = false;

		if (f1front_hit_operating && f1front_punch_hit_count)
			f1front_punch_hit_count = false;
		if (f2front_hit_operating && f2front_punch_hit_count)
			f2front_punch_hit_count = false;

		if ((f1front_punch_hit) && (abs(f1PosX - f2PosX + 250) < HITDISTANCE) && (abs(f1PosY - f2PosY + 250) < HITDISTANCE))
			f2being_hit = true;
		if ((f1far_punch_hit_weapon) && (abs(f1farpunchhitweaponPosX - f2PosX + 30) < HITDISTANCE) && (abs(f1farpunchhitweaponPosY - f2PosY + 50) < HITDISTANCE))
			f2being_hit = true;
		if ((f2front_punch_hit) && (abs(f1PosX - f2PosX + 250) < HITDISTANCE) && (abs(f1PosY - f2PosY + 250) < HITDISTANCE))
			f1being_hit = true;
		if ((f2far_punch_hit_weapon) && (abs(f2farpunchhitweaponPosX - f1PosX + 30) < HITDISTANCE) && (abs(f2farpunchhitweaponPosY - f1PosY + 50) < HITDISTANCE))
			f1being_hit = true;
		//判斷defend有沒有效
		if (f1defend_)
		{
			if (f1being_hit_different_way)
				f1defend_effective = false;
			else
				f1defend_effective = true;
		}
		else
		{
			if (f1being_hit)
				f1defend_effective = false;
		}

		if (f2defend_)
		{

			if (f2being_hit_different_way)
				f2defend_effective = false;
			else
				f2defend_effective = true;
		}
		else
		{
			if (f2being_hit)
				f2defend_effective = false;
		}

		if (f1front_punch_hit)
			f2being_hit_start_counting_time = true;

		//判斷要不要執行被打動畫&&扣血
		if (!f2defend_effective && f1front_punch_hit && ((abs(f1PosX - f2PosX) < HITDISTANCE) && (abs(f1PosY - f2PosY) < HITDISTANCE)))
		{
			if (f2being_hit_start_counting_time)
			{
				f2lasttime = f2curtime;
				f2timediv = 0;
				f2being_hit_start_counting_time = false;
			}


			if (f1front_punch_hit_count)
			{
				//f2HP -= 10;
				//f2HPwidth -= 5 / 500 * F2HPORWIDTH;
				Mix_PlayChannel(1, f2beinghiteffect, 0);
				f2HPIndex++;
				if (f1BUFFIndex < F1BUFFFRAME - 1)
					f1BUFFIndex++;

				//if(f2beingpunchhittimediv < 500)
				if (f1front_punch_hit_count)
					f2being_punch_hit_count = (f2being_punch_hit_count + 1) % 4;
			}


			if (f2timediv >= f2period - 50)
			{
				f2being_hit = false;
				f2being_hit_operating = false;
			}
			else
				f2being_hit_operating = true;



			if (f2standing_right)
				imgRender(renderer, f2beinghitright[f2beinghitrightIndex], f2PosX, f2PosY);

			if (f2standing_left)
				imgRender(renderer, f2beinghitleft[f2beinghitleftIndex], f2PosX, f2PosY);
		}
		else
			f2being_hit_operating = false;

		//判斷f1遠攻武器會不會傷害f2
		if (!f2defend_effective && f1far_punch_hit_weapon_effective && ((abs(f1farpunchhitweaponPosX - f2PosX) < HITDISTANCE) && (abs(f1farpunchhitweaponPosY - f2PosY) < HITDISTANCE)))
		{
			Mix_PlayChannel(1, f2beinghiteffect, 0);
			f2HPIndex++;
			if (f1BUFFIndex < F1BUFFFRAME - 1)
				f1BUFFIndex++;

			f1far_punch_hit_weapon_effective = false;

			if (f2being_hit_start_counting_time)
			{
				f2lasttime = f2curtime;
				f2timediv = 0;
				f2being_hit_start_counting_time = false;
			}
			if (f2timediv >= f2period - 50)
			{
				f2being_hit = false;
				f2being_hit_operating = false;
			}
			else
				f2being_hit_operating = true;

			if (f2standing_right)
				imgRender(renderer, f2beinghitright[f2beinghitrightIndex], f2PosX, f2PosY);

			if (f2standing_left)
				imgRender(renderer, f2beinghitleft[f2beinghitleftIndex], f2PosX, f2PosY);
		}

		if (f2front_punch_hit || f2far_punch_hit_weapon)
			f1being_hit_start_counting_time = true;

		//判斷要不要執行被打動畫&&扣血
		if (!f1defend_effective && f2front_punch_hit && ((abs(f1PosX - f2PosX) < HITDISTANCE) && (abs(f1PosY - f2PosY) < HITDISTANCE)))
		{
			if (f1being_hit_start_counting_time)
			{
				f1beinghitlasttime = f1beinghitcurtime;
				f1beinghittimediv = 0;
				f1being_hit_start_counting_time = false;
			}

			if (f2front_punch_hit_count)
			{
				//f1HP -= 10;
				//f1HPwidth -= 1 / 500 * F1HPORWIDTH;
				Mix_PlayChannel(1, f1beinghiteffect, 0);
				f1HPIndex++;
				if (f2BUFFIndex < F2BUFFFRAME - 1)
					f2BUFFIndex++;
				if (f2front_punch_hit_count)
					f2being_punch_hit_count = (f2being_punch_hit_count + 1) % 4;
			}

			if (f1beinghittimediv >= f1period - 50)
			{
				printf("f1beinghittimediv = %d\n", f1beinghittimediv);
				f1being_hit = false;
				f1being_hit_operating = false;
			}
			else
				f1being_hit_operating = true;

			if (f1standing_right)
				imgRender(renderer, f1beinghitright[f1beinghitrightIndex], f1PosX, f1PosY);

			if (f1standing_left)
				imgRender(renderer, f1beinghitleft[f1beinghitleftIndex], f1PosX, f1PosY);
		}
		else
			f1being_hit_operating = false;

		// 判斷f2遠攻武器會不會傷害f1
		// 問題:站很遠遠攻直接扣血
		if (!f1defend_effective && f2far_punch_hit_weapon_effective && ((abs(f2farpunchhitweaponPosX - f1PosX) < HITDISTANCE) && (abs(f2farpunchhitweaponPosY - f1PosY) < HITDISTANCE)))
		{
			Mix_PlayChannel(1, f1beinghiteffect, 0);
			f1HPIndex++;
			if (f2BUFFIndex < F2BUFFFRAME - 1)
			{
				f2BUFFIndex++;
				printf("buff = %d\n", f2BUFFIndex);
			}
			//f2BUFFIndex++;
			f2far_punch_hit_weapon_effective = false;

			if (f1being_hit_start_counting_time)
			{
				f1lasttime = f1curtime;
				f1timediv = 0;
				f1being_hit_start_counting_time = false;
			}
			if (f1timediv >= f1period - 50)
			{
				f1being_hit = false;
				f1being_hit_operating = false;
			}
			else
				f1being_hit_operating = true;

			if (f1standing_right)
				imgRender(renderer, f1beinghitright[f1beinghitrightIndex], f1PosX, f1PosY);

			if (f1standing_left)
				imgRender(renderer, f1beinghitleft[f1beinghitleftIndex], f1PosX, f1PosY);
		}


		if (f1front_punch_hit || f1far_punch_hit_weapon)
			f2being_hit_start_counting_time = true;

		//判斷要不要被擊飛
		if (f1being_punch_hit_count == 3)
		{
			f1being_punch_away = true;
			f1being_punch_hit_count = 0;
		}

		if (f2being_punch_hit_count == 3)
		{
			f2being_punch_away = true;
			f1being_punch_hit_count = 0;
		}

		//被擊飛
		/*if (f1being_punch_away)
		{
			f1PosX -= f1VelX * f1beingpunchawayspeed;
			if (f1timediv >= f1period - 50)
			{
				f1being_punch_away = false;
				f1being_punch_away_operating = false;
			}
			else
				f1being_punch_away_operating = true;
			if (f1standing_right)
			{
				f1beingpunchawayrightIndex = f1timediv * F1BEINGPUNCHAWAYRIGHTFRAME / f1period;
				imgRender(renderer, f1beingpunchawayright[f1beingpunchawayrightIndex], f1PosX, f1PosY);
			}

			if (f1standing_left)
			{
				f1beingpunchawayleftIndex = f1timediv * F1BEINGPUNCHAWAYLEFTFRAME / f1period;
				imgRender(renderer, f1beingpunchawayleft[f1beingpunchawayleftIndex], f1PosX, f1PosY);
			}
		}
		if (f2being_punch_away)
		{
			f1PosX -= f1VelX * f2beingpunchawayspeed;
			if (f2timediv >= f2period - 50)
			{
				f2being_punch_away = false;
				f2being_punch_away_operating = false;
			}
			else
				f2being_punch_away_operating = true;
			if (f2standing_right)
			{
				f2beingpunchawayrightIndex = f2timediv * F2BEINGPUNCHAWAYRIGHTFRAME / f2period;
				imgRender(renderer, f2beingpunchawayright[f2beingpunchawayrightIndex], f2PosX, f2PosY);
			}

			if (f2standing_left)
			{
				f2beingpunchawayleftIndex = f2timediv * F2BEINGPUNCHAWAYLEFTFRAME / f2period;
				imgRender(renderer, f2beingpunchawayleft[f2beingpunchawayleftIndex], f2PosX, f2PosY);
			}
		}*/

		//f1遠攻武器要不要跑
		if (f1far_punch_hit_weapon)
		{
			if (!f1far_punch_hit_operating)
			{
				f1farpunchhitweaponPosY = f1PosY;
				if (f1standing_left)
				{
					f1farpunchhitweaponPosX = f1PosX;
					f1farpunchhitweapondir = -1;
				}
				if (f1standing_right)
				{
					f1farpunchhitweaponPosX = f1PosX + 30;
					f1farpunchhitweapondir = 1;
				}
				f1far_punch_hit_operating = true;
			}
			else
				f1farpunchhitweaponPosX += F1FARPUNCHHITWEAPONVELOCITY * f1farpunchhitweapondir;

			if (f1farpunchhitweaponPosX > WIDTH || f1farpunchhitweaponPosX < 0)
			{
				f1far_punch_hit_weapon = false;
				f1far_punch_hit_operating = false;
			}

			if (f1standing_right)
				imgRender(renderer, f1farpunchhitweaponright, f1farpunchhitweaponPosX, f1farpunchhitweaponPosY);

			if (f1standing_left)
				imgRender(renderer, f1farpunchhitweaponleft, f1farpunchhitweaponPosX, f1farpunchhitweaponPosY);

		}

		if (initial)
		{
			f1standing_right = true;
			f2standing_left = true;
			initial = false;
		}

		//f1
		else if (f1front_punch_hit)
		{
			if (f1frontpunchhitspeedtimediv >= f1frontpunchhitspeedperiod - 50)
			{
				f1front_punch_hit = false;
				f1front_hit_operating = false;
				printf("f1timediv = %d", f1timediv);
			}
			else
				f1front_hit_operating = true;
			if (f1standing_right)
			{
				f1frontpunchhitrightIndex = f1frontpunchhitspeedtimediv * F1FRONTPUNCHHITRIGHTFRAME / f1frontpunchhitspeedperiod;
				imgRender(renderer, f1frontpunchhitright[f1frontpunchhitrightIndex], f1PosX, f1PosY);
			}

			if (f1standing_left)
			{
				f1frontpunchhitleftIndex = f1frontpunchhitspeedtimediv * F1FRONTPUNCHHITLEFTFRAME / f1frontpunchhitspeedperiod;
				imgRender(renderer, f1frontpunchhitleft[f1frontpunchhitleftIndex], f1PosX, f1PosY);
			}
		}

		else if (f1far_punch_hit)
		{
			if (f1farpunchhitspeedtimediv >= f1farpunchhitspeedperiod - 50)
			{
				f1far_punch_hit = false;
				f1far_punch_hit_operating = false;
				//printf("f1timediv = %d", f1timediv);
				f1far_punch_hit_weapon = true;

			}
			else
				f1far_punch_hit_operating = true;

			if (f1standing_right)
			{
				f1farpunchhitrightIndex = f1farpunchhitspeedtimediv * F1FARPUNCHHITRIGHTFRAME / f1farpunchhitspeedperiod;
				imgRender(renderer, f1farpunchhitright[f1farpunchhitrightIndex], f1PosX, f1PosY);
			}

			if (f1standing_left)
			{
				f1farpunchhitleftIndex = f1farpunchhitspeedtimediv * F1FARPUNCHHITLEFTFRAME / f1farpunchhitspeedperiod;
				imgRender(renderer, f1farpunchhitleft[f1farpunchhitleftIndex], f1PosX, f1PosY);
			}
		}

		else if (f1jump)
		{
			if (f1standing_right)
			{
				f1jumpingrightIndex = f1timediv * F1JUMPINGRIGHTFRAME / f1period;
				imgRender(renderer, f1jumpingright[f1jumpingrightIndex], f1PosX, f1PosY);
			}

			if (f1standing_left)
			{
				f1jumpingleftIndex = f1timediv * F1JUMPINGLEFTFRAME / f1period;
				imgRender(renderer, f1jumpingleft[f1jumpingleftIndex], f1PosX, f1PosY);
			}
		}

		else if (f1walking)
		{
			if ((f1timediv >= f1period - 50) && (!f1pressed_walking))
			{
				f1walking = false;
				f1walking_operating = false;
			}
			else
				f1walking_operating = true;
			if (f1standing_right)
			{
				f1walkingrightIndex = f1timediv * F1WALKINGRIGHTFRAME / f1period;
				imgRender(renderer, f1walkingright[f1walkingrightIndex], f1PosX, f1PosY);
			}

			if (f1standing_left)
			{
				f1walkingleftIndex = f1timediv * F1WALKINGLEFTFRAME / f1period;
				imgRender(renderer, f1walkingleft[f1walkingleftIndex], f1PosX, f1PosY);
			}

		}

		else if (f1defend_)
		{
			if ((f1timediv >= f1period - 50) && (!f1pressed_defend))
			{
				f1defend_ = false;
				f1defend_effective = false;
				f1defend_operating = false;
			}
			else
				f1defend_operating = true;
			if (f1standing_right)
			{
				f1defendrightIndex = f1timediv * F1DEFENDRIGHTFRAME / f1period;
				imgRender(renderer, f1defendright[f1defendrightIndex], f1PosX, f1PosY);
			}

			if (f1standing_left)
			{
				f1defendleftIndex = f1timediv * F1DEFENDLEFTFRAME / f1period;
				imgRender(renderer, f1defendleft[f1defendleftIndex], f1PosX, f1PosY);
			}
		}

		else if (!f1fucking_kill_triller && !f1fucking_kill_ex && !f1being_hit_operating && f1standing_right)
		{
			f1standingrightIndex = f1timediv * F1STANDINGRIGHTFRAME / f1period;
			imgRender(renderer, f1standingright[f1standingrightIndex], f1PosX, f1PosY);
		}

		else if (!f1fucking_kill_triller && !f1fucking_kill_ex && !f1being_hit_operating && f1standing_left)
		{
			f1standingleftIndex = f1timediv * F1STANDINGLEFTFRAME / f1period;
			imgRender(renderer, f1standingleft[f1standingleftIndex], f1PosX, f1PosY);
		}

		//f2遠攻武器要不要跑
		if (f2far_punch_hit_weapon)
		{
			if (!f2far_punch_hit_operating)
			{

				f2farpunchhitweaponPosY = f2PosY;
				if (f2standing_left)
				{
					f2farpunchhitweaponPosX = f2PosX;
					f2farpunchhitweapondir = -1;
				}
				if (f2standing_right)
				{
					f2farpunchhitweaponPosX = f2PosX + 30;
					f2farpunchhitweapondir = 1;
				}

				f2far_punch_hit_operating = true;
			}
			else
				f2farpunchhitweaponPosX += F2FARPUNCHHITWEAPONVELOCITY * f2farpunchhitweapondir;

			if (f2farpunchhitweaponPosX > WIDTH || f2farpunchhitweaponPosX < 0)
			{
				f2far_punch_hit_weapon = false;
				f2far_punch_hit_operating = false;
			}

			if (f2standing_right)
				imgRender(renderer, f2farpunchhitweaponright, f2farpunchhitweaponPosX, f2farpunchhitweaponPosY);

			if (f2standing_left)
				imgRender(renderer, f2farpunchhitweaponleft, f2farpunchhitweaponPosX, f2farpunchhitweaponPosY);
		}

		//f2
		if (f2front_punch_hit)
		{
			if (f2frontpunchhitspeedtimediv >= f2frontpunchhitspeedperiod - 50)
			{
				f2front_punch_hit = false;
				f2front_hit_operating = false;
			}
			else
				f2front_hit_operating = true;

			if (f2standing_right)
			{
				f2frontpunchhitrightIndex = f2frontpunchhitspeedtimediv * F2FRONTPUNCHHITRIGHTFRAME / f2frontpunchhitspeedperiod;
				imgRender(renderer, f2frontpunchhitright[f2frontpunchhitrightIndex], f2PosX, f2PosY);
			}

			if (f2standing_left)
			{
				f2frontpunchhitleftIndex = f2frontpunchhitspeedtimediv * F2FRONTPUNCHHITLEFTFRAME / f2frontpunchhitspeedperiod;
				imgRender(renderer, f2frontpunchhitleft[f2frontpunchhitleftIndex], f2PosX, f2PosY);
			}
		}

		else if (f2far_punch_hit)
		{
			if (f2farpunchhitspeedtimediv >= f2farpunchhitspeedperiod - 50)
			{
				f2far_punch_hit = false;
				f2far_punch_hit_operating = false;
				f2far_punch_hit_weapon = true;
			}
			else
				f2far_punch_hit_operating = true;

			if (f2standing_right)
			{
				f2farpunchhitrightIndex = f2farpunchhitspeedtimediv * F2FARPUNCHHITRIGHTFRAME / f2farpunchhitspeedperiod;
				imgRender(renderer, f2farpunchhitright[f2farpunchhitrightIndex], f2PosX, f2PosY);
			}

			if (f2standing_left)
			{
				f2farpunchhitleftIndex = f2farpunchhitspeedtimediv * F2FARPUNCHHITLEFTFRAME / f2farpunchhitspeedperiod;
				imgRender(renderer, f2farpunchhitleft[f2farpunchhitleftIndex], f2PosX, f2PosY);
			}
		}

		else if (f2jump)
		{
			if (f2standing_right)
			{
				f2jumpingrightIndex = f2timediv * F2JUMPINGRIGHTFRAME / f2period;
				imgRender(renderer, f2jumpingright[f2jumpingrightIndex], f2PosX, f2PosY);
			}

			if (f2standing_left)
			{
				f2jumpingleftIndex = f2timediv * F2JUMPINGLEFTFRAME / f2period;
				imgRender(renderer, f2jumpingleft[f2jumpingleftIndex], f2PosX, f2PosY);
			}
		}

		else if (f2walking)
		{
			if ((f2timediv >= f2period - 50) && (!f2pressed_walking))
			{
				f2walking = false;
				f2walking_operating = false;
			}
			else
				f2walking_operating = true;

			if (f2standing_right)
			{
				f2walkingrightIndex = f2timediv * F2WALKINGRIGHTFRAME / f2period;
				imgRender(renderer, f2walkingright[f2walkingrightIndex], f2PosX, f2PosY);
			}

			if (f2standing_left)
			{
				f2walkingleftIndex = f2timediv * F2WALKINGLEFTFRAME / f2period;
				imgRender(renderer, f2walkingleft[f2walkingleftIndex], f2PosX, f2PosY);
			}

		}

		else if (f2defend_)
		{
			if ((f2timediv >= f2period - 50) && (!f2pressed_defend))
			{
				f2defend_ = false;
				f2defend_effective = false;
				f2defend_operating = false;
			}
			else
				f2defend_operating = true;
			if (f2standing_right)
			{
				f2defendrightIndex = f2timediv * F2DEFENDRIGHTFRAME / f2period;
				imgRender(renderer, f2defendright[f2defendrightIndex], f2PosX, f2PosY);
			}

			if (f2standing_left)
			{
				f2defendleftIndex = f2timediv * F2DEFENDLEFTFRAME / f2period;
				imgRender(renderer, f2defendleft[f2defendleftIndex], f2PosX, f2PosY);
			}
		}

		else if (!f2fucking_kill_triller && !f2fucking_kill_ex && !f2being_hit_operating && f2standing_right)
		{
			f2standingrightIndex = f2timediv * F2STANDINGRIGHTFRAME / f2period;
			imgRender(renderer, f2standingright[f2standingrightIndex], f2PosX, f2PosY);
		}

		else if (!f2fucking_kill_triller && !f2fucking_kill_ex && !f2being_hit_operating && f2standing_left)
		{
			f2standingleftIndex = f2timediv * F2STANDINGLEFTFRAME / f2period;
			imgRender(renderer, f2standingleft[f2standingleftIndex], f2PosX, f2PosY);
			//printf("fuck left");
		}

		//f1大招前置
		if (f1fucking_kill_on && f1fucking_kill_triller)
		{
			fucking_operating = true;
			if (f1fuckingkilltrillerspeedtimediv >= f1fuckingkilltrillerspeedperiod - 50)
			{
				f1fucking_kill_triller = false;
				f1fucking_kill_triller_operating = false;
				//printf("f1timediv = %d", f1timediv);
				f1fucking_kill_ex = true;
				f1lasttime = f1curtime;
				f1fucking_kill_on = false;
				Mix_PlayChannel(1, f1fuckingkillexeffect, 0);
			}
			else
				f1fucking_kill_triller_operating = true;
		}

		if (f1fucking_kill_on && f1fucking_kill_triller && !f1fucking_kill_ex)
		{
			SDL_RenderClear(renderer);
			imgRender(renderer, f1fuckingkilltriller, 150, 0);
			SDL_RenderPresent(renderer);
			continue;
		}
		//f1大招施行
		if (f1fucking_kill_ex)
		{
			if (f1PosX - f2PosX > 0)
			{
				f1standing_left = true;
				f1standing_right = false;
			}
			if (f1PosX - f2PosX < 0)
			{
				f1standing_left = false;
				f1standing_right = true;
			}

			if (f1fuckingkillexspeedtimediv >= f1fuckingkillexspeedperiod - 100)
			{
				f1fucking_kill_ex = false;
				//printf("f1timediv = %d", f1timediv);
				f1fucking_kill_weapon = true;
				f1lasttime = f1curtime;
				Mix_PlayChannel(1, f1fuckingkillweaponeffect, 0);
			}

			if (f1standing_right)
			{
				f1fuckingkillexrightIndex = f1fuckingkillexspeedtimediv * F1FUCKINGKILLEXRIGHTFRAME / f1fuckingkillexspeedperiod;
				imgRender(renderer, f1fuckingkillexright[f1fuckingkillexrightIndex], f1PosX, f1PosY);
			}

			if (f1standing_left)
			{
				f1fuckingkillexleftIndex = f1fuckingkillexspeedtimediv * F1FUCKINGKILLEXLEFTFRAME / f1fuckingkillexspeedperiod;
				imgRender(renderer, f1fuckingkillexleft[f1fuckingkillexleftIndex], f1PosX, f1PosY);
			}
		}

		//f1大招武器
		if (f1fucking_kill_weapon)
		{
			f1fuckingkillweaponPosX = f2PosX - 105;
			f1fuckingkillweaponPosY = f2PosY - 30;
			if (f1fuckingkillweaponspeedtimediv >= f1fuckingkillweaponspeedperiod - 50)
			{
				f1fucking_kill_weapon = false;
				fucking_operating = false;
				f2HPIndex += 2;
				f1BUFFIndex = 0;
			}
			if (f1fuckingkillweaponIndex == 1)
				f1fuckingkillweaponPosY -= 100;
			f1fuckingkillweaponIndex = f1fuckingkillweaponspeedtimediv * F1FUCKINGKILLWEAPONFRAME / f1fuckingkillweaponspeedperiod;
			imgRender(renderer, f1fuckingkillweapon[f1fuckingkillweaponIndex], f1fuckingkillweaponPosX, f1fuckingkillweaponPosY);

		}

		//f2大招前置

		if (f2fucking_kill_on && f2fucking_kill_triller)
		{
			fucking_operating = true;
			if (f2fuckingkilltrillerspeedtimediv >= f2fuckingkilltrillerspeedperiod - 50)
			{
				f2fucking_kill_triller = false;
				f2fucking_kill_triller_operating = false;
				//printf("f2timediv = %d", f2timediv);
				f2fucking_kill_ex = true;
				f2lasttime = f2curtime;
				f2fucking_kill_on = false;
				Mix_PlayChannel(1, f2fuckingkillexeffect, 0);
			}
			else
				f2fucking_kill_triller_operating = true;
		}
		if (f2fucking_kill_on && f2fucking_kill_triller && !f2fucking_kill_ex)
		{
			SDL_RenderClear(renderer);
			imgRender(renderer, f2fuckingkilltriller, 150, 0);
			SDL_RenderPresent(renderer);
			continue;
		}


		//f2大招施行
		if (f2fucking_kill_ex)
		{
			if (f2PosX - f1PosX > 0)
			{
				f2standing_left = true;
				f2standing_right = false;
			}
			if (f2PosX - f1PosX < 0)
			{
				f2standing_left = false;
				f2standing_right = true;
			}


			if (f2fuckingkillexspeedtimediv >= f2fuckingkillexspeedperiod - 100)
			{
				f2fucking_kill_ex = false;
				//printf("f2timediv = %d", f2timediv);
				f2fucking_kill_weapon = true;
				f2lasttime = f2curtime;
				Mix_PlayChannel(1, f2fuckingkillweaponeffect, 0);
			}
			else
				f2fucking_kill_triller_operating = true;

			if (f2standing_right)
			{
				f2fuckingkillexrightIndex = f2fuckingkillexspeedtimediv * F2FUCKINGKILLEXRIGHTFRAME / f2fuckingkillexspeedperiod;
				imgRender(renderer, f2fuckingkillexright[f2fuckingkillexrightIndex], f2PosX, f2PosY);
			}

			if (f2standing_left)
			{
				f2fuckingkillexleftIndex = f2fuckingkillexspeedtimediv * F2FUCKINGKILLEXLEFTFRAME / f2fuckingkillexspeedperiod;
				imgRender(renderer, f2fuckingkillexleft[f2fuckingkillexleftIndex], f2PosX, f2PosY);
			}
		}
		//f2大招武器
		if (f2fucking_kill_weapon)
		{

			f2fuckingkillweaponPosX = f1PosX - 105;
			f2fuckingkillweaponPosY = f1PosY - 30;
			if (f2fuckingkillweaponspeedtimediv >= f2fuckingkillweaponspeedperiod - 50)
			{
				f2fucking_kill_weapon = false;
				fucking_operating = false;
				f1HPIndex += 2;
				f2BUFFIndex = 0;
			}
			if (f2fuckingkillweaponIndex == 1)
				f2fuckingkillweaponPosY -= 100;
			f2fuckingkillweaponIndex = f2fuckingkillweaponspeedtimediv * F2FUCKINGKILLWEAPONFRAME / f2fuckingkillweaponspeedperiod;
			imgRender(renderer, f2fuckingkillweapon[f2fuckingkillweaponIndex], f2fuckingkillweaponPosX, f2fuckingkillweaponPosY);
		}


		/*
		//f1 win
		if (f2HPIndex == F2HPFRAME-1)
		{
//			f1HPempty = true;
//			if (f1HPempty)
			{
				printf("Yes!!\n");
				imgRender(renderer, f1win[f1winIndex], restartPosX, restartPosY);
			}
		}

		//f2 win
		if (f2HPIndex == F2HPFRAME - 1)
		{
//			f2HPempty = true;
//			if (f2HPempty)
			{
				printf("Yes!!\n");
				imgRender(renderer, f2win[f2winIndex], restartPosX, restartPosY);
			}
		}

		*/

		switch (mouseState)
		{
		case NONE:
			break;

		case OUT: // Mouse out
			break;

		case IN_LB_SC:  // Inside, Left Button, Single Click
			printf("Inside, Left Button, Single Click: (%d, %d)\n", mouseX, mouseY);
			clickmenu = true;
			break;

		case IN_RB_SC:  // Inside, RIGHT Button, Single Click
			printf("Inside, Right Button, Single Click: (%d, %d)\n", mouseX, mouseY);
			break;

		case IN_LB_DC:  // Inside, Left Button, Double Click
			printf("Inside, Left Button, Double Click: (%d, %d)\n", mouseX, mouseY);
			break;

		case IN_RB_DC:  // Inside, RIGHT Button, Double Click
			printf("Inside, Right Button, Double Click: (%d, %d)\n", mouseX, mouseY);
			break;

		case IN_LB_PR:  // Inside, Left Button, Press
			printf("Inside, Left Button, Press: (%d, %d)\n", mouseX, mouseY);
			break;

		case IN_RB_PR:  // Inside, Left Button, Press
			printf("Inside, Right Button, Press: (%d, %d)\n", mouseX, mouseY);
			break;

		case IN_WU:  // Inside, Wheel UP
			printf("Inside, Wheel Up: y=%d\n", mouseY);
			break;

		case IN_WD:  // Inside, Wheel DOWN
			printf("Inside, Wheel Down: y=%d\n", mouseY);
			break;

		case HOVER: // Mouse hover
			printf("Inside, Hover: (%d, %d)\n", mouseX, mouseY);
			if (menu_finish && mouseX>restartPosX && mouseX<restartPosX + restartwidth && mouseY>restartPosY && mouseY < restartPosY+restartheight)
			{
				printf("Yes!!\n");
				imgRender(renderer, restart2, restartPosX, restartPosY);
//				hoverrestart = true;
			}
			break;
		}

		if (menu_initial)
		{
			imgRender(renderer, menu, 0, 0);
			tapIndex = tapspeedtimediv * TAPFRAME / tapspeedperiod;
			printf("tapIndex = %d\n", tapIndex);
			imgRender(renderer, tap[tapIndex], 320, 510);
			

			if (clickmenu )
			{
				menu_initial = false;
				menu_finish = true;
			}
		}

			restart1 = loadTexture(restart1Path, false, 0xFF, 0xFF, 0xFF);


/*		if (hoverrestart)
		{
			printf("Yes!!\n");
			char st[100];
			sprintf_s(st, 100, "%s%d.png", restartPath, 1);
			restart[1] = loadTexture(st, true, 0xFF, 0xFF, 0xFF);
		}
	*/	


		// Update screen
		if (f1finishf2 || f2finishf1)
		{
			//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
			SDL_RenderClear(renderer);
			if (f1finishf2)
				quit = true;
			//textRender(renderer, textfinish1, (WIDTH - textfinish1.width) / 2, 50, NULL, NULL, NULL, SDL_FLIP_NONE);
			if (f2finishf1)
				quit = true;
			//textRender(renderer, textfinish2, (WIDTH - textfinish2.width) / 2, 50, NULL, NULL, NULL, SDL_FLIP_NONE);

		//textRender(renderer, text, (WIDTH - text.width) / 2, 100, NULL, NULL, NULL, SDL_FLIP_NONE);

		}
		SDL_RenderPresent(renderer);


		//debug
		/*
		printf("f1this_attack = %c f2this_attack = %c\n", f1this_attack, f2this_attack);

		if (f1being_hit_different_way)
		{
			printf("f1being_hit_different_way = true\n");
		}

		else
			printf("f1being_hit_different_way = false\n");
		if (f2being_hit_different_way)
		{
			printf("f2being_hit_different_way = true\n");
		}

		else
			printf("f2being_hit_different_way = false\n");
		if (f1defend_)
			printf("f1defend_ = true\n");
		else
			printf("f1defend_ = false\n");
		if (f2defend_)
			printf("f2defend_ = true\n");
		else
			printf("f2defend_ = false\n");
		if (f1defend_effective)
		{
			printf("f1defend_effective = true\n");
			getchar();
		}

		else
		{
			printf("f1defend_effective = false\n");
		}


		if (f2defend_effective)
		{
			printf("f2defend_effective = true\n");
			getchar();
		}

		else
		{
			printf("f2defend_effective = false\n");

		}


		if (f1HP < 500 || f2HP < 500)
			getchar();
		printf("f1HP = %d   f2HP = %d\n", f1HP, f2HP);

		if ((f1front_punch_hit) && (abs(f1PosX - f2PosX + 250) < 500) && (abs(f1PosY - f2PosY + 250) < 500))
			getchar();
		if ((f2front_punch_hit) && (abs(f1PosX - f2PosX + 250) < 500) && (abs(f1PosY - f2PosY + 250) < 500))
			getchar();*/

	}
	//SDL_StopTextInput();

	//Free resources and close SDL
	closeSDL();

	return 0;
}