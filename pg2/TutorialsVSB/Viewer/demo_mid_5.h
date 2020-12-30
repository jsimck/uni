#ifndef __DEMO4_H_
#define __DEMO4_H_

#include <sceneInitializer.h>

class Demo_mid_5 : public SceneInitializer
{
private:
	void initShaders();
	void initModels();
	void initMaterials();
	void initVAOs();
	void initInfoEntities();
	void initSceneEntities();

public:
	Demo_mid_5(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif