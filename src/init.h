#ifndef _INIT_FILE_
#define _INIT_FILE_
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "config.h"


extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_TIMER *timer ;
extern ALLEGRO_BITMAP *bouncer;
extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_FONT *font;

bool InitAllegro( float fps = FPS,  int width = SCREEN_W,  int height = SCREEN_H);

#endif