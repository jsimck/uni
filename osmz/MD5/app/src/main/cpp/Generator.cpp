#import "Generator.h"
#import <cstring>

Generator::Generator(const int size) : size(size), buffer(new char[size]), i(new char[size]) {
    for (int j = 0; j < size; j++) {
        i[j] = 0;
    }

    len = 0;
}

Generator::~Generator() {
    delete i;
    delete buffer;
}

char *Generator::generate() {
    // Increase size of buffer
    if (len >= size) {
        resize();
    }

    // Generate key
    for (int l = 0; l <= len; l++) {
        buffer[l] = start + i[l];
    }
    buffer[len + 1] = '\0';

    // Update indexes to the start
    for (int l = len; l >= 0; l--) {
        i[l]++;
        if (i[l] + start > end) {
            i[l] = 0;

            // End
            if (l == 0) {
                len++;
                break;
            }
        } else {
            break;
        }
    }

    return buffer;
}

void Generator::resize() {
    char *newI = new char[size * 2];
    std::memcpy(i, newI, sizeof(char) * size);

    // Fill the rest with zeros
    for (int j = size; j < size * 2; j++) {
        newI[j] = 0;
    }

    // Reassign new index buffer
    delete i;
    i = newI;

    // Resize buffer, can be destroyed directly
    delete buffer;
    buffer = new char[size * 2];
    size *= 2;
}
