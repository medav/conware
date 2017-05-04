/*
 * conway.h
 *
 *  Created on: May 4, 2017
 *      Author: quanta
 */

#ifndef CONWAY_H_
#define CONWAY_H_

#define DEAD_CELL 0x0000
#define ALIVE_CELL 0x00F0

#define GAME_HEIGHT 128
#define GAME_WIDTH 128

void raster_game(char game_grid[GAME_HEIGHT][GAME_WIDTH], short current_frame[480][640], int x, int y, int width, int height);
void place_block(char game_grid[GAME_HEIGHT][GAME_WIDTH], int x, int y);
void place_glider(char game_grid[GAME_HEIGHT][GAME_WIDTH], int x, int y);
void place_blinker(char game_grid[GAME_HEIGHT][GAME_WIDTH], int x, int y);
void evolve(char game_grid[GAME_HEIGHT][GAME_WIDTH]);


#endif /* CONWAY_H_ */
