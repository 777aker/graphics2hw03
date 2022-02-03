/*
* HW03: Performance
* Kelley Kelley
* 
* Creates two windows to compare performance, one done to the best
* of my ability, one done to the best worst of my ability
* 
* Key bindings:
* m    switch between good and bad shader
* r    refresh shaders (for editing shader files)
* arrows    change view angle
* pgdn/pgup    zoom
* 0    reset view angle
* ESC    Exit
*/
#include "CSCIx239.h"
#include <stdbool.h>
int zh = 0; // light position
int th = 0, ph = 0; // view angles
int fov = 57; // fov
float asp = 1; // aspect ratio
float dim = 3; // size of world
int snowshader; // use regular blinn per pixel lighting
int pershader; // use performance shader
bool good = true; // whether or not this is the good or bad shader
int tex; // pi texture

// display function
void display(GLFWwindow* window) {
	// erase window and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// enable z buffer
	glEnable(GL_DEPTH_TEST);
	// set view
	View(th, ph, fov, dim);
	// Enable lighting
	Lighting(0, 3 * Cos(zh), 3 * Sin(zh), 0.3, 0.5, 0.8);

	// use weird shader on big sphere
	glUseProgram(pershader);
	float t = glfwGetTime();
	int id = glGetUniformLocation(pershader, "time");
	glUniform1f(id, t);
	// draw a sky sphere
	Sphere(0, 0, 0, 6, 0, 15, tex);

	// select shader
	glUseProgram(snowshader);
	id = glGetUniformLocation(snowshader, "time");
	glUniform1f(id, t);
	id = glGetUniformLocation(snowshader, "loc");
	glUniform3f(id, 0, 0, 1);
	// Draw scene
	TexturedCube(tex);

	// revert to fixed pipeline
	glUseProgram(0);
	glDisable(GL_LIGHTING);

	// display stuff
	SetColor(1, 1, 1);
	glWindowPos2i(5, 5);
	Print("Good Shader = %d, FPS=%d", good, FramesPerSecond());
	// render scene and make it visible
	ErrCheck("display");
	glFlush();
	glfwSwapBuffers(window);
}

// call this when key pressed
void key(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// discard key releases
	if (action == GLFW_RELEASE) return;

	// switch on key (I just like switch statements)
	switch (key) {
	// escape on escape key
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, 1);
		break;
	// reset view angle
	case GLFW_KEY_0:
		th = ph = 0;
		dim = 3;
		break;
	// change light position
	case GLFW_KEY_RIGHT:
		th += 5;
		break;
	case GLFW_KEY_LEFT:
		th -= 5;
		break;
	case GLFW_KEY_UP:
		ph += 5;
		break;
	case GLFW_KEY_DOWN:
		ph -= 5;
		break;
	// zoom in and out
	case GLFW_KEY_PAGE_DOWN:
		dim += 0.1;
		break;
	case GLFW_KEY_PAGE_UP:
		dim -= 0.1;
		break;
	// switch between good and bad shader
	case GLFW_KEY_M:
		good = !good;
		// don't break so we reload shaders
	// reload shaders
	case GLFW_KEY_R:
		if (good) {
			pershader = CreateShaderProg("performancegood.vert", "performancegood.frag");
			snowshader = CreateShaderProg("snowgood.vert", "snowgood.frag");
		}
		else {
			pershader = CreateShaderProg("performancebad.vert", "performancebad.frag");
			snowshader = CreateShaderProg("snowbad.vert", "snowbad.frag");
		}
			
		break;
	}
	// wrap angles
	th %= 360;
	ph %= 360;
	// Update projection
	Projection(fov, asp, dim);
}

// call this when resized
void reshape(GLFWwindow* window, int width, int height) {
	// get framebuffer dimensions for apple
	glfwGetFramebufferSize(window, &width, &height);
	// ratio of width to height of window
	asp = (height > 0) ? (double)width / height : 1;
	// set viewport to entire window
	glViewport(0, 0, width, height);
	// set projection
	Projection(fov, asp, dim);
}

// main and start GLFW event loops
int main(int argc, char* argv[]) {
	// initialize GLFW
	GLFWwindow* window = InitWindow("HW03 Kelley Kelley", 0, 600, 600, &reshape, &key);
	// load special shaders and set good to false (only used when reloading shaders)
	pershader = CreateShaderProg("performancegood.vert", "performancegood.frag");
	// snow shaders
	snowshader = CreateShaderProg("snowgood.vert", "snowgood.frag");
	// load textures
	tex = LoadTexBMP("pi.bmp");

	// event loop
	ErrCheck("init");
	while (!glfwWindowShouldClose(window)) {
		// light animation
		zh = fmod(90 * glfwGetTime(), 360);
		// display
		display(window);
		// process events
		glfwPollEvents();
	}
	// shut down GLFW
	glfwDestroyWindow(window);
	glfwTerminate(); 
	return 0;
}
