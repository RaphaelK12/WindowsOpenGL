#include "pch.h"
#include "texture.h"

std::vector<texture*> g_texture_list;

uint texture::g_count = 0;

texture::texture(void):
 globj(0),
 mIndex(0),
 type(0),
 res(0),
 format(0),
 dataType(0)
{
	g_count++;
	g_texture_list.push_back(this);
	mIndex = g_texture_list.size() - 1;
}

texture::texture(string &txt) :
	globj(0),
	mIndex(0),
	type(0),
	res(0),
	format(0),
	dataType(0)
{
	g_count++;
	g_texture_list.push_back(this);
	mIndex = g_texture_list.size() - 1;
	fileName = txt;
}


texture::~texture(void) {
	if (globj)
		glDeleteTextures(1, &globj);
	g_count--;
}

void texture::atach(void){

}

void texture::detach(void){

}

int texture::bind() {
	if (globj) {
		glBindTexture(GL_TEXTURE_2D, globj);
		return globj;
	}
	return 0;
}

int texture::read() {
	img_basis* im = 0;
	if (!fileName.size() > 1)
		return 0;
	im = OpenImg(fileName.c_str());
	if (!im)
		return 0;
	im->convertoToOPenGLCompatible();
	glGenTextures(1, &globj);
	glBindTexture(GL_TEXTURE_2D, globj);
	glTexImage2D(GL_TEXTURE_2D, 0, im->glInternalFormat, im->xres, im->yres, GL_FALSE, im->glFormat, im->glType, im->pixels);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//float borderColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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
}










