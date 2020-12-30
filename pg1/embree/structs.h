#ifndef PG1_STRUCTS_H
#define PG1_STRUCTS_H

// struktury pro ukládání dat pro Embree
namespace embree_structs {
    struct Vertex {
        float x, y, z, a;
    };
    typedef Vertex Normal;
    struct Triangle {
        int v0, v1, v2;
    };
};

#endif
