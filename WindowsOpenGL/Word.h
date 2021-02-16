#pragma once
class Word {
	uint time;
	uint frame;
	cTimer timer;

	vector<objeto*> objetos;
	vector<malha*> malhas;
	vector<shader*> shaders;
	vector<texture*> textures;
	vector<skeleton*> skeletons;
	vector<material*> materials;
	vector<ligith*> ligiths;
	vector<texto*> textos;
	vector<camera*> cameras;
	vector<projetor*> projetors;

};

