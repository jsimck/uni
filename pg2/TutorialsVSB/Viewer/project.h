#ifndef __DEMO4_H_
#define __DEMO4_H_

#include <sceneInitializer.h>
#include <cstdlib>

class Project : public SceneInitializer
{
private:
	const int blurPassCount = 20;
	const float dNear = 0.0009f;
	const float dFar = 1.0f;

	void initShaders();
	void initModels();
	void initMaterials();
	void initLights();
	void initTextures();

	void initVAOs();
	void initFBOs();

	void initInfoEntities();
	void initSceneEntities();

	void update();
public:
	Project(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif