#include "pch.h"
#include "texture.h"

std::map<string, TextureData*> g_texture_list; // filename, texture

TextureData::TextureData(void) :
	mFileName(" "),
	mName(" "),
	filter_mag(GL_LINEAR),
	filter_min(GL_LINEAR_MIPMAP_LINEAR),
	wrap_s(GL_REPEAT),
	wrap_t(GL_REPEAT),
	wrap_r(GL_REPEAT),

	globj(0),
	res(0),
	format(0),
	dataType(0) {};
TextureData::~TextureData(void) {
	if (globj) 
		glDeleteTextures(1, &globj);
}


Texture::Texture(void):	 mData(0), mName("default"),mFileName(" "){

}
Texture::Texture(const json& j) : mData(0), mName("default"), mFileName("") {
	img_basis* im = 0;
	j["name"].get_to(mName);
	mFileName = searchTextureFileName(mName);
	if (mFileName.length() < 5)
		return;
	mData = getTexture(mFileName);
	if (mData)
		return;
	mData = new TextureData();
	if (!mData)
		return;
	mData->mFileName = mFileName;
	mData->mName = mName;
	g_texture_list.insert(std::pair<std::string, TextureData*>(mFileName, mData));

	from_json(j,*mData );
	im = OpenImg(mFileName.c_str());
	if (!im)
		return;
	im->convertoToOPenGLCompatible();
	glGenTextures(1, &mData->globj);
	glBindTexture(GL_TEXTURE_2D, mData->globj);
	glTexImage2D(GL_TEXTURE_2D, 0, im->glInternalFormat, im->xres, im->yres, GL_FALSE, im->glFormat, im->glType, im->pixels);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mData->wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mData->wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, mData->wrap_r);
	//int a[GL_LINEAR_MIPMAP_LINEAR];
	//float borderColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mData->filter_min);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mData->filter_mag);
	GLfloat value, max_anisotropy = 16.0f; /* don't exceed this value...*/
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &value);
	value = (value > max_anisotropy) ? max_anisotropy : value;
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, value);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	delete im;
	return;
}

Texture::Texture(string& mname) : mData(0), mName(mname), mFileName("") {
	mFileName = searchTextureFileName(mname);
	if (mFileName.length() < 5) {
		mName = "default";
	}
	mFileName = searchTextureFileName(mName);
	mData = loadTexture(mFileName);
	mData->mFileName = mFileName;
	mData->mName = mName;

}

Texture::~Texture(void) {
	//mName.clear();
	//mFileName.clear();
	if (mData)
		if (mData->RemoveReference()) {
			if (mFileName.length() > 2)
				g_texture_list.erase(mFileName);
			delete mData;
			mData = 0;;
		}
}

void Texture::setTextureVariables() {
	if (!mData)
		return;
	if (mData->wrap_s		)	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     mData->wrap_s		);	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	if (mData->wrap_t		)	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     mData->wrap_t		);	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (mData->wrap_r		)	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R,	  mData->wrap_r		);	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	if (mData->filter_min	)	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mData->filter_min	);	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	if (mData->filter_mag	)	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mData->filter_mag	);	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

int Texture::bind() {
	if(mData)
	if (mData->globj) {
		glBindTexture(GL_TEXTURE_2D, mData->globj);
		return mData->globj;
	}
	return 0;
}

int Texture::read() {
	img_basis* im = 0;
	if (mFileName.length() < 5)
		return 0;
	im = OpenImg(mFileName.c_str());
	if (!im)
		return 0;
	im->convertoToOPenGLCompatible();
	glGenTextures(1, &mData->globj);
	glBindTexture(GL_TEXTURE_2D, mData->globj);
	glTexImage2D(GL_TEXTURE_2D, 0, im->glInternalFormat, im->xres, im->yres, GL_FALSE, im->glFormat, im->glType, im->pixels);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//int a[GL_LINEAR_MIPMAP_LINEAR];
	//float borderColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLfloat value, max_anisotropy = 16.0f; /* don't exceed this value...*/
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &value);
	value = (value > max_anisotropy) ? max_anisotropy : value;
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, value);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	delete im;
	return true;
}

string Texture::searchTextureFileName(string& filename) {
	static const vector<string> dirs = {
		"",
		"../",
				"texture/",
				"textures/",
			 "../texture/",
			 "../textures/",
		  "../../texture/",
		  "../../textures/",
	};
	static const vector<string> exts = {
		"",
		".tga",
		".jpg",".jpeg",
		".png",
		".pcx",
		".pbm",".pgm",".pmm",".pnm",".ppm",
	};
	FILE* f = 0;
	string fname = " ";
	for (uint ext = 0; ext < exts.size(); ext++) {
		for (uint dir = 0; dir < dirs.size(); dir++) {
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

TextureData* Texture::getTexture(string& name) {
	TextureData* m = 0;
	std::map<std::string, TextureData*>::const_iterator it = g_texture_list.find(name);
	if (it != g_texture_list.end()) {
		m = it->second;
		m->AddReference();
	}
	return m;
}
TextureData* Texture::loadTexture(string& name) {
	TextureData* m = 0;
	m = getTexture(name);
	if (m)
		return m;

	if (mFileName.length() > 4) {
		img_basis* im = 0;
		m = new TextureData;
		if (!m)
			return 0;
		im = OpenImg(mFileName.c_str());
		if (!im) {
			delete m;
			return 0;
		}
		im->convertoToOPenGLCompatible();
		g_texture_list.insert(std::pair<std::string, TextureData*>(name, m));
		m->res.x = im->xres;
		m->res.y = im->yres;
		glGenTextures(1, &mData->globj);
		glBindTexture(GL_TEXTURE_2D, mData->globj);
		glTexImage2D(GL_TEXTURE_2D, 0, im->glInternalFormat, im->xres, im->yres, GL_FALSE, im->glFormat, im->glType, im->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		GLfloat value, max_anisotropy = 16.0f; /* don't exceed this value...*/
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &value);
		value = (value > max_anisotropy) ? max_anisotropy : value;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, value);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		delete im;

	}
	return m;
}





void to_json(json& j, const TextureData& p) {
	j = json{
		{"name"         , p.mName           },
		{"filter_mag"   , p.filter_mag      },
		{"filter_min"   , p.filter_min      },
		{"wrap_s"       , p.wrap_s          },
		{"wrap_t"       , p.wrap_t          },
		{"wrap_r"       , p.wrap_r          }
	};
}
void from_json(const json& j, TextureData& p) {
		j.at("name").get_to(p.mName);
		j.at("filter_mag").get_to(p.filter_mag);
		j.at("filter_min").get_to(p.filter_min);
		j.at("wrap_s").get_to(p.wrap_s);
		j.at("wrap_t").get_to(p.wrap_t);
		j.at("wrap_r").get_to(p.wrap_r);
}







