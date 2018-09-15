#include <stdio.h>
#include <stdlib.h>

#include "../headers/temp.h"

uint16_t get_temperature(void)
{
	char temp[LENGTH_TEMP_READ];
	uint16_t temperature;

	FILE * temp_file = fopen ("/sys/class/thermal/thermal_zone0/temp", "rb");

	if (temp_file) {
	  fread (temp, 1, LENGTH_TEMP_READ, temp_file);
	  fclose (temp_file);
	}
	else {
	  /* File couldn't be read */
	  return 1;
	}

	if(temp[0]>='1') {
	  temp[3]=0;
	}
	else {
	/* Pi temperature under 10 degrees ! */
	  return 2;
	}

	temperature = strtol(temp, NULL, 10);
	return temperature;
}
