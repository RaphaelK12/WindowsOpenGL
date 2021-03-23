#include "pch.h"
#include "objetob.h"
#include "shader.h"
#include "texture.h"
#include "material.h"
#include "malha.h"
#include "objeto.h"
#include "camera.h"

std::vector<objeto*> g_objeto_list;

uint objeto::g_count = 0;



objeto::objeto():
	count(0),
	mIndex(0),
	parent(0)
{
	g_count++;
	g_objeto_list.push_back(this);
	mIndex = g_objeto_list.size() - 1;
}

objeto::objeto(objeto* prev) :
	count(0),
	mIndex(0),
	parent(0)
{
	g_count++;
	g_objeto_list.push_back(this);
	mIndex = g_objeto_list.size() - 1;
}

objeto::objeto(objeto* prev, objType t, vec3 Pos, vec3 Rot, vec3 Size, uint3 Res, string matName) :
	count(0),
	mIndex(0),
	parent(0)
{
	g_count++;
	g_objeto_list.push_back(this);
	mIndex = g_objeto_list.size() - 1;
	this->lpos = Pos;
	this->gpos = Pos;
	switch (t)
	{
	case objType::objNone:
		break;
	case objType::objTorus:
		malhas.push_back(new malha());
		malhas[malhas.size() - 1]->atach();
		malhas[malhas.size() - 1]->makeTorus(Res.x, Res.y, Size, 1.0f, 1.5f);
		malhas[malhas.size() - 1]->mMaterial = new Material(matName);
		//malhas[malhas.size() - 1]->mMaterial->atach();
		break;
	case objType::objEsfera:
		malhas.push_back(new malha());
		malhas[malhas.size() - 1]->atach();
		malhas[malhas.size() - 1]->makeSphere(Res.x, Res.y, Size);
		malhas[malhas.size() - 1]->mMaterial = new Material(matName);
		//malhas[malhas.size() - 1]->mMaterial->atach();
		break;
	case objType::objCilindro:
		malhas.push_back(new malha());
		malhas[malhas.size() - 1]->atach();
		malhas[malhas.size() - 1]->makeCilinder(Res.x, Res.y, Size);
		malhas[malhas.size() - 1]->mMaterial = new Material(matName);
		//malhas[malhas.size() - 1]->mMaterial->atach();
		break;
	case objType::objCone:
		malhas.push_back(new malha());
		malhas[malhas.size() - 1]->atach();
		malhas[malhas.size() - 1]->makeCone(Res.x, Res.y, Size);
		malhas[malhas.size() - 1]->mMaterial = new Material(matName);
		//malhas[malhas.size() - 1]->mMaterial->atach();
		break;
	case objType::objBox:
		malhas.push_back(new malha());
		malhas[malhas.size() - 1]->atach();
		malhas[malhas.size() - 1]->makeBox(Res.x, Res.y, Size);
		malhas[malhas.size() - 1]->mMaterial = new Material(matName);
		//malhas[malhas.size() - 1]->mMaterial->atach();
		break;
	case objType::objSkyBox:
		malhas.push_back(new malha());
		malhas[malhas.size() - 1]->atach();
		malhas[malhas.size() - 1]->makeSkyBox(Res.x, Res.y, Size);
		malhas[malhas.size() - 1]->mMaterial = new Material(matName);
		//malhas[malhas.size() - 1]->mMaterial->atach();
		break;
	case objType::objQuad:
		malhas.push_back(new malha());
		malhas[malhas.size() - 1]->atach();
		malhas[malhas.size() - 1]->makeQuad(Res.x, Res.y, Size);
		malhas[malhas.size() - 1]->mMaterial = new Material(matName);
		//malhas[malhas.size() - 1]->mMaterial->atach();
		break;
	case objType::objTriangulo:
		malhas.push_back(new malha());
		malhas[malhas.size() - 1]->atach();
		malhas[malhas.size() - 1]->makeTriangle(Res.x, Res.y, Size);
		malhas[malhas.size() - 1]->mMaterial = new Material(matName);
		//malhas[malhas.size() - 1]->mMaterial->atach();
		break;
	case objType::objGrid:
		malhas.push_back(new malha());
		malhas[malhas.size() - 1]->atach();
		malhas[malhas.size() - 1]->makeGrid(Res.x, Res.y, Size);
		malhas[malhas.size() - 1]->mMaterial = new Material(matName);
		//malhas[malhas.size() - 1]->mMaterial->atach();
		break;
	case objType::objAxis:
		malhas.push_back(new malha());
		malhas[malhas.size() - 1]->atach();
		malhas[malhas.size() - 1]->makeAxis(Res.x, Res.y, Size);
		malhas[malhas.size() - 1]->mMaterial = new Material(matName);
		//malhas[malhas.size() - 1]->mMaterial->atach();
		break;
	case objType::objAxisS:
		malhas.push_back(new malha());
		malhas[malhas.size() - 1]->atach();
		malhas[malhas.size() - 1]->makeAxisS(Res.x, Res.y, Size);
		malhas[malhas.size() - 1]->mMaterial = new Material(matName);
		//malhas[malhas.size() - 1]->mMaterial->atach();
		break;
	default:
		break;
	}
}


objeto::~objeto()
{
	//for (int i = 0; i < Material.size(); i++) {
	//	Material[i]->detach();
	//	delete Material[i];
	//	Material[i] = 0;
	//}
	for (uint i = 0; i < malhas.size(); i++) {
		malhas[i]->detach();
		delete malhas[i];
		malhas[i] = 0;
	}
	for (uint i = 0; i < child.size(); i++) {
		child[i]->detach();
		delete child[i];
		child[i] = 0;
	}
	if (g_objeto_list.size() >= mIndex)
		g_objeto_list[mIndex] = 0;
	g_count--;
}

int objeto::atach() {
	count++;
	return count;
}

void objeto::detach() {
	count--;
	if (count <= 0) {
		g_objeto_list[mIndex] = NULL;
		delete this;
	}
}


vec4 ToQuaternion(vec3 v/*, float yaw, double pitch, double roll*/) // yaw (Z), pitch (Y), roll (X)
{
	// Abbreviations for the various angular functions
	float cy = cos(v.x * 0.5f);
	float sy = sin(v.x * 0.5f);
	float cp = cos(v.y * 0.5f);
	float sp = sin(v.y * 0.5f);
	float cr = cos(v.z * 0.5f);
	float sr = sin(v.z * 0.5f);

	vec4 q(0);
	q.w = cr * cp * cy + sr * sp * sy;
	q.x = sr * cp * cy - cr * sp * sy;
	q.y = cr * sp * cy + sr * cp * sy;
	q.z = cr * cp * sy - sr * sp * cy;

	return q;
}

void ToAxisAngle(const vec3& inAxis, float angle, mat4 &m) {
	vec3 axis = normalize(inAxis);
	float s = (float)sin(angle);
	float c = (float)cos(angle);
	float x = axis.x, y = axis.y, z = axis.z;
	m[1-1][1-1] = x * x * (1 - c) + c;
	m[2-1][1-1] = x * y * (1 - c) - (z * s);
	m[3-1][1-1] = x * z * (1 - c) + (y * s);
	m[4-1][1-1] = 0;
	m[1-1][2-1] = y * x * (1 - c) + (z * s);
	m[2-1][2-1] = y * y * (1 - c) + c;
	m[3-1][2-1] = y * z * (1 - c) - (x * s);
	m[4-1][2-1] = 0;
	m[1-1][3-1] = z * x * (1 - c) - (y * s);
	m[2-1][3-1] = z * y * (1 - c) + (x * s);
	m[3-1][3-1] = z * z * (1 - c) + c;
	m[4-1][3-1] = 0;
	m[1-1][4-1] = 0;
	m[2-1][4-1] = 0;
	m[3-1][4-1] = 0;
	m[4-1][4-1] = 1;
}

void setEulerZYX(vec3 euler, mat4& m) {
	///@todo proposed to reverse this since it's labeled zyx but takes arguments xyz and it will match all other parts of the code
	float ci(cos(euler.x));
	float cj(cos(euler.y));
	float ch(cos(euler.z));
	float si(sin(euler.x));
	float sj(sin(euler.y));
	float sh(sin(euler.z));
	float cc = ci * ch;
	float cs = ci * sh;
	float sc = si * ch;
	float ss = si * sh;

	m[0][0] = cj * ch;
	m[0][1] = sj * sc - cs;
	m[0][2] = sj* cc + ss;
	m[1][0] = cj * sh;
	m[1][1] = sj * ss + cc;
	m[1][2] = sj* cs - sc;
	m[2][0] = -sj;
	m[2][1] = cj * si;
	m[2][2] = cj* ci;
}

void objeto::calcMatrix() {
	grot = vec4(vmod(grot, F_2PI), grot.w);


	matrix.V = activecamera->matrix.V;
	matrix.P = activecamera->matrix.P;
	//vec4 q = ToQuaternion(grot);
	mat4 m = mat4(1.0f);


	 //m = glm::rotate(m, q.w, vec3(grot));
	 //m = glm::rotate(m, grot.y, vec3(1, 0, 0));
	 //m = glm::rotate(m, grot.z, vec3(0, 1, 0));
	 //m = glm::rotate(m, grot.x, vec3(0, 0, 1));
	//ToAxisAngle(grot, length(grot), m);
	setEulerZYX(grot,  m);
	 //m = glm::rotate(mat4(1.0f), length(vec3(grot)), vec3(grot.y,grot.z,grot.x));
	matrix.M = glm::scale(m, gsize);
	//matrix.M = glm::rotate(matrix.M, 1.0f, grot);
	matrix.M = glm::translate(matrix.M, gpos);
	matrix.MV = matrix.V * matrix.M;
	matrix.MVP = matrix.P * matrix.V * matrix.M;
	//vec3 light_position = vec3(-20.0f, -20.0f, 0.0f);
	//vec3 view_position = vec3(0.0f, 0.0f, 50.0f);
	//mat4 light_proj_matrix = glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 200.0f);	//frustum( left,  right,  bottom,  top,  nearVal,  farVal)
	//mat4 light_view_matrix = glm::lookAt(light_position, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
}

void objeto::setMatrix() {

}

void objeto::draw() {
	calcMatrix();
	//setMatrix();

	uint i = 0;
	if (malhas.size() >= 1) {
		for (i = 0; i < malhas.size(); i++)
			if (malhas[i])
				malhas[i]->draw(&matrix);
	}
	//printf("	*rendered %i malhas %s\n", i, name.c_str());
	for (i = 0; i < child.size(); i++)
		if (child[i]) {
			child[i]->refreshGlobal(gpos, gsize, grot);
			child[i]->draw();
		}
	//printf("	*rendered %i objetos %s\n", i, name.c_str());

}

void objeto::refreshGlobal(vec3 p, vec3 s, vec4 r) {
	gpos = lpos + p;
	gsize = lsize * s;
	grot = lrot + r;
}

