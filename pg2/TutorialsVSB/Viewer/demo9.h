#ifndef __DEMO9_H_
#define __DEMO9_H_

#include <sceneInitializer.h>

class Demo9 : public SceneInitializer
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
	Demo9(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif