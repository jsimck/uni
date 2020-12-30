#include "demo5.h"

#include "vao_SceneOrigin.h"
#include "vao_GridXY.h"
#include "vao.h"

#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_OBJ.h"

void Demo5::initShaders()
{
	addResPath("shaders/");
	initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag", 0);
	initShaderProgram("matte_v3_n3_t3.vert", "matte_v3_n3_t3.frag", 0);
	initShaderProgram("phong_t_v3_n3_t3.vert", "phong_t_v3_n3_t3.frag", 0);
	resetResPath();
}

void Demo5::initModels()
{
	ObjLoader objL;
	Model* m;

	addResPath("models/");

	m = objL.loadModel(getResFile("vase/vase.obj"));
	m_sceneData->models.push_back(m);

	resetResPath();
}

void Demo5::initVAOs()
{
	VAO_SceneOrigin* vao0 = new VAO_SceneOrigin();
	vao0->init();
	m_sceneData->vaos.push_back(vao0);

	VAO_GridXY* vao1 = new VAO_GridXY();
	vao1->init();
	m_sceneData->vaos.push_back(vao1);

	VAO* vao2 = new VAO();
	vao2->createFromModel(m_sceneData->models[0]);
	m_sceneData->vaos.push_back(vao2);
}

void Demo5::initMaterials()
{
	Material *m = new Material();
	m->setName("White_opaque");
	m->m_ambient[0] = m->m_ambient[1] = m->m_ambient[2] = 1.0f;	 m->m_ambient[3] = 0.5f;
	m->m_diffuse[0] = 0.8f; m->m_diffuse[1] = 1.0; m->m_diffuse[2] = 0.8f;	 m->m_diffuse[3] = 0.5f;
	m->m_specular[0] = m->m_specular[1] = m->m_specular[2] = 0.2f; m->m_specular[3] = 1.0f;
	m->m_transparency = 0.0f;
	m_sceneData->materials.push_back(m);
}

void Demo5::initInfoEntities()
{
	Entity_SceneOrigin *e0 = new Entity_SceneOrigin(m_sceneData->vaos[0]);
	e0->init();
	m_sceneData->infoEntities.push_back(e0);

	Entity_GridXY *e1 = new Entity_GridXY(m_sceneData->vaos[1]);
	e1->init();
	m_sceneData->infoEntities.push_back(e1);
}

void Demo5::initSceneEntities()
{
	Entity_OBJ *obj = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[2]);
	obj->setPosition(0, 0, 0);
	obj->setOrientation(0, 0, 90);
	obj->m_material = m_sceneData->materials[0];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);

	obj = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[2]);
	obj->setPosition(0, 1.5f, 0);
	obj->setOrientation(0, 0, 90);
	obj->m_material = m_sceneData->materials[0];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);

	obj = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[2]);
	obj->setPosition(0, -1.5f, 0);
	obj->setOrientation(0, 0, 90);
	obj->m_material = m_sceneData->materials[0];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);
}

void Demo5::render()
{
	Entity *e = nullptr;
	SceneSetting *ss = SceneSetting::GetInstance();

#pragma region Draw Info Entities

	ss->m_activeShader = m_sceneData->shaderPrograms[0];
	ss->m_activeShader->enable();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	int uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "MVPMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewProjectionMatrix());

	for (unsigned int i = 0; i < m_sceneData->infoEntities.size(); i++)
		m_sceneData->infoEntities[i]->draw();

#pragma endregion

#pragma region Draw Scene Entities

	// Draw matte shader
	ss->m_activeShader = m_sceneData->shaderPrograms[1];
	ss->m_activeShader->enable();

	Light::setShaderUniform(m_sceneData->lights.at(0), ss->m_activeShader, "light");

	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());

	e = m_sceneData->sceneEntities[0];
	Material::setShaderUniform(e->m_material, ss->m_activeShader, "material");
	e->draw();
	
	// Draw transparent shader
	ss->m_activeShader = m_sceneData->shaderPrograms[2];
	ss->m_activeShader->enable();

	// Disable cull face, render both sides
	glDisable(GL_CULL_FACE);

	// Enable blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_ZERO, GL_SRC_COLOR);

	// Disable depth mask
	glDepthMask(GL_FALSE);

	Light::setShaderUniform(m_sceneData->lights.at(0), ss->m_activeShader, "light");

	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());

	// Draw transparent entities
	for (Entity **e = m_sceneData->sceneEntities.front() + 1; e != m_sceneData->sceneEntities.end(); e++) {
		Material::setShaderUniform((*e)->m_material, ss->m_activeShader, "material");
		(*e)->draw();
	}

	// Disable shader
	ss->m_activeShader->disable();

	// Reset transparency specific options
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);


	// Copy images -- START
	// Shader
	int const width = 500, height = 500;
	static unsigned char buffer[3 * width * height];

	// Set from wich gpu buffer FRONT/LEFT we want to read
	glReadBuffer(GL_FRONT);

	// Read pixels to buffer
	glReadPixels(200, 200, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);

	// Set position to draw data we read
	glRasterPos3f(0, 0, 0);

	// Draw pixels
	glDrawBuffer(GL_BACK);
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);

	// Set draw buffer back to front for next cycle (state machine)
	glDrawBuffer(GL_FRONT);
	// Copy images -- END


#pragma endregion

}
