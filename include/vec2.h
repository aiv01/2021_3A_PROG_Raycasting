#ifndef VEC2_H
#define VEC2_H

struct vec2{
    float x;
    float y;
}; 
typedef struct vec2 vec2;

vec2 vec2_new(float x, float y);

vec2 vec2_sum(vec2* v1, vec2* v2);
vec2 vec2_sub(vec2* v1, vec2* v2);
vec2 vec2_scale(vec2* v1, float n);
vec2 vec2_rotate(vec2* v1, float angle_degrees);

#endif
