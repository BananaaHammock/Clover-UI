/**
    Clover-UI
    Source.cpp
    Purpose: Launches ROMs with a user interface similar to Clover-UI

    @author SamTheBreadman
    @version 1.0 10/9/17 
*/

/* Include standard libraries */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

/* Include SDL libraries */
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

/* Initialize struct */
struct config_item 
{
	string filename;
	string title;
	string publisher;
	string release_date;
	string number_of_players;
};
	
/* Include function prototypes */
//int get_total_game_count();
void read_config(vector<config_item>);

/* Begin main program execution */
int main(int argc, char *argv[])  
{
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
	
	/* Initialize and declare counts */
	//int total_game_count = get_total_game_count();
	
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
	
	/* Initialize and declare arrays */
    /*char filename_array[total_game_count][255];
	char title_array[total_game_count][255];
	char publisher_array[total_game_count][255];
	char release_date_array[total_game_count][255];
	char number_of_players_array[total_game_count][255];
	//populate_arrays(filename_array, title_array, publisher_array, release_date_array, number_of_players_array);
	
	/* Initalize and declare vectors */
	vector<config_item> config;
	read_config(config);

	
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
	
	/* Initalize video, image, and audio libraries */
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	IMG_Init(IMG_INIT_PNG);
	Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);

	/* Initalize and declare window and renderer */
	SDL_Window *window = SDL_CreateWindow("Clover-UI",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,1280,720,0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	SDL_RenderClear(renderer);
	
	/* Initalize and declare surfaces */
	SDL_Surface *wallpaperSurface = IMG_Load("Resources/Images/wallpaperImage.png");
	SDL_Surface *userInterfaceSurface = IMG_Load("Resources/Images/userInterfaceImage.png");
	SDL_Surface *testThumbnailsSurface = IMG_Load("Resources/Images/testThumbnailsImage.png");
	
	/* Initalize and declare textures */
	SDL_Texture *wallpaperTexture = SDL_CreateTextureFromSurface(renderer,wallpaperSurface);
	SDL_Texture *userInterfaceTexture = SDL_CreateTextureFromSurface(renderer,userInterfaceSurface);
	//SDL_Texture *boxart_array[total_game_count];
	/*for(int i = 0; i < total_game_count; i++)
	{
		char first_string[256]="Users/Boxart/";
		char *second_string= filename_array[i];
		char third_string[256]=".png";
		strcat(first_string,second_string);
		strcat(first_string,third_string);
		SDL_Surface *tempSurface = IMG_Load(first_string);
		boxart_array[i] = SDL_CreateTextureFromSurface(renderer,tempSurface);
		SDL_FreeSurface(tempSurface);
	}*/
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
		/*if (boxart_index == -6)
		{
			boxart_index = total_game_count-6;
		}
		else if (boxart_index == total_game_count-5)
		{
			boxart_index = -5;
		}
		if (boxart_index == -5)
		{
			SDL_RenderCopy(renderer,boxart_array[total_game_count-7],NULL,&leftOffscreenBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-6],NULL,&firstBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-5],NULL,&secondBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-4],NULL,&thirdBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-3],NULL,&fourthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-2],NULL,&fifthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-1],NULL,&sixthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[0],NULL,&rightOffscreenBoxartDestinationRectangle);
		}
		else if (boxart_index == -4)
		{
			SDL_RenderCopy(renderer,boxart_array[total_game_count-6],NULL,&leftOffscreenBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-5],NULL,&firstBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-4],NULL,&secondBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-3],NULL,&thirdBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-2],NULL,&fourthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-1],NULL,&fifthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[0],NULL,&sixthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[1],NULL,&rightOffscreenBoxartDestinationRectangle);
		}
		else if (boxart_index == -3)
		{
			SDL_RenderCopy(renderer,boxart_array[total_game_count-5],NULL,&leftOffscreenBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-4],NULL,&firstBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-3],NULL,&secondBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-2],NULL,&thirdBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-1],NULL,&fourthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[0],NULL,&fifthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[1],NULL,&sixthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[2],NULL,&rightOffscreenBoxartDestinationRectangle);
		}
		else if (boxart_index == -2)
		{
			SDL_RenderCopy(renderer,boxart_array[total_game_count-4],NULL,&leftOffscreenBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-3],NULL,&firstBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-2],NULL,&secondBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-1],NULL,&thirdBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[0],NULL,&fourthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[1],NULL,&fifthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[2],NULL,&sixthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[3],NULL,&rightOffscreenBoxartDestinationRectangle);
		}
		else if (boxart_index == -1)
		{
			SDL_RenderCopy(renderer,boxart_array[total_game_count-3],NULL,&leftOffscreenBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-2],NULL,&firstBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-1],NULL,&secondBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[0],NULL,&thirdBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[1],NULL,&fourthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[2],NULL,&fifthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[3],NULL,&sixthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[4],NULL,&rightOffscreenBoxartDestinationRectangle);
		}
		else if (boxart_index == 0)
		{
			SDL_RenderCopy(renderer,boxart_array[total_game_count-2],NULL,&leftOffscreenBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[total_game_count-1],NULL,&firstBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[0],NULL,&secondBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[1],NULL,&thirdBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[2],NULL,&fourthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[3],NULL,&fifthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[4],NULL,&sixthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[5],NULL,&rightOffscreenBoxartDestinationRectangle);
		}
		else if (boxart_index == 1)
		{
			SDL_RenderCopy(renderer,boxart_array[total_game_count-1],NULL,&leftOffscreenBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[0],NULL,&firstBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[1],NULL,&secondBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[2],NULL,&thirdBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[3],NULL,&fourthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[4],NULL,&fifthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[5],NULL,&sixthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[6],NULL,&rightOffscreenBoxartDestinationRectangle);
		}
		else
		{
			SDL_RenderCopy(renderer,boxart_array[boxart_index-2],NULL,&leftOffscreenBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[boxart_index-1],NULL,&firstBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[boxart_index],NULL,&secondBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[boxart_index+1],NULL,&thirdBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[boxart_index+2],NULL,&fourthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[boxart_index+3],NULL,&fifthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[boxart_index+4],NULL,&sixthBoxartDestinationRectangle);
			SDL_RenderCopy(renderer,boxart_array[boxart_index+5],NULL,&rightOffscreenBoxartDestinationRectangle);
		}*/
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
		if(config[0].filename == "superMarioWorld")
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
	for(int i = 0; i < total_game_count; i++)
	{
		SDL_DestroyTexture(boxart_array[i]);
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

/*int get_total_game_count()
{
    FILE *file;
	file = fopen("Users/Config.txt", "r");
	int line = 0;
    char buffer[256];
    while (fgets(buffer, 256, file) != NULL) 
    {
        line++;
    }
	fclose(file);
    return line / 5;
	
}*/

void read_config(vector<config_item> config)
{
    ifstream file("Users/Config.txt");
	int line = 0;
	int filename_array_index = 0;
	int title_array_index = 0;
	int publisher_array_index = 0;
	int release_date_array_index = 0;
	int number_of_players_array_index = 0;
	string buffer;
    while (!file.eof())
    {
		getline(file, buffer, '\n');
		if (line == 0 + filename_array_index * 5)
		{
			config.push_back(config_item());
            config[filename_array_index++].filename = buffer;
		}
        else if (line == 1 + (title_array_index * 5)) 
        {
            config[title_array_index++].title = buffer;
        }
        else if (line == 2 + (publisher_array_index * 5)) 
        {
            config[publisher_array_index++].publisher = buffer;
        }
        else if (line == 3 + (release_date_array_index * 5)) 
        {
            config[release_date_array_index++].release_date = buffer;
        }
        else if (line == 4 + (number_of_players_array_index * 5)) 
        {
            config[number_of_players_array_index++].number_of_players = buffer;
        }
        line++;
    }
}

/*void populate_arrays(char filename_array[][255], char title_array[][255], char publisher_array[][255], char release_date_array[][255], char number_of_players_array[][255]) 
{
    
    FILE *file;
	file = fopen("Users/Config.txt", "r");
	int line = 0;
	int filename_array_index = 0;
	int title_array_index = 0;
	int publisher_array_index = 0;
	int release_date_array_index = 0;
	int number_of_players_array_index = 0;
	char buffer[256];
    while (fgets(buffer, 256, file) != NULL) 
    {
		if (line == 0 + filename_array_index * 5)
		{
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(filename_array[filename_array_index++],buffer);
		}
        else if (line == 1 + (title_array_index * 5)) 
        {
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(title_array[title_array_index++],buffer);
        }
        else if (line == 2 + (publisher_array_index * 5)) 
        {
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(publisher_array[publisher_array_index++],buffer);
        }
        else if (line == 3 + (release_date_array_index * 5)) 
        {
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(release_date_array[release_date_array_index++],buffer);
        }
        else if (line == 4 + (number_of_players_array_index * 5)) 
        {
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(number_of_players_array[number_of_players_array_index++],buffer);
        }
        line++;
    }
    fclose(file);
}*/