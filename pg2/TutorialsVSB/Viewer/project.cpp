#include "project.h"
#include <iostream>
#include <vector>
#include <ctime>

#include "vao_SceneOrigin.h"
#include "vao_QuadV3C4N3T2.h"
#include "vao_GridXY.h"
#include "vao_Vertex.h"
#include "vao_Skybox.h"
#include "vao.h"

#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_OBJ.h"
#include "entity_QuadV3C4N3T2.h"
#include "entity_Skybox.h"
#include "entity_Electron.h"
#include "entity_Particle.h"
#include "entity_Atom.h"

void Project::initShaders()
{
	addResPath("shaders/");
	initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag", 0); // 0
	initShaderProgram("pp/atom.vert", "pp/atom.frag", 0); // 1
	initShaderProgram("pp/electron.vert", "pp/electron.frag", 0); // 2
	initShaderProgram("pp/bloom.vert", "pp/bloom.frag", 0); // 3
	initShaderProgram("pp/bloom_gaussian.vert", "pp/bloom_gaussian.frag", 0); // 4
	initShaderProgram("pp/plane.vert", "pp/plane.frag", 0); // 5
	initShaderProgram("pp/particle.vert", "pp/particle.frag", 0); // 6
	initShaderProgram("pp/skybox.vert", "pp/skybox.frag", 0); // 7
	initShaderProgram("pp/depthoffield.vert", "pp/depthoffield.frag", 0); // 8
	resetResPath();
}

void Project::initModels()
{
	ObjLoader objL;
	addResPath("models/");

	Model *sphere = objL.loadModel(getResFile("sphere/ball_250x250.obj")); // 0
	m_sceneData->models.push_back(sphere);

	Model *plane = objL.loadModel(getResFile("basic/plane.obj")); // 1
	m_sceneData->models.push_back(plane);

	resetResPath();
}

void Project::initVAOs()
{
	VAO_SceneOrigin* vao0 = new VAO_SceneOrigin();
	vao0->init();
	m_sceneData->vaos.push_back(vao0);

	VAO_GridXY* vao1 = new VAO_GridXY();
	vao1->init();
	m_sceneData->vaos.push_back(vao1);

	VAO* vaoSphere = new VAO(); // 2
	vaoSphere->createFromModel(m_sceneData->models[0]);
	m_sceneData->vaos.push_back(vaoSphere);

	VAO_QuadV3C4T2 *vaoQuad = new VAO_QuadV3C4T2(); // 3
	vaoQuad->init();
	m_sceneData->vaos.push_back(vaoQuad);

	VAO* vaoPlane = new VAO(); // 4
	vaoPlane->createFromModel(m_sceneData->models[1]);
	m_sceneData->vaos.push_back(vaoPlane);

	VAO_Vertex *vaoVertex = new VAO_Vertex(); // 5
	vaoVertex->init();
	m_sceneData->vaos.push_back(vaoVertex);

	VAO_Skybox* skybox = new VAO_Skybox(); // 6
	skybox->init();
	m_sceneData->vaos.push_back(skybox);
}

void Project::initTextures() {
	// Load texture
	addResPath("textures/");
	FIBITMAP *image = ImageManager::GenericLoader(getResFile("energy.jpg"), 0);
	
	// Gen texture
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP_TO_EDGE);

	// Load tex image based on depth
	if (FreeImage_GetBPP(image) == 8) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(image), FreeImage_GetHeight(image), 0, GL_RED, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));
	}
	else if (FreeImage_GetBPP(image) == 24) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(image), FreeImage_GetHeight(image), 0, GL_BGR, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));
	}
	else if (FreeImage_GetBPP(image) == 32) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(image), FreeImage_GetHeight(image), 0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));
	}

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, 1.5f);

	// Push new texture
	m_sceneData->textures.push_back(texID);

	FreeImage_Unload(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Skybox texture
	std::vector<char*> fileNames = {
		"bkg1_left.png",
		"bkg1_right.png",
		"bkg1_bot.png",
		"bkg1_top.png",
		"bkg1_back.png",
		"bkg1_front.png",
	}; 

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	for (int i = 0; i < fileNames.size(); i++) {
		image = ImageManager::GenericLoader(getResFile(fileNames[i]), 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, FreeImage_GetWidth(image), FreeImage_GetHeight(image), 0, GL_BGR, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));
		FreeImage_Unload(image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	m_sceneData->textures.push_back(texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	resetResPath();
;}

void Project::initMaterials()
{
	Material *mAtom = new Material(); // 0
	mAtom->m_ambient[0] = 0.05f;
	mAtom->m_ambient[1] = 0.05f;
	mAtom->m_ambient[2] = 0.05f;
	mAtom->m_ambient[3] = 1.0f;

	mAtom->m_diffuse[0] = 0.0f;
	mAtom->m_diffuse[1] = 0.3f;
	mAtom->m_diffuse[2] = 0.7f;
	mAtom->m_diffuse[3] = 1.0f;

	mAtom->m_specular[0] = 0.0f;
	mAtom->m_specular[1] = 0.7f;
	mAtom->m_specular[2] = 1.0f;
	mAtom->m_specular[3] = 1.0f;

	mAtom->m_diffuseTextureGL = m_sceneData->textures[0];
	m_sceneData->materials.push_back(mAtom);


	Material *mElectron = new Material(); // 1
	mElectron->m_ambient[0] = 0.05f;
	mElectron->m_ambient[1] = 0.05f;
	mElectron->m_ambient[2] = 0.05f;
	mElectron->m_ambient[3] = 1.0f;

	mElectron->m_diffuse[0] = 0.8f;
	mElectron->m_diffuse[1] = 0.1f;
	mElectron->m_diffuse[2] = 0.2f;
	mElectron->m_diffuse[3] = 1.0f;

	mElectron->m_specular[0] = 0.8f;
	mElectron->m_specular[1] = 0.3f;
	mElectron->m_specular[2] = 0.4f;
	mElectron->m_specular[3] = 1.0f;
	m_sceneData->materials.push_back(mElectron);


	Material *mPlane = new Material(); // 2
	mPlane->m_ambient[0] = 0.3f;
	mPlane->m_ambient[1] = 0.3f;
	mPlane->m_ambient[2] = 0.3f;
	mPlane->m_ambient[3] = 1.0f;

	mPlane->m_diffuse[0] = 1.2f;
	mPlane->m_diffuse[1] = 0.2f;
	mPlane->m_diffuse[2] = 0.2f;
	mPlane->m_diffuse[3] = 1.0f;

	mPlane->m_specular[0] = 0.2f;
	mPlane->m_specular[1] = 0.2f;
	mPlane->m_specular[2] = 0.2f;
	mPlane->m_specular[3] = 1.0f;

	mPlane->m_shininess = 32;
	m_sceneData->materials.push_back(mPlane);
}

void Project::initLights() {
	// Generate lights for atom and electrons, since they're our light source
	Light *lAtom = new Light();
	Material *mAtom = m_sceneData->materials[0];

	// Copy material colors as light colors
	memcpy(lAtom->m_ambient, mAtom->m_ambient, 4 * sizeof(float));
	memcpy(lAtom->m_diffuse, mAtom->m_diffuse, 4 * sizeof(float));
	memcpy(lAtom->m_specular, mAtom->m_specular, 4 * sizeof(float));
	lAtom->intensity = 5.0f;
	m_sceneData->lights.push_back(lAtom);

	// Generate lights for electrons
	for (int i = 0; i < 3; i++) {
		Light *lElectron = new Light();
		Material *mElectron = m_sceneData->materials[1];

		// Copy material colors as light colors
		memcpy(lElectron->m_ambient, mElectron->m_ambient, 4 * sizeof(float));
		memcpy(lElectron->m_diffuse, mElectron->m_diffuse, 4 * sizeof(float));
		memcpy(lElectron->m_specular, mElectron->m_specular, 4 * sizeof(float));
		m_sceneData->lights.push_back(lElectron);
	}
}

void Project::initInfoEntities()
{
	Entity_SceneOrigin *e0 = new Entity_SceneOrigin(m_sceneData->vaos[0]);
	e0->init();
	m_sceneData->infoEntities.push_back(e0);

	Entity_GridXY *e1 = new Entity_GridXY(m_sceneData->vaos[1]);
	e1->init();
	m_sceneData->infoEntities.push_back(e1);
}

void Project::initSceneEntities()
{
	// Atom center
	Entity_Atom *objAtom = new Entity_Atom(m_sceneData->models[0], m_sceneData->vaos[2]);
	objAtom->setOrientation(90, 0, 0);
	objAtom->setPosition(0, 0, 0);
	objAtom->setScale(0.5f, 0.5f, 0.5f);
	objAtom->init();
	objAtom->m_material = m_sceneData->materials[0];
	m_sceneData->sceneEntities.push_back(objAtom);

	// Atom electrons
	Entity_Electron *obj = new Entity_Electron(m_sceneData->models[0], m_sceneData->vaos[2], m_sceneData->vaos[5]);
	obj->setOrientation(0, 0, 90);
	obj->init();
	obj->m_material = m_sceneData->materials[0];
	obj->angle = 75.0f;
	obj->velocity = 0.035f;
	obj->scale = 0.22f;
	obj->position = glm::normalize(glm::vec3(1.0f, 0, 1.0f)) * glm::vec3(2.0f);
	obj->rotationAxis = glm::normalize(glm::vec3(0, 1.0f, 0));
	m_sceneData->sceneEntities.push_back(obj);

	obj = new Entity_Electron(m_sceneData->models[0], m_sceneData->vaos[2], m_sceneData->vaos[5]);
	obj->setOrientation(0, 0, 90);
	obj->init();
	obj->m_material = m_sceneData->materials[0];
	obj->angle = 175.0f;
	obj->velocity = 0.037f;
	obj->scale = 0.22f;
	obj->position = glm::normalize(glm::vec3(1.0f, 0, 0)) * glm::vec3(2.0f);
	obj->rotationAxis = glm::normalize(glm::vec3(0, 1.0f, 1.0f));
	m_sceneData->sceneEntities.push_back(obj);

	obj = new Entity_Electron(m_sceneData->models[0], m_sceneData->vaos[2], m_sceneData->vaos[5]);
	obj->setOrientation(0, 0, 90);
	obj->init();
	obj->m_material = m_sceneData->materials[0];
	obj->angle = 275.0f;
	obj->velocity = 0.039f;
	obj->scale = 0.22f;
	obj->position = glm::normalize(glm::vec3(1.0f, 0, 0)) * glm::vec3(2.0f);
	obj->rotationAxis = glm::normalize(glm::vec3(0, -1.0f, 1.0f));
	m_sceneData->sceneEntities.push_back(obj);

	// Quad
	Entity_QuadV3C4N3T2 *entityQuad = new Entity_QuadV3C4N3T2(m_sceneData->vaos[3]);
	entityQuad->init();
	m_sceneData->sceneEntities.push_back(entityQuad);

	// Plane
	Entity_OBJ *objPlane = new Entity_OBJ(m_sceneData->models[1], m_sceneData->vaos[4]);
	objPlane->setOrientation(0, 0, 90);
	objPlane->init();
	objPlane->setPosition(0, 0, -2.5f);
	objPlane->setScale(40.0f, 40.0f, 40.0f);
	m_sceneData->sceneEntities.push_back(objPlane);

	// Skybox
	Entity_Skybox *skybox = new Entity_Skybox(m_sceneData->vaos[6]);
	skybox->setOrientation(-90, 0, 0);
	skybox->setPosition(0, 0, 1.0f);
	skybox->setScale(40.0f, 40.0f, 40.0f);
	skybox->init();
	m_sceneData->sceneEntities.push_back(skybox);
}

void Project::initFBOs() {
	// Scene settings
	SceneSetting *ss = SceneSetting::GetInstance();

	// Generate frame buffer
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Generate 2 color buffers for HDR and one for depth
	GLuint colorBuffers[3];
	glGenTextures(3, colorBuffers);

	for (GLuint i = 0; i < 3; i++) {
		// Create texture attachments
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);

		// Only one channel for depth buffer
		if (i == 2) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ss->m_screen[0], ss->m_screen[1], 0, GL_RED, GL_FLOAT, NULL);
		} else{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ss->m_screen[0], ss->m_screen[1], 0, GL_RGBA, GL_FLOAT, NULL);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Attach texture to frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
		m_sceneData->textures.push_back(colorBuffers[i]);
	}

	// Save frame buffer id
	m_sceneData->fboIds.push_back(fbo);

	// Create render buffer to hold stencil and depth test since we want to do it but not wor with it
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ss->m_screen[0], ss->m_screen[1]);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Attach depth and stencil to depth buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// We tell draw buffer to draw into both attachments, otherwise it renders only into first one
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// Check frame buffer status
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "FRAMEBUFFER error: framebuffer is not complete!";
	}

	// Unbind frame buffers
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Blur buffer for bluring texture with gaussian blur
	GLuint blurFbos[2];
	glGenFramebuffers(2, blurFbos);
	GLuint blurTextures[2];
	glGenTextures(2, blurTextures);

	for (int i = 0; i < 2; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, blurFbos[i]);
		glBindTexture(GL_TEXTURE_2D, blurTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ss->m_screen[0], ss->m_screen[1], 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Attach texture to frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurTextures[i], 0);
		m_sceneData->textures.push_back(blurTextures[i]);
	}

	m_sceneData->fboIds.push_back(blurFbos[0]);
	m_sceneData->fboIds.push_back(blurFbos[1]);

	// Unbind frame buffers
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Depth of field FBO
	GLuint depthFbo, depthTexture;
	glGenFramebuffers(1, &depthFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFbo);
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ss->m_screen[0], ss->m_screen[1], 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Attach texture to frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, depthTexture, 0);
	m_sceneData->textures.push_back(depthTexture);
	m_sceneData->fboIds.push_back(depthFbo);

	// Unbind frame buffers
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Project::update()
{
	// Call update on all entities
	for (Entity** it = m_sceneData->sceneEntities.front(); it <= m_sceneData->sceneEntities.back(); it++) {
		(*it)->update();
	}

	// Update lights positions with electron new world coordinates
	for (int i = 0; i < 4; i++) {
		glm::vec4 pos = m_sceneData->sceneEntities[i]->getPosition();
		memcpy(m_sceneData->lights[i]->m_position, pos.data, 4 * sizeof(float));
	}
}

void Project::render()
{
	SceneSetting *ss = SceneSetting::GetInstance();
	update();

#pragma region First Pass
	glBindFramebuffer(GL_FRAMEBUFFER, m_sceneData->fboIds[0]);
	glClearColor(0.1, 0.1, 0.1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

#pragma region Draw Info Entities
	//ss->m_activeShader = m_sceneData->shaderPrograms[0];
	//ss->m_activeShader->enable();

	//glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE);

	//uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "MVPMatrix");
	//glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewProjectionMatrix());

	//for (unsigned int i = 0; i < m_sceneData->infoEntities.size(); i++)
	//	m_sceneData->infoEntities[i]->draw();
#pragma endregion

#pragma region Draw Scene Entities
#pragma region Skybox
	ss->m_activeShader = m_sceneData->shaderPrograms[7];
	ss->m_activeShader->enable();

	Entity *e = m_sceneData->sceneEntities[6];
	glm::mat4 tmp = ss->m_activeCamera->getVM() * (e)->m_modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "MVMatrix"), 1, GL_FALSE, (float*)&tmp[0]);
	glUniformMatrix4fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix"), 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_sceneData->textures[1]);
	glUniform1i(glGetUniformLocation(ss->m_activeShader->m_programObject, "skybox"), 0);
	glUniform1f(glGetUniformLocation(ss->m_activeShader->m_programObject, "far"), dFar);
	glUniform1f(glGetUniformLocation(ss->m_activeShader->m_programObject, "near"), dNear);
	e->draw();
	#pragma endregion

#pragma region Plane
	// Draw plane
	ss->m_activeShader = m_sceneData->shaderPrograms[5];
	ss->m_activeShader->enable();

	e = m_sceneData->sceneEntities[5];
	Entity_Atom *a = (Entity_Atom*)m_sceneData->sceneEntities[0];
	tmp = ss->m_activeCamera->getVM() * (e)->m_modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "MVMatrix"), 1, GL_FALSE, (float*)&tmp[0]);
	glUniformMatrix3fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "NormalMatrix"), 1, GL_FALSE, (float*)&(glm::inverseTranspose(glm::mat3(tmp)))[0]);
	glUniformMatrix4fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix"), 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
	glUniformMatrix4fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix"), 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());
	Material::setShaderUniform(m_sceneData->materials[2], ss->m_activeShader, "material");
	glUniform1f(glGetUniformLocation(ss->m_activeShader->m_programObject, "pulse"), a->pulse);
	glUniform1f(glGetUniformLocation(ss->m_activeShader->m_programObject, "far"), dFar);
	glUniform1f(glGetUniformLocation(ss->m_activeShader->m_programObject, "near"), dNear);

	// Set light uniforms
	for (int i = 0; i < m_sceneData->lights.size(); i++) {
		Light::setShaderUniform(m_sceneData->lights[i], ss->m_activeShader, "light", i);
	}

	e->draw();
#pragma endregion

#pragma region Atom
	// Draw atom
	ss->m_activeShader = m_sceneData->shaderPrograms[1];
	ss->m_activeShader->enable();

	a = (Entity_Atom*)m_sceneData->sceneEntities[0];
	tmp = ss->m_activeCamera->getVM() * a->m_modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "MVMatrix"), 1, GL_FALSE, (float*)&tmp[0]);
	glUniformMatrix3fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "NormalMatrix"), 1, GL_FALSE, (float*)&(glm::inverseTranspose(glm::mat3(tmp)))[0]);
	glUniformMatrix4fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix"), 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
	glUniformMatrix4fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix"), 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());

	Material::setShaderUniform(m_sceneData->materials[0], ss->m_activeShader, "material");
	glUniform1f(glGetUniformLocation(ss->m_activeShader->m_programObject, "shift"), a->textureShift);
	glUniform1f(glGetUniformLocation(ss->m_activeShader->m_programObject, "pulse"), a->pulse);
	glUniform1f(glGetUniformLocation(ss->m_activeShader->m_programObject, "far"), dFar);
	glUniform1f(glGetUniformLocation(ss->m_activeShader->m_programObject, "near"), dNear);
	a->draw();
#pragma endregion

#pragma region Electrons
	// Draw electrons
	ss->m_activeShader = m_sceneData->shaderPrograms[2];
	ss->m_activeShader->enable();

	for (Entity** it = m_sceneData->sceneEntities.front() + 1; it <= m_sceneData->sceneEntities.back() - 3; it++) {
		tmp = ss->m_activeCamera->getVM() * (*it)->m_modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix"), 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
		glUniformMatrix4fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix"), 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());
		glUniformMatrix4fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "MVMatrix"), 1, GL_FALSE, (float*)&tmp[0]);
		glUniformMatrix3fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "NormalMatrix"), 1, GL_FALSE, (float*)&(glm::inverseTranspose(glm::mat3(tmp)))[0]);
		Material::setShaderUniform(m_sceneData->materials[1], ss->m_activeShader, "material");
		glUniform1f(glGetUniformLocation(ss->m_activeShader->m_programObject, "far"), dFar);
		glUniform1f(glGetUniformLocation(ss->m_activeShader->m_programObject, "near"), dNear);
		(*it)->draw();
	}
#pragma endregion

#pragma region Particles
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ss->m_activeShader = m_sceneData->shaderPrograms[6];
	ss->m_activeShader->enable();

	for (int i = 1; i < 4; i++) {
		Entity_Electron *el = (Entity_Electron*)m_sceneData->sceneEntities[i];

		for (int j = 0; j < el->particleEntities.size(); j++) {
			Entity_Particle *p = (Entity_Particle *)el->particleEntities[j];

			if (p->life > 0) {
				tmp = ss->m_activeCamera->getVM() * p->m_modelMatrix;
				glUniformMatrix4fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "MVMatrix"), 1, GL_FALSE, (float*)&tmp[0]);
				glUniformMatrix4fv(glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix"), 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
				glUniform1f(glGetUniformLocation(ss->m_activeShader->m_programObject, "life"), p->life);
				glUniform3f(glGetUniformLocation(ss->m_activeShader->m_programObject, "color"), p->color.r, p->color.g, p->color.b);
				glUniform1f(glGetUniformLocation(ss->m_activeShader->m_programObject, "far"), dFar);
				glUniform1f(glGetUniformLocation(ss->m_activeShader->m_programObject, "near"), dNear);
				p->draw();
			}
		}
	}

	glDisable(GL_BLEND);
#pragma endregion

	// Unbind frame buffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion
#pragma endregion

#pragma region Frame Buffer (blur)
	// Blur using gaussian blur
	GLboolean horizontal = true, firstIt = true;

	// Enable blur shader
	ss->m_activeShader = m_sceneData->shaderPrograms[4];
	ss->m_activeShader->enable();

	for (int i = 0; i < blurPassCount; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_sceneData->fboIds[1 + horizontal]);
		glActiveTexture(GL_TEXTURE0);
		// We switch first between HDR rendered texture and then between color buffers horizontal and vertical
		glBindTexture(GL_TEXTURE_2D, firstIt ? m_sceneData->textures[3] : m_sceneData->textures[!horizontal + 5]);
		glUniform1i(glGetUniformLocation(ss->m_activeShader->m_programObject, "scene"), 0); // Bind texture
		glUniform1i(glGetUniformLocation(ss->m_activeShader->m_programObject, "horizontal"), horizontal); // Bind horizontal uniform

		// Draw quad
		e = m_sceneData->sceneEntities[4];
		e->draw();

		horizontal = !horizontal;
		firstIt = false;
	}

	// Unbind frame buffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

#pragma region Second Pass (depth of field preaparation, bloom finalization)
	// Enable bloom shader
	ss->m_activeShader = m_sceneData->shaderPrograms[3];
	ss->m_activeShader->enable();

	glBindFramebuffer(GL_FRAMEBUFFER, m_sceneData->fboIds[3]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[2]);
	glUniform1i(glGetUniformLocation(ss->m_activeShader->m_programObject, "scene"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[5]);
	glUniform1i(glGetUniformLocation(ss->m_activeShader->m_programObject, "bloom"), 1);

	e = m_sceneData->sceneEntities[4];
	e->draw();

	// Unbind frame buffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

#pragma region Fourth pass, appply gaussian to final scene (reuse frame buffers for gaussian)
	// Blur using gaussian blur
	horizontal = true, firstIt = true;

	// Enable blur shader
	ss->m_activeShader = m_sceneData->shaderPrograms[4];
	ss->m_activeShader->enable();

	for (int i = 0; i < blurPassCount; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_sceneData->fboIds[1 + horizontal]);
		glActiveTexture(GL_TEXTURE0);
		// We switch first between HDR rendered texture and then between color buffers horizontal and vertical
		glBindTexture(GL_TEXTURE_2D, firstIt ? m_sceneData->textures[7] : m_sceneData->textures[!horizontal + 5]);
		glUniform1i(glGetUniformLocation(ss->m_activeShader->m_programObject, "scene"), 0); // Bind texture
		glUniform1i(glGetUniformLocation(ss->m_activeShader->m_programObject, "horizontal"), horizontal);
		e->draw();

		horizontal = !horizontal;
		firstIt = false;
	}

	// Unbind frame buffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

#pragma region Fifth Pass (Apply depth of field effect)
	ss->m_activeShader = m_sceneData->shaderPrograms[8];
	ss->m_activeShader->enable();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[7]);
	glUniform1i(glGetUniformLocation(ss->m_activeShader->m_programObject, "scene"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[5]);
	glUniform1i(glGetUniformLocation(ss->m_activeShader->m_programObject, "blur"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[4]);
	glUniform1i(glGetUniformLocation(ss->m_activeShader->m_programObject, "depth"), 2);

	e = m_sceneData->sceneEntities[4];
	e->draw();
#pragma endregion
}
