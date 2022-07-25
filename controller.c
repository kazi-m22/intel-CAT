#define _GNU_SOURCE
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define CTR_ID "/SHM_CTR"
#define STORAGE_ID "/SHM_TEST"
#define CTR_SIZE 2
#define STORAGE_SIZE 32


char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


int main(int argc, char *argv[])
{
        int res, res2;
        int fd, fd2;
        char data[STORAGE_SIZE];
        char ctr[CTR_SIZE];
        pid_t pid;
        void *addr, *addr2;

        pid = getpid();

        fd = shm_open(STORAGE_ID, O_RDONLY, S_IRUSR | S_IWUSR);
        if (fd == -1)
        {
                perror("open");
                return 10;
        }

        fd2 = shm_open(CTR_ID, O_RDONLY, S_IRUSR | S_IWUSR);
        if (fd2 == -1)
        {
               perror("open");
               return 10;
        }

        addr = mmap(NULL, STORAGE_SIZE, PROT_READ, MAP_SHARED, fd, 0);
        if (addr == MAP_FAILED)
        {
                perror("mmap");
                return 30;
        }

        addr2 = mmap(NULL, CTR_SIZE, PROT_READ, MAP_SHARED, fd2, 0);
        if (addr2 == MAP_FAILED)
        {
               perror("mmap");
               return 30;
        }

        memcpy(data, addr, STORAGE_SIZE);

        printf("PID %d: Read from shared memory: \"%s\"\n", pid, data);



        while (1){
                memcpy(ctr, addr2, 2);
                printf("\nfound from address 2: %c\n", ctr[0]);
                
                if (ctr[0] == '1'){
                        char* cmd = concat("pid:1=", data);
                        int id = fork();
                        if (id == 0)
                        {
                                execlp("sudo", "pqos",  "pqos", "-I","-a",cmd, (char *)0);
                        }
                        else{
                                wait(NULL);
                                kill(id, SIGKILL);
                                printf("\nkilled child\n");
                        }
                }

                else if (ctr[0] == '0'){
                        char* cmd = concat("pid:0=", data);
                        int id = fork();
                        if (id == 0)
                        {
                                execlp("sudo", "pqos",  "pqos", "-I","-a",cmd, (char *)0);
                        }
                        else{
                                wait(NULL);
                                kill(id, SIGKILL);
                                printf("\nkilled child for 0\n");
                        }
                }


                else
                        printf("not matched, found: \n", ctr[0]);


        }
        return 0;

}


