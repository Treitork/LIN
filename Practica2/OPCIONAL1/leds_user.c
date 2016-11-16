#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/errno.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#include <unistd.h>

#ifdef __i386__
#define __NR_LED 353
#else
#define __NR_LED 316
#endif

/*Marta Rodenas de Miguel & Cristian Pinto Lozano*/

long ledctl(int num);

int main(int argc, char **argv) {
	
	int i, pollingDelay = 750;
	

		for(i = 1; i <= 7; i++){
			if(ledctl(i) == -1) {
			perror("Se ha producido un error");
			return -EXIT_FAILURE;
			}
			usleep(pollingDelay*1000);
		}
		for(i = 6; i >= 0; i--){
			if(ledctl(i) == -1) {
			perror("Se ha producido un error");
			return -EXIT_FAILURE;
			}
			usleep(pollingDelay*1000);
		}
	
	return EXIT_SUCCESS;

}


/*call the syscall 316(ledctl) return 0 if ok and return -1 if any error*/
long ledctl(int num) {	
	return (long)syscall(__NR_LED, num);	
}




























