#include "MainGame.h"
#include "Errors.h"

#include <iostream>
#include <string>

MainGame::MainGame()
{
	_window = nullptr;
	_screenWidth = 1024;
	_screenHeight = 768;
	_gameState = GameState::PLAY;
}

MainGame::~MainGame()
{
	
}

void MainGame::Run()
{
	InitSystems();
	_sprite.Init(-1.0f, -1.0f, -1.0f, -1.0f);
	GameLoop();
}

void MainGame::InitSystems()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
	if(_window == nullptr)
	{
		fatalError("SDL Window could not be created!");
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if(glContext == nullptr)
	{
		fatalError("SDL_GLContext could not be created!");
	}


	GLenum error = glewInit();
	if(error != GLEW_OK)
	{
		fatalError("Could not initialize glew!");
	}

	//double buffer window, prevent flickering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	InitShaders();
}

void MainGame::InitShaders()
{
	_shaderProgram.CreateShaders("Shaders/shader.vert", "Shaders/shader.frag");
	_shaderProgram.AddAttribute("vertexPosition");
	_shaderProgram.LinkShaders();
}

void MainGame::GameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		ProcessInput();
		DrawGame();
	}
}

void MainGame::ProcessInput()
{
	SDL_Event evnt;
	while(SDL_PollEvent(&evnt))
	{
		switch(evnt.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
			break;
		}
	}
}

void MainGame::DrawGame()
{
	//base depth
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_shaderProgram.Use();
	_sprite.Draw();
	_shaderProgram.Unuse();
	SDL_GL_SwapWindow(_window);
}





