/**
    Clover-UI
    Source.cpp
    Purpose: Launches ROMs with a user interface similar to Clover-UI

    @author SamTheBreadman
    @version 1.0 10/9/17 
*/

/* Include standard libraries */
#include <fstream>
#include <string>
#include <vector>
using namespace std;

/* Include SDL libraries */
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

/* Define structs */
struct config_item 
{
	string filename;
	string title;
	string publisher;
	string release_date;
	string number_of_players;
	SDL_Texture *boxart;
};

/* Include function prototypes */
void read_config(vector<config_item> &config,SDL_Renderer *renderer);

/* Begin main program execution */	
int main(int argc, char *argv[])  
{
	/* Initalize video, image, and audio libraries */
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	IMG_Init(IMG_INIT_PNG);
	Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);

	/* Initalize and declare window and renderer */
	SDL_Window *window = SDL_CreateWindow("Clover-UI",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,1280,720,0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	SDL_RenderClear(renderer);
	
	/* Initialize and declare key states */
	enum key_states {is_released, is_pressed};
	key_states left_key_state = is_released;
	key_states right_key_state = is_released;
	
	/* Initialize and declare main program states */
	enum main_states {is_in_carousel, is_exiting};
	main_states main_state = is_in_carousel;
	
	/* Initalize and declare pulsing cursor states */
	enum cursor_pulse_states {is_pulsing_out, is_pulsing_in};
	cursor_pulse_states cursor_pulse_state = is_pulsing_out;
	
	/* Initialize and declare music states */
	enum music_states {is_in_intro, is_in_loop};
	music_states music_state = is_in_intro;
	
	/* Initialize and declare indexes */
	int game_index = 0;
	int boxart_index = 0;
	int carousel_slot_index = 0;
	
	/* Initialize and declare timers */
	float carousel_horizontal_movement_timer = 1;
	int cursor_hold_timer = 0;
	int key_hold_timer = 0;
	
	/* Initialize and declare positions */
	float wallpaper_position = 0;
	float carousel_horizontal_position = 0;
	float carousel_horizontal_destination_position = 0;
	float cursor_horizontal_position = 0;
	
	/* Initialize and declare offsets */
	float carousel_offset = 0;

	/* Initialize and declare speeds */
	float wallpaper_speed = 0.75;
	float carousel_horizontal_speed = 0.025;
	float carousel_slot_alpha_speed = 10;
	float cursor_pulse_speed = 0;
	
	/* Initialize and declare alphas */
	float far_left_carousel_slot_alpha = 0;
	float left_carousel_slot_alpha = 255;
	float middle_left_carousel_slot_alpha = 0;
	float middle_right_carousel_slot_alpha = 0;
	float right_carousel_slot_alpha = 0;
	float far_right_carousel_slot_alpha = 0;
	
	/* Initialize and declare color channels */
	float cursor_red_color_channel = 255;
	float cursor_green_color_channel = 255;
	
	/* Initalize and declare vectors */
	vector<config_item> config;
	read_config(config, renderer);
	
	/* Initialize and declare source rectangles */
	SDL_Rect topBorderSourceRectangle = {3,49,428,34};
	SDL_Rect topBorderDestinationRectangle = {-2,-6,1284,102};
	SDL_Rect topBorderInsetSourceRectangle = {502,397,10,22};
	SDL_Rect topBorderInsetDestinationRectangle = {400,30,480,66};
	SDL_Rect displayIconSourceRectangle = {29,1002,21,17};
	SDL_Rect displayIconDestinationRectangle = {418,38,63,51};
	SDL_Rect optionsIconSourceRectangle = {82,998,19,19};
	SDL_Rect optionsIconDestinationRectangle = {517,35,57,57};
	SDL_Rect languageIconSourceRectangle = {56,1001,18,19};
	SDL_Rect languageIconDestinationRectangle = {613,35,54,57};
	SDL_Rect legalNoticesIconSourceRectangle = {5,1001,17,19};
	SDL_Rect legalNoticesIconDestinationRectangle = {712,35,51,57};
	SDL_Rect manualsIconSourceRectangle = {242,850,19,19};
	SDL_Rect manualsIconDestinationRectangle = {805,35,57,57};
	SDL_Rect titleBorderSourceRectangle = {4,327,342,18};
	SDL_Rect titleBorderDestinationRectangle = {127,151,1026,54};
	SDL_Rect deselectedGameBorderSourceRectangle = {417,379,84,92};
	SDL_Rect selectedGameBorderSourceRectangle = {409,473,84,92};
	SDL_Rect topLeftCursorBorderSourceRectangle = {113,951,23,23};
	SDL_Rect cursorBorderSourceRectangle = {136,863,6,6};
	SDL_Rect topRightCursorBorderSourceRectangle = {121,905,23,23};
	SDL_Rect bottomRightCursorBorderSourceRectangle = {144,808,23,23};
	SDL_Rect bottomLeftCursorBorderSourceRectangle = {200,894,23,23};
	SDL_Rect dpadUpInstructionIconSourceRectangle = {85,881,12,12};
	SDL_Rect dpadUpInstructionIconDestinationRectangle = {278,591,36,36};
	SDL_Rect dpadDownInstructionIconSourceRectangle = {57,881,12,12};
	SDL_Rect dpadDownInstructionIconDestinationRectangle = {400,591,36,36};
	SDL_Rect selectInstructionIconSourceRectangle  = {88,930,31,9};
	SDL_Rect selectInstructionIconDestinationRectangle = {640,597,93,27};
	SDL_Rect startInstructionIconSourceRectangle = {481,784,31,9};
	SDL_Rect startInstructionIconDestinationRectangle = {809,597,93,27};
	SDL_Rect spinningArrowSourceRectangle = {146,882,10,6};
	SDL_Rect spinningArrowDestinationRectangle = {215,517,30,18};
	SDL_Rect testThumbnailsDestinationRectangle = {210,538,860,28};
	SDL_Rect bottomBorderSourceRectangle = {3,1,428,30};
	SDL_Rect bottomBorderDestinationRectangle = {-2,630,1284,90};
	
	/* Initalize and declare surfaces */
	SDL_Surface *wallpaperSurface = IMG_Load("Resources/Images/wallpaperImage.png");
	SDL_Surface *userInterfaceSurface = IMG_Load("Resources/Images/userInterfaceImage.png");
	SDL_Surface *testThumbnailsSurface = IMG_Load("Resources/Images/testThumbnailsImage.png");
	
	/* Initalize and declare textures */
	SDL_Texture *wallpaperTexture = SDL_CreateTextureFromSurface(renderer,wallpaperSurface);
	SDL_Texture *userInterfaceTexture = SDL_CreateTextureFromSurface(renderer,userInterfaceSurface);
	SDL_Texture *testThumbnailsTexture = SDL_CreateTextureFromSurface(renderer,testThumbnailsSurface);

	/* Initialize and declare music and sounds */
	Mix_Music *bootBackgroundMusic = Mix_LoadMUS("Resources/Sounds/bootBackgroundMusic.wav");
	Mix_Music *homeBackgroundMusic = Mix_LoadMUS("Resources/Sounds/homeBackgroundMusic.wav");
	Mix_Chunk *cursorSound = Mix_LoadWAV( "Resources/Sounds/cursorSound.wav" );

	/* Begin main loop */
	while(main_state != is_exiting)
	{
		// Gather inputs and change values
		SDL_Event event;
		SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
		{
			main_state = is_exiting;
		}
		if (event.type == SDL_KEYDOWN)
		{
			int keySymbol = event.key.keysym.sym;
			if (keySymbol == SDLK_ESCAPE)
			{
				main_state = is_exiting;
			}
			if (keySymbol == SDLK_LEFT)
			{
				if (main_state == is_in_carousel)
				{
					if (left_key_state == is_released)
					{
						Mix_PlayChannel( -1, cursorSound, 0 );
						if (carousel_slot_index != 0)
						{
							carousel_slot_index -= 1;
						}
						else
						{
							carousel_horizontal_movement_timer=0;
							carousel_horizontal_speed = 0.025;
							far_left_carousel_slot_alpha = 0;
							left_carousel_slot_alpha = 0;
							middle_left_carousel_slot_alpha = 255;
							middle_right_carousel_slot_alpha = 0;
							right_carousel_slot_alpha = 0;
							far_right_carousel_slot_alpha = 0;
							carousel_offset -=262;
							boxart_index -= 1;
							carousel_horizontal_destination_position -= 1;
						}
						game_index -= 1;
						key_hold_timer = SDL_GetTicks();
						left_key_state = is_pressed;
					}
					if (SDL_GetTicks() > (key_hold_timer + 175))
					{
						if (cursor_hold_timer == 6)
						{
							Mix_PlayChannel( -1, cursorSound, 0 );
							if (carousel_slot_index != 0)
							{
								carousel_slot_index -= 1;
							}
							else
							{
								carousel_horizontal_movement_timer=0;
								carousel_horizontal_speed = 0.05;
								carousel_slot_alpha_speed = 5;
								far_left_carousel_slot_alpha = 0;
								left_carousel_slot_alpha = 0;
								middle_left_carousel_slot_alpha = 255;
								middle_right_carousel_slot_alpha = 0;
								right_carousel_slot_alpha = 0;
								far_right_carousel_slot_alpha = 0;
								carousel_offset -=262;
								boxart_index -= 1;
								carousel_horizontal_destination_position -= 1;
							}
							game_index -= 1;
							cursor_hold_timer = 0;
						}
						else
						{
							cursor_hold_timer += 1;
						}
					}	
				}
			}
			if (keySymbol == SDLK_RIGHT)
			{
				if (main_state == is_in_carousel)
				{
					if (right_key_state == is_released)
					{
						Mix_PlayChannel( -1, cursorSound, 0 );
						if (carousel_slot_index != 3)
						{
							carousel_slot_index += 1;
						}
						else
						{
							carousel_horizontal_movement_timer=0;
							carousel_horizontal_speed = 0.025;
							far_left_carousel_slot_alpha = 0;
							left_carousel_slot_alpha = 0;
							middle_left_carousel_slot_alpha = 0;
							middle_right_carousel_slot_alpha = 255;
							right_carousel_slot_alpha = 0;
							far_right_carousel_slot_alpha = 0;
							carousel_offset +=262;
							boxart_index += 1;
							carousel_horizontal_destination_position += 1;
						}
						game_index += 1;
						key_hold_timer = SDL_GetTicks();
						right_key_state = is_pressed;
					}
					if (SDL_GetTicks() > (key_hold_timer + 175))
					{
						if (cursor_hold_timer == 6)
						{
							Mix_PlayChannel( -1, cursorSound, 0 );
							if (carousel_slot_index != 3)
							{
								carousel_slot_index += 1;
							}
							else
							{
								carousel_horizontal_movement_timer=0;
								carousel_horizontal_speed = 0.05;
								carousel_slot_alpha_speed = 5;
								far_left_carousel_slot_alpha = 0;
								left_carousel_slot_alpha = 0;
								middle_left_carousel_slot_alpha = 0;
								middle_right_carousel_slot_alpha = 255;
								right_carousel_slot_alpha = 0;
								far_right_carousel_slot_alpha = 0;
								carousel_offset +=262;
								boxart_index += 1;
								carousel_horizontal_destination_position += 1;
							}
							game_index += 1;
							cursor_hold_timer = 0;
						}
						else
						{
							cursor_hold_timer += 1;
						}
					}
				}
			}
		}
		else
		{
			if (main_state == is_in_carousel)
			{
				carousel_horizontal_speed = 0.025;
				carousel_slot_alpha_speed = 10;
			    cursor_hold_timer = 0;
				left_key_state = is_released;
				right_key_state = is_released;
			}
		}
		
		// Change values based on other values
		if (main_state == is_in_carousel)
		{
			if (carousel_horizontal_movement_timer>=0 && carousel_horizontal_movement_timer<1)
			{
				carousel_horizontal_movement_timer+=carousel_horizontal_speed;
			}
			if (carousel_horizontal_movement_timer>1)
			{
				carousel_horizontal_movement_timer=1;
			}
			if (game_index == -1)
			{
				game_index = 20;
			}
			else if (game_index == 21)
			{
				game_index = 0;
			}
			if (carousel_slot_index == 0)
			{
				carousel_horizontal_position =  (carousel_horizontal_destination_position*-262 - carousel_horizontal_position+0.1)*carousel_horizontal_movement_timer + carousel_horizontal_position;
				if (cursor_horizontal_position < 0.5 && cursor_horizontal_position > -0.5)
				{
					cursor_horizontal_position = 0;
				}
				cursor_horizontal_position += (0 - cursor_horizontal_position) / 4;
				
				far_left_carousel_slot_alpha += (0 - far_left_carousel_slot_alpha) / carousel_slot_alpha_speed;
				left_carousel_slot_alpha += (255 - left_carousel_slot_alpha) / carousel_slot_alpha_speed;
				middle_left_carousel_slot_alpha += (0 - middle_left_carousel_slot_alpha) / carousel_slot_alpha_speed;
				middle_right_carousel_slot_alpha += (0 - middle_right_carousel_slot_alpha) / carousel_slot_alpha_speed;
				right_carousel_slot_alpha += (0 - right_carousel_slot_alpha) / carousel_slot_alpha_speed;
				far_right_carousel_slot_alpha += (0 - far_right_carousel_slot_alpha) / carousel_slot_alpha_speed;
			}
			if (carousel_slot_index == 1)
			{
				carousel_horizontal_position =  (carousel_horizontal_destination_position*-262 - carousel_horizontal_position+0.1)*carousel_horizontal_movement_timer + carousel_horizontal_position;
				if (cursor_horizontal_position < 262.5 && cursor_horizontal_position > 261.5)
				{
					cursor_horizontal_position = 262;
				}
				cursor_horizontal_position += (262 - cursor_horizontal_position) / 4;
				
				far_left_carousel_slot_alpha += (0 - far_left_carousel_slot_alpha) / carousel_slot_alpha_speed;
				left_carousel_slot_alpha += (0 - left_carousel_slot_alpha) / carousel_slot_alpha_speed;
				middle_left_carousel_slot_alpha += (255 - middle_left_carousel_slot_alpha) / carousel_slot_alpha_speed;
				middle_right_carousel_slot_alpha += (0 - middle_right_carousel_slot_alpha) / carousel_slot_alpha_speed;
				right_carousel_slot_alpha += (0 - right_carousel_slot_alpha) / carousel_slot_alpha_speed;
				far_right_carousel_slot_alpha += (0 - far_right_carousel_slot_alpha) / carousel_slot_alpha_speed;
			}
			if (carousel_slot_index == 2)
			{
				carousel_horizontal_position =  (carousel_horizontal_destination_position*-262 - carousel_horizontal_position+0.1)*carousel_horizontal_movement_timer + carousel_horizontal_position;
				if (cursor_horizontal_position < 524.5 && cursor_horizontal_position > 523.5)
				{
					cursor_horizontal_position = 524;
				}
				cursor_horizontal_position += (524 - cursor_horizontal_position) / 4;
				far_left_carousel_slot_alpha += (0 - far_left_carousel_slot_alpha) / carousel_slot_alpha_speed;
				left_carousel_slot_alpha += (0 - left_carousel_slot_alpha) / carousel_slot_alpha_speed;
				middle_left_carousel_slot_alpha += (0 - middle_left_carousel_slot_alpha) / carousel_slot_alpha_speed;
				middle_right_carousel_slot_alpha += (255 - middle_right_carousel_slot_alpha) / carousel_slot_alpha_speed;
				right_carousel_slot_alpha += (0 - right_carousel_slot_alpha) / carousel_slot_alpha_speed;
				far_right_carousel_slot_alpha += (0 - far_right_carousel_slot_alpha) / carousel_slot_alpha_speed;
			}
			if (carousel_slot_index == 3)
			{
				carousel_horizontal_position =  (carousel_horizontal_destination_position*-262 - carousel_horizontal_position+0.1)*carousel_horizontal_movement_timer + carousel_horizontal_position;
				if (cursor_horizontal_position < 786.5 && cursor_horizontal_position > 785.5)
				{
					cursor_horizontal_position = 786;
				}
				cursor_horizontal_position += (786 - cursor_horizontal_position) / 4;
				far_left_carousel_slot_alpha += (0 - far_left_carousel_slot_alpha) / carousel_slot_alpha_speed;
				left_carousel_slot_alpha += (0 - left_carousel_slot_alpha) / carousel_slot_alpha_speed;
				middle_left_carousel_slot_alpha += (0 - middle_left_carousel_slot_alpha) / carousel_slot_alpha_speed;
				middle_right_carousel_slot_alpha += (0 - middle_right_carousel_slot_alpha) / carousel_slot_alpha_speed;
				right_carousel_slot_alpha += (255 - right_carousel_slot_alpha) / carousel_slot_alpha_speed;
				far_right_carousel_slot_alpha += (0 - far_right_carousel_slot_alpha) / carousel_slot_alpha_speed;
			}
		}
		if (wallpaper_position<=-1561)
		{
			wallpaper_position += 1561-wallpaper_speed;
		}
		else if (wallpaper_position >= 1561)
		{
			wallpaper_position -=1561-wallpaper_speed;
		}
		else
		{
			wallpaper_position -= wallpaper_speed;
		}
		if (cursor_pulse_state == is_pulsing_out)
		{
			cursor_pulse_speed += 0.05;
			if (cursor_red_color_channel - cursor_pulse_speed > 63)
			{
				cursor_red_color_channel = cursor_red_color_channel - cursor_pulse_speed;
			}
			else
			{
				cursor_red_color_channel = 63;
			}
			if (cursor_green_color_channel - cursor_pulse_speed > 191)
			{
				cursor_green_color_channel = cursor_green_color_channel - cursor_pulse_speed;
			}
			else
			{
				cursor_green_color_channel = 191;
			}
			if (cursor_pulse_speed >= 3)
			{

				cursor_pulse_speed = 3;
				cursor_pulse_state = is_pulsing_in;
			}
		}
		else if (cursor_pulse_state == is_pulsing_in)
		{
			cursor_pulse_speed -= 0.05;
			if (cursor_red_color_channel + cursor_pulse_speed < 255)
			{
				cursor_red_color_channel = cursor_red_color_channel + cursor_pulse_speed;
			}
			else
			{
				cursor_red_color_channel == 255;
			}
			if (cursor_green_color_channel + cursor_pulse_speed < 255)
			{
				cursor_green_color_channel = cursor_green_color_channel + cursor_pulse_speed;
			}
			else
			{
				cursor_green_color_channel = 255;
			}
			if (cursor_pulse_speed <= 0)
			{
				cursor_pulse_speed = 0;
				cursor_pulse_state = is_pulsing_out;
			}
		}
		int spinningArrowAnimationFrameCounter = SDL_GetTicks();
		int spinningArrowAnimationFrame = (spinningArrowAnimationFrameCounter / 115) % 4;
		if ((spinningArrowAnimationFrameCounter / 115) % 4==3)
		{
			spinningArrowAnimationFrame = 1;
		}
		SDL_Rect spinningArrowSourceRectangle = {146+spinningArrowAnimationFrame*14,882,10,6};
		SDL_Rect spinningArrowDestinationRectangle = {215+(game_index*41),517,30,18};
		
		// Define and update destination rectangles
		SDL_Rect farLeftWallpaperDestinationRectangle = {wallpaper_position-(1561*2),80,1561,564};
		SDL_Rect leftWallpaperDestinationRectangle = {wallpaper_position-1561,80,1561,564};
		SDL_Rect middleWallpaperDestinationRectangle = {wallpaper_position,80,1561,564};
		SDL_Rect rightWallpaperDestinationRectangle = {wallpaper_position+1561,80,1561,564};
		SDL_Rect farRightWallpaperDestinationRectangle = {wallpaper_position+(1561*2),80,1561,564};
		SDL_Rect leftOffscreenDeselectedGameBorderDestinationRectangle = {-403+carousel_horizontal_position+carousel_offset,222,252,276};
		SDL_Rect firstDeselectedGameBorderDestinationRectangle = {-141+carousel_horizontal_position+carousel_offset,222,252,276};
		SDL_Rect secondDeselectedGameBorderDestinationRectangle = {121+carousel_horizontal_position+carousel_offset,222,252,276};
		SDL_Rect thirdDeselectedGameBorderDestinationRectangle = {383+carousel_horizontal_position+carousel_offset,222,252,276};
		SDL_Rect fourthDeselectedGameBorderDestinationRectangle = {645+carousel_horizontal_position+carousel_offset,222,252,276};
		SDL_Rect fifthDeselectedGameBorderDestinationRectangle = {907+carousel_horizontal_position+carousel_offset,222,252,276};
		SDL_Rect sixthDeselectedGameBorderDestinationRectangle = {1169+carousel_horizontal_position+carousel_offset,222,252,276};
		SDL_Rect rightOffscreenDeselectedGameBorderDestinationRectangle = {1431+carousel_horizontal_position+carousel_offset,222,252,276};
		SDL_Rect leftOffscreenBoxartDestinationRectangle = {-391+carousel_horizontal_position+carousel_offset,256,228,160};
		SDL_Rect firstBoxartDestinationRectangle = {-129+carousel_horizontal_position+carousel_offset,256,228,160};
		SDL_Rect secondBoxartDestinationRectangle = {133+carousel_horizontal_position+carousel_offset,256,228,160};
		SDL_Rect thirdBoxartDestinationRectangle = {395+carousel_horizontal_position+carousel_offset,256,228,160};
		SDL_Rect fourthBoxartDestinationRectangle = {657+carousel_horizontal_position+carousel_offset,256,228,160};
		SDL_Rect fifthBoxartDestinationRectangle = {919+carousel_horizontal_position+carousel_offset,256,228,160};
		SDL_Rect sixthBoxartDestinationRectangle = {1181+carousel_horizontal_position+carousel_offset,256,228,160};
		SDL_Rect rightOffscreenBoxartDestinationRectangle = {1443+carousel_horizontal_position+carousel_offset,256,228,160};
		SDL_Rect topLeftCursorBorderDestinationRectangle = {118+cursor_horizontal_position,219,23,23};
		SDL_Rect topCursorBorderDestinationRectangle = {141+cursor_horizontal_position,219,212,6};
		SDL_Rect topRightCursorBorderDestinationRectangle = {353+cursor_horizontal_position,219,23,23};	
		SDL_Rect rightCursorBorderDestinationRectangle = {370+cursor_horizontal_position,242,6,236};	
		SDL_Rect bottomRightCursorBorderDestinationRectangle = {353+cursor_horizontal_position,478,23,23};	
		SDL_Rect bottomCursorBorderDestinationRectangle = {141+cursor_horizontal_position,495,212,6};	
		SDL_Rect bottomLeftCursorBorderDestinationRectangle = {118+cursor_horizontal_position,478,23,23};
		SDL_Rect leftCursorBorderDestinationRectangle = {118+cursor_horizontal_position,242,6,236};
		
		// Clear the screen back buffer
		SDL_RenderClear(renderer);
		
		// Copy our textures onto the main renderer
		SDL_RenderCopy(renderer,wallpaperTexture,NULL,&farLeftWallpaperDestinationRectangle);
		SDL_RenderCopy(renderer,wallpaperTexture,NULL,&leftWallpaperDestinationRectangle);
		SDL_RenderCopy(renderer,wallpaperTexture,NULL,&middleWallpaperDestinationRectangle);
		SDL_RenderCopy(renderer,wallpaperTexture,NULL,&rightWallpaperDestinationRectangle);
		SDL_RenderCopy(renderer,wallpaperTexture,NULL,&farRightWallpaperDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&topBorderSourceRectangle,&topBorderDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&topBorderInsetSourceRectangle,&topBorderInsetDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&displayIconSourceRectangle,&displayIconDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&optionsIconSourceRectangle,&optionsIconDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&languageIconSourceRectangle,&languageIconDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&legalNoticesIconSourceRectangle,&legalNoticesIconDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&manualsIconSourceRectangle,&manualsIconDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&titleBorderSourceRectangle,&titleBorderDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&deselectedGameBorderSourceRectangle,&leftOffscreenDeselectedGameBorderDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&deselectedGameBorderSourceRectangle,&firstDeselectedGameBorderDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&deselectedGameBorderSourceRectangle,&secondDeselectedGameBorderDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&deselectedGameBorderSourceRectangle,&thirdDeselectedGameBorderDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&deselectedGameBorderSourceRectangle,&fourthDeselectedGameBorderDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&deselectedGameBorderSourceRectangle,&fifthDeselectedGameBorderDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&deselectedGameBorderSourceRectangle,&sixthDeselectedGameBorderDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&deselectedGameBorderSourceRectangle,&rightOffscreenDeselectedGameBorderDestinationRectangle);
		SDL_SetTextureAlphaMod(userInterfaceTexture,far_left_carousel_slot_alpha);
		SDL_RenderCopy(renderer,userInterfaceTexture,&selectedGameBorderSourceRectangle,&firstDeselectedGameBorderDestinationRectangle);
		SDL_SetTextureAlphaMod(userInterfaceTexture,left_carousel_slot_alpha);
		SDL_RenderCopy(renderer,userInterfaceTexture,&selectedGameBorderSourceRectangle,&secondDeselectedGameBorderDestinationRectangle);
		SDL_SetTextureAlphaMod(userInterfaceTexture,middle_left_carousel_slot_alpha);
		SDL_RenderCopy(renderer,userInterfaceTexture,&selectedGameBorderSourceRectangle,&thirdDeselectedGameBorderDestinationRectangle);
		SDL_SetTextureAlphaMod(userInterfaceTexture,middle_right_carousel_slot_alpha);
		SDL_RenderCopy(renderer,userInterfaceTexture,&selectedGameBorderSourceRectangle,&fourthDeselectedGameBorderDestinationRectangle);
		SDL_SetTextureAlphaMod(userInterfaceTexture,right_carousel_slot_alpha);
		SDL_RenderCopy(renderer,userInterfaceTexture,&selectedGameBorderSourceRectangle,&fifthDeselectedGameBorderDestinationRectangle);
		SDL_SetTextureAlphaMod(userInterfaceTexture,far_right_carousel_slot_alpha);
		SDL_RenderCopy(renderer,userInterfaceTexture,&selectedGameBorderSourceRectangle,&sixthDeselectedGameBorderDestinationRectangle);
		SDL_SetTextureAlphaMod(userInterfaceTexture,255);
		if (boxart_index == -6)
		{
			boxart_index = 21-6;
		}
		else if (boxart_index == 21-5)
		{
			boxart_index = -5;
		}
		if (boxart_index == -5)
		{
			SDL_RenderCopy(renderer,config[21-7].boxart,NULL,&leftOffscreenBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-6].boxart,NULL,&firstBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-5].boxart,NULL,&secondBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-4].boxart,NULL,&thirdBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-3].boxart,NULL,&fourthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-2].boxart,NULL,&fifthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-1].boxart,NULL,&sixthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[0].boxart,NULL,&rightOffscreenBoxartDestinationRectangle);
		}
		else if (boxart_index == -4)
		{
			SDL_RenderCopy(renderer,config[21-6].boxart,NULL,&leftOffscreenBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-5].boxart,NULL,&firstBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-4].boxart,NULL,&secondBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-3].boxart,NULL,&thirdBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-2].boxart,NULL,&fourthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-1].boxart,NULL,&fifthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[0].boxart,NULL,&sixthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[1].boxart,NULL,&rightOffscreenBoxartDestinationRectangle);
		}
		else if (boxart_index == -3)
		{
			SDL_RenderCopy(renderer,config[21-5].boxart,NULL,&leftOffscreenBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-4].boxart,NULL,&firstBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-3].boxart,NULL,&secondBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-2].boxart,NULL,&thirdBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-1].boxart,NULL,&fourthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[0].boxart,NULL,&fifthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[1].boxart,NULL,&sixthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[2].boxart,NULL,&rightOffscreenBoxartDestinationRectangle);
		}
		else if (boxart_index == -2)
		{
			SDL_RenderCopy(renderer,config[21-4].boxart,NULL,&leftOffscreenBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-3].boxart,NULL,&firstBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-2].boxart,NULL,&secondBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-1].boxart,NULL,&thirdBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[0].boxart,NULL,&fourthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[1].boxart,NULL,&fifthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[2].boxart,NULL,&sixthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[3].boxart,NULL,&rightOffscreenBoxartDestinationRectangle);
		}
		else if (boxart_index == -1)
		{
			SDL_RenderCopy(renderer,config[21-3].boxart,NULL,&leftOffscreenBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-2].boxart,NULL,&firstBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-1].boxart,NULL,&secondBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[0].boxart,NULL,&thirdBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[1].boxart,NULL,&fourthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[2].boxart,NULL,&fifthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[3].boxart,NULL,&sixthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[4].boxart,NULL,&rightOffscreenBoxartDestinationRectangle);
		}
		else if (boxart_index == 0)
		{
			SDL_RenderCopy(renderer,config[21-2].boxart,NULL,&leftOffscreenBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[21-1].boxart,NULL,&firstBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[0].boxart,NULL,&secondBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[1].boxart,NULL,&thirdBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[2].boxart,NULL,&fourthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[3].boxart,NULL,&fifthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[4].boxart,NULL,&sixthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[5].boxart,NULL,&rightOffscreenBoxartDestinationRectangle);
		}
		else if (boxart_index == 1)
		{
			SDL_RenderCopy(renderer,config[21-1].boxart,NULL,&leftOffscreenBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[0].boxart,NULL,&firstBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[1].boxart,NULL,&secondBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[2].boxart,NULL,&thirdBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[3].boxart,NULL,&fourthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[4].boxart,NULL,&fifthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[5].boxart,NULL,&sixthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[6].boxart,NULL,&rightOffscreenBoxartDestinationRectangle);
		}
		else
		{
			SDL_RenderCopy(renderer,config[boxart_index-2].boxart,NULL,&leftOffscreenBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[boxart_index-1].boxart,NULL,&firstBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[boxart_index].boxart,NULL,&secondBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[boxart_index+1].boxart,NULL,&thirdBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[boxart_index+2].boxart,NULL,&fourthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[boxart_index+3].boxart,NULL,&fifthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[boxart_index+4].boxart,NULL,&sixthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,config[boxart_index+5].boxart,NULL,&rightOffscreenBoxartDestinationRectangle);
		}
		SDL_SetTextureColorMod(userInterfaceTexture,cursor_red_color_channel,cursor_green_color_channel,255);
		SDL_RenderCopy(renderer,userInterfaceTexture,&topLeftCursorBorderSourceRectangle,&topLeftCursorBorderDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&cursorBorderSourceRectangle,&topCursorBorderDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&topRightCursorBorderSourceRectangle,&topRightCursorBorderDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&cursorBorderSourceRectangle,&rightCursorBorderDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&bottomRightCursorBorderSourceRectangle,&bottomRightCursorBorderDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&cursorBorderSourceRectangle,&bottomCursorBorderDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&bottomLeftCursorBorderSourceRectangle,&bottomLeftCursorBorderDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&cursorBorderSourceRectangle,&leftCursorBorderDestinationRectangle);
		SDL_SetTextureColorMod(userInterfaceTexture,255,255,255);
		SDL_RenderCopy(renderer,userInterfaceTexture,&dpadUpInstructionIconSourceRectangle,&dpadUpInstructionIconDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&dpadDownInstructionIconSourceRectangle,&dpadDownInstructionIconDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&selectInstructionIconSourceRectangle,&selectInstructionIconDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&startInstructionIconSourceRectangle,&startInstructionIconDestinationRectangle);
		SDL_RenderCopy(renderer,userInterfaceTexture,&bottomBorderSourceRectangle,&bottomBorderDestinationRectangle);
		SDL_RenderCopyEx(renderer,userInterfaceTexture,&spinningArrowSourceRectangle,&spinningArrowDestinationRectangle,0,NULL,SDL_FLIP_VERTICAL);
		SDL_RenderCopy(renderer,testThumbnailsTexture,NULL,&testThumbnailsDestinationRectangle);
		// Show the screen back buffer
		SDL_RenderPresent(renderer);
		
		// Play music
		if (!Mix_PlayingMusic())
		{
			if (music_state == is_in_intro)
			{
				Mix_PlayMusic(bootBackgroundMusic,0);
				music_state = is_in_loop;
			}
			else if (music_state == is_in_loop)
			{
				Mix_PlayMusic(homeBackgroundMusic,-1);
			}
		}
	}
	
	// Unload music and sounds
	Mix_FreeChunk(cursorSound);
	Mix_FreeMusic(homeBackgroundMusic);
	Mix_FreeMusic(bootBackgroundMusic);
	
	/*// Unload textures and surfaces
	for(int i = 0; i < 21; i++)
	{
		SDL_DestroyTexture(config[i]);
	}*/
	SDL_DestroyTexture(testThumbnailsTexture);
	SDL_FreeSurface(testThumbnailsSurface);
	SDL_DestroyTexture(userInterfaceTexture);
	SDL_FreeSurface(userInterfaceSurface);
	SDL_DestroyTexture(wallpaperTexture);
	SDL_FreeSurface(wallpaperSurface);
	
	// Deinitalize video, image, and audio libraries
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}

void read_config(vector<config_item> &config,SDL_Renderer *renderer)
{
    ifstream file("Users/Config.txt");
	int i = 0;
    while (!file.eof())
    {
		config_item current_item = config_item();
		getline(file, current_item.filename, '\n');
		SDL_Surface *tempSurface = IMG_Load(("Users/Boxart/" + current_item.filename + ".png").c_str());
		current_item.boxart = SDL_CreateTextureFromSurface(renderer,tempSurface);
		SDL_FreeSurface(tempSurface);
		getline(file, current_item.title, '\n');
		getline(file, current_item.publisher, '\n');
		getline(file, current_item.release_date, '\n');
		getline(file, current_item.number_of_players, '\n');
		config.push_back(current_item);
    }
}