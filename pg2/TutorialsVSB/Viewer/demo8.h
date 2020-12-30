#ifndef __DEMO7_H_
#define __DEMO7_H_

#include <sceneInitializer.h>

class Demo8 : public SceneInitializer
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
	Demo8(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif