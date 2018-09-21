#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../headers/temp.h"
#include "../headers/led-matrix-c.h"

#define WIDTH 64
#define LENGTH 32
#define AXIS_Y_MIN 20
#define AXIS_Y_MAX 52


int main(int argc, char **argv) {
	uint16_t cpu_temp; /*< Température du CPU en 3 digits 423 pour 42.3 degrés celcius*/
	char temp_string[6];

	const char *bdf_font_file;
	struct LedFont *font;

	uint8_t time=0, 
			cpt,
			t_values[WIDTH],
			i,
			tmp_tab[WIDTH];

	struct RGBLedMatrixOptions options;
	struct RGBLedMatrix *matrix;
	struct LedCanvas *graph_canvas;
	int width, height;

	/* Création de la matrice */
	memset(&options, 0, sizeof(options));
	options.rows = LENGTH;
	options.cols = WIDTH;
	options.chain_length = 1;

	matrix = led_matrix_create_from_options(&options, &argc, &argv);
	if (matrix == NULL)
		return 1;

	/***************************
	 * Création du canvas
	*****************************/
	graph_canvas = led_matrix_create_offscreen_canvas(matrix);

	led_canvas_get_size(graph_canvas, &width, &height);

	/************************
	 * Chargement de la police
	 *************************/
	bdf_font_file = "/etc/4x6.bdf";
	font = load_font(bdf_font_file);

	while(1) {

		/** Clear screen (black). */
		led_canvas_clear(graph_canvas);

		/* Récupération de la température */
		cpu_temp = get_temperature();
		/* Formattage en une chaine de caractère affichable */
		sprintf(temp_string,"%d",cpu_temp);
		temp_string[3] = temp_string[2];
		temp_string[2] = '.';
		temp_string[4] = 'C';
		temp_string[5] = 0;
		printf("%s\n", temp_string);

		/* Affichage de la température en text */
		draw_text(graph_canvas, font, 30, 8, 200, 200, 200, temp_string, 0);

		/* Décalage des echantillons dans le tableau*/
		if(time){
			for(i=0;i<WIDTH;++i) {
				tmp_tab[i] = t_values[i];
			}
			for(i=1;i<WIDTH;++i) {
				t_values[i] = tmp_tab[i-1];
			}
		}

		/* Enregistrement de la valeur actuelle*/
		t_values[0]=(cpu_temp/10)-AXIS_Y_MIN;

		/** Set pixel at (x, y) with color (r,g,b). */
		cpt=0;

		while(cpt<=time) {
			draw_line(graph_canvas, cpt, height-t_values[cpt], cpt, height, 112, 243, 166);
			led_canvas_set_pixel(graph_canvas, cpt, height-t_values[cpt], 255, 56, 96);
			cpt++;
		}

		graph_canvas = led_matrix_swap_on_vsync(matrix, graph_canvas);

		time++;
		sleep(1);
	}

	/*
	* Make sure to always call led_matrix_delete() in the end to reset the
	* display. Installing signal handlers for defined exit is a good idea.
	*/
	led_matrix_delete(matrix);

	return 0;
}
