#include <stdio.h>
#include "../headers/temp.h"

int main(int argc, char const *argv[]) {
	uint16_t t;

	t=get_temperature();
	printf("%d\n",t);

	return 0;
}

