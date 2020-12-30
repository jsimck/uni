#include <omp.h>
#include "renderer.h"
#include "../utils/timer.h"

cv::Mat Renderer::render(Shader *shader) {
    Camera *camera = shader->getCamera();
    cv::Mat result = cv::Mat::zeros(camera->height(), camera->width(), CV_32FC3);

    // Progress tracker
    int progTotal = result.rows;
    int stepsCompleted = 0;

    // Stop render time
    Timer timer;

    #pragma omp parallel for schedule(dynamic, 5) shared(result, camera, shader, stepsCompleted, progTotal)
    for (int y = 0; y < result.rows; y++) {
        for (int x = 0; x < result.cols; x++) {
            Color4 color = shader->getPixel(x, y);
            result.at<cv::Vec3f>(y, x) = cv::Vec3f(color.b, color.g, color.r);
        }

        #pragma omp atomic
        ++stepsCompleted;

        if (stepsCompleted % 10 == 1) {
            #pragma omp critical
            std::cout << "Progress: " << stepsCompleted << " of " << progTotal << " (" << std::fixed << std::setprecision(1) << (100.0 * stepsCompleted / progTotal) << "%)\n";
        }
    }

    // Print render time
    Timer::printElapsed(timer.elapsed());

    return result;
}

cv::Mat Renderer::renderOut(Shader *shader) {
    return render(shader);
}

void Renderer::renderInteractive(const char *windowName, Shader *shader, float multiplier) {
    // Get camera from shader
    Camera *camera = shader->getCamera();

    // Vytiskneme info o kameře
    camera->Print();

    // Helper variables
    int keyPressed = 0;
    bool loop = true;

    while (loop) {
        // Render & get pressed key
        cv::imshow(windowName, render(shader));
        keyPressed = cv::waitKey(0);

        switch (keyPressed) {
            case LEFT_ARROW:
                camera->Rotate(-15 * multiplier);
                break;

            case RIGHT_ARROW:
                camera->Rotate(15 * multiplier);
                break;

            case UP_ARROW:
                camera->MoveUp(50 * multiplier);
                break;

            case DOWN_ARROW:
                camera->MoveDown(50 * multiplier);
                break;

            case ENTER:
                camera->ZoomIn();
                break;

            case SPACE:
                camera->ZoomOut();
                break;

            case NUM_1:
                camera->ResetPosition();
                break;

            case ESC:
                loop = false;
                break;

            default:
                break;
        }
    }
}