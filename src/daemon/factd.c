#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int fact(int x) {
	if (x <= 1) {
		return 1;
	} else {
		return x * fact(x - 1);
	}
}

int main() {
	size_t pagesize = getpagesize();

	int shm = shm_open("/memzone1", O_RDWR | O_CREAT, S_IRWXU);
	ftruncate(shm, pagesize);

	char* buf = mmap(NULL, pagesize, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);

	sem_t* in  = sem_open("/tfactd-sem", O_CREAT, S_IRWXU, 0);
	sem_t* out = sem_open("/ffactd-sem", O_CREAT, S_IRWXU, 0);
	while (1) {
		// wait for an integer
		sem_wait(in);
		// read an integer
		int x;
		sscanf(buf, "%d", &x);
		printf("Got integer %d\n", x);
		// compute the factorial
		int fx = fact(x);
		// send it back
		printf("Sending back %d\n", fx);
		sprintf(buf, "%d", fx);
		sem_post(out);
	}

	munmap(buf, sizeof(int));
	shm_unlink("/memzone1");
	return 0;
}
