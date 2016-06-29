#include <GL\glew.h>
#define GLFW_NO_GLU 1
#include <GLFW\glfw3.h>
#include "sphere.h"
#include "sphere_graphics.h"
#include "body_particles.h"
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

	glClearColor(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	/* Enable lighting */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	// Create buffers for particle drawing
	GLuint part_v_buff, part_i_buff, part_num_elements;
	pb::SphereGraphic::createSphereGraphic(10, 10, &part_v_buff, &part_i_buff, &part_num_elements);

	// Create buffers for sphere drawing
	GLuint sphere_v_buff, sphere_i_buff, sphere_num_elements;
	pb::SphereGraphic::createSphereGraphic(30, 30, &sphere_v_buff, &sphere_i_buff, &sphere_num_elements);

	// Create sphere
	pb::Body * sphere0 = new pb::Sphere(pb::math::vec3f({ 1.f, 0.f, 0.f }), 1.f, pb::math::quaternionFromAngleAxis(45.f, pb::math::vec3f({1.f, 0.f, 0.f})), 1.f);
	pb::Body * sphere1 = new pb::Sphere(pb::math::vec3f({ -2.f, 0.f, 0.f }), 1.f, pb::math::quaternionFromAngleAxis(0.f, pb::math::vec3f({ 1.f, 0.f, 0.f })), 2.f);

	// Create sphere discretisation
	pb::BodyParticlesDiscretisation sphere_discretisation0 = pb::BodyParticlesDiscretisation(sphere0, 0.1f);
	pb::BodyParticlesDiscretisation sphere_discretisation1 = pb::BodyParticlesDiscretisation(sphere1, 0.1f);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Poll for and process events
		glfwPollEvents();

		// Call resize function
		resize(window);

		update_view(window, glm::vec3(0.f, 0.f, 0.f));

		// Draw particles
		sphere_discretisation0.drawParticles(part_v_buff, part_i_buff, part_num_elements);
		sphere_discretisation1.drawParticles(part_v_buff, part_i_buff, part_num_elements);

		// Draw objects
		sphere0->drawBody(sphere_v_buff, sphere_i_buff, sphere_num_elements);
		sphere1->drawBody(sphere_v_buff, sphere_i_buff, sphere_num_elements);

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
		r += .5f;
	} else if (y > 0) {
		if (r > 2.f) {
			r -= .5f;
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
	glFrustum(-ratio, ratio, -1.f, 1.f, 0.5f, 10.f);
	//glOrtho(-4.f, 4.f, -4.f, 4.f, 0.5f, 10.f);
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

	M = glm::lookAt(cam_pos, look_at, glm::vec3(0.f, 1.f, 0.f));

	// Compute light position
	GLfloat lightpos0[] = { 1.f, 1.f, 1.f, 0.f };
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(&M[0][0]);
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos0);
}

