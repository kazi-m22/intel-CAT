all: app.c controller.c 
	gcc app.c -o app -lrt
	gcc controller.c -o controller -lrt

app:
	gcc app.c -o app -lrt && ./app
	ls -l /dev/shm

controller:
	gcc controller.c -o controller -lrt && ./controller

clear:
	rm *.o

switch:
	gcc cos_switch.c -o switch && ./switch


.PHONY: all app clear switch controller
