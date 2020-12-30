////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	program.cpp
//
// summary:	Implements the entry point for the SimpleView program
// author:	Petr Gajdo�
// 
// Copyright � 2014 Petr Gajdo�. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SceneManager.h"

#include "model.h"
#include "objLoader.h"

#include <sorting.h>

int main(int argc, char* argv[])
{
	SceneManager* sceneManager = SceneManager::GetInstance();
	sceneManager->Init(argc, argv);
	sceneManager->start();
	return 0;		
}


