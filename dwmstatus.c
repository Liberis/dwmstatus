#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <alsa/asoundlib.h>
#include <alsa/control.h>

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

char* getVol(void){

    	int val;
    	static char vol[1024];
    	snd_hctl_t *hctl;
    	snd_ctl_elem_id_t *id;
    	snd_ctl_elem_value_t *control;

// To find card and subdevice: /proc/asound/, aplay -L, amixer controls
    	snd_hctl_open(&hctl, "hw:0", 0);
    	snd_hctl_load(hctl);

    	snd_ctl_elem_id_alloca(&id);
    	snd_ctl_elem_id_set_interface(id, SND_CTL_ELEM_IFACE_MIXER);

// amixer controls
    	snd_ctl_elem_id_set_name(id, "Master Playback Volume");

    	snd_hctl_elem_t *elem = snd_hctl_find_elem(hctl, id);

    	snd_ctl_elem_value_alloca(&control);
    	snd_ctl_elem_value_set_id(control, id);

    	snd_hctl_elem_read(elem, control);
    	val = (int)snd_ctl_elem_value_get_integer(control,0);
    	snd_hctl_close(hctl);
	val = val - 74;
	snprintf(vol,sizeof(vol), "| vol %d db ", val);
    	return vol;
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
	while(1)
	{
	char* vol = getVol();
	char* temp = getTemp();
	char* bat = getBat();
	char* date = getDate();
	char* status;
	status = malloc(strlen(vol) + strlen(temp) + strlen(date) + strlen(bat) + 4);
	strcpy(status, vol);
	strcat(status, temp);
	strcat(status, bat);
	strcat(status, date);
	setstatus(status);
	free(status);
	sleep(1);
	}
	return 0;
}

