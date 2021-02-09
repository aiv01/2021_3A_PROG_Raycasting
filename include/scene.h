#ifndef SCENE_H
#define SCENE_H
#include <SDL.h>
#include "vec2.h"

typedef struct{
    vec2 pos;
    vec2 dir;
    float fov_angle;
    vec2 plane_dir;
    float fov_tan;
} camera;

typedef struct {
    camera* camera;
    const int* map;
    int screen_width;
    int screen_height;

} scene;

typedef struct{
    float distance;
    int cell_type;
    vec2 cell;
}raycast_hit;

scene* scene_create(int w, int h);
void scene_update(scene*, SDL_Renderer* renderer, float delta_time);
void scene_destroy(scene*);

void draw_column(raycast_hit *ray, scene *s, SDL_Renderer *r, int col);
void choose_color(int type, SDL_Renderer *r);

#endif //SCENE_H