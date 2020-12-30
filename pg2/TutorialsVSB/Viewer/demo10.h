#ifndef __DEMO10_H_
#define __DEMO10_H_

#include <sceneInitializer.h>

class Demo10 : public SceneInitializer
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
	Demo10(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif