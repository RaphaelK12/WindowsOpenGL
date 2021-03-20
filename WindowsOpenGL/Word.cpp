#include "pch.h"

#include "cTimer.h"
#include "texture.h"
#include "objetob.h"
#include "shader.h"
#include "material.h"
#include "projetor.h"
#include "malha.h"
#include "bone.h"
#include "skeleton.h"
#include "objeto.h"
#include "ligith.h"
#include "texto.h"
#include "camera.h"
#include "Word.h"


uint Word::draw() {
	uint count = 0;
	for (uint i = 0; i < objetos.size(); i++) {

		objetos[i]->malhas[0]->mMaterial->mShader->setUniform("time", time);
		objetos[i]->malhas[0]->mMaterial->mShader->setUniform("frame", frame);
		objetos[i]->malhas[0]->mMaterial->mShader->setUniform("sintime", sintime);
		objetos[i]->malhas[0]->mMaterial->mShader->setUniform("costime", costime);
		objetos[i]->draw();
		objetos[i]->malhas[0]->mMaterial->mShader->setUniform("time", time);
		objetos[i]->malhas[0]->mMaterial->mShader->setUniform("frame", frame);
		objetos[i]->malhas[0]->mMaterial->mShader->setUniform("sintime", sintime);
		objetos[i]->malhas[0]->mMaterial->mShader->setUniform("costime", costime);
		count++;

	}
	return count;
}
void Word::refreshShaders() {
	for (uint i = 0; i < objetos.size(); i++) {
		//objetos[i]->draw();
		objetos[i]->malhas[0]->mMaterial->read(objetos[i]->malhas[0]->mMaterial->mName);

	}
}
void Word::addObj(objType t, vec3 Pos, vec3 Rot, vec3 Size, uint3 Res, string matName) {
	objetos.push_back(new objeto(0,t, Pos, Rot, Size, Res, matName));
}


