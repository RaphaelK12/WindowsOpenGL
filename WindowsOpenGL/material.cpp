#include "pch.h"
#include "shader.h"
#include "texture.h"
#include "material.h"
#include "objetob.h"
#include "camera.h"

std::map<string, MaterialData*> g_material_list; // filename, material

MaterialData::MaterialData() :
	castShadows(1),
	receiveShadows(1),
	renderable(1),
	transparent(0),
	alphaBlendMode(0),
	colorBlendMode(0),
	cullFace(0),
	dephtTest(1),
	dephtWrite(1),
	shadingModel(1),
	lineWidth(1),

	polygonOffset(0),
	mColor1(1),			// color passed to program
	mColor2(1),			// color passed to program
	mColor3(1),			// color passed to program
	mDifuse(1),			// color passed to program
	mAmbient(0),			// color passed to program
	mEmission(0),			// color passed to program
	mTranslucenci(0),		// color passed to program
	mSadowsColor(0),		// color passed to program

	mSpecular(1),			// color passed to program
	mReflex(1),			// color passed to program
	mShinines(16),			// color passed to program

	mName("default"),
	mFileName(" "),
	mShaderName("default"),
	//mTextures({ "defaultDiff", "defaultSpec" , "defaultNor" , "defaultCube" }),	// textures objects
	mShader(0) 
{
}

//MaterialData::MaterialData(const MaterialData& m) {
//	castShadows = m.castShadows;
//	receiveShadows = m.receiveShadows;
//	renderable = m.renderable;
//	transparent = m.transparent;
//	alphaBlendMode = m.alphaBlendMode;
//	colorBlendMode = m.colorBlendMode;
//	cullFace = m.cullFace;
//	dephtTest = m.dephtTest;
//	dephtWrite = m.dephtWrite;
//	shadingModel = m.shadingModel;
//	lineWidth = m.lineWidth;
//
//	polygonOffset = m.polygonOffset;
//	mColor1 = m.mColor1;
//	mColor2 = m.mColor2;
//	mColor3 = m.mColor3;
//	mDifuse = m.mDifuse;
//	mAmbient = m.mAmbient;
//	mEmission = m.mEmission;
//	mTranslucenci = m.mTranslucenci;
//	mSadowsColor = m.mSadowsColor;
//
//	mSpecular = m.mSpecular;
//	mReflex = m.mReflex;
//	mShinines = m.mShinines;
//
//	mName = m.mName;
//	mFileName = m.mFileName;
//	mShaderName = m.mShaderName;
//	mShaderNames = m.mShaderNames;
//	mTextures = m.mTextures;
//	mShader = m.mShader;
//
//}
MaterialData::~MaterialData() {
	if (mShader)
		delete mShader;
	for (uint i = 0; i < mTextures.size(); i++) {
		if(mTextures[i])
			delete mTextures[i];
		 mTextures[i]=0;
	}
}



Material::Material(void) :
	mData(0),
	mName("default"),
	mFileName("default.json")
{
}

Material::Material(string mname) : mData(0), mName(mname), mFileName(" ") {
	mFileName = searchMaterialFileName(mname);
	if (mFileName.length() < 5) {
		mname = "default";
	}
	mFileName = searchMaterialFileName(mname);
	mData = loadMaterial(mFileName);
}

Material::~Material(void)
{
	mName.clear();
	mFileName.clear();
	if (mData && mData->RemoveReference()) 	{
		if (mFileName.length() > 4)
			g_material_list.erase(mFileName);
		delete mData;
	}
}

int Material::reload() {

	mFileName = searchMaterialFileName(mName);
	if (mFileName.length() > 4) {
		std::ifstream f;
		f.open(mFileName);
		if (f.is_open()) {
			json j = json::parse(f);
			if (mData)
				delete mData;
				mData = new MaterialData;
			from_json(j, *mData);
			//*mData = j;
			f.close();
			return 1;
		}
	}
	return 0;
	/*
	string file = filename;
	string fr = "Material";
	int i;
	if (file.size() >= 1) {
		FILE* f = fopen(filename.c_str(), "r");
		string s;
		if (!f) {
			file = filename + string(".mat");
			f = fopen(file.c_str(), "r");
		}
		if (!f)
			return 0;
		char* txt = new char[100];
		i = fscanf(f, "%s\n", txt);
		if (fr != txt) {
			fclose(f);
			delete[] txt;
			return 0;
		}
		i = fscanf(f, "i  type: 				%i\n", &type);
		i = fscanf(f, "i  filter_mag:			%i\n", &filter_mag);
		i = fscanf(f, "i  filter_min:			%i\n", &filter_min);
		i = fscanf(f, "i  wrap_s:			%i\n", &wrap_s);
		i = fscanf(f, "i  wrap_t:			%i\n", &wrap_t);
		i = fscanf(f, "i  wrap_r:			%i\n", &wrap_r);
		i = fscanf(f, "i  castShadows:			%i\n", &castShadows);
		i = fscanf(f, "i  receiveShadows:		%i\n", &receiveShadows);
		i = fscanf(f, "i  renderable:			%i\n", &renderable);
		i = fscanf(f, "i  transparent:			%i\n", &transparent);
		i = fscanf(f, "i  alphaBlendMode:		%i\n", &alphaBlendMode);
		i = fscanf(f, "i  colorBlendMode:		%i\n", &colorBlendMode);
		i = fscanf(f, "i  cullFace:				%i\n", &cullFace);
		i = fscanf(f, "i  dephtTest:			%i\n", &dephtTest);
		i = fscanf(f, "i  dephtWrite:			%i\n", &dephtWrite);
		i = fscanf(f, "i  shadingModel:			%i\n", &shadingModel);
		i = fscanf(f, "v2 polygonOffset:		%f %f\n", &polygonOffset.x, &polygonOffset.y);
		i = fscanf(f, "v4 color1:				%f %f %f %f\n", &mColor1.r, &mColor1.g, &mColor1.b, &mColor1.a);
		i = fscanf(f, "v4 color2:				%f %f %f %f\n", &mColor2.r, &mColor2.g, &mColor2.b, &mColor2.a);
		i = fscanf(f, "v4 color3:				%f %f %f %f\n", &mColor3.r, &mColor3.g, &mColor3.b, &mColor3.a);
		i = fscanf(f, "v4 diffuse:				%f %f %f %f\n", &mDifuse.r, &mDifuse.g, &mDifuse.b, &mDifuse.a);
		i = fscanf(f, "v4 ambient:				%f %f %f %f\n", &mAmbient.r, &mAmbient.g, &mAmbient.b, &mAmbient.a);
		i = fscanf(f, "v4 emission:				%f %f %f %f\n", &mEmission.r, &mEmission.g, &mEmission.b, &mEmission.a);
		i = fscanf(f, "v4 translucenci:			%f %f %f %f\n", &mTranslucenci.r, &mTranslucenci.g, &mTranslucenci.b, &mTranslucenci.a);
		i = fscanf(f, "v4 sadowsColor:			%f %f %f %f\n", &mSadowsColor.r, &mSadowsColor.g, &mSadowsColor.b, &mSadowsColor.a);
		i = fscanf(f, "v4 specular:				%f %f %f %f\n", &mSpecular.r, &mSpecular.g, &mSpecular.b, &mSpecular.a);
		i = fscanf(f, "v4 reflex:				%f %f %f %f\n", &mReflex.r, &mReflex.g, &mReflex.b, &mReflex.a);
		i = fscanf(f, "v4 shinines:				%f %f %f %f\n", &mShinines.r, &mShinines.g, &mShinines.b, &mShinines.a);
		i = fscanf(f, "s  name:					%s\n", txt);	mName = txt;
		i = fscanf(f, "s  fileName:				%s\n", txt);	mFileName = txt;
		i = fscanf(f, "s  shaderName:			%s\n", txt);	mShaderName = txt;

		i = fscanf(f, "s  textures:				%s ", txt);
		s = txt;
		texture* tx = 0;
		if (s.size() > 1) {
			texture* tx = new texture(s);
			tx->read();
			mTextures.push_back(tx);
		}
		for (int c = 0; c < 10; c++) {
			i = fscanf(f, "%s ", txt);
			s = txt;
			if (i > 0 && s.size() > 1) {
				tx = new texture(s);
				tx->read();
				mTextures.push_back(tx);
			}
			else
				break;
		}

		fclose(f);
		delete[] txt;

		if (mShader) {
			mShader->detach();
			mShader = new shader(mShaderName);
			mShader->atach();
		}
	}
	return 1;
	*/
}

void Material::setShaderVariables() {
	if (!mData)
		return;
	if (!mData->mShader)
		mData->mShader = new shader(mData->mShaderName);
	if (!mData->mShader)
		return;
	//mShader->setUniform("mt.color", mColor);
	//mShader->setInt("texture1", 0);
	mData->mShader->setUniform("mt.color1", mData->mColor1);
	mData->mShader->setUniform("mt.color2", mData->mColor2);
	mData->mShader->setUniform("mt.color3", mData->mColor3);
	mData->mShader->setUniform("mt.diffuse", mData->mDifuse);
	mData->mShader->setUniform("mt.ambient", mData->mAmbient);
	mData->mShader->setUniform("mt.emission", mData->mEmission);
	mData->mShader->setUniform("mt.translucenci", mData->mTranslucenci);
	mData->mShader->setUniform("mt.sadowsColor", mData->mSadowsColor);
	mData->mShader->setUniform("mt.specular", mData->mSpecular);
	mData->mShader->setUniform("mt.reflex", mData->mReflex);
	mData->mShader->setUniform("mt.shinines", mData->mShinines);

	for (int i = 0; uint(i) < mData->mTextures.size(); i++) {
		char txt[3] = { '0',0,0 };
		glActiveTexture(GL_TEXTURE0+i); // activate the texture unit first before binding texture
		mData->mTextures[i]->bind();
		//mData->mTextures[i]->setTextureVariables();

		txt[0] += i+1;
		string s = "texture" + string(txt);
		//glUniform1i(glGetUniformLocation(mShader->program, s.c_str()), i);
		mData->mShader->setUniform(s, i);
	}
}

int Material::active(matrix_block* m) {
	if (!mData)
		return 0;
	if (!mData->mShader) {
		mData->mShader = new shader(mData->mShaderName);
		mData->mShader->atach();
	}
	mData->mShader->use();
	setShaderVariables();
	setMatrix(m);
	return 1;
}

void Material::setMatrix(matrix_block* m) {
	if (!mData ||!mData->mShader || !m)
		return;
	mData->mShader->setUniform("mt_model", m->M);						  //	uniform mat4 mt_model;
	mData->mShader->setUniform("mt_view", m->V);						  //	uniform mat4 mt_view;
	mData->mShader->setUniform("mt_projection", m->P);					  //	uniform mat4 mt_projection;
	mData->mShader->setUniform("mt_modelView", m->MV);					  //	uniform mat4 mt_modelView;
	mData->mShader->setUniform("mt_modelViewProjection", m->MVP);		  // 	uniform mat4 mt_modelViewProjection;
	mData->mShader->setUniform("viewPos", vec3(activecamera->gpos));		  // 	uniform mat4 mt_modelViewProjection;

}


string Material::searchMaterialFileName(string & filename) const {
	static const vector<string> dirs = {
		"",
		"../",
		"material/",
		"materials/",
		"../material/",
		"../materials/",
		"../../material/",
		"../../materials/",
	};
	static const vector<string> exts = {
		"",
		".json",
		".mat",
		".mtl",
		".material",
	};
	FILE* f = 0;
	string fname = "";
	for (uint ext = 0; ext < exts.size(); ext++) {
		for (uint dir = 0; dir < exts.size(); dir++) {
			fname = dirs[dir] + filename + exts[ext];
			f = fopen(fname.c_str(), "r");
			if (f) {
				fclose(f);
				return fname;
			}
		}
	}
	return " ";
}
MaterialData* Material::getMaterial(string& name) {
	MaterialData* m =0;
	std::map<std::string, MaterialData*>::const_iterator it = g_material_list.find(name);
	if (it != g_material_list.end()) {
		m = it->second;
		m->AddReference();
	}
	return m;
}
MaterialData* Material::loadMaterial(string& name) {
	MaterialData* m = 0;
	m = getMaterial(name);
	if (m)
		return m;
	m = new MaterialData;
	g_material_list.insert(std::pair<std::string, MaterialData*>(name,m));
	if (mFileName.length() > 4) {
		std::ifstream f;
		f.open(mFileName);
		if (f.is_open()) {
			json j = json::parse(f);
			from_json(j, *m);
			//*m = j;
		}
	}
	return m;
}

void from_json(const json& j, MaterialData& v) {
	j.at("castShadows"		).get_to(v.castShadows       );
	j.at("receiveShadows"	).get_to(v.receiveShadows    );
	j.at("renderable"		).get_to(v.renderable        );
	j.at("transparent"		).get_to(v.transparent       );
	j.at("alphaBlendMode"	).get_to(v.alphaBlendMode    );
	j.at("colorBlendMode"	).get_to(v.colorBlendMode    );
	j.at("cullFace"			).get_to(v.cullFace          );
	j.at("dephtTest"		).get_to(v.dephtTest         );
	j.at("dephtWrite"		).get_to(v.dephtWrite        );
	j.at("shadingModel"		).get_to(v.shadingModel      );
	j.at("lineWidth"		).get_to(v.lineWidth		 );
	j.at("polygonOffset") [0].get_to(v.polygonOffset.x);j.at("polygonOffset")[1].get_to(v.polygonOffset .y);
	j.at("color1")		  [0].get_to(v.mColor1		.x);j.at("color1")		 [1].get_to(v.mColor1		.y);j.at("color1")		[2].get_to(v.mColor1		.z);j.at("color1")		[3].get_to(v.mColor1		.w);
	j.at("color2")		  [0].get_to(v.mColor2		.x);j.at("color2")		 [1].get_to(v.mColor2		.y);j.at("color2")		[2].get_to(v.mColor2		.z);j.at("color2")		[3].get_to(v.mColor2		.w);
	j.at("color3")		  [0].get_to(v.mColor3		.x);j.at("color3")		 [1].get_to(v.mColor3		.y);j.at("color3")		[2].get_to(v.mColor3		.z);j.at("color3")		[3].get_to(v.mColor3		.w);
	j.at("diffuse")		  [0].get_to(v.mDifuse		.x);j.at("diffuse")		 [1].get_to(v.mDifuse		.y);j.at("diffuse")		[2].get_to(v.mDifuse		.z);j.at("diffuse")		[3].get_to(v.mDifuse		.w);
	j.at("ambient")		  [0].get_to(v.mAmbient		.x);j.at("ambient")		 [1].get_to(v.mAmbient		.y);j.at("ambient")		[2].get_to(v.mAmbient		.z);j.at("ambient")		[3].get_to(v.mAmbient		.w);
	j.at("emission")	  [0].get_to(v.mEmission	.x);j.at("emission")	 [1].get_to(v.mEmission		.y);j.at("emission")	[2].get_to(v.mEmission		.z);j.at("emission")	[3].get_to(v.mEmission		.w);
	j.at("translucenci")  [0].get_to(v.mTranslucenci.x);j.at("translucenci") [1].get_to(v.mTranslucenci	.y);j.at("translucenci")[2].get_to(v.mTranslucenci	.z);j.at("translucenci")[3].get_to(v.mTranslucenci	.w);
	j.at("sadowsColor")	  [0].get_to(v.mSadowsColor	.x);j.at("sadowsColor")	 [1].get_to(v.mSadowsColor	.y);j.at("sadowsColor")	[2].get_to(v.mSadowsColor	.z);j.at("sadowsColor")	[3].get_to(v.mSadowsColor	.w);
	j.at("specular")	  [0].get_to(v.mSpecular	.x);j.at("specular")	 [1].get_to(v.mSpecular		.y);j.at("specular")	[2].get_to(v.mSpecular		.z);j.at("specular")	[3].get_to(v.mSpecular		.w);
	j.at("reflex")		  [0].get_to(v.mReflex		.x);j.at("reflex")		 [1].get_to(v.mReflex		.y);j.at("reflex")		[2].get_to(v.mReflex		.z);j.at("reflex")		[3].get_to(v.mReflex		.w);
	j.at("shinines")	  [0].get_to(v.mShinines	.x);j.at("shinines")	 [1].get_to(v.mShinines		.y);j.at("shinines")	[2].get_to(v.mShinines		.z);j.at("shinines")	[3].get_to(v.mShinines		.w);
	j.at("name")		     .get_to(v.mName		);
	j.at("fileName")	     .get_to(v.mFileName	);
	j.at("shaderName")	     .get_to(v.mShaderName	);
	v.mShader = new shader(v.mShaderName);
	for (unsigned int i = 0; i < j["shaderList"].size(); i++) {
		v.mShaderNames.push_back(j["shaderList"][i]);
	}
	for (unsigned int i = 0; i < j["textures"].size(); i++) {
		v.mTextures.push_back(new Texture( j["textures"][i]));
	}
	//for(uint i = 0; i<)
	printf("%s", "");
}
void to_json(json& j, const MaterialData& p) {

	j = json{
		{"castShadows" 		     , p.castShadows       },
		{"receiveShadows" 	     , p.receiveShadows    },
		{"renderable" 		     , p.renderable        },
		{"transparent" 		     , p.transparent       },
		{"alphaBlendMode" 	     , p.alphaBlendMode    },
		{"colorBlendMode" 	     , p.colorBlendMode    },
		{"cullFace" 		     , p.cullFace          },
		{"dephtTest" 		     , p.dephtTest         },
		{"dephtWrite" 		     , p.dephtWrite        },
		{"shadingModel" 	     , p.shadingModel      },
		{"lineWidth" 		     , p.lineWidth		   },
		{"polygonOffset" 	     , p.polygonOffset	   .x, p.polygonOffset	   .y												 },
		{"color1" 			     , p.mColor1           .x, p.mColor1           .y, p.mColor1           .x, p.mColor1           .w},
		{"color2" 			     , p.mColor2           .x, p.mColor2           .y, p.mColor2           .x, p.mColor2           .w},
		{"color3" 			     , p.mColor3           .x, p.mColor3           .y, p.mColor3           .x, p.mColor3           .w},
		{"diffuse" 			     , p.mDifuse           .x, p.mDifuse           .y, p.mDifuse           .x, p.mDifuse           .w},
		{"ambient" 			     , p.mAmbient          .x, p.mAmbient          .y, p.mAmbient          .x, p.mAmbient          .w},
		{"emission" 		     , p.mEmission         .x, p.mEmission         .y, p.mEmission         .x, p.mEmission         .w},
		{"translucenci" 	     , p.mTranslucenci     .x, p.mTranslucenci     .y, p.mTranslucenci     .x, p.mTranslucenci     .w},
		{"sadowsColor" 		     , p.mSadowsColor      .x, p.mSadowsColor      .y, p.mSadowsColor      .x, p.mSadowsColor      .w},
		{"specular" 		     , p.mSpecular         .x, p.mSpecular         .y, p.mSpecular         .x, p.mSpecular         .w},
		{"reflex" 			     , p.mReflex           .x, p.mReflex           .y, p.mReflex           .x, p.mReflex           .w},
		{"shinines" 		     , p.mShinines         .x, p.mShinines         .y, p.mShinines         .x, p.mShinines         .w},
		{"name" 			     , p.mName             },
		{"fileName" 		     , p.mFileName         },
		{"shaderName" 		     , p.mShaderName       }
	};
	for (unsigned int i = 0; i < p.mShaderNames.size(); i++) {
		j["shaderList"][i] = p.mShaderNames[i];
	}
	for (unsigned int i = 0; i < p.mTextures.size(); i++) {
		j["textures"][i] = *p.mTextures[i]->mData;
	}
}
