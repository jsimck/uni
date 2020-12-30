#ifndef FILTERS_H_
#define FILTERS_H_

#include "../geom/ray.h"

void filter_intersection(void *user_ptr, Ray &ray);
void filter_occlusion(void *user_ptr, Ray &ray);

#endif