// Main entry point for application.
// Initialises main window, captures user input and passes onto appropriate class for handling.
// Utilises FreeGLUT API for window.
// Initialises Scene and Input class.
// @author Paul Robertson

// Include glut, opengl libraries and custom classes
#include "glew.h"
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Input.h"
#include "Scene.h"
#include "GameScene.h"
#include "LoadScene.h"

// Required variables; pointer to scene and input objects. Initialise variable used in delta time calculation.

Input *input;
__declspec(selectany) LoadScene *load_scene;
__declspec(selectany) GameScene *game_scene;
__declspec(selectany) Scene *scene;

// GameScene
// LoadingScene

int oldTimeSinceStart = 0;
// Window properties
const unsigned windowWidth = 1366, windowHeight = 768;
const unsigned initWindowX = 25, initWindowY = 10;

// Called when the window detects a change in size.
// GLUT handles the window refresh, this function passes the new width and height to the
// scene object to refresh the OpenGL buffers to the new dimensions.
void changeSize(int w, int h)
{
	scene = game_scene;

	scene->resize(w, h);
}

// Called as part of the GLUT main loop.
// Calculates delta time (the time between frames, in seconds)
// Calls Scene update and render for the next frame.
void renderScene(void)
{
	
	// Calculate delta time.
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (float)timeSinceStart - (float)oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;
	deltaTime = deltaTime / 100.0f;

	// switch game state
	/*
		switch(gameState) {
			case Loading:
				scene = loadingScene;

			case Game:
				scene = gameScene;
		}
	*/


	// Update Scene and render next frame.
	scene->update(deltaTime);
	scene->render();
}

// Handles keyboard input events from GLUT.
// Called whenever a "normal" key is pressed.
// Normal keys are defined as any key not including the F keys, CTRL, SHIFT, ALT, etc.
// Key press is recorded in Input class
// Parameters include key pressed and current mouse x, y coordinates.
// Mouse coordinates are handled separately.
void processNormalKeys(unsigned char key, int x, int y)
{
	// If the ESCAPE key was pressed, exit application.
	if (key == 27)	// Escape key (in non-windows you can use 27, the ASCII value for escape)
		exit(0);
	// Send key down to input class.
	input->SetKeyDown(key);
}

// Handles keyboard input events from GLUT.
// Called whenever a "normal" key is released.
// Normal keys are defined as any key not including the F keys, CTRL, SHIFT, ALT, arrow keys, etc.
// Key press is recorded in Input class
// Parameters include key pressed and current mouse x, y coordinates.
// Mouse coordinates are handled separately.
void processNormalKeysUp(unsigned char key, int x, int y)
{
	// Send key up to input class.
	input->SetKeyUp(key);
}

// Handles keyboard input events from GLUT.
// Called whenever a "special" key is pressed.
// Special keys are defined as F keys, CTRL, SHIFT, ALT, arrow keys, etc
// Currently a place holder function, can be utilised if required.
// Parameters include key pressed and current mouse x, y coordinates.
// Mouse coordinates are handled separately.
void processSpecialKeys(int key, int x, int y)
{
	input->SetSpecialKeyDown(key);
}

// Handles keyboard input events from GLUT.
// Called whenever a "special" key is released.
// Special keys are defined as F keys, CTRL, SHIFT, ALT, arrow keys, etc
// Currently a place holder function, can be utilised if required.
// Parameters include key pressed and current mouse x, y coordinates.
// Mouse coordinates are handled separately.
void processSpecialKeysUp(int key, int x, int y)
{
	input->SetSpecialKeyUp(key);
}

// Handles mouse movement events from GLUT.
// Active movement is define as mouse movement while a mouse button is pressed.
// Called every loop. Parameters are the new x, y coordinates of the mouse.
void processActiveMouseMove(int x, int y)
{
	// Record mouse position in Input class.
	input->setMousePos(x, y);
}

// Handles mouse movement events from GLUT.
// Passive mouse movement is define as mouse movement without a mouse button is pressed.
// Called every loop. Parameters are the new x, y coordinates of the mouse.
void processPassiveMouseMove(int x, int y)
{
	// Record mouse position in Input class.
	input->setMousePos(x, y);
}

// Handles mouse button events from GLUT.
// Parameters include mouse button that fired the event (GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON),
// button state (up and down), and current cursor position.
void processMouseButtons(int button, int state, int x, int y)
{
	enum
	{
		MOUSE_LEFT_BUTTON = 0,
		MOUSE_MIDDLE_BUTTON = 1,
		MOUSE_RIGHT_BUTTON = 2,
		MOUSE_SCROLL_UP = 3,
		MOUSE_SCROLL_DOWN = 4,
		MOUSE_BUTTON_DOWN = 0,
		MOUSE_BUTTON_UP = 1
	};

	switch (button) {
		// Detect left button press/released
	case MOUSE_LEFT_BUTTON: {
		switch (state) {
		case MOUSE_BUTTON_DOWN: input->setLeftMouseButton(true);
			break;
		default: input->setLeftMouseButton(false);
			break;
		}
	} break;
		// Detect middle button press/released
	case MOUSE_MIDDLE_BUTTON: {
		switch (state) {
		case MOUSE_BUTTON_DOWN: input->setMiddleMouseButton(true);
			break;
		default: input->setMiddleMouseButton(false);
			break;
		}
	} break;
		// Detect right button press/released
	case MOUSE_RIGHT_BUTTON: {
		switch (state) {
		case MOUSE_BUTTON_DOWN: input->setRightMouseButton(true);
			break;
		default: input->setRightMouseButton(false);
			break;
		}
	} break;
		// Detect mouse wheel scroll up (setScrollUpMouseWheel(false) must be called in update() after isScrollUpMouseWheel())  
		//case MOUSE_SCROLL_UP: {
		//	switch (state) {
		//	case MOUSE_BUTTON_DOWN: input->setScrollUpMouseWheel(true);
		//		break;
		//	}
		//} break;
		//// Detect mouse wheel scroll down (setScrollDownMouseWheel(false) must be called in update() after isScrollDownMouseWheel())
		//case MOUSE_SCROLL_DOWN: {
		//	switch (state) {
		//	case MOUSE_BUTTON_DOWN: input->setScrollDownMouseWheel(true);
		//		break;
		//	}
		//} break;
	}
}

void processMouseWheel(int wheel, int direction, int x, int y) {
	enum {
		UP = 1,
		DOWN = -1,
	};

	switch (direction) {
	case UP:
		input->setScrollUpMouseWheel(true);
		break;
	case DOWN:
		input->setScrollDownMouseWheel(true);
		break;
	}
}

// Main entery point for application.
// Initialises GLUT and application window.
// Registers callback functions for handling GLUT input events
// Registers callback functions for window resizing and rendering.
// Initialises Input and Scene class, prior to starting Main Loop.
int main(int argc, char **argv)
{
	// Init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
	glutInitWindowPosition(initWindowX, initWindowY);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("OpenGL");

	// Register callback functions for change in size and rendering.
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	// Register Input callback functions.
	// 'Normal' keys processing
	glutKeyboardFunc(processNormalKeys);
	glutKeyboardUpFunc(processNormalKeysUp);
	// Special keys processing
	glutSpecialFunc(processSpecialKeys);
	glutSpecialUpFunc(processSpecialKeysUp);

	// Mouse callbacks
	glutMotionFunc(processActiveMouseMove);
	glutPassiveMotionFunc(processPassiveMouseMove);
	// void glutMouseFunc(void(*func)(int button, int state, int x, int y))
	glutMouseFunc(processMouseButtons);
	glutMouseWheelFunc(processMouseWheel);
	// Position mouse in centre of windows before main loop (window not resized yet)
	glutWarpPointer(windowWidth / 2, windowHeight / 2);
	// Hide mouse cursor
	glutSetCursor(GLUT_CURSOR_NONE);
	// warp
	
	// Initialise input and scene objects.
	input = new Input();

	//scene = new LoadingScene(input);
	load_scene = new LoadScene();
	game_scene = new GameScene(input);

	// load scene 
	// Create GameScene();
	// Create LoadingScene(GameScene *)
	// delete LoadingScene();
	// LoadingScene thread tells game to start loafing assetss

	// Enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}
