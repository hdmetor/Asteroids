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
#include "init.h"
#include "objects.h"
#include "config.h"
#include "engine.h"

using namespace std;

enum MYKEYS {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ESCAPE, KEY_SPACE
};
enum NAMES {
    OBJECT, SPACESHIP, ASTEROID, SHOOT
};


int main(int argc, char **argv)
{

    srand(static_cast<unsigned int>(time(0)));
    
    bool key[] = { false, false, false, false, false, false };
    bool redraw = true;
    bool doexit = false;
    if (init()) {
        return -1;
    }
    float spaceshipStartSpeed  = 0;
    Spaceship* spaceship = new Spaceship(300,300, spaceshipStartSpeed);
    vector<Spaceship*> spaceships;
    
    spaceships.push_back(spaceship);
    vector<Asteroid*> asteroids;
    vector<Shoot*> shoots;
    CreateAsteoroids(asteroids);

    al_flip_display();
    al_start_timer(timer);

    while(!doexit) {

        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
 

        if(event.type == ALLEGRO_EVENT_TIMER) {
            //Updating everything
            // if (asteroids.size() == 0) {
            //     al_clear_to_color(al_map_rgb(0, 0, 0 ));
            //     al_draw_line(600,500, 700,500,al_map_rgb(120,120,120),5.0f);
            //     al_draw_text(font, al_map_rgb(120,120,120), 300, 400, ALLEGRO_ALIGN_RIGHT, "You is the winner!\n");
 
            //     al_flip_display();
 
            //     al_rest(2.0);
 
            //     doexit = true;
            // }

            spaceship->Update();
            for (int i = 0; i < asteroids.size(); i++) {
                Asteroid* asteroid = asteroids[i];
                asteroid->Update();

                if (debug) {
                
                    asteroid->DebugPrint();
                }
                // asteroid on vertical borders 
                if (asteroid->x - 20 <= 0 ||
                     asteroid->x + 20 >= SCREEN_W) {
                    asteroids[i]->direction = -asteroids[i]->direction; 

                }
                else if (asteroid->y - 20 <= 0 ||
                     asteroid->y + 20 >= SCREEN_H
                     ) {

                    asteroids[i]->direction = +M_PI - asteroids[i]->direction;
                    //cout << "---BOUNCE---" <<endl;
                    //asteroids[i]->direction += M_PI/2;
                    //cout << "---AfterBOunce---" <<endl;
                  //asteroid->DebugPrint();
                    /*asteroids[i]->Update();
                    asteroids[i]->Update();
                    asteroids[i]->Update();
                    asteroids[i]->Update();*/
                }




            }
            for (int i = 0; i < shoots.size(); i++) {
                shoots[i]->Update();
            }
            // TODO: make this an iterator
            /*

            for (int i = 0; i < objects.size(); i++) {
                Object* currentObject = objects[i];
                currentObject->Update();
                
            };
            */

            for (int i = 0; i < shoots.size(); i++){
                Shoot* shoot = shoots[i];
                    // if the shoot it ouside the boundaries
                    if(
                        (shoot->x * shoot->y) < 0 ||
                        shoot->x > SCREEN_W || 
                        shoot->y > SCREEN_H ) {
                        
                        delete shoot;
                        shoots.erase(shoots.begin() + i);

                    } else {

                    for (int j = 0; j < asteroids.size(); j++) {
                        Asteroid * asteroid = asteroids[j];
                        if (
                             shoot->x > asteroid->x - 25 &&
                             shoot->x < asteroid->x + 20 &&
                             shoot->y > asteroid->y - 20 &&
                             shoot->y < asteroid->y + 20
                             ) {
                            delete shoot;
                            delete asteroids[j];
                            shoots.erase(shoots.begin() + i);
                            asteroids.erase(asteroids.begin() + j);
                        }
                    }
                }
                    // if it shoot an asteroid:
                    
                }


                // check if the object has to be destroied
            
            redraw = true;

            if(key[KEY_UP]) {
                //cout <<"UPPING" << endl;
                spaceship->accelerate();
            }
 
            if(key[KEY_DOWN]) {
                //cout <<"DOWNING" << endl;
                spaceship->decelerate();
            }
 
            if(key[KEY_LEFT]) {
                //cout <<"LEFTING" << endl;
                spaceship->moveLeft();
            }
 
            if(key[KEY_RIGHT]) {
                //cout <<"RIGHTING" << endl;
                spaceship->moveRight();
            }
 
            if(key[KEY_SPACE]) {
                //cout <<"FIRING" << endl;
                Shoot* blast = spaceship->Fire();
                //objects.push_back(blast);
                shoots.push_back(blast);
            }

        }
        // press a key
        else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    key[KEY_UP] = true;
                    break;
 
                case ALLEGRO_KEY_DOWN:
                    key[KEY_DOWN] = true;
                    break;
 
                case ALLEGRO_KEY_LEFT: 
                    key[KEY_LEFT] = true;
                    break;
 
                case ALLEGRO_KEY_RIGHT:
                    key[KEY_RIGHT] = true;
                    break;

                case ALLEGRO_KEY_SPACE:
                    key[KEY_SPACE] = true;
                    break;
            }
        }
        // release a key
        else if(event.type == ALLEGRO_EVENT_KEY_UP) {
            switch(event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    key[KEY_UP] = false;
                    break;
 
                case ALLEGRO_KEY_DOWN:
                    key[KEY_DOWN] = false;
                    break;
 
                case ALLEGRO_KEY_LEFT: 
                    key[KEY_LEFT] = false;
                    break;
 
                case ALLEGRO_KEY_RIGHT:
                    key[KEY_RIGHT] = false;
                    break;

                case ALLEGRO_KEY_SPACE:
                    key[KEY_SPACE] = false;
                    break;
 
                case ALLEGRO_KEY_ESCAPE:
                    doexit = true;
                    break;
            }
        }
        // close display
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        // redraw
        if(redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(0,0,0));

            spaceship->Draw();
            for (int i = 0; i < asteroids.size(); i++) {
                asteroids[i]->Draw();
            }

            for (int i = 0; i < shoots.size(); i++) {
                shoots[i]->Draw();
            }
            al_flip_display();
        }
    } 

    // game was quitted
    al_destroy_display(display);
 /*
    for (int i = 0; i < objects.size(); i++) {
        delete objects[i];
    }
*/
    
    return 0;
}