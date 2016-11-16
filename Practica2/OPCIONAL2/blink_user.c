#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h> 
#include <time.h>

#define LEDS 8
#define SIZE 87

MODULE_DESCRIPTION("BlinkUser");
MODULE_AUTHOR("Marta Rodenas de Miguel & Cristian Pinto Lozano");

const char *file = "/dev/usb/blinkstick0";

void show_error();
int get_opt(const char *sec);
int random_colors();
int power_off_leds();

int main( int argc, const char *argv[] )
{
	if(argc != 2){
		show_error();
		return 1;
	}
	if(get_opt(argv[1]) != 0 || power_off_leds() != 0){
		printf("%s\n", "Blinkstick Error");
		return 1;
	}

	return 0;
}


void show_error(){
	printf("Usage: ./blink_user.c [option]\n");
	printf("where [option] should be replaced by one of the following options\n");
	printf("\t- random - generate random colors for the leds\n");
	printf("\t-  \n");
	printf("\t-  \n");
	printf("\t-  off = turn off all the leds\n");
}

int get_opt(const char *sec){
	if(strcmp(sec,"random")==0){
		int val = 0;
		for(val; val < 5; val++){
			if(random_colors() != 0)
				return -1;
			usleep(1000000);
		}
		return 0;
	}
	else if(strcmp(sec,"pares")==0){
		int val = 0;
		for(val; val < 5; val++){
			if(col_pares() != 0)
				return -1;
			usleep(1000000);
		}
		return 0;
	}
	else if(strcmp(sec,"impares")==0){
		int val = 0;
		for(val; val < 5; val++){
			if(col_impares() != 0)
				return -1;
			usleep(1000000);
		}
		return 0;
	}
	else if(strcmp(sec,"off")==0)
		return power_off_leds();
	else {
		show_error();
		return -1;
	}
}

int random_colors(){
	unsigned char str[LEDS][LEDS] = {};
	const char *hex_digits = "0123456789ABCDEF";
	int i, j, k;
	time_t t;
	FILE *fd = fopen(file, "w");
	if(fd == NULL)
		return -1;
	char *string = malloc(SIZE);
	srand((unsigned) time(&t));
	for(j = 0; j < LEDS; j++){
		str[j][0] = j + '0';
		str[j][1] = ':';
		str[j][2] = '0';
		str[j][3] = 'x';
		for( i = 4 ; i < LEDS + 2; i++ ) {
			str[j][i] = hex_digits[ ( rand() % 16 ) ];
		}
		strcat(string, str[j]);
		if(j != LEDS - 1)
			strcat(string, ",");
	}
	fprintf(fd, "%s", string);
	free(string);
	fclose(fd);
	return 0;
}

int col_pares(){
	unsigned char str[LEDS][LEDS] = {};
	const char *hex_digits = "0123456789ABCDEF";
	int i, j;
	time_t t;
	FILE *fd = fopen(file, "w");
	if(fd == NULL)
		return -1;
	char *string = malloc(SIZE);
	srand((unsigned) time(&t));
	for(j = 0; j < LEDS; j+=2){
		str[j][0] = j + '0';
		str[j][1] = ':';
		str[j][2] = '0';
		str[j][3] = 'x';
		for( i = 4 ; i < LEDS + 2; i++ ) {
			str[0][i] = hex_digits[ ( rand() % 16 ) ];
			str[2][i] = hex_digits[ ( rand() % 16 ) ];
			str[4][i] = hex_digits[ ( rand() % 16 ) ];
			str[6][i] = hex_digits[ ( rand() % 16 ) ];
		}
		strcat(string, str[j]);
		if(j != LEDS - 1)
			strcat(string, ",");
	}
	fprintf(fd, "%s", string);
	free(string);
	fclose(fd);
	return 0;
}

int col_impares(){
	unsigned char str[LEDS][LEDS] = {};
	const char *hex_digits = "0123456789ABCDEF";
	int i, j;
	time_t t;
	FILE *fd = fopen(file, "w");
	if(fd == NULL)
		return -1;
	char *string = malloc(SIZE);
	srand((unsigned) time(&t));
	for(j = 1; j < LEDS; j+=2){
		str[j][0] = j + '0';
		str[j][1] = ':';
		str[j][2] = '0';
		str[j][3] = 'x';
		for( i = 4 ; i < LEDS + 2; i++ ) {
			str[1][i] = hex_digits[ ( rand() % 16 ) ];
			str[3][i] = hex_digits[ ( rand() % 16 ) ];
			str[5][i] = hex_digits[ ( rand() % 16 ) ];
			str[7][i] = hex_digits[ ( rand() % 16 ) ];
		}
		strcat(string, str[j]);
		if(j != LEDS - 1)
			strcat(string, ",");
	}
	fprintf(fd, "%s", string);
	free(string);
	fclose(fd);
	return 0;
}

int power_off_leds() {
	FILE *fd = fopen(file, "w");
	if(fd == NULL){
		return -1;
	}
	fprintf(fd, "%s", " ");
	fclose(fd);
	return 0;
}
