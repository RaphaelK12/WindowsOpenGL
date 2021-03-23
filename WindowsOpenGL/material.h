#pragma once

class MaterialData :public ReferenceCounter{
public:

	MaterialData();
	//MaterialData(const MaterialData& m);
	~MaterialData();


	uint castShadows;
	uint receiveShadows;
	uint renderable;
	uint transparent;
	uint alphaBlendMode;
	uint colorBlendMode;
	uint cullFace;
	uint dephtTest;
	uint dephtWrite;
	uint shadingModel;
	uint lineWidth;

	vec2 polygonOffset;
	vec4 mColor1;			// color passed to program
	vec4 mColor2;			// color passed to program
	vec4 mColor3;			// color passed to program
	vec4 mDifuse;			// color passed to program
	vec4 mAmbient;			// color passed to program
	vec4 mEmission;			// color passed to program
	vec4 mTranslucenci;		// color passed to program
	vec4 mSadowsColor;		// color passed to program

	vec4 mSpecular;			// color passed to program
	vec4 mReflex;			// color passed to program
	vec4 mShinines;			// color passed to program

	std::string mName;
	std::string mFileName;
	std::string mShaderName;
	std::vector<std::string> mShaderNames;
	std::vector<Texture*> mTextures;	// textures objects
	shader* mShader;
};

class Material
{
public:
	Material(void);
	Material(string name);
	~Material(void);
	void setShaderVariables();
	int active(matrix_block* m);
	int reload();
	void setMatrix(matrix_block* m);

	MaterialData* getMaterial(string& name);
	MaterialData* loadMaterial(string& name);
	string searchMaterialFileName(string& filename) const;
	std::string mName;
	std::string mFileName;
	MaterialData* mData;
};

extern std::map<string, MaterialData*> g_material_list;

void from_json(const json& j, MaterialData& v);
void to_json(json& j, const MaterialData& p);




