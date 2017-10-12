/**
    Clover-UI
    Source.cpp
    Purpose: Launches ROMs with a user interface similar to Clover-UI

    @author samthebreadman
    @version 1.0 10/9/17 
*/

/* Include standard libraries using standard namespace */
using namespace std;
#include <fstream>
#include <string>
#include <vector>

/* Include Standard Direct-Media Layer libraries */
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

/* Define necessary struct definitions */
struct config_item {
	string filename;
	string title;
	string publisher;
	string release_date;
	string number_of_players;
	SDL_Texture *boxart;
};

/* Include function prototypes */
void read_config(vector<config_item> &config,SDL_Renderer *main_renderer);

/* Begin main program execution */	
int main(int argc, char *argv[]) {
	
	/* Initalize video, image, and audio libraries */
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	IMG_Init(IMG_INIT_PNG);
	Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);

	/* Initalize and declare main window and renderer */
	SDL_Window *window = SDL_CreateWindow("Clover-UI",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,1280,720,0);
	SDL_Renderer *main_renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(main_renderer,255,255,255,255);
	SDL_RenderClear(main_renderer);
	
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
	
	/* Initialize and declare timers */
	float carousel_horizontal_movement_timer = 1;
	int cursor_hold_timer = 0;
	int key_hold_timer = 0;
	int spinning_arrow_animation_timer = 0;
	
	/* Initialize and declare indexes */
	int boxart_index = 0;
	int carousel_slot_index = 0;
	int spinning_arrow_animation_index = 0;
	
	/* Initialize and declare positions */
	float wallpaper_position = 0;
	float carousel_horizontal_position = 0;
	float carousel_horizontal_destination_position = 0;
	float cursor_horizontal_position = 0;
	
	/* Initialize and declare offsets */
	float carousel_offset = 0;
	int spinning_arrow_offset = 0;

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
	read_config(config, main_renderer);
	
	/* Initialize and declare source rectangles */
	SDL_Rect top_banner_source_rectangle = {3,49,428,34};
	SDL_Rect top_banner_inset_source_rectangle = {502,397,10,22};
	SDL_Rect display_icon_source_rectangle = {29,1002,21,17};
	SDL_Rect options_icon_source_rectangle = {82,998,19,19};
	SDL_Rect language_icon_source_rectangle = {56,1001,18,19};
	SDL_Rect legal_notices_icon_source_rectangle = {5,1001,17,19};
	SDL_Rect manuals_icon_source_rectangle = {242,850,19,19};
	SDL_Rect title_slot_source_rectangle = {4,327,342,18};
	SDL_Rect carousel_slot_source_rectangle = {417,379,84,92};
	SDL_Rect selected_carousel_slot_source_rectangle = {409,473,84,92};
	SDL_Rect top_left_cursor_corner_source_rectangle = {113,951,23,23};
	SDL_Rect cursor_edge_source_rectangle = {136,863,6,6};
	SDL_Rect top_right_cursor_corner_source_rectangle = {121,905,23,23};
	SDL_Rect bottom_right_cursor_corner_source_rectangle = {144,808,23,23};
	SDL_Rect bottom_left_cursor_corner_source_rectangle = {200,894,23,23};
	SDL_Rect spinning_arrow_source_rectangle = {146,882,10,6};
	SDL_Rect dpad_up_icon_source_rectangle = {85,881,12,12};
	SDL_Rect dpad_down_icon_source_rectangle = {57,881,12,12};
	SDL_Rect select_icon_source_rectangle  = {88,930,31,9};
	SDL_Rect start_icon_source_rectangle = {481,784,31,9};
	SDL_Rect bottom_banner_source_rectangle = {3,1,428,30};
	
	/* Initialize and declare destination rectangles */
	SDL_Rect far_left_wallpaper_destination_rectangle = {3122,80,1561,564};
	SDL_Rect left_wallpaper_destination_rectangle = {-1561,80,1561,564};
	SDL_Rect middle_wallpaper_destination_rectangle = {0,80,1561,564};
	SDL_Rect right_wallpaper_destination_rectangle = {1561,80,1561,564};
	SDL_Rect far_right_wallpaper_destination_rectangle = {3122,80,1561,564};
	SDL_Rect top_banner_destination_rectangle = {-2,-6,1284,102};
	SDL_Rect top_banner_inset_destination_rectangle = {400,30,480,66};
	SDL_Rect display_icon_destination_rectangle = {418,38,63,51};
	SDL_Rect options_icon_destination_rectangle = {517,35,57,57};
	SDL_Rect language_icon_destination_rectangle = {613,35,54,57};
	SDL_Rect legal_notices_icon_destination_rectangle = {712,35,51,57};
	SDL_Rect manuals_icon_destination_rectangle = {805,35,57,57};
	SDL_Rect title_slot_destination_rectangle = {127,151,1026,54};
	SDL_Rect farthest_left_carousel_slot_destination_rectangle = {-403,222,252,276};
	SDL_Rect far_left_carousel_slot_destination_rectangle = {-141,222,252,276};
	SDL_Rect left_carousel_slot_destination_rectangle = {121,222,252,276};
	SDL_Rect middle_left_carousel_slot_destination_rectangle = {383,222,252,276};
	SDL_Rect middle_right_carousel_slot_destination_rectangle = {645,222,252,276};
	SDL_Rect right_carousel_slot_destination_rectangle = {907,222,252,276};
	SDL_Rect far_right_carousel_slot_destination_rectangle = {1169,222,252,276};
	SDL_Rect farthest_right_carousel_slot_destination_rectangle = {1431,222,252,276};
	SDL_Rect farthest_left_boxart_destination_rectangle = {-391,256,228,160};
	SDL_Rect far_left_boxart_destination_rectangle = {-129,256,228,160};
	SDL_Rect left_boxart_destination_rectangle = {133,256,228,160};
	SDL_Rect middle_left_boxart_destination_rectangle = {395,256,228,160};
	SDL_Rect middle_right_boxart_destination_rectangle = {657,256,228,160};
	SDL_Rect right_boxart_destination_rectangle = {919,256,228,160};
	SDL_Rect far_right_boxart_destination_rectangle = {1181,256,228,160};
	SDL_Rect farthest_right_boxart_destination_rectangle = {1443,256,228,160};
	SDL_Rect top_left_cursor_corner_destination_rectangle = {118,219,23,23};
	SDL_Rect top_cursor_edge_destination_rectangle = {141,219,212,6};
	SDL_Rect top_right_cursor_corner_destination_rectangle = {353,219,23,23};	
	SDL_Rect right_cursor_edge_destination_rectangle = {370,242,6,236};	
	SDL_Rect bottom_right_cursor_corner_destination_rectangle = {353,478,23,23};	
	SDL_Rect bottom_cursor_edge_destination_rectangle = {141,495,212,6};	
	SDL_Rect bottom_left_cursor_corner_destination_rectangle = {118,478,23,23};
	SDL_Rect left_cursor_edge_destination_rectangle = {118,242,6,236};
	SDL_Rect spinning_arrow_destination_rectangle = {215,517,30,18};
	SDL_Rect dpad_up_icon_destination_rectangle = {278,591,36,36};
	SDL_Rect dpad_down_icon_destination_rectangle = {400,591,36,36};
	SDL_Rect select_icon_destination_rectangle = {640,597,93,27};
	SDL_Rect start_icon_destination_rectangle = {809,597,93,27};
	SDL_Rect thumbnails_destination_rectangle = {210,538,860,28};
	SDL_Rect bottom_banner_destination_rectangle = {-2,630,1284,90};
	
	/* Initalize and declare surfaces */
	SDL_Surface *wallpaper_surface = IMG_Load("Resources/Images/wallpaperImage.png");
	SDL_Surface *user_interface_surface = IMG_Load("Resources/Images/userInterfaceImage.png");
	SDL_Surface *thumbnails_surface = IMG_Load("Resources/Images/testThumbnailsImage.png");
	
	/* Initalize and declare textures */
	SDL_Texture *wallpaper_texture = SDL_CreateTextureFromSurface(main_renderer,wallpaper_surface);
	SDL_Texture *user_interface_texture = SDL_CreateTextureFromSurface(main_renderer,user_interface_surface);
	SDL_Texture *thumbnails_texture = SDL_CreateTextureFromSurface(main_renderer,thumbnails_surface);

	/* Initialize and declare music and sounds */
	Mix_Music *intro_music = Mix_LoadMUS("Resources/Sounds/bootBackgroundMusic.wav");
	Mix_Music *loop_music = Mix_LoadMUS("Resources/Sounds/homeBackgroundMusic.wav");
	Mix_Chunk *cursor_sound = Mix_LoadWAV( "Resources/Sounds/cursorSound.wav" );

	/* Begin main loop */
	while(main_state != is_exiting) {
		
		/* Receive input and change states */
		SDL_Event event;
		SDL_PollEvent(&event);
		
		/* If the window is closed then exit the program */
        if (event.type == SDL_QUIT)
			main_state = is_exiting;
		
		/* If any key is down then check what key is down */
		if (event.type == SDL_KEYDOWN) {
			
			/* If the escape key is pressed then exit the program */
			if (event.key.keysym.sym == SDLK_ESCAPE)
				main_state = is_exiting;
			
			/* If the left key is pressed then update necessary values */
			if (event.key.keysym.sym == SDLK_LEFT) {
				if (main_state == is_in_carousel) {
					if (left_key_state == is_released) {
						Mix_PlayChannel( -1, cursor_sound, 0 );
						if (carousel_slot_index != 0) 
							carousel_slot_index -= 1;
						else {
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
						spinning_arrow_offset -= 1;
						key_hold_timer = SDL_GetTicks();
						left_key_state = is_pressed;
					}
					if (SDL_GetTicks() > (key_hold_timer + 175)) {
						if (cursor_hold_timer == 6) {
							Mix_PlayChannel( -1, cursor_sound, 0 );
							if (carousel_slot_index != 0)
								carousel_slot_index -= 1;
							else {
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
							spinning_arrow_offset -= 1;
							cursor_hold_timer = 0;
						}
						else {
							cursor_hold_timer += 1;
						}
					}	
				}
			}
			
			/* If the right key is pressed then update necessary values */
			if (event.key.keysym.sym == SDLK_RIGHT) {
				if (main_state == is_in_carousel) {
					if (right_key_state == is_released) {
						Mix_PlayChannel( -1, cursor_sound, 0 );
						if (carousel_slot_index != 3)
							carousel_slot_index += 1;
						else {
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
						spinning_arrow_offset += 1;
						key_hold_timer = SDL_GetTicks();
						right_key_state = is_pressed;
					}
					if (SDL_GetTicks() > (key_hold_timer + 175)) {
						if (cursor_hold_timer == 6) {
							Mix_PlayChannel( -1, cursor_sound, 0 );
							if (carousel_slot_index != 3)
								carousel_slot_index += 1;
							else {
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
							spinning_arrow_offset += 1;
							cursor_hold_timer = 0;
						}
						else
							cursor_hold_timer += 1;
					}
				}
			}
		}
		
		/* If no keys are pressed then update values */
		else
		{
			if (main_state == is_in_carousel) {
				carousel_horizontal_speed = 0.025;
				carousel_slot_alpha_speed = 10;
			    cursor_hold_timer = 0;
				left_key_state = is_released;
				right_key_state = is_released;
			}
		}

		/* Update the wallpaper position */
		if (wallpaper_position<=-1561)
			wallpaper_position += 1561-wallpaper_speed;
		else if (wallpaper_position >= 1561)
			wallpaper_position -=1561-wallpaper_speed;
		else
			wallpaper_position -= wallpaper_speed;
		far_left_wallpaper_destination_rectangle.x = wallpaper_position-3122;
		left_wallpaper_destination_rectangle.x = wallpaper_position-1561;
		middle_wallpaper_destination_rectangle.x = wallpaper_position;
		right_wallpaper_destination_rectangle.x = wallpaper_position+1561;
		far_right_wallpaper_destination_rectangle.x = wallpaper_position+3122;
		
		/* Update the carousel position*/
		if (carousel_horizontal_movement_timer>=0 && carousel_horizontal_movement_timer<1)
			carousel_horizontal_movement_timer+=carousel_horizontal_speed;
		if (carousel_horizontal_movement_timer>1)
			carousel_horizontal_movement_timer=1;
		if (carousel_slot_index == 0) {
			carousel_horizontal_position =  (carousel_horizontal_destination_position*-262 - carousel_horizontal_position+0.1)*carousel_horizontal_movement_timer + carousel_horizontal_position;
		far_left_carousel_slot_alpha += (0 - far_left_carousel_slot_alpha) / carousel_slot_alpha_speed;
		left_carousel_slot_alpha += (255 - left_carousel_slot_alpha) / carousel_slot_alpha_speed;
		middle_left_carousel_slot_alpha += (0 - middle_left_carousel_slot_alpha) / carousel_slot_alpha_speed;
		middle_right_carousel_slot_alpha += (0 - middle_right_carousel_slot_alpha) / carousel_slot_alpha_speed;
		right_carousel_slot_alpha += (0 - right_carousel_slot_alpha) / carousel_slot_alpha_speed;
		far_right_carousel_slot_alpha += (0 - far_right_carousel_slot_alpha) / carousel_slot_alpha_speed;
		}
		if (carousel_slot_index == 1) {
			carousel_horizontal_position =  (carousel_horizontal_destination_position*-262 - carousel_horizontal_position+0.1)*carousel_horizontal_movement_timer + carousel_horizontal_position;
			far_left_carousel_slot_alpha += (0 - far_left_carousel_slot_alpha) / carousel_slot_alpha_speed;
			left_carousel_slot_alpha += (0 - left_carousel_slot_alpha) / carousel_slot_alpha_speed;
			middle_left_carousel_slot_alpha += (255 - middle_left_carousel_slot_alpha) / carousel_slot_alpha_speed;
			middle_right_carousel_slot_alpha += (0 - middle_right_carousel_slot_alpha) / carousel_slot_alpha_speed;
			right_carousel_slot_alpha += (0 - right_carousel_slot_alpha) / carousel_slot_alpha_speed;
			far_right_carousel_slot_alpha += (0 - far_right_carousel_slot_alpha) / carousel_slot_alpha_speed;
			}
		if (carousel_slot_index == 2) {
			carousel_horizontal_position =  (carousel_horizontal_destination_position*-262 - carousel_horizontal_position+0.1)*carousel_horizontal_movement_timer + carousel_horizontal_position;
			far_left_carousel_slot_alpha += (0 - far_left_carousel_slot_alpha) / carousel_slot_alpha_speed;
			left_carousel_slot_alpha += (0 - left_carousel_slot_alpha) / carousel_slot_alpha_speed;
			middle_left_carousel_slot_alpha += (0 - middle_left_carousel_slot_alpha) / carousel_slot_alpha_speed;
			middle_right_carousel_slot_alpha += (255 - middle_right_carousel_slot_alpha) / carousel_slot_alpha_speed;
			right_carousel_slot_alpha += (0 - right_carousel_slot_alpha) / carousel_slot_alpha_speed;
			far_right_carousel_slot_alpha += (0 - far_right_carousel_slot_alpha) / carousel_slot_alpha_speed;
		}
		if (carousel_slot_index == 3) {
			carousel_horizontal_position =  (carousel_horizontal_destination_position*-262 - carousel_horizontal_position+0.1)*carousel_horizontal_movement_timer + carousel_horizontal_position;
			far_left_carousel_slot_alpha += (0 - far_left_carousel_slot_alpha) / carousel_slot_alpha_speed;
			left_carousel_slot_alpha += (0 - left_carousel_slot_alpha) / carousel_slot_alpha_speed;
			middle_left_carousel_slot_alpha += (0 - middle_left_carousel_slot_alpha) / carousel_slot_alpha_speed;
			middle_right_carousel_slot_alpha += (0 - middle_right_carousel_slot_alpha) / carousel_slot_alpha_speed;
			right_carousel_slot_alpha += (255 - right_carousel_slot_alpha) / carousel_slot_alpha_speed;
			far_right_carousel_slot_alpha += (0 - far_right_carousel_slot_alpha) / carousel_slot_alpha_speed;
		}
		farthest_left_carousel_slot_destination_rectangle.x = -403+carousel_horizontal_position+carousel_offset;
		far_left_carousel_slot_destination_rectangle.x = -141+carousel_horizontal_position+carousel_offset;
		left_carousel_slot_destination_rectangle.x = 121+carousel_horizontal_position+carousel_offset;
		middle_left_carousel_slot_destination_rectangle.x = 383+carousel_horizontal_position+carousel_offset;
		middle_right_carousel_slot_destination_rectangle.x = 645+carousel_horizontal_position+carousel_offset;
		right_carousel_slot_destination_rectangle.x = 907+carousel_horizontal_position+carousel_offset;
		far_right_carousel_slot_destination_rectangle.x = 1169+carousel_horizontal_position+carousel_offset;
		farthest_right_carousel_slot_destination_rectangle.x = 1431+carousel_horizontal_position+carousel_offset;
		
		/* Update the boxarts position */
		farthest_left_boxart_destination_rectangle.x = -391+carousel_horizontal_position+carousel_offset;
		far_left_boxart_destination_rectangle.x = -129+carousel_horizontal_position+carousel_offset;
		left_boxart_destination_rectangle.x = 133+carousel_horizontal_position+carousel_offset;
		middle_left_boxart_destination_rectangle.x = 395+carousel_horizontal_position+carousel_offset;
		middle_right_boxart_destination_rectangle.x = 657+carousel_horizontal_position+carousel_offset;
		right_boxart_destination_rectangle.x = 919+carousel_horizontal_position+carousel_offset;
		far_right_boxart_destination_rectangle.x = 1181+carousel_horizontal_position+carousel_offset;
		farthest_right_boxart_destination_rectangle.x = 1443+carousel_horizontal_position+carousel_offset;
		
		/* Update the pulsing cursor position */
		if (carousel_slot_index == 0) {
			if (cursor_horizontal_position < 0.5 && cursor_horizontal_position > -0.5)
				cursor_horizontal_position = 0;	
			cursor_horizontal_position += (0 - cursor_horizontal_position) / 4;
		}
		else if (carousel_slot_index == 1) {
			if (cursor_horizontal_position < 262.5 && cursor_horizontal_position > 261.5)
				cursor_horizontal_position = 262;
			cursor_horizontal_position += (262 - cursor_horizontal_position) / 4;
		}
		else if (carousel_slot_index == 2) {
			if (cursor_horizontal_position < 524.5 && cursor_horizontal_position > 523.5)
				cursor_horizontal_position = 524;
			cursor_horizontal_position += (524 - cursor_horizontal_position) / 4;
		}	
		else if (carousel_slot_index == 3) {	
			if (cursor_horizontal_position < 786.5 && cursor_horizontal_position > 785.5)
				cursor_horizontal_position = 786;
			cursor_horizontal_position += (786 - cursor_horizontal_position) / 4;
		}
		top_left_cursor_corner_destination_rectangle.x = 118+cursor_horizontal_position;
		top_cursor_edge_destination_rectangle.x = 141+cursor_horizontal_position;
		top_right_cursor_corner_destination_rectangle.x = 353+cursor_horizontal_position;	
		right_cursor_edge_destination_rectangle.x = 370+cursor_horizontal_position;
		bottom_right_cursor_corner_destination_rectangle.x = 353+cursor_horizontal_position;
		bottom_cursor_edge_destination_rectangle.x = 141+cursor_horizontal_position;
		bottom_left_cursor_corner_destination_rectangle.x = 118+cursor_horizontal_position;
		left_cursor_edge_destination_rectangle.x = 118+cursor_horizontal_position;
		
		/* Animate the pulsing cursor animation */
		if (cursor_pulse_state == is_pulsing_out) {
			cursor_pulse_speed += 0.05;
			if (cursor_red_color_channel - cursor_pulse_speed > 63)
				cursor_red_color_channel = cursor_red_color_channel - cursor_pulse_speed;
			else
				cursor_red_color_channel = 63;
			if (cursor_green_color_channel - cursor_pulse_speed > 191)
				cursor_green_color_channel = cursor_green_color_channel - cursor_pulse_speed;
			else
				cursor_green_color_channel = 191;
			if (cursor_pulse_speed >= 3) {
				cursor_pulse_speed = 3;
				cursor_pulse_state = is_pulsing_in;
			}
		}
		else if (cursor_pulse_state == is_pulsing_in) {
			cursor_pulse_speed -= 0.05;
			if (cursor_red_color_channel + cursor_pulse_speed < 255)
				cursor_red_color_channel = cursor_red_color_channel + cursor_pulse_speed;
			else
				cursor_red_color_channel == 255;
			if (cursor_green_color_channel + cursor_pulse_speed < 255)
				cursor_green_color_channel = cursor_green_color_channel + cursor_pulse_speed;
			else
				cursor_green_color_channel = 255;
			if (cursor_pulse_speed <= 0) {
				cursor_pulse_speed = 0;
				cursor_pulse_state = is_pulsing_out;
			}
		}
		
		/* Update the spinning arrow position */
		if (spinning_arrow_offset == -1)
			spinning_arrow_offset = 20;
		else if (spinning_arrow_offset == 21)
			spinning_arrow_offset = 0;
		spinning_arrow_source_rectangle.x = 146+spinning_arrow_animation_index*14;
		spinning_arrow_destination_rectangle.x = 215+(spinning_arrow_offset*41);
		
		/* Animate the spinning arrow animation */
		spinning_arrow_animation_timer = SDL_GetTicks();
		spinning_arrow_animation_index = (spinning_arrow_animation_timer / 115) % 4;
		if ((spinning_arrow_animation_timer / 115) % 4 == 3)
			spinning_arrow_animation_index = 1;
		
		/* Clear the screen back buffer */
		SDL_RenderClear(main_renderer);
		
		/* Copy textures onto the main main_renderer */
		SDL_RenderCopy(main_renderer,wallpaper_texture,NULL,&far_left_wallpaper_destination_rectangle);
		SDL_RenderCopy(main_renderer,wallpaper_texture,NULL,&left_wallpaper_destination_rectangle);
		SDL_RenderCopy(main_renderer,wallpaper_texture,NULL,&middle_wallpaper_destination_rectangle);
		SDL_RenderCopy(main_renderer,wallpaper_texture,NULL,&right_wallpaper_destination_rectangle);
		SDL_RenderCopy(main_renderer,wallpaper_texture,NULL,&far_right_wallpaper_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&top_banner_source_rectangle,&top_banner_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&top_banner_inset_source_rectangle,&top_banner_inset_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&display_icon_source_rectangle,&display_icon_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&options_icon_source_rectangle,&options_icon_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&language_icon_source_rectangle,&language_icon_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&legal_notices_icon_source_rectangle,&legal_notices_icon_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&manuals_icon_source_rectangle,&manuals_icon_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&title_slot_source_rectangle,&title_slot_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&carousel_slot_source_rectangle,&farthest_left_carousel_slot_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&carousel_slot_source_rectangle,&far_left_carousel_slot_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&carousel_slot_source_rectangle,&left_carousel_slot_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&carousel_slot_source_rectangle,&middle_left_carousel_slot_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&carousel_slot_source_rectangle,&middle_right_carousel_slot_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&carousel_slot_source_rectangle,&right_carousel_slot_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&carousel_slot_source_rectangle,&far_right_carousel_slot_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&carousel_slot_source_rectangle,&farthest_right_carousel_slot_destination_rectangle);
		SDL_SetTextureAlphaMod(user_interface_texture,far_left_carousel_slot_alpha);
		SDL_RenderCopy(main_renderer,user_interface_texture,&selected_carousel_slot_source_rectangle,&far_left_carousel_slot_destination_rectangle);
		SDL_SetTextureAlphaMod(user_interface_texture,left_carousel_slot_alpha);
		SDL_RenderCopy(main_renderer,user_interface_texture,&selected_carousel_slot_source_rectangle,&left_carousel_slot_destination_rectangle);
		SDL_SetTextureAlphaMod(user_interface_texture,middle_left_carousel_slot_alpha);
		SDL_RenderCopy(main_renderer,user_interface_texture,&selected_carousel_slot_source_rectangle,&middle_left_carousel_slot_destination_rectangle);
		SDL_SetTextureAlphaMod(user_interface_texture,middle_right_carousel_slot_alpha);
		SDL_RenderCopy(main_renderer,user_interface_texture,&selected_carousel_slot_source_rectangle,&middle_right_carousel_slot_destination_rectangle);
		SDL_SetTextureAlphaMod(user_interface_texture,right_carousel_slot_alpha);
		SDL_RenderCopy(main_renderer,user_interface_texture,&selected_carousel_slot_source_rectangle,&right_carousel_slot_destination_rectangle);
		SDL_SetTextureAlphaMod(user_interface_texture,far_right_carousel_slot_alpha);
		SDL_RenderCopy(main_renderer,user_interface_texture,&selected_carousel_slot_source_rectangle,&far_right_carousel_slot_destination_rectangle);
		SDL_SetTextureAlphaMod(user_interface_texture,255);
		if (boxart_index == -6)
			boxart_index = 21-6;
		else if (boxart_index == 21-5)
			boxart_index = -5;
		if (boxart_index == -5) {
			SDL_RenderCopy(main_renderer,config[21-7].boxart,NULL,&farthest_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-6].boxart,NULL,&far_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-5].boxart,NULL,&left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-4].boxart,NULL,&middle_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-3].boxart,NULL,&middle_right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-2].boxart,NULL,&right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-1].boxart,NULL,&far_right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[0].boxart,NULL,&farthest_right_boxart_destination_rectangle);
		}
		else if (boxart_index == -4) {
			SDL_RenderCopy(main_renderer,config[21-6].boxart,NULL,&farthest_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-5].boxart,NULL,&far_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-4].boxart,NULL,&left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-3].boxart,NULL,&middle_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-2].boxart,NULL,&middle_right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-1].boxart,NULL,&right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[0].boxart,NULL,&far_right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[1].boxart,NULL,&farthest_right_boxart_destination_rectangle);
		}
		else if (boxart_index == -3) {
			SDL_RenderCopy(main_renderer,config[21-5].boxart,NULL,&farthest_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-4].boxart,NULL,&far_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-3].boxart,NULL,&left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-2].boxart,NULL,&middle_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-1].boxart,NULL,&middle_right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[0].boxart,NULL,&right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[1].boxart,NULL,&far_right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[2].boxart,NULL,&farthest_right_boxart_destination_rectangle);
		}
		else if (boxart_index == -2) {
			SDL_RenderCopy(main_renderer,config[21-4].boxart,NULL,&farthest_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-3].boxart,NULL,&far_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-2].boxart,NULL,&left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-1].boxart,NULL,&middle_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[0].boxart,NULL,&middle_right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[1].boxart,NULL,&right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[2].boxart,NULL,&far_right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[3].boxart,NULL,&farthest_right_boxart_destination_rectangle);
		}
		else if (boxart_index == -1) {
			SDL_RenderCopy(main_renderer,config[21-3].boxart,NULL,&farthest_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-2].boxart,NULL,&far_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-1].boxart,NULL,&left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[0].boxart,NULL,&middle_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[1].boxart,NULL,&middle_right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[2].boxart,NULL,&right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[3].boxart,NULL,&far_right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[4].boxart,NULL,&farthest_right_boxart_destination_rectangle);
		}
		else if (boxart_index == 0) {
			SDL_RenderCopy(main_renderer,config[21-2].boxart,NULL,&farthest_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[21-1].boxart,NULL,&far_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[0].boxart,NULL,&left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[1].boxart,NULL,&middle_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[2].boxart,NULL,&middle_right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[3].boxart,NULL,&right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[4].boxart,NULL,&far_right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[5].boxart,NULL,&farthest_right_boxart_destination_rectangle);
		}
		else if (boxart_index == 1) {
			SDL_RenderCopy(main_renderer,config[21-1].boxart,NULL,&farthest_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[0].boxart,NULL,&far_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[1].boxart,NULL,&left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[2].boxart,NULL,&middle_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[3].boxart,NULL,&middle_right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[4].boxart,NULL,&right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[5].boxart,NULL,&far_right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[6].boxart,NULL,&farthest_right_boxart_destination_rectangle);
		}
		else {
			SDL_RenderCopy(main_renderer,config[boxart_index-2].boxart,NULL,&farthest_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[boxart_index-1].boxart,NULL,&far_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[boxart_index].boxart,NULL,&left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[boxart_index+1].boxart,NULL,&middle_left_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[boxart_index+2].boxart,NULL,&middle_right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[boxart_index+3].boxart,NULL,&right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[boxart_index+4].boxart,NULL,&far_right_boxart_destination_rectangle);
			SDL_RenderCopy(main_renderer,config[boxart_index+5].boxart,NULL,&farthest_right_boxart_destination_rectangle);
		}
		SDL_SetTextureColorMod(user_interface_texture,cursor_red_color_channel,cursor_green_color_channel,255);
		SDL_RenderCopy(main_renderer,user_interface_texture,&top_left_cursor_corner_source_rectangle,&top_left_cursor_corner_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&cursor_edge_source_rectangle,&top_cursor_edge_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&top_right_cursor_corner_source_rectangle,&top_right_cursor_corner_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&cursor_edge_source_rectangle,&right_cursor_edge_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&bottom_right_cursor_corner_source_rectangle,&bottom_right_cursor_corner_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&cursor_edge_source_rectangle,&bottom_cursor_edge_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&bottom_left_cursor_corner_source_rectangle,&bottom_left_cursor_corner_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&cursor_edge_source_rectangle,&left_cursor_edge_destination_rectangle);
		SDL_SetTextureColorMod(user_interface_texture,255,255,255);
		SDL_RenderCopy(main_renderer,user_interface_texture,&dpad_up_icon_source_rectangle,&dpad_up_icon_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&dpad_down_icon_source_rectangle,&dpad_down_icon_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&select_icon_source_rectangle,&select_icon_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&start_icon_source_rectangle,&start_icon_destination_rectangle);
		SDL_RenderCopy(main_renderer,user_interface_texture,&bottom_banner_source_rectangle,&bottom_banner_destination_rectangle);
		SDL_RenderCopyEx(main_renderer,user_interface_texture,&spinning_arrow_source_rectangle,&spinning_arrow_destination_rectangle,0,NULL,SDL_FLIP_VERTICAL);
		SDL_RenderCopy(main_renderer,thumbnails_texture,NULL,&thumbnails_destination_rectangle);
		
		/* Show the screen back buffer */
		SDL_RenderPresent(main_renderer);
		
		/* Play music intro followed by loop */
		if (!Mix_PlayingMusic()) {
			if (music_state == is_in_intro) {
				Mix_PlayMusic(intro_music,0);
				music_state = is_in_loop;
			}
			else if (music_state == is_in_loop)
				Mix_PlayMusic(loop_music,-1);
		}
	}
	
	/* Deinitalize sounds and music */
	Mix_FreeChunk(cursor_sound);
	Mix_FreeMusic(loop_music);
	Mix_FreeMusic(intro_music);
	
	/* Deinitalize textures */
	for(int i = 0; i < 21; i++)
		SDL_DestroyTexture(config[i].boxart);
	SDL_DestroyTexture(thumbnails_texture);
	SDL_DestroyTexture(user_interface_texture);
	SDL_DestroyTexture(wallpaper_texture);
	
	/* Deinitalize surfaces */
	SDL_FreeSurface(thumbnails_surface);
	SDL_FreeSurface(user_interface_surface);
	SDL_FreeSurface(wallpaper_surface);
	
	/* Deinitalize main window and renderer */
	SDL_DestroyRenderer(main_renderer);
	SDL_DestroyWindow(window);
	
	/* Deinitalize audio, image, and video libraries */
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	
	/* Return zero to the operating system. We're out of here! */
	return 0;
}

void read_config(vector<config_item> &config,SDL_Renderer *main_renderer) {
    ifstream file("Users/Config.txt");
	string buffer;
	for (int i = 0; i < 9; i++)
		getline(file, buffer, '\n');
    while (!file.eof()) {
		config_item current_item = config_item();
		getline(file, current_item.filename, '\n');
		SDL_Surface *tempSurface = IMG_Load(("Users/Boxart/" + current_item.filename + ".png").c_str());
		current_item.boxart = SDL_CreateTextureFromSurface(main_renderer,tempSurface);
		SDL_FreeSurface(tempSurface);
		getline(file, current_item.title, '\n');
		getline(file, current_item.publisher, '\n');
		getline(file, current_item.release_date, '\n');
		getline(file, current_item.number_of_players, '\n');
		config.push_back(current_item);
    }
}