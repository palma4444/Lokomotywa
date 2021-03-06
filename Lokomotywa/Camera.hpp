#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

const float FIELD_OF_VIEW = 70.0;
const float Z_NEAR = 0.01;
const float Z_FAR = 1000.0;

const float AMBIENT_LIGHT_PERC = 0.25;

class Camera
{
private:
	float radius;
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 forward = glm::vec3(0, 0, 1);
	glm::mat4 perspective;
	
public:
	glm::vec2 light_power = glm::vec2(0.2, 0.8);

	double alpha_horizontal = M_PI / 2;
	double alpha_vertical = M_PI / 2.5;
	glm::vec3 position;

	Camera(float radius, float aspect): radius(radius)
	{
		perspective = glm::perspective(FIELD_OF_VIEW, aspect, Z_NEAR, Z_FAR);
		calculatePosition();
	}

	inline glm::mat4 getViewProjection() const
	{
		return perspective * glm::lookAt(position, position + forward, up);
	}

	void moveHorizontallyByAngle(float angle)
	{
		alpha_horizontal += angle;
		if (alpha_horizontal > 2 * M_PI)
			alpha_horizontal -= 2 * M_PI;

		if (alpha_horizontal < 0)
			alpha_horizontal += 2 * M_PI;

		calculatePosition();
	}

	void moveVerticallyByAngle(float angle)
	{
		if((angle < 0 && alpha_vertical > -2 * angle) || (angle > 0 && alpha_vertical < M_PI / 2 - angle))
			alpha_vertical += angle;

		calculatePosition();
	}

	void moveAlongRadius(float step)
	{
		if(step > 0 || step < 0 && radius > 1 + step)
			radius += step;

		calculatePosition();
	}

	void calculatePosition()
	{
		position.z = radius * sin(alpha_vertical) * cos(alpha_horizontal);
		position.x = radius * sin(alpha_vertical) * sin(alpha_horizontal);
		position.y = radius * cos(alpha_vertical);

		forward.z = -1 * position.z;
		forward.x = -1 * position.x;
		forward.y = -1 * position.y;
	}

	void adjustLight(float point_light)
	{
		light_power.y += point_light;

		if (light_power.y < 0)
			light_power.y = 0;
		else if (light_power.y > 1)
			light_power.y = 1;

		light_power.x = light_power.y * AMBIENT_LIGHT_PERC;
	}
};

#endif