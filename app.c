#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define STORAGE_ID "/SHM_TEST"
#define CTR_ID "/SHM_CTR"
#define STORAGE_SIZE 32
#define CTR_SIZE 32

void *addr, *addr2;

int shm_establish()
{
	int res, res2;
	int fd, fd2;

	fd = shm_open(STORAGE_ID, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("open");
		return 10;
	}

        fd2 = shm_open(CTR_ID, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        if (fd2 == -1)
        {
                perror("open");
                return 10;
        }
	
	// extend shared memory object as by default it's initialized with size 0
	res = ftruncate(fd, STORAGE_SIZE);
	if (res == -1)
	{
		perror("ftruncate");
		return 20;
	}


        res2 = ftruncate(fd2, CTR_SIZE);
        if (res2 == -1)
        {
                perror("ftruncate");
                return 20;
        }

        addr = mmap(NULL, STORAGE_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED)
	{
		perror("mmap");
		return 30;
	}

        addr2 = mmap(NULL, CTR_SIZE, PROT_WRITE, MAP_SHARED, fd2, 0);
        if (addr2 == MAP_FAILED)
        {
                perror("mmap");
                return 30;
        }

        printf("connection establiehed");


}




int main(int argc, char *argv[])
{

	int len;
	pid_t pid;
	char * ctr = malloc(10);


	pid = getpid();

	char * data = malloc(10); 
	sprintf(data, "%d", pid);
	


	shm_establish();
	len = strlen(data) + 1;
	memcpy(addr, data, len);

	int counter = 0;

	while (1) {

		printf("\nin the while loop\n");
		sleep(3);
		printf("writing 0 to the address\n");
		memcpy(addr2, "0", 2);
		sleep(3);
		printf("writing 1 to the address\n");
		memcpy(addr2, "1", 2);
		sleep(3);


	}



	return 0;
}
