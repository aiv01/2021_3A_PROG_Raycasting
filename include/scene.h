#ifndef SCENE_H
#define SCENE_H
#include <SDL.h>
#include "vec2.h"

typedef struct{
    vec2 pos;
    vec2 dir;
} camera;

typedef struct {
    camera* camera;
} scene;


scene* scene_create(int w, int h);
void scene_update(scene*, SDL_Renderer* renderer, float delta_time);
void scene_destroy(scene*);

#endif //SCENE_H