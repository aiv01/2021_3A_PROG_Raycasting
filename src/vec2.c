#include "vec2.h"

vec2 vec2_new(float x, float y){
    vec2 v;
    v.x = x;
    v.y = y;
    return v;
}

vec2 vec2_sum(vec2* v1, vec2* v2){
    vec2 v;
    v.x = v1->x + v2->x;
    v.y = v1->y + v2->y;
    return v;
}

vec2 vec2_sub(vec2* v1, vec2* v2){
    vec2 v;
    v.x = v1->x - v2->x;
    v.y = v1->y - v2->y;
    return v;
}

vec2 vec2_scale(vec2* v1, float n){
    vec2 v;
    v.x = v1->x * n;
    v.y = v1->y * n;
    return v;
}

vec2 vec2_rotate(vec2* v1, float angle_degrees)
{
    vec2 rotated;
    rotated.x = cosf(angle_degrees) * v1->x - sinf(angle_degrees) * v1->y;
    rotated.y = sinf(angle_degrees) * v1->x + cosf(angle_degrees) * v1->y;
    return rotated;
}