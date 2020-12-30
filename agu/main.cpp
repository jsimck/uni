#include "excercises/e1_polygons.h"
#include "excercises/e2_plane_sweeping.h"
#include "excercises/e3_convex_hull.h"

int main(int argc, char *argv[]) {
    E1Polygons().run(argc, argv);
    E2PlaneSweeping().run(argc, argv);
    E3ConvexHull().run(argc, argv);

    return 0;
}