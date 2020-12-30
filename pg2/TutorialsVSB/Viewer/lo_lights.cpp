#include "lo_lights.h"

#include "vao_SceneOrigin.h"
#include "vao_GridXY.h"
#include "vao_CubeV3C4N3T2.h"

#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_CubeV3C4N3.h"

void LO_Lights::initShaders()
{
	addResPath("shaders/");
	initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag", 0);
	initShaderProgram("lo/lightV3C4N3.vert", "lo/lightV3C4N3.frag", 0);
	initShaderProgram("lo/simpleV3C4N3.vert", "lo/simpleV3C4N3.frag", 0);
	resetResPath();
}

void LO_Lights::initVAOs()
{
	VAO_SceneOrigin* vao0 = new VAO_SceneOrigin();
	vao0->init();
	m_sceneData->vaos.push_back(vao0);

	VAO_GridXY* vao1 = new VAO_GridXY();
	vao1->init();
	m_sceneData->vaos.push_back(vao1);

	VAO_CubeV3C4N3T2* vao2 = new VAO_CubeV3C4N3T2();
	vao2->init();
	m_sceneData->vaos.push_back(vao2);
}


void LO_Lights::initInfoEntities()
{
	Entity_SceneOrigin *e0 = new Entity_SceneOrigin(m_sceneData->vaos[0]);
	e0->init();
	m_sceneData->infoEntities.push_back(e0);

	Entity_GridXY *e1 = new Entity_GridXY(m_sceneData->vaos[1]);
	e1->init();
	m_sceneData->infoEntities.push_back(e1);
}

void LO_Lights::initSceneEntities()
{
	Light *l = m_sceneData->lights[0];
	Entity_CubeV3C4N3 *e = new Entity_CubeV3C4N3(m_sceneData->vaos[2]);
	e->init();
	e->setPosition(l->m_position[0], l->m_position[1], l->m_position[2]);
	e->setScale(0.2f, 0.2f, 0.2f);
	m_sceneData->sceneEntities.push_back(e);

	e = new Entity_CubeV3C4N3(m_sceneData->vaos[2]);
	e->init();
	e->setPosition(0, 0, 0);
	m_sceneData->sceneEntities.push_back(e);
}

void LO_Lights::initTextures()
{
	addResPath("textures/");
	const int count = 3;

	// Load sprite textures
	FIBITMAP *image[count];
	image[0] = ImageManager::GenericLoader(getResFile("container2.png"), 0);
	image[1] = ImageManager::GenericLoader(getResFile("container2_specular.png"), 0);
	image[2] = ImageManager::GenericLoader(getResFile("matrix.jpg"), 0);

	// Generate textures
	for (int i = 0; i < count; i++) {
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Load tex image based on depth
		if (FreeImage_GetBPP(image[i]) == 8) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(image[i]), FreeImage_GetHeight(image[i]), 0, GL_RED, GL_UNSIGNED_BYTE, FreeImage_GetBits(image[i]));
		}
		else if (FreeImage_GetBPP(image[i]) == 24) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(image[i]), FreeImage_GetHeight(image[i]), 0, GL_BGR, GL_UNSIGNED_BYTE, FreeImage_GetBits(image[i]));
		}
		else if (FreeImage_GetBPP(image[i]) == 32) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(image[i]), FreeImage_GetHeight(image[i]), 0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits(image[i]));
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
		glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, 1.5f);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind

		FreeImage_Unload(image[i]);
		m_sceneData->textures.push_back(texture);
	}

	resetResPath();
}

void LO_Lights::initLights()
{
	Light *l = new Light();

	l->m_ambient[0] = 0.2f;
	l->m_ambient[1] = 0.2f;
	l->m_ambient[2] = 0.2f;
	l->m_ambient[3] = 1.0f;

	l->m_diffuse[0] = 1.0f;
	l->m_diffuse[1] = 1.0f;
	l->m_diffuse[2] = 1.0f;
	l->m_diffuse[3] = 1.0f;

	l->m_specular[0] = 1.0f;
	l->m_specular[1] = 0.8f;
	l->m_specular[2] = 0.8f;
	l->m_specular[3] = 1.0f;

	m_sceneData->lights.push_back(l);
}

void LO_Lights::initMaterials()
{
	Material *m = new Material();

	m->m_ambient[0] = 0.2f;
	m->m_ambient[1] = 0.2f;
	m->m_ambient[2] = 0.2f;
	m->m_ambient[3] = 1.0f;

	m->m_diffuse[0] = 0.07568f;
	m->m_diffuse[1] = 0.61424f;
	m->m_diffuse[2] = 0.07568f;
	m->m_diffuse[3] = 1.0f;

	m->m_specular[0] = 0.633f;
	m->m_specular[1] = 0.727811f;
	m->m_specular[2] = 0.633f;
	m->m_specular[3] = 1.0f;

	m->m_shininess = 128 * 0.6;
	m->m_transparency = 0.0f;
	m->m_illumination = 0;
	m->m_diffuseTextureGL = 0;

	m_sceneData->materials.push_back(m);
}

void LO_Lights::render()
{
	SceneSetting *ss = SceneSetting::GetInstance();

	// Clear buffer
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw info entities
	ss->m_activeShader = m_sceneData->shaderPrograms[0];
	ss->m_activeShader->enable();

	int uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "MVPMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewProjectionMatrix());

	for (unsigned int i = 0; i < m_sceneData->infoEntities.size(); i++)
		m_sceneData->infoEntities[i]->draw();

	
	// Draw lights
	ss->m_activeShader = m_sceneData->shaderPrograms[1];
	ss->m_activeShader->enable();

	Entity *e = m_sceneData->sceneEntities[0];
	e->draw();

	Light::setShaderUniform(m_sceneData->lights[0], ss->m_activeShader, "light");
	glm::mat4 entityMVPMatrix = ss->m_activeCamera->getVPM() * (e)->m_modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "MVPatrix"), 1, GL_FALSE, (float *)&entityMVPMatrix[0]);


	// Draw Cube
	ss->m_activeShader = m_sceneData->shaderPrograms[2];
	ss->m_activeShader->enable();

	e = m_sceneData->sceneEntities[1];
	e->draw();

	Light::setShaderUniform(m_sceneData->lights[0], ss->m_activeShader, "light");
	Material::setShaderUniform(m_sceneData->materials[0], ss->m_activeShader, "material");

	glm::mat4 entityMMatrix = (e)->m_modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix"), 1, GL_FALSE, (float *)&ss->m_activeCamera->getVM()[0]);
	glUniformMatrix4fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "MMatrix"), 1, GL_FALSE, (float *)&entityMMatrix[0]);
	glUniformMatrix3fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "NormalMatrix"), 1, GL_FALSE, (float *)&(glm::inverseTranspose(glm::mat3(ss->m_activeCamera->getVM() * entityMMatrix)))[0]);
	glUniformMatrix4fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix"), 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
	glUniformMatrix4fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix"), 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());
	glUniform3f(glGetUniformLocation(ss->m_activeShader->m_programObject, "objectColor"), 1.0f, 0.5f, 0.31f);

	// Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[0]);
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "diffuseTex");
	glUniform1i(uniform, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[1]);
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "specularTex");
	glUniform1i(uniform, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[2]);
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "emissionTex");
	glUniform1i(uniform, 2);
}
