#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/errno.h>
#include <sys/syscall.h>
#include <linux/unistd.h>

#ifdef __i386__
#define __NR_LED 353
#else
#define __NR_LED 316
#endif

MODULE_DESCRIPTION("LedSctl");
MODULE_AUTHOR("Marta Rodenas de Miguel & Cristian Pinto Lozano");

long ledctl(int num);

int main(int argc, char **argv) {
	
	//check num of arguments
	if(argc !=2) {
		fprintf(stderr,"El nº de argumentos es incorrecto.\n");
		return -EXIT_FAILURE;	
	}

	if(ledctl(strtol(argv[1], NULL, 16)) == -1) {
		perror("Se ha producido un error");
		return -EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;

}


/*call the syscall 316(ledctl) return 0 if ok and return -1 if any error*/
long ledctl(int num) {	
	return (long)syscall(__NR_LED, num);	
}




























