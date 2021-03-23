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

Word::Word() : time(0), frame(0), sintime(vec4(0)), costime(vec4(0)) {};
Word::~Word(){};


uint Word::draw() {
	uint count = 0;
	for (uint i = 0; i < objetos.size(); i++) {
		if (objetos[i])
			if (objetos[i]->malhas[0])
				if (objetos[i]->malhas[0]->mMaterial)
					if (objetos[i]->malhas[0]->mMaterial->mData)
						if (objetos[i]->malhas[0]->mMaterial->mData->mShader) {
							objetos[i]->malhas[0]->mMaterial->mData->mShader->setUniform("time", time);
							objetos[i]->malhas[0]->mMaterial->mData->mShader->setUniform("frame", frame);
							objetos[i]->malhas[0]->mMaterial->mData->mShader->setUniform("sintime", sintime);
							objetos[i]->malhas[0]->mMaterial->mData->mShader->setUniform("costime", costime);
							objetos[i]->draw();
							objetos[i]->malhas[0]->mMaterial->mData->mShader->setUniform("time", time);
							objetos[i]->malhas[0]->mMaterial->mData->mShader->setUniform("frame", frame);
							objetos[i]->malhas[0]->mMaterial->mData->mShader->setUniform("sintime", sintime);
							objetos[i]->malhas[0]->mMaterial->mData->mShader->setUniform("costime", costime);
							count++;
						}
	}
	return count;
}
void Word::refreshShaders() {
	for (uint i = 0; i < g_shader_list.size(); i++) {
		//objetos[i]->draw();
		if (g_shader_list[i]->readSrcFromFilenames() > 1)
			g_shader_list[i]->generateShadersAndProgram(1);

		//g_shader_list[i]->readSrcFromFilenames();
	}
}
void Word::addObj(objType t, vec3 Pos, vec3 Rot, vec3 Size, uint3 Res, string matName) {
	objetos.push_back(new objeto(0,t, Pos, Rot, Size, Res, matName));
}


