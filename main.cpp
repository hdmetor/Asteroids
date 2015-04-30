// Core
#include <iostream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
//Allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
// Self
#include "src/init.h"
#include "src/objects.h"
#include "src/config.h"
#include "src/engine.h"

using namespace std;

int main(int argc, char **argv)
{

    srand(static_cast<unsigned int>(time(0)));
    
    bool redraw = true;
    bool doexit = false;
    if (!InitAllegro()) {
        return -1;
    }

    InitGame();

    al_flip_display();
    al_start_timer(timer);

    while(!doexit) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
 
        if(event.type == ALLEGRO_EVENT_TIMER) {
            if ((doexit = !GameContinues())) {
                break;
            } else {
            DispatchKeys();
            UpdateObjects();
            redraw = true;
            }
        }

        // press a key
        else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
            KeyDown(event);
        }
        // release a key
        else if(event.type == ALLEGRO_EVENT_KEY_UP) {
            doexit = KeyUp(event);            
        }
        // close display
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
        // redraw
        if(redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            Redraw();
        }
    } 

    al_destroy_display(display);
    Cleanup();    
    return 0;
}