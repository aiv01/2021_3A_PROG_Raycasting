#include "scene.h"
#include <math.h>

const int world_map[20][20] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

scene *scene_create(int screen_width, int screen_height)
{
    scene *s = malloc(sizeof(scene));
    s->map = (int*)world_map;

    s->camera = malloc(sizeof(camera));
    s->camera->pos = vec2_new(1, 2);
    s->camera->dir = vec2_new(1.f, 0);
    s->camera->fov_angle = 60.f;
    s->camera->plane_dir = vec2_new(0, 1.f);
    float half_fov_rads = (float)((s->camera->fov_angle * 0.5f) * M_PI) / 180.f;
    s->camera->fov_tan = tanf(half_fov_rads);

    s->screen_height = screen_height;
    s->screen_width = screen_width;

    return s;
}
void ray_cast(vec2 *origin, vec2 *ray, scene* s, raycast_hit *out_ray_hit)
{
    float pos_x = origin->x;
    float pos_y = origin->y;

    int cell_x = (int)pos_x;
    int cell_y = (int)pos_y;

    float delta_x = sqrtf(1.f + (ray->y * ray->y) / (ray->x * ray->x));
    float delta_y = sqrtf(1.f + (ray->x * ray->x) / (ray->y * ray->y));

    float ray_length_x;
    float ray_length_y;

    int cell_incr_x;
    int cell_incr_y;

    if (ray->x < 0)
    {
        cell_incr_x = -1;
        ray_length_x = (pos_x - (float)cell_x) * delta_x;
    }
    else
    {
        cell_incr_x = 1;
        ray_length_x = ( (float)cell_x + 1.f - (float)pos_x) * delta_x;
    }

    if (ray->y < 0)
    {
        cell_incr_y = -1;
        ray_length_y = (pos_y - (float)cell_y) * delta_y;
    }
    else
    {
        cell_incr_y = 1;
        ray_length_y = ((float)cell_y + 1.f - pos_y) * delta_y;
    }

    int hit_side = 0; //0 NORTH_SOUTH  1 EAST_WEST
    while(s->map[cell_y * 20 + cell_x] == 0)
    {
        if(ray_length_x < ray_length_y)
        {
            ray_length_x += delta_x;
            cell_x += cell_incr_x;
            hit_side = 0;
        }
        else
        {
            ray_length_y += delta_y;
            cell_y += cell_incr_y;
            hit_side = 1;
        }
    }

    out_ray_hit->cell_type = s->map[cell_y * 20 + cell_x];
    out_ray_hit->cell = vec2_new((float)cell_x, (float)cell_y);
    // Euclidean Distance gives fish eye effects
    //out_ray_hit->distance = sqrtf(ray_length_x * ray_length_x + ray_length_y * ray_length_y);
    
    
    float pos_to_wall_distance;
    //NORTH
    if (hit_side == 0) {
        //(1 - cell_incr_x) / 2 => [0, 1]
        pos_to_wall_distance = ((float)cell_x - pos_x + (float)(1 - cell_incr_x) / 2.f ) / ray->x;
    } else {
        pos_to_wall_distance = ((float)cell_y - pos_y + (float)(1 - cell_incr_y) / 2.f ) / ray->y;
    }
    out_ray_hit->distance = pos_to_wall_distance;
}

void scene_update(scene *s, SDL_Renderer *r, float delta_time)
{
    handle_input(s, delta_time);
    float delta = 2.f / s->screen_width;
    for (int col = 0; col < s->screen_width; col++)
    {
        float col_mapped = delta * col - 1.f;

        /* BUG: Erronamente moltiplicavamo per l'angolo del fov, invece che per la tangente.
                Quindi il raggio che castiamo aveva sicuramente direzione errata
        float ray_dir_x = s->camera->dir.x + s->camera->plane_dir.x * col_mapped * s->camera->fov_angle;
        float ray_dir_y = s->camera->dir.y + s->camera->plane_dir.y * col_mapped * s->camera->fov_angle;
         */

        float ray_dir_x = s->camera->dir.x + s->camera->plane_dir.x * col_mapped * s->camera->fov_tan;
        float ray_dir_y = s->camera->dir.y + s->camera->plane_dir.y * col_mapped * s->camera->fov_tan;

        vec2 ray = vec2_new(ray_dir_x, ray_dir_y);
        raycast_hit hit;
        ray_cast(&s->camera->pos, &ray, s, &hit);
        draw_column(&hit, s, r, col);
    }
}

void draw_column(raycast_hit *ray, scene *s, SDL_Renderer *r, int col)
{
    int col_height = (int)((float)s->screen_height / ray->distance);
    int start = s->screen_height/2 - col_height/2;
    int end = start + col_height;
    choose_color(ray->cell_type, r);
    SDL_RenderDrawLine(r, col, start, col, end);
}

void choose_color(int type, SDL_Renderer *r)
{
    if(type == 1)
    {
        SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    } else if (type == 2) {
        SDL_SetRenderDrawColor(r, 0, 0, 255, 255);
    }
}

void handle_input(scene* s, float delta_time){
    float speed = 10.f;
    float rotation_speed = 60.f;

    Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_DOWN]){
        vec2 move = vec2_scale(&s->camera->dir, speed * delta_time);
        s->camera->pos = vec2_sub(&s->camera->pos, &move);
    }
    else if(state[SDL_SCANCODE_UP]){
        vec2 move = vec2_scale(&s->camera->dir, speed * delta_time);
        s->camera->pos = vec2_sum(&s->camera->pos, &move);
    }

    if(state[SDL_SCANCODE_RIGHT]){
        s->camera->dir = vec2_rotate(&s->camera->dir, rotation_speed * delta_time);
        s->camera->plane_dir = vec2_rotate(&s->camera->plane_dir, rotation_speed * delta_time);
    }
    else if(state[SDL_SCANCODE_LEFT]){
        s->camera->dir = vec2_rotate(&s->camera->dir, -rotation_speed * delta_time);
        s->camera->plane_dir = vec2_rotate(&s->camera->plane_dir, -rotation_speed * delta_time);
    }
}

void scene_destroy(scene *s)
{

}