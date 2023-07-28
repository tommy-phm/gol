/**
 * @file gl.c
 * @brief Main program for Conway Game of Life
 * @details 
 * Run Convey's Game of Life
 * Accept setting for the following:
 * Screen size (widht & hight)
 * Cell color and upscale ratio (sprite size)
 * Test condition if cell is alive or dead
 * Update status to next generation
 * Import patterns to matrix
 * Clear array from memoary
 * 
 * @author Tommy Pham
 * @date Fall 2020
 * @bugs None
 * @todo none
 */

#include <stdio.h>
#include <stdlib.h>
#include "sdl.h"
#include "life.h"
#include <string.h>
#include <ctype.h>
#include <unistd.h> /* used for getopt */
#include <errno.h>

/** Run Convey's Game of Life. Accept input as settings.
 * @remark Extra Crdit: Pattern 1 is store in fp. Pattern 2 is store in Qp. Pattern 3 is tore in Pp. Argument are compatable with BOTH 1.05 and 1.06.
 * @param argc Number of command line arguments
 * @param argv String or arguments.
 * @param c arument letter
 * @param width size of screen width
 * @param height size of screen height
 * @param sprite_size ratio to upscale pixles
 * @param red pixle's intensity value for red
 * @param green ixle's intensity value for green
 * @param blue pixle's intensity value for blue
 * @param type type of edge - hedge, torus, klein
 * @param m_row number of row in matrix
 * @param m_col number of collum in matrix
 * @param p present matrix - current generation
 * @param f future matrix - next generation
 * @param t temporary matrix to swap
 * @param m_row numbers of rows in matrix
 * @param n_col numbers of collums in matrix
 * @param fp file containing pattern
 * @param x initial x cordinate for f pattern
 * @param y initial y cordinate for f pattern
 * @param Qp file containing Q pattern
 * @param PP file containing P pattern
 * @param q_x initial x cordinate for Q pattern
 * @param q_yinitial y cordinate for Q pattern
 * @param p_xinitial x cordinate for P pattern
 * @param p_y initial y cordinate for P pattern
 */
int main(int argc, char *argv[])
{
	FILE *fp = NULL, *Qp = NULL, *Pp = NULL;
	int c, width = 400, height = 400, x = 0, y = 0, q_x = 0, q_y = 0, p_x = 0, p_y = 0; /* either 2, 4, 8, or 16 */
	unsigned char red = 255, green = 255, blue = 255, sprite_size = 16, type = 'h';

	while((c = getopt(argc, argv, "w:h:e:r:g:b:s:f:P:Q:o:p:q:H")) != -1)
		switch(c) {
		case 'w':
			width = atoi(optarg);
			if( !(width>0) ){
				printf("Invalid width value. Value must be greater than 0.\n");
				exit(EXIT_FAILURE);
			}
 			break;
		case 'h':
			height = atoi(optarg);
			if( !(height>0) ){
				printf("Invalid height value. Value must be greater than 0.\n");
				exit(EXIT_FAILURE);
			}
			break;
		case 'e':
			if( !( !(strcmp(optarg,"hedge")) || !(strcmp(optarg,"torus")) || !(strcmp(optarg,"klein")) ) ){
			printf("Invalid edge value. Value must be \"hedge\" \"torus\" \"klein\".\n");
			exit(EXIT_FAILURE);
			}
			type = optarg[0];
			break;
		case 'r':
			if( !(atoi(optarg) >= 0 && atoi(optarg) <=255) ){
				printf("Invalid red value. [0-255]\n");
				exit(EXIT_FAILURE);
			}
			red = atoi(optarg);
			break;
		case 'g':
			if( !(atoi(optarg) >= 0 && atoi(optarg) <=255) ){
				printf("Invalid green value. [0-255]\n");
				exit(EXIT_FAILURE);
			}
			green = atoi(optarg);
			break;
		case 'b':
			if( !(atoi(optarg) >= 0 && atoi(optarg) <=255) ){
				printf("Invalid blue value. [0-255]\n");
				exit(EXIT_FAILURE);
			}
			blue = atoi(optarg);
			break;
		case 's':
			if( 16%atoi(optarg) != 0 || atoi(optarg) == 1 ){
				printf("Invalid spritesize value. Valid values are 2, 4, 8, or 16 only.\n");
				exit(EXIT_FAILURE);
				}
			sprite_size = atoi(optarg);
			break;
		case 'P':
			errno = 0; /* set to 0 so can process it if an error occurs */
                        /* assume filename comes after -f */
			/* optarg contains the argument for the option */
			Pp = fopen(optarg, "r");
			if (Pp == NULL) {
				/* strerror */
				fprintf(stderr, "%s: argument to option '-P' failed: %s\n", argv[0], strerror(errno));
				exit(EXIT_FAILURE);
			}
			break;
		case 'Q':
			errno = 0; /* set to 0 so can process it if an error occurs */
                        /* assume filename comes after -Q */
			/* optarg contains the argument for the option */
			Qp = fopen(optarg, "r");
			if (Qp == NULL) {
				/* strerror */
				fprintf(stderr, "%s: argument to option '-Q' failed: %s\n", argv[0], strerror(errno));
				exit(EXIT_FAILURE);
			}
			break;
		case 'f':
			errno = 0; /* set to 0 so can process it if an error occurs */
                        /* assume filename comes after -f */
			/* optarg contains the argument for the option */
			fp = fopen(optarg, "r");
			if (fp == NULL) {
				/* strerror */
				fprintf(stderr, "%s: argument to option '-f' failed: %s\n", argv[0], strerror(errno));
				exit(EXIT_FAILURE);
			}

			break;
		case 'o':
			sscanf(optarg,"%d,%d",&x, &y);
			break;
		case 'p':
			sscanf(optarg,"%d,%d",&p_x, &p_y);
			break;
		case 'q':
            sscanf(optarg,"%d,%d",&q_x, &q_y);
			break;

		case 'H': 	/* help */
			printf("usage: life -w -h -e -r -g -b -s -f filename pattern -o \n");
			printf("-w width of the screen argument 640, 800, 1024, etc.\n");
			printf("-h height of the screen argument 480, 600, 768, etc.\n");
			printf("-e type of edge. Values are hedge, torus or klein (strings)\n");
			printf("-r the red color value, an integer between [0, 255]\n");
			printf("-g the green color value, an integer between [0, 255]\n");
			printf("-b the blue color value, an integer between [0, 255]\n");
			printf("-s size of the sprite. Valid values are 2, 4, 8, or 16 only. An integer.\n");
			printf("-f filename, a life pattern in file format 1.06.\n");
			printf("-o x,y the initial x,y coordinate of the pattern found in the file. Nospace between the x and y.\n");
			printf("help, print out usage information and a brief description of each, option.\n");
			printf("-P filename, a life pattern in file format 1.06\n");
			printf("-Q filename, a life pattern in file format 1.05\n");
			printf("-p x,y the initial coordinate of pattern P\n");
			printf("-q x,y the initial coordinate of pattern Q\n");
			exit(EXIT_SUCCESS);
		case ':':
			/* missing option argument */
			/* optopt contains the option */
			/* argv[0] is the programs name */
			/* stderr is 1 of 3 files open for you -- stdin, stdout, stderr. Usually they are mapped to the same location */
			fprintf(stderr, "%s: option '-%c' requires an argument\n", argv[0], optopt);
			exit(EXIT_FAILURE);
		case '?': 	/* getopt default invalid option */
		default:
			printf("illegal option %c - ignored\n", optopt);
			printf("usage: life -w -h -r -g -b -s -f filename pattern -o -e\n");
			break;
				
		}
		//printf("w%d h%d e%c r%d g%d b%d s%d f%p %d %d\n", width, height, type, red, green, blue, sprite_size, fp, x, y);

	//return 0;
    int m_row = width/sprite_size, n_col = height/sprite_size;
	struct sdl_info_t sdl_info; /* this is needed to graphically display the game */
	init_sdl_info(&sdl_info, width, height, sprite_size, red, green, blue);

	unsigned char **a = init_matrix(m_row,n_col), **b = init_matrix(m_row,n_col), **tmp;

	if( !(a) || !(b) ){
		printf("Matrix Initialization has failed.\n");
		exit(EXIT_FAILURE);
	}

	if ((fp != NULL))
		pattern_in(a, type, fp, x, y, m_row, n_col);
	if ((Qp != NULL))
		pattern_in(a, type, Qp, q_x, q_y, m_row, n_col);
	if ((Pp != NULL))
		pattern_in(a, type, Pp, p_x, p_y, m_row, n_col);
	else{
		a[1][0] = 1;
		a[1][1] = 1;
		a[1][2] = 1;
	}

	while (1)
	{
		/* your game of life code goes here  */		

		/* change the  modulus value to slow the rendering */
		if (SDL_GetTicks() % 1 == 0){

		sdl_render_life(&sdl_info, a);

		switch(type){
			case 'h':
				hedge(a, b, m_row, n_col);
				break;
			case 't':
				torus(a, b, m_row, n_col);
				break;
			case 'k':
				klein(a, b, m_row, n_col);
				break;
		}

		mid(a, b, m_row, n_col);

		tmp = a;
		a = b;
		b = tmp;

		/* Poll for events, and handle the ones we care about. 
		* You can click the X button to close the window
		*/
		SDL_Event event;

		while (SDL_PollEvent(&event)) 
		{
			switch (event.type) 
			{
			case SDL_KEYDOWN:
				break;
			case SDL_KEYUP:
                    /* If escape is pressed, return (and thus, quit) */
				if (event.key.keysym.sym == SDLK_ESCAPE){
					free_matrix(a, m_row);
    				free_matrix(b, m_row);
					fclose(fp);
					return 0;}
				break;
			case SDL_QUIT:
				return(0);
			}
		}
		}
	}
	return 0;
}
