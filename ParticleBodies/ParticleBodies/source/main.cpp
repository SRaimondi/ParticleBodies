#include <GL\glew.h>
#define GLFW_NO_GLU 1
#include <GLFW\glfw3.h>
#include "sphere.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define M_PI 3.14159265f

/* Define function prototypes */
static void error_callback(int error, const char* description);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scrollCallback(GLFWwindow* window, double x, double y);
void update_view(GLFWwindow*, glm::vec3 const &);
void resize(GLFWwindow* window);

/* Data for the camera navigation */
static glm::vec3 prev_position;
static glm::vec3 forward_dir;
static float phi, tao, r;

int main(void) {

	/* Variables declaration */
	GLFWwindow* window;
	GLenum err;

	r = 5.f;
	phi = M_PI / 4.f;
	tao = M_PI / 4.f;

	if (!glfwInit()) {
		return 1;
	}

	// Set window hint
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Get monitor handle
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	if (monitor == nullptr) {
		fprintf(stderr, "Error while trying to get primary monitor\n");
		return 1;
	}

	// Get monitor resolution
	const GLFWvidmode * mode = glfwGetVideoMode(monitor);

	double window_width = mode->width;
	double window_height = mode->height;

	window = glfwCreateWindow(800, 800, "Particle bodies", NULL, NULL);

	if (!window) {
		glfwTerminate();
		fprintf(stderr, "Failed to create window\n");
		getchar();

		return 1;
	}

	/* Make context current */
	glfwMakeContextCurrent(window);

	err = glewInit();
	if (GLEW_OK != err) {
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	glfwSwapInterval(1);

	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glClearColor(0.f, 0.f, 0.f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	/* Enable lighting */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Create sphere
	pb::Sphere sphere = pb::Sphere(pb::math::vec3f({ 0.f, 0.f, 0.f }), 1.f);
	sphere.generateBodyParticles(0.1f);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Poll for and process events
		glfwPollEvents();

		// Call resize function
		resize(window);

		update_view(window, glm::vec3(0.f, 0.f, 0.f));

		// Draw particles
		sphere.drawAllParticles();

		// Swap front and back buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}

/* Function implementation */
static void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void scrollCallback(GLFWwindow* window, double x, double y) {
	if (y < 0) {
		r += 1.f;
	} else if (y > 0) {
		if (r > 3.f) {
			r -= 1.f;
		}
	}
}

void resize(GLFWwindow* window) {
	float ratio;
	int width, height;

	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float)height;
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 150.f);
	//glOrtho(-4.f, 4.f, -4.f, 4.f, 1.f, 10.f);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glLightfv(GL_LIGHT0, GL_POSITION, lightpos0);
	//glm::mat4 look_at = glm::lookAt(glm::vec3(10.f, 10.f, 10.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	//glMultMatrixf(&look_at[0][0]);
}


void update_view(GLFWwindow* window, glm::vec3 const & look_at) {
	double x, y;
	double delta_x, delta_y;
	glm::mat4 M;

	glfwGetCursorPos(window, &x, &y);

	delta_x = prev_position.x - x;
	delta_y = prev_position.y - y;

	prev_position.x = x;
	prev_position.y = y;

	if (delta_x > 0.f) {
		phi += 0.05f;
	} else if (delta_x < 0) {
		phi -= 0.05f;
	} else if (delta_y > 0.f) {
		tao -= 0.05f;
		if (tao < 0.01f) {
			tao = 0.01f;
		}
	} else if (delta_y < 0.f) {
		tao += 0.05f;
		if (tao > M_PI / 2.f) {
			tao = M_PI / 2.f;
		}
	}

	glm::vec3 cam_pos;

	cam_pos.x = look_at.x + r * sin(tao) * sin(phi);
	cam_pos.y = look_at.y + r * cos(tao);
	cam_pos.z = look_at.z + r * sin(tao) * cos(phi);

	forward_dir = look_at - cam_pos;
	forward_dir.y = 0.f;
	forward_dir = glm::normalize(forward_dir);

	M = glm::lookAt(cam_pos, look_at, glm::vec3(0.f, 1.f, 0.f));

	// Compute light position
	GLfloat lightpos0[] = { look_at.x, look_at.y + 10.f, look_at.z, 1.f };

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos0);
	glMultMatrixf(&M[0][0]);
}

