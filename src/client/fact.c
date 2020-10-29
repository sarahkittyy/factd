#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
	size_t pagesize = getpagesize();

	int shm = shm_open("/memzone1", O_RDWR, S_IRWXU);
	ftruncate(shm, pagesize);

	char* buf = mmap(NULL, pagesize, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);

	sem_t* in  = sem_open("/ffactd-sem", 0);
	sem_t* out = sem_open("/tfactd-sem", 0);
	while (1) {
		int x;
		printf("Enter an integer: ");
		scanf("%d", &x);
		sprintf(buf, "%d", x);
		sem_post(out);
		// wait for the semaphore
		sem_wait(in);
		int rec;
		sscanf(buf, "%d", &rec);
		printf("Got back %d\n", rec);
	}
	munmap(buf, sizeof(int));
	return 0;
}
