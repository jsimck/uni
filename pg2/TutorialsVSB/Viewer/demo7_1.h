#ifndef __DEMO8_H_
#define __DEMO8_H_

#include <sceneInitializer.h>

class Demo7_1 : public SceneInitializer
{
private:
	void initShaders();
	void initTextures();
	void initModels();
	void initMaterials();
	void initVAOs();
	void initInfoEntities();
	void initSceneEntities();

public:
	Demo7_1(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif