#ifndef PG1_ERRORCHECKS_H
#define PG1_ERRORCHECKS_H

#include <vector>

#include <embree2/rtcore_ray.h>

#include "../geom/surface.h"

void rtc_error_function(const RTCError code, const char *str);
RTCError check_rtc_or_die(RTCDevice &device);
int test(RTCScene &scene, std::vector<Surface *> &surfaces);

#endif