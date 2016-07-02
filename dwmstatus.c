#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>


static void setstatus(const char *str) {
	Display *display;
	
	if ((display = XOpenDisplay(0x0)) == NULL ) {
	perror("Cannot Open Display"); 
	exit(1);
	}
	/*The status bar text of dwm is stored in the WM_NAME X11 property of the root window,
	which is managed by dwm*/
	XStoreName(display,DefaultRootWindow(display), str);
	XSync(display, False);
	XCloseDisplay(display);
}


char* getTemp(void){
	FILE* f;
	int val;
	static char temp[128];
	f = fopen("/sys/class/thermal/thermal_zone1/temp", "r");
	fscanf(f, "%d" , &val);
	fclose(f);
	snprintf(temp, sizeof(temp), "| %d°C ", val/1000);
	return temp;
}

char* getBat(void){
	FILE* f;
	int val;
	static char bat[128];
	f = fopen("/sys/class/power_supply/BAT0/capacity", "r");
	fscanf(f, "%d", &val);
	fclose(f);
	snprintf(bat, sizeof(bat), "| bat %d%% |", val);
	return bat;
}

char* getDate(){
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	static char s[64];
	strftime(s, sizeof(s), " %c", tm);
	return s;
}

	
int main(void){
	while(1){
	char* temp = getTemp();
	char* bat = getBat();
	char* s = getDate();
	char* status;
	status = malloc(strlen(temp) + strlen(s) + strlen(bat) + 3);
	strcpy(status, temp);
	strcat(status, bat);
	strcat(status, s);
	setstatus(status);
	free(status);
	sleep(1);
	}
	return 0;
	}

