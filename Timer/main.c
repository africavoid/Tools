/* ncurses timer */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>

#ifdef __OpenBSD__
#include <signal.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <dev/wscons/wsconsio.h>
#endif

/* todo */
// add support for -m 03 -s 3
int timer_time;

static int
help(int error)
{
	switch(error)
	{
		case 0:
		printf("USAGE: -s [secs] -m [mins]\n");
		break;
		case 1:
		printf("Could not start curses\n");
		break;
	}
	return error;
}

#ifdef __OpenBSD__
static void
bsd_beep(long duration_ms, int freq_hz, int consolefd)
{
	/* Thank you to https://github.com/jnlon/beep 
	* for this implementation */
	struct wskbd_bell_data wsb;
	
	wsb.which = WSKBD_BELL_DOALL;
	wsb.pitch = freq_hz;
	wsb.period = duration_ms;
	wsb.volume = 100;
	
	ioctl(consolefd, WSKBDIO_COMPLEXBELL, &wsb);
	
	usleep(duration_ms*1000);
}
#endif

static void
win_timer(int mode)
{	
	/* prints time in seconds */
	int y, x, maxy, maxx, ch;

	getmaxyx(stdscr, maxy, maxx);
	x = maxx;
	y = maxy;
	
	while(timer_time != 0)
	{
		curs_set(0);
		if(x != maxx || y != maxx)
		{
			clear();
			refresh();
		}

		getmaxyx(stdscr, maxy, maxx);
		mvprintw(maxy/2, maxx/2,"%d", timer_time);
		refresh();
		sleep(1);
		timer_time--;
	}
	
	clear();
	mvprintw(maxy/2, maxx/2 - 10 , "DONE");
	refresh();
	
	#ifdef __OpenBSD__
	for(int i = 0; i < 10; i++)
		bsd_beep(200, 400, open("/dev/console", O_WRONLY));
	#endif
}

static int
flag_parser(char *argv[])
{	
	/* mode 1 is sec 2 min 3 hour */
	int mode = 0;
	char *f = argv[1];

	if(!strcmp(f, "-s"))
	{
		mode = 1;
		timer_time = atoi(argv[2]);
	}

	else if(!strcmp(f, "-m"))
	{
		mode = 2;
		timer_time = atoi(argv[2])*60;
	}

	else if(!strcmp(f, "-h"))
		return help(0);
	else
		return help(0);

	return mode;
}

int
main(int argc, char *argv[])
{
	/* see flag_parser */
	int mode = 0;

	if(argc < 2)
		return help(0);
	
	mode = flag_parser(argv);

	if((initscr()) == NULL)
		return help(1);

	win_timer(timer_time);
	
	endwin();
	return 0;
}
