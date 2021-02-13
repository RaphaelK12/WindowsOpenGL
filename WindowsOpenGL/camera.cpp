#include "pch.h"
#include "objetob.h"
#include "shader.h"
#include "texture.h"
#include "material.h"
#include "malha.h"
#include "objeto.h"
#include "camera.h"

camera* activecamera = new camera(0, vec3(-2, 0, 0), vec3(0, 0, 0), vec3(0.f, 1.f, 0.f));


camera::camera(void):
	nearClip(0.1f),
	farClip(100.f),
	left(-1.f),
	right(1.f),
	botton(-1.f),
	top(1.f),
	fov(0.500f),
	aspect(1.0f),
	center(0,0,0),
	//rot(0,0,0),
	up(0.0f, 1.0f, 0.0f)
{
	lpos = vec3(-5, -5, 0);
	gpos = vec3(-10, 0, 0);
	center = vec3(0, 0, 0);

	matrix.V = glm::lookAt(gpos, center, up); //pos, center, up
	matrix.P = glm::perspective(fov, aspect, nearClip, farClip);	//perspective(fovy, aspect, zNear, zFar)

}

camera::camera(objetob* Parent, vec3 localPos, vec3 lookAt, vec3 vectorUp,
			   float Aspect, float NearClip, float FarClip, float Left, float Right,
			   float Botton, float Top, float Fov
			   /*, float Rot(0,0,0)*/):
	center			(lookAt			),
	nearClip		(NearClip		),
	farClip			(FarClip		),
	left			(Left			),
	right			(Right			),
	botton			(Botton			),
	top				(Top			),
	fov				(Fov			),
	aspect			(Aspect			),
	//rot			(Rot			),
	up				(vectorUp		)
{
	lpos = vec3(localPos);
	gpos = vec3(localPos);
	if (parent)
		gpos = parent->gpos + lpos;
	//gpos = vec3(-10, 0, 0);
	//center = vec3(0, 0, 0);

	matrix.V = glm::lookAt(gpos, center, up); //pos, center, up
	matrix.P = glm::perspective(fov*3.14f, aspect, nearClip, farClip);	//perspective(fovy, aspect, zNear, zFar)

}


camera::~camera(void)
{
}

void camera::calcMatrix()
{
	mat4 m = mat4(1.0f);
	m = glm::rotate(m, grot.y, vec3(0, 1, 0));
	m = glm::rotate(m, grot.x, vec3(0, 0, 1));
	//m = glm::rotate(m, length(grot), vec3(grot));

	matrix.V = glm::lookAt(gpos, center, vec3(0.0f, 0.0f, 1.0f)) *m ; //pos, center, up
	matrix.P = glm::perspective(fov, aspect, nearClip, farClip);	//perspective(fovy, aspect, zNear, zFar)
}




void MVPexample() {
	vec3 objPoss = vec3(1, 1, 1);
	vec3 light_position = vec3(-20.0f, -20.0f, 0.0f);
	vec3 view_position = vec3(0.0f, 0.0f, 50.0f);

	mat4 model_matrix = glm::translate(glm::identity<mat4>(), vec3(0, 0, 0));	//mat4, pos
	mat4 view_matrix = glm::lookAt(view_position, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)); //pos, center, up
	mat4 proj_matrix = glm::perspective(50.0f, 1.0f, 0.1f, 1000.0f);	//perspective(fovy, aspect, zNear, zFar)

	mat4 mv_matrix = view_matrix * model_matrix;

	model_matrix = glm::scale(model_matrix, vec3(1, 1, 1));
	model_matrix = glm::rotate(model_matrix, 1.0f, vec3(1, 1, 1));
	mat4 light_proj_matrix = glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 200.0f);	//frustum( left,  right,  bottom,  top,  nearVal,  farVal)
	mat4 light_view_matrix = glm::lookAt(light_position, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
}

