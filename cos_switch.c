#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()

{

    execlp("sudo", "pqos",  "pqos", "-I","-a","pid:1=32492;", (char *)0);
	
return 0;
}
