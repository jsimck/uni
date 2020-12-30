#ifndef PG1_RENDERER_H
#define PG1_RENDERER_H


#include "camera.h"
#include "../shaders/shader.h"

class Renderer {
public:
    void renderInteractive(const char *windowName, Shader *shader, float multiplier = 1.0);
    cv::Mat renderOut(Shader *shader);

    static enum {
        LEFT_ARROW = 63234,
        RIGHT_ARROW = 63235,
        UP_ARROW = 63232,
        DOWN_ARROW = 63233,
        NUM_1 = 43,
        NUM_2 = 283,
        SPACE = 32,
        ENTER = 13,
        ESC = 27
    } keyCodes;
private:
    cv::Mat render(Shader *shader);
};


#endif //PG1_RENDERER_H