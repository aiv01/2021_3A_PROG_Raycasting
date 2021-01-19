#ifndef SCENE_H
#define SCENE_H
#include <SDL.h>

typedef struct {
    int todo;
} scene;

scene* scene_create(int w, int h);
void scene_update(scene*, SDL_Renderer* renderer, float delta_time);
void scene_destroy(scene*);

#endif //SCENE_H