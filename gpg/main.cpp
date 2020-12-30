#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>
#include <random>
#include <omp.h>

#include "geom/ray.h"
#include "geom/objloader.h"

#include "utils/utils.h"

#include "tracing/camera.h"

#include "embree/errorchecks.h"
#include "embree/structs.h"
#include "tracing/cubemap.h"
#include "shaders/shader.h"
#include "shaders/shadernormal.h"
#include "tracing/renderer.h"
#include "shaders/shaderlambert.h"
#include "shaders/shaderphong.h"
#include "shaders/shaderpathtracer.h"
#include "geom/omnilight.h"

#define RENDER_MODEL_SPHERE 0
#define RENDER_MODEL_ALIEN 1
#define RENDERER_ANALYTIC true

int main(int argc, char *argv[]) {
    int renderModel = RENDER_MODEL_SPHERE;

    // Flush to Zero, Denormals are Zero mode of the MXCSR
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

    // Musíme vytvořit alespoň jedno Embree zařízení
    RTCDevice device = rtcNewDevice(NULL);
    check_rtc_or_die(device); // ověření úspěšného vytvoření Embree zařízení
    rtcDeviceSetErrorFunction(device, rtc_error_function); // registrace call-back funkce pro zachytávání chyb v Embree

    std::vector<Surface *> surfaces;
    std::vector<Material *> materials;

    // Načtení geometrie
    Vector3 defaultColor = Vector3(0.5f, 0.5f, 0.5f);
    if (renderModel == RENDER_MODEL_SPHERE) {
        if (LoadOBJ("data/ball_80x80.obj", defaultColor, surfaces, materials) < 0) {
            return -1;
        }
    } else {
        if (LoadOBJ("data/6887_allied_avenger.obj", defaultColor, surfaces, materials) < 0) {
            return -1;
        }
    }

    // Vytvoření scény v rámci Embree
    RTCScene scene = rtcDeviceNewScene(
            device,
            RTC_SCENE_STATIC | RTC_SCENE_HIGH_QUALITY,
           RTC_INTERSECT1/* | RTC_INTERPOLATE*/ // RTC_INTERSECT1 = enables the rtcIntersect and rtcOccluded functions
    );

    // Nakopírování všech modelů do bufferů Embree
    for (std::vector<Surface *>::const_iterator iter = surfaces.begin(); iter != surfaces.end(); ++iter) {
        Surface *surface = *iter;
        unsigned geom_id = rtcNewTriangleMesh(scene, RTC_GEOMETRY_STATIC, surface->no_triangles(), surface->no_vertices());

        //rtcSetUserData, rtcSetBoundsFunction, rtcSetIntersectFunction, rtcSetOccludedFunction,
        rtcSetUserData(scene, geom_id, surface);
        //rtcSetOcclusionFilterFunction, rtcSetIntersectionFilterFunction
        //rtcSetOcclusionFilterFunction( scene, geom_id, reinterpret_cast< RTCFilterFunc >( &filter_occlusion ) );
        //rtcSetIntersectionFilterFunction( scene, geom_id, reinterpret_cast< RTCFilterFunc >( &filter_intersection ) );

        // Kopírování samotných vertexů trojúhelníků
        embree_structs::Vertex *vertices = static_cast< embree_structs::Vertex * >(
                rtcMapBuffer(scene, geom_id, RTC_VERTEX_BUFFER));

        for (int t = 0; t < surface->no_triangles(); ++t) {
            for (int v = 0; v < 3; ++v) {
                embree_structs::Vertex &vertex = vertices[t * 3 + v];

                vertex.x = surface->get_triangles()[t].vertex(v).position.x;
                vertex.y = surface->get_triangles()[t].vertex(v).position.y;
                vertex.z = surface->get_triangles()[t].vertex(v).position.z;
            }
        }

        rtcUnmapBuffer(scene, geom_id, RTC_VERTEX_BUFFER);

        // Vytváření indexů vrcholů pro jednotlivé trojúhelníky
        embree_structs::Triangle *triangles = static_cast< embree_structs::Triangle * >(
                rtcMapBuffer(scene, geom_id, RTC_INDEX_BUFFER));

        for (int t = 0, v = 0; t < surface->no_triangles(); ++t) {
            embree_structs::Triangle &triangle = triangles[t];

            triangle.v0 = v++;
            triangle.v1 = v++;
            triangle.v2 = v++;
        }

        rtcUnmapBuffer(scene, geom_id, RTC_INDEX_BUFFER);

        /*embree_structs::Normal * normals = static_cast< embree_structs::Normal * >(
            rtcMapBuffer( scene, geom_id, RTC_USER_VERTEX_BUFFER0 ) );
            rtcUnmapBuffer( scene, geom_id, RTC_USER_VERTEX_BUFFER0 );*/
    }

    rtcCommit(scene);


    // Scene render
    Camera camera;
    if (renderModel == RENDER_MODEL_SPHERE) {
        camera = Camera(1000, 1000, Vector3(6.0f, 0.0f, 3.0f), Vector3(0.0f, 0.0f, 0.0f), DEG2RAD(42.185f));
    }

    Shader *shader;
    CubeMap *cubeMap = new CubeMap("data/cubemap");
    OmniLight light(camera.view_from(), 0.07f, 1, 1);

//    shader = new ShaderNormal(camera, scene, surfaces, materials, RENDERER_ANALYTIC, nullptr);
//    shader = new ShaderLambert(camera, scene, surfaces, materials, RENDERER_ANALYTIC, nullptr);
    shader = new ShaderPhong(camera, scene, surfaces, materials, light, RENDERER_ANALYTIC, cubeMap);
//    shader = new ShaderPathTracer(camera, scene, surfaces, materials, light, RENDERER_ANALYTIC, nullptr);

    // Render
    Renderer renderer;
    renderer.renderInteractive("Result", shader);

    SAFE_DELETE(shader);
    SAFE_DELETE(cubeMap);

    // Delete embree scene
    rtcDeleteScene(scene);

    // Delete materials
    SafeDeleteVectorItems<Material *>(materials);
    SafeDeleteVectorItems<Surface *>(surfaces);

    // Embree zařízení musíme také uvolnit před ukončením aplikace
    rtcDeleteDevice(device);

    return 0;
}
