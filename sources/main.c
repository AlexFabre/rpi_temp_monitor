#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../headers/temp.h"
#include "../headers/led-matrix-c.h"


int main(int argc, char **argv) {
	uint16_t cpu_temp; /*< Température du CPU en 3 digits 423 pour 42.3 degrés celcius*/

	uint8_t time=0, 
			cpt,
			decalage = 0,
			taby[64];
	uint8_t tabptr;

	struct RGBLedMatrixOptions options;
	struct RGBLedMatrix *matrix;
	struct LedCanvas *graph_canvas;
	int width, height;

	/* Création de la matrice */
	memset(&options, 0, sizeof(options));
	options.rows = 32;
	options.cols = 64;
	options.chain_length = 1;

	matrix = led_matrix_create_from_options(&options, &argc, &argv);
	if (matrix == NULL)
		return 1;

	/***************************
	 * Création du canvas
	*****************************/
	graph_canvas = led_matrix_create_offscreen_canvas(matrix);

	led_canvas_get_size(graph_canvas, &width, &height);

	while(1) {

		/** Clear screen (black). */
		led_canvas_clear(graph_canvas);

		/* Récupération de la température */
		cpu_temp = get_temperature();
		printf("%d\n",cpu_temp);

		taby[time]=(cpu_temp/10)-20; /* A scaler entre 20 et 20+32 */
		printf("%dvaleur tab:\n",taby[time]);


		/** Set pixel at (x, y) with color (r,g,b). */
		cpt=time;
		do {
			led_canvas_set_pixel(graph_canvas, cpt, height-taby[cpt], 100, 100, 100);
			cpt--;
		}while(cpt);

		if(time < width){
			time++;
		}
		else {
			/* tabptr=taby++; */;
		}

		graph_canvas = led_matrix_swap_on_vsync(matrix, graph_canvas);

		sleep(1);
	}

	/*
	* Make sure to always call led_matrix_delete() in the end to reset the
	* display. Installing signal handlers for defined exit is a good idea.
	*/
	led_matrix_delete(matrix);

	return 0;
}
