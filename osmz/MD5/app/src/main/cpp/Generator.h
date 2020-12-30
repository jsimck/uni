#ifndef md5_GENERATOR_H
#define md5_GENERATOR_H

#include <stdint.h>

class Generator {
private:
    const char start = 65;
    const char end = 90;
    int size;
    int len;
    char *i;
    char *buffer;

    void resize();
public:
    Generator(const int size);
    ~Generator();
    char *generate();
};

#endif //md5_GENERATOR_H
