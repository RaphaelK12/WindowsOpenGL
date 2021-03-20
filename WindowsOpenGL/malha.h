#pragma once

class vertex {
public:
	vertex();
	~vertex();
	vertex(vec3 _position,	vec3 _normal,	vec3 _tangent,	vec3 _bitangent,	vec2 _uv,	byte4 _color);
	vec3 position;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	vec2 uv;
	byte4 color;
};

enum ERenderMode : uint {
	RenderModePoints,
	RenderModeLines,
	RenderModeTriangles,
};

class malha
{
public:
	malha(void);
	malha(string matName);
	~malha(void);
	void free();
	void makeSphere(uint xres, uint yres, float3 size);
	void makeSphere2(uint xres, uint yres, float3 size);
	void makeSphere3(uint xres, uint yres, float3 size);
	void makeTorus(uint xres, uint yres, float3 size, float internSize);
	void makeTorus2(uint xres, uint yres, float3 size, float internSize, float externSize);
	void makeCone(uint xres, uint yres, float3 size);
	void makeCone2(uint xres, uint yres, float3 size);
	void makeCilinder(uint xres, uint yres, float3 size);
	void makeCilinder2(uint xres, uint yres, float3 size);
	void makeBox(uint xres, uint yres, float3 size);
	void makeSkyBox(uint xres, uint yres, float3 size);
	void makeQuad(uint xres, uint yres, float3 size);
	void makeTriangle(uint xres, uint yres, float3 size);

	void makeGrid (int xres, int yres, float3 size);
	void makeAxis (uint xres, uint yres, float3 size);
	void makeAxisS(uint xres, uint yres, float3 size);


	void makeNormals();
	void makeBiTangent();
	void CreateBuffer();
	void draw(matrix_block *m);
	void setMatrix(matrix_block *m);
	int atach();
	void detach();
	ERenderMode renderMode;
	static uint g_count;	// total number of shaders actual in the program
	uint count;				// number of times the shader is used
	uint mIndex;			// index of this shader in global list of shaders

	std::string name;
	std::string fileName;

	uint nIndex;	// numero de indices, numero de faces * 3
	uint nVertex;	// numero de vertices

	uint VBO;	//Vertex buffer object
	uint EBO;	//Elemet buffer object
	uint VAO;	//Vertex array object
	GLuint pBuffers[10]= { 0,0,0,0,0,0,0,0,0,0 }; // buffers used for vertex, normal, uv, color and others (tangent, bitangent, ...)

	std::vector<usvec3> pIndex;
	std::vector<usvec2> pIndex2;
	std::vector<vertex> pVertex; // standard vertex, more commom use
	// standard atributes separated
	std::vector<vec3> pPosition;
	std::vector<vec3> pNormal;
	std::vector<vec3> pTangent;
	std::vector<vec3> pBiTangent;
	std::vector<vec2> pUv;
	std::vector<vec4> pColor;
	// optional
	std::vector<vec2> pUv1;
	std::vector<vec2> pUv2;
	std::vector<vec4> pColor1;
	std::vector<vec4> pColor2;

	material *mMaterial;	// material usado




};

extern std::vector<malha*> g_malha_list;