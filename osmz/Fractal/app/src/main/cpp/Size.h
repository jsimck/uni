#ifndef FRACTAL_SIZE_H
#define FRACTAL_SIZE_H

template<typename T>
struct Size {
public:
    T width;
    T height;

    Size(T width, T height) : width(width), height(height) {}

    T area() { return width * height; }
    T ratioWH() { return width / height; }
    T ratioHW() { return height / width; }
};

#endif //FRACTAL_SIZE_H
