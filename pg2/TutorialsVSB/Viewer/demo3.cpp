#include "demo3.h"

#include "vao_SceneOrigin.h"
#include "vao_GridXY.h"
#include "vao_CubeV3C4N3.h"

#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_CubeV3C4N3.h"

void Demo3::initShaders()
{
	addResPath("shaders/");
	initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag", 0);
	initShaderProgram("lambert_v3_c4_n3.vert", "lambert_v3_c4_n3.frag", 0);
	resetResPath();
}

void Demo3::initVAOs()
{
	VAO_SceneOrigin* vao0 = new VAO_SceneOrigin();
	vao0->init();
	m_sceneData->vaos.push_back(vao0);

	VAO_GridXY* vao1 = new VAO_GridXY();
	vao1->init();
	m_sceneData->vaos.push_back(vao1);

	VAO_CubeV3C4N3* vao2 = new VAO_CubeV3C4N3();
	vao2->init();
	m_sceneData->vaos.push_back(vao2);
}


void Demo3::initInfoEntities()
{
	Entity_SceneOrigin *e0 = new Entity_SceneOrigin(m_sceneData->vaos[0]);
	e0->init();
	m_sceneData->infoEntities.push_back(e0);

	Entity_GridXY *e1 = new Entity_GridXY(m_sceneData->vaos[1]);
	e1->init();
	m_sceneData->infoEntities.push_back(e1);
}

void Demo3::initSceneEntities()
{
	/*Entity_Cube_Simple *e = new Entity_Cube_Simple(m_sceneData->vaos[2]);
	e->init();
	m_sceneData->sceneEntities.push_back(e);*/

	Entity_CubeV3C4N3 *e = new Entity_CubeV3C4N3(m_sceneData->vaos[2]);
	e->init();
	m_sceneData->sceneEntities.push_back(e);
}

void Demo3::render()
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


	// ------------  Shader pro kostku -----------------
	ss->m_activeShader = m_sceneData->shaderPrograms[1];
	ss->m_activeShader->enable();

	Light::setShaderUniform(m_sceneData->lights.at(0), ss->m_activeShader, "light");

	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());

	for (Entity **e = m_sceneData->sceneEntities.front(); e <= m_sceneData->sceneEntities.back(); e++)
	{
		glm::mat4 tmp = ss->m_activeCamera->getVM() * (*e)->m_modelMatrix;
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "MVMatrix");
		glUniformMatrix4fv(uniform, 1, GL_FALSE, (float*)&tmp[0]);
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "NormalMatrix");
		glUniformMatrix3fv(uniform, 1, GL_FALSE, (float*)&(glm::inverseTranspose(glm::mat3(tmp)))[0]);

		Material::setShaderUniform((*e)->m_material, ss->m_activeShader, "material");
		(*e)->draw();
	}

}

