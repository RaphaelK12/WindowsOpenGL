#pragma once
class Word {
public:
	Word();
	~Word();
	vec4 time;
	uint frame;
	vec4 sintime;
	vec4 costime;
	cTimer timer;

	vector<objeto*> objetos;
	vector<malha*> malhas;
	vector<shader*> shaders;
	vector<Texture*> textures;
	vector<skeleton*> skeletons;
	vector<Material*> materials;
	vector<ligith*> ligiths;
	vector<texto*> textos;
	vector<camera*> cameras;
	vector<projetor*> projetors;


	uint draw();
	void addObj(objType t, vec3 Pos, vec3 Rot, vec3 Size, uint3 Res, string matName);
	void refreshShaders();
};

