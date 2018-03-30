#include "Display.hpp"
#include "Shader.h"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Figures.h"

const float ROT_ANGLE = 0.03;
Camera *camera = nullptr;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_RIGHT:
			camera->moveHorizontallyByAngle(ROT_ANGLE);
			break;
		case GLFW_KEY_LEFT:
			camera->moveHorizontallyByAngle(-1 * ROT_ANGLE);
			break;
		}
	}
}

int main()
{
	Display* display = new Display(&key_callback);

	Vertices vertices = connectedRings(80, 0.2, 0.8);
	Mesh mesh(vertices);

	Shader shader("shader");

	Texture texture;
	texture.loadRGB("red_painted_metal.jpg");

	camera = new Camera(glm::vec3(0, 0, -3), 70.0f, (float)WIDTH / (float)HEIGHT, 0.01f, 1000.0f);

	Transform transform;

	while (!display->windowShouldClose())
	{
		display->checkEvents();

		display->clearColor(0.2f, 0.2f, 1.0f, 1.0f);

		shader.bind();
		texture.bind(0);
		shader.update(transform, *camera);
		mesh.draw();

		display->swapBuffers();

		transform.rotation.z -= 0.0005;
	}

	delete display;
}
