#ifndef __DEMO4_H_
#define __DEMO4_H_

#include <sceneInitializer.h>

class Demo4 : public SceneInitializer
{
private:
	void initShaders();
	void initModels();
	void initMaterials();
	void initVAOs();
	void initInfoEntities();
	void initSceneEntities();

public:
	Demo4(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif