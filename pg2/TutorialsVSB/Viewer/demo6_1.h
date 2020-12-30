#ifndef __DEMO6_H_
#define __DEMO6_H_

#include <sceneInitializer.h>

class Demo6_1 : public SceneInitializer
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
	Demo6_1(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif