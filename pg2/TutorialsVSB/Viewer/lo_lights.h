#ifndef __DEMO2_H_
#define __DEMO2_H_

#include <sceneInitializer.h>

class LO_Lights : public SceneInitializer
{
private:
	void initShaders();
	void initVAOs();
	void initInfoEntities();
	void initSceneEntities();
	void initLights();
	void initMaterials();
	void initTextures();

public:
	LO_Lights(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif