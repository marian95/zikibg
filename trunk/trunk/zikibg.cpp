/* *************************************************************************
 * zikibg.cpp :
 * This is the mainline code for the zikibg game.
 * *************************************************************************
 * Author: Amer El-Abed
 * Email : amer.elabed@gmail.com
 * Blog  : aelabed.blogspot.com
 * 
 * Copyright 2009 Amer El-Abed
 * *************************************************************************
 * License: 
	This file is part of zikibg.

    zikibg is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    zikibg is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with zikibg.  If not, see <http://www.gnu.org/licenses/>.
 * *************************************************************************
 */

#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "resize.h"
#include "Triangle.h"
#include "Chip.h"
#include "Player.h"
#include "aea-zikilib.h"
#define SCREEN_MODE SDL_ANYFORMAT
//#define SCREEN_MODE SDL_FULLSCREEN
#define SCREEN_WIDTH 	1200
#if	SCREEN_WIDTH == 1200
	#define SCREEN_HEIGHT 		700
	#define TRIANGLE_OFFSET_X 	35
	#define TRIANGLE_OFFSET_TY 	17	
	#define TRIANGLE_OFFSET_BY 	9	
	#define TRIANGLE_SPACING 	85	
	#define CHIPS_INITIAL_X  	1120
	#define CHIPS_INITIAL_TY  	17
	#define CHIPS_INITIAL_BY 	620
	#define CHIPS_SPACING_Y		18
	#define CHIPS_SPACING_X		10
	#define DICE1_POS_X		845
	#define DICE1_POS_Y		350
	#define DICE2_POS_X		900
	#define DICE2_POS_Y		350
#elif	SCREEN_WIDTH == 1024
	#define SCREEN_HEIGHT 		700
	#define TRIANGLE_OFFSET_X 	28
	#define TRIANGLE_OFFSET_TY 	16
	#define TRIANGLE_OFFSET_BY 	9	
	#define TRIANGLE_SPACING 	74	
	#define CHIPS_INITIAL_X  	955	
	#define CHIPS_INITIAL_TY  	15
	#define CHIPS_INITIAL_BY 	630
	#define CHIPS_SPACING_Y		18
	#define CHIPS_SPACING_X		10
	#define DICE1_POS_X		695
	#define DICE1_POS_Y		350
	#define DICE2_POS_X		750
	#define DICE2_POS_Y		350
#elif	SCREEN_WIDTH == 800
	#define SCREEN_HEIGHT 		600
	#define TRIANGLE_OFFSET_X 	18
	#define TRIANGLE_OFFSET_TY 	15	
	#define TRIANGLE_OFFSET_BY 	9	
	#define TRIANGLE_SPACING 	58	
	#define CHIPS_INITIAL_X  	745	
	#define CHIPS_INITIAL_TY  	17
	#define CHIPS_INITIAL_BY 	545
	#define CHIPS_SPACING_Y		15
	#define CHIPS_SPACING_X		10
	#define DICE1_POS_X		545
	#define DICE1_POS_Y		300
	#define DICE2_POS_X		600
	#define DICE2_POS_Y		300
#endif
#define SCREEN_DEPTH 		32

#define SCALING_FILTER 		1
// original is 640 x 480

int main(int argc, char **argv)
{
  	char *name = "Ziki Backgammon";
	int dice1=0, dice2=0;
	float scale_triangle = 0;
	float scale_chip = 0;
	bool mouseIsFree=true;
	if (SCREEN_WIDTH == 1200)
	{	scale_triangle = 1.7;
		scale_chip = 1.3;
	} else if (SCREEN_WIDTH == 1024)
	{	scale_triangle = 1.5;
		scale_chip = 1.2;
	}else if (SCREEN_WIDTH == 800)
	{	scale_triangle = 1.2;
		scale_chip = 1.0;
	}
	Triangle triangle[26];
	Player p1, p2;
	
	SDL_Event event;

	SDL_Surface *screen = NULL;
	SDL_Surface *bmp_loading;	/* loading screen		*/
	SDL_Surface *bmp_board;		/* game board background	*/
	SDL_Surface *bmp_triangle_tb;	/* top black triangle		*/
	SDL_Surface *bmp_triangle_tw;	/* top white triangle		*/
	SDL_Surface *bmp_triangle_bb;	/* bottom black triangle 	*/
	SDL_Surface *bmp_triangle_bw;	/* bottom white triangle 	*/
	SDL_Surface *bmp_chip_w;	/* white chip			*/
	SDL_Surface *bmp_chip_b;	/* black chip			*/
	SDL_Surface *bmp_chip_turn;	/* turn chip			*/

	SDL_Surface *bmp_dice[6];
	SDL_Rect rect_board, rect_triangle_src, rect_loading, rect_chip,rect_dice, rect_dice1, rect_dice2, rect_chip_turn;

	bool done = false;
 
	char namet[50];

	srand(time(0));

	

	int g = sprintf(namet,"WiggaG");
	p1 = Player('w', 't', namet);

	g = sprintf(namet,"NiggaG");
	p2 = Player('b', 'b', namet);

	p1.turnToPlay = true;
	p2.turnToPlay = false;
	/*******************************************************************
		Init SDL
	*******************************************************************/
	if (-1 == SDL_Init(SDL_INIT_EVERYTHING))
	  {
	    printf("Can't initialize SDL\n");
	    exit(1);
	  }

	atexit(SDL_Quit);

	/*******************************************************************
		Init screen
	*******************************************************************/
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SCREEN_MODE | SDL_SWSURFACE );
	if (NULL == screen)
	{
		printf("Can't set video mode\n");
		exit(1);
	}

	/*	Set window title
	*/
	SDL_WM_SetCaption(name, name);

	/*******************************************************************
		Load images	
	*******************************************************************/
        if ((bmp_loading = SDL_LoadBMP("loading.bmp")) == NULL) {
                printf("Unable to load bitmap: %s\n", SDL_GetError());
                return 1;
        }
        bmp_loading = SDL_DisplayFormat(bmp_loading);

        if ((bmp_board = SDL_LoadBMP("board.bmp")) == NULL) {
                printf("Unable to load bitmap: %s\n", SDL_GetError());
                return 1;
        }
        bmp_board = SDL_DisplayFormat(bmp_board);

        if ((bmp_chip_turn = SDL_LoadBMP("chip-turn.bmp")) == NULL) {
                printf("Unable to load bitmap: %s\n", SDL_GetError());
                return 1;
        }
        bmp_chip_turn = SDL_DisplayFormat(bmp_chip_turn);

        if ((bmp_chip_b = SDL_LoadBMP("chip-b.bmp")) == NULL) {
                printf("Unable to load bitmap: %s\n", SDL_GetError());
                return 1;
        }
        bmp_chip_b = SDL_DisplayFormat(bmp_chip_b);

        if ((bmp_chip_w = SDL_LoadBMP("chip-w.bmp")) == NULL) {
                printf("Unable to load bitmap: %s\n", SDL_GetError());
                return 1;
        }
        bmp_chip_w = SDL_DisplayFormat(bmp_chip_w);

        if ((bmp_triangle_tb = SDL_LoadBMP("triangle-tb.bmp")) == NULL) {
                printf("Unable to load bitmap: %s\n", SDL_GetError());
                return 1;
        }
        bmp_triangle_tb = SDL_DisplayFormat(bmp_triangle_tb);

        if ((bmp_triangle_tw = SDL_LoadBMP("triangle-tw.bmp")) == NULL) {
                printf("Unable to load bitmap: %s\n", SDL_GetError());
                return 1;
        }
        bmp_triangle_tw = SDL_DisplayFormat(bmp_triangle_tw);

        if ((bmp_triangle_bb = SDL_LoadBMP("triangle-bb.bmp")) == NULL) {
                printf("Unable to load bitmap: %s\n", SDL_GetError());
                return 1;
        }
        bmp_triangle_bb = SDL_DisplayFormat(bmp_triangle_bb);

        if ((bmp_triangle_bw = SDL_LoadBMP("triangle-bw.bmp")) == NULL) {
                printf("Unable to load bitmap: %s\n", SDL_GetError());
                return 1;
        }
        bmp_triangle_bw = SDL_DisplayFormat(bmp_triangle_bw);

	char file[11]; int x;
	for(int d=0;d<6;d++)
	{	
		x = sprintf(file,"dice%d.bmp", (d+1) );
		bmp_dice[d] = SDL_LoadBMP(file);
		if (bmp_dice[d] == NULL) {
			printf("Unable to load bitmap: %s\n", SDL_GetError());
			return 1;
		}
	}
        bmp_triangle_bw = SDL_DisplayFormat(bmp_triangle_bw);
	
	rect_loading.x = 0;
        rect_loading.y = 0;
        rect_loading.w = SCREEN_WIDTH;
        rect_loading.h = SCREEN_HEIGHT;
	SDL_FillRect(screen, NULL, 0);
        SDL_BlitSurface(bmp_loading, &rect_loading, screen, &rect_loading);
        SDL_Flip(screen);
	//        SDL_FreeSurface(temp);


	/*******************************************************************
		Size up / Scale objects
	*******************************************************************/
	bmp_board = SDL_Resize(bmp_board, SCREEN_WIDTH, SCREEN_HEIGHT, SCALING_FILTER);
	bmp_chip_w = SDL_Resize(bmp_chip_w, scale_chip, SCALING_FILTER);
	bmp_chip_b = SDL_Resize(bmp_chip_b, scale_chip, SCALING_FILTER);
	bmp_chip_turn = SDL_Resize(bmp_chip_turn, scale_chip, SCALING_FILTER);
	bmp_triangle_tb = SDL_Resize(bmp_triangle_tb, scale_triangle , SCALING_FILTER);
	bmp_triangle_tw = SDL_Resize(bmp_triangle_tw, scale_triangle , SCALING_FILTER);
	bmp_triangle_bb = SDL_Resize(bmp_triangle_bb, scale_triangle ,SCALING_FILTER );
	bmp_triangle_bw = SDL_Resize(bmp_triangle_bw, scale_triangle ,SCALING_FILTER );

	
	rect_board.x = 0;
        rect_board.y = 0;
        rect_board.w = SCREEN_WIDTH;
        rect_board.h = SCREEN_HEIGHT;

	rect_chip.x = 0;
	rect_chip.y = 0;
	rect_chip.w = bmp_chip_w->w;
	rect_chip.h = bmp_chip_w->h;

	rect_chip_turn.x = 8 *(SCREEN_WIDTH / 18);
	rect_chip_turn.y = SCREEN_HEIGHT / 4;
	rect_chip_turn.w = bmp_chip_w->w;
	rect_chip_turn.h = bmp_chip_w->h;


	rect_triangle_src.x = 0;
        rect_triangle_src.y = 0;
        rect_triangle_src.w = bmp_triangle_tb->w;
        rect_triangle_src.h = bmp_triangle_tb->h;

	rect_dice.x=0;
	rect_dice.y=0;
	rect_dice.w=bmp_dice[0]->w;
	rect_dice.h=bmp_dice[0]->h;

	rect_dice1.x=DICE1_POS_X;
	rect_dice1.y=DICE1_POS_Y;
	rect_dice1.w=bmp_dice[0]->w;
	rect_dice1.h=bmp_dice[0]->h;

	rect_dice2.x=DICE2_POS_X;
	rect_dice2.y=DICE2_POS_Y;
	rect_dice2.w=bmp_dice[0]->w;
	rect_dice2.h=bmp_dice[0]->h;
	/*******************************************************************
		Set up object positions
	*******************************************************************/
	/* Set up triangles */
	int pX = TRIANGLE_OFFSET_X;
	int i = 0;
	char colour = 'b';
	for( i=12; i>=7; i--)
	{	
		colour=(colour=='b')?'w':'b';
		triangle[i] =  Triangle( i, colour, pX, TRIANGLE_OFFSET_TY, scale_triangle, 't');
		pX+= TRIANGLE_SPACING;
	}
		pX+= TRIANGLE_OFFSET_X;
	for( i=6; i>=0; i--)
	{	
		colour=(colour=='b')?'w':'b';
		triangle[i] =  Triangle( i, colour, pX, TRIANGLE_OFFSET_TY, scale_triangle, 't');
		pX+= TRIANGLE_SPACING;
	}
	pX = TRIANGLE_OFFSET_X;
	colour = 'w';
	for( i=13; i<=18; i++)
	{	
		colour=(colour=='b')?'w':'b';
		triangle[i] = Triangle( i, colour, pX, (SCREEN_HEIGHT - bmp_triangle_tb->h - TRIANGLE_OFFSET_BY ), scale_triangle, 'b');
		pX+= TRIANGLE_SPACING;
	}
		pX+= TRIANGLE_OFFSET_X;
	for( i=19; i<=25; i++)
	{	
		colour=(colour=='b')?'w':'b';
		triangle[i] = Triangle( i, colour, pX, (SCREEN_HEIGHT - bmp_triangle_tb->h - TRIANGLE_OFFSET_BY ), scale_triangle, 'b');
		pX+= TRIANGLE_SPACING;
	}

	/* Set up chips */
	p1.setChips(CHIPS_INITIAL_TY, CHIPS_INITIAL_BY, CHIPS_INITIAL_X, CHIPS_SPACING_Y, bmp_chip_w->w, bmp_chip_w->h, scale_chip);
	p2.setChips(CHIPS_INITIAL_TY, CHIPS_INITIAL_BY, CHIPS_INITIAL_X, CHIPS_SPACING_Y, bmp_chip_w->w, bmp_chip_w->h, scale_chip);
     
	/*******************************************************************
		Set transparency
	*******************************************************************/
        if(SDL_SetColorKey(bmp_board, SDL_SRCCOLORKEY, SDL_MapRGB(bmp_board->format, 255, 255, 255)) < 0) {
                puts ("Set Color key failed");
                return 1;
        }
	if(SDL_SetColorKey(bmp_chip_w, SDL_SRCCOLORKEY, SDL_MapRGB(bmp_chip_w->format, 0, 0, 0)) < 0) {
                puts ("Set Color key failed");
                return 1;
        }
	if(SDL_SetColorKey(bmp_chip_b, SDL_SRCCOLORKEY, SDL_MapRGB(bmp_chip_b->format, 0, 0, 0)) < 0) {
                puts ("Set Color key failed");
                return 1;
        }
	if(SDL_SetColorKey(bmp_chip_turn, SDL_SRCCOLORKEY, SDL_MapRGB(bmp_chip_turn->format, 0, 0, 0)) < 0) {
                puts ("Set Color key failed");
                return 1;
        }

	if(SDL_SetColorKey(bmp_triangle_tb, SDL_SRCCOLORKEY, SDL_MapRGB(bmp_triangle_tb->format, 255, 255, 255)) < 0) {
                puts ("Set Color key failed");
                return 1;
        }
	if(SDL_SetColorKey(bmp_triangle_tw, SDL_SRCCOLORKEY, SDL_MapRGB(bmp_triangle_tw->format, 255, 255, 255)) < 0) {
                puts ("Set Color key failed");
                return 1;
        }
	if(SDL_SetColorKey(bmp_triangle_bb, SDL_SRCCOLORKEY, SDL_MapRGB(bmp_triangle_bb->format, 255, 255, 255)) < 0) {
                puts ("Set Color key failed");
                return 1;
        }
	if(SDL_SetColorKey(bmp_triangle_bw, SDL_SRCCOLORKEY, SDL_MapRGB(bmp_triangle_bw->format, 255, 255, 255)) < 0) {
                puts ("Set Color key failed");
                return 1;
        }


	triangle[0].rect.x = p1.chips[0].rectd.x;
	triangle[25].rect.x = p1.chips[0].rectd.x;

	/*******************************************************************
		Main loop
	*******************************************************************/

        while (!done)
        {	
		//  Check if player has played both turns
		if (p1.dice1Played && p1.dice2Played)
		{
			printf("Player 1's turn is over.\n");
			p1.turnToPlay=false;
			p2.turnToPlay=true;
			p1.dice1Played=false;
			p1.dice2Played=false;
			p1.dice1Rolled=false;
			p1.dice2Rolled=false;
			printf("Player 2's turn has begun.\n");
		}
		if (p2.dice1Played && p2.dice2Played)
		{
			printf("Player 2's turn is over.\n");
			p2.turnToPlay=false;
			p1.turnToPlay=true;
			p2.dice1Played=false;
			p2.dice2Played=false;
			p2.dice1Rolled=false;
			p2.dice2Rolled=false;
			printf("Player 1's turn has begun.\n");
		}
		/*******************************************
			Check for events
		*******************************************/
                while (!done && SDL_PollEvent(&event))
                {
                        if (event.type == SDL_KEYDOWN)
			{
                                switch (event.key.keysym.sym)
                                {
					case SDLK_ESCAPE:
                                                done =1 ;
                                                break;
				}	
			} else if ( event.type == SDL_MOUSEBUTTONDOWN )
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					//printf("mouse left ");
					if(event.button.state == SDL_PRESSED)
					{
						//printf(" pressed at %d , %d\n", event.button.x, event.button.y);
						if (wasItemClicked(&rect_dice1, event.button.x, event.button.y))
						{
							printf("D1: %d\n", dice1);
							if(p1.turnToPlay && !p1.dice1Rolled)
							{	dice1 = rand() %6;
								p1.dice1Rolled=true;
							}
							if(p2.turnToPlay && !p2.dice1Rolled)
							{	dice1 = rand() %6;
								p2.dice1Rolled=true;
							}
						}

						if (wasItemClicked(&rect_dice2, event.button.x, event.button.y))
						{
							printf("D2: %d\n", dice2);
							if(p1.turnToPlay && !p1.dice2Rolled)
							{	dice2 = rand() %6;
								p1.dice2Rolled=true;
							}
							if(p2.turnToPlay && !p2.dice2Rolled)
							{	dice2 = rand() %6;
								p2.dice2Rolled=true;
							}
						}

						for (i=0; i<=25; i++)
						{
							if(wasItemClicked(&triangle[i].rect, event.button.x, event.button.y))
							{
								printf("Triangle %d clicked\n",i);
								int dc = doChip(triangle[i].posId, &p1, &p2, (p1.turnToPlay)?1:2, dice1+1, dice2+1);
							}
						}

					}
					if(event.button.state == SDL_RELEASED)
						printf(" released at %d , %d\n", event.button.x, event.button.y);
				}
				if (event.button.button == SDL_BUTTON_MIDDLE)
					printf("mouse middle\n");
				if (event.button.button == SDL_BUTTON_RIGHT)
				{
					printf("mouse right\n");
					cancelPlay( &p1, &p2, (p1.turnToPlay)?1:2 );
				}
			}
		}

		/*******************************************
			Paint objects
		*******************************************/
		SDL_FillRect(screen, NULL, 0);
		/**********************************************************************
			Temp solution
		**********************************************************************/
		i=0;
			if(triangle[i].top_bottom == 't' && triangle[i].colour=='w')
	                	SDL_BlitSurface(bmp_triangle_tw, &rect_triangle_src, screen, &triangle[i].rect);

			else if(triangle[i].top_bottom == 't' && triangle[i].colour=='b')
	                	SDL_BlitSurface(bmp_triangle_tb, &rect_triangle_src, screen, &triangle[i].rect);

			else if(triangle[i].top_bottom == 'b' && triangle[i].colour=='w')
	                	SDL_BlitSurface(bmp_triangle_bw, &rect_triangle_src, screen, &triangle[i].rect);

			else if(triangle[i].top_bottom == 'b' && triangle[i].colour=='b')
	                	SDL_BlitSurface(bmp_triangle_bb, &rect_triangle_src, screen, &triangle[i].rect);
			else
				puts("No triangle ");
		i=25;
			if(triangle[i].top_bottom == 't' && triangle[i].colour=='w')
	                	SDL_BlitSurface(bmp_triangle_tw, &rect_triangle_src, screen, &triangle[i].rect);

			else if(triangle[i].top_bottom == 't' && triangle[i].colour=='b')
	                	SDL_BlitSurface(bmp_triangle_tb, &rect_triangle_src, screen, &triangle[i].rect);

			else if(triangle[i].top_bottom == 'b' && triangle[i].colour=='w')
	                	SDL_BlitSurface(bmp_triangle_bw, &rect_triangle_src, screen, &triangle[i].rect);

			else if(triangle[i].top_bottom == 'b' && triangle[i].colour=='b')
	                	SDL_BlitSurface(bmp_triangle_bb, &rect_triangle_src, screen, &triangle[i].rect);
			else
				puts("No triangle ");
		/**********************************************************************
			Temp solution
		**********************************************************************/
                SDL_BlitSurface(bmp_board, &rect_board, screen, &rect_board);
		for(i=1;i<=24;i++)
		{
			if(triangle[i].top_bottom == 't' && triangle[i].colour=='w')
	                	SDL_BlitSurface(bmp_triangle_tw, &rect_triangle_src, screen, &triangle[i].rect);

			else if(triangle[i].top_bottom == 't' && triangle[i].colour=='b')
	                	SDL_BlitSurface(bmp_triangle_tb, &rect_triangle_src, screen, &triangle[i].rect);

			else if(triangle[i].top_bottom == 'b' && triangle[i].colour=='w')
	                	SDL_BlitSurface(bmp_triangle_bw, &rect_triangle_src, screen, &triangle[i].rect);

			else if(triangle[i].top_bottom == 'b' && triangle[i].colour=='b')
	                	SDL_BlitSurface(bmp_triangle_bb, &rect_triangle_src, screen, &triangle[i].rect);
			else
				puts("No triangle ");
		}

		for(i=0;i<=25;i++)
			reconcileChipsXY(&triangle[i] , &p1, &p2, CHIPS_SPACING_Y, CHIPS_INITIAL_TY, CHIPS_INITIAL_BY);
		drawChips(&p1, &p2, screen, bmp_chip_b, bmp_chip_w, &event);
	
		if (p1.turnToPlay)
			rect_chip_turn.y= SCREEN_HEIGHT / 10;
		else
			rect_chip_turn.y= SCREEN_HEIGHT - (SCREEN_HEIGHT / 10);
			
		SDL_BlitSurface(bmp_chip_turn, &rect_chip , screen, &rect_chip_turn);
		SDL_BlitSurface(bmp_dice[dice1], &rect_dice, screen, &rect_dice1);
		SDL_BlitSurface(bmp_dice[dice2], &rect_dice, screen, &rect_dice2);
                SDL_Flip(screen);
	}

	SDL_Quit();
}
