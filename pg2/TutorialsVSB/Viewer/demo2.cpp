#include "demo2.h"

#include "vao_SceneOrigin.h"
#include "vao_GridXY.h"
#include "vao_CubeV3C4.h"
#include "vao_TetraV3C4.h"

#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_Cube_Simple.h"
#include "entity_Tetra_Simple.h"

void Demo2::initShaders()
{
	addResPath("shaders/");
	initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag", 0);
	resetResPath();
}

void Demo2::initVAOs()
{
	VAO_SceneOrigin* vao0 = new VAO_SceneOrigin();
	vao0->init();
	m_sceneData->vaos.push_back(vao0);

	VAO_GridXY* vao1 = new VAO_GridXY();
	vao1->init();
	m_sceneData->vaos.push_back(vao1);

	//VAO_CubeV3C4* vao2 = new VAO_CubeV3C4();
	//vao2->init();
	//m_sceneData->vaos.push_back(vao2);

	VAO_TetraV3C4* vao2 = new VAO_TetraV3C4();
	vao2->init();
	m_sceneData->vaos.push_back(vao2);


}


void Demo2::initInfoEntities()
{
	Entity_SceneOrigin *e0 = new Entity_SceneOrigin(m_sceneData->vaos[0]);
	e0->init();
	m_sceneData->infoEntities.push_back(e0);

	Entity_GridXY *e1 = new Entity_GridXY(m_sceneData->vaos[1]);
	e1->init();
	m_sceneData->infoEntities.push_back(e1);
}

void Demo2::initSceneEntities()
{
	/*Entity_Cube_Simple *e = new Entity_Cube_Simple(m_sceneData->vaos[2]);
	e->init();
	m_sceneData->sceneEntities.push_back(e);*/

	Entity_Tetra_Simple *e = new Entity_Tetra_Simple(m_sceneData->vaos[2]);
	e->init();
	m_sceneData->sceneEntities.push_back(e);
}

void Demo2::render()
{
	SceneSetting *ss = SceneSetting::GetInstance();
	ss->m_activeShader = m_sceneData->shaderPrograms[0];

	ss->m_activeShader->enable();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	int uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "MVPMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewProjectionMatrix());

	for (unsigned int i = 0; i < m_sceneData->infoEntities.size(); i++)
		m_sceneData->infoEntities[i]->draw();

	for (unsigned int i = 0; i < m_sceneData->sceneEntities.size(); i++)
		m_sceneData->sceneEntities[i]->draw(i);
}

