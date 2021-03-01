#include "pch.h"
#include "objetob.h"
#include "camera.h"
#include "frameBuffer.h"

frameBuffer::frameBuffer() :
	size(100, 100),
	status(0),
	fbo(0),
	rbo(0) {
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	textures.push_back(0);
	glGenTextures(1, &textures[0]);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0], 0);

	//glBindTexture(GL_TEXTURE_2D, textures[1]);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, size.x, size.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textures[1], 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);

	status = 1;
	GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (e != GL_FRAMEBUFFER_COMPLETE) {
		printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		status = 0;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

frameBuffer::frameBuffer(uint n, uivec2 sz) :
	status(0),
	size(sz),
	fbo(0),
	rbo(0) {
	n = clamp(n, 1u, 8u);
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	textures.resize(n);
	glGenTextures(n, &textures[0]);
	for (int i = 0; i < n; i++) {
		glBindTexture(GL_TEXTURE_2D, textures[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i , GL_TEXTURE_2D, textures[i], 0);
	}

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	status = 1;
	GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	GLenum f = GL_FRAMEBUFFER_COMPLETE;
	if (e != GL_FRAMEBUFFER_COMPLETE) {
		printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		status = 0;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void frameBuffer::resize(uivec2 sz) {
	size = sz;
	for (int i = 0; i < textures.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	}
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);

}

int frameBuffer::gen(uint n, uivec2 sz)	{
	size = sz;
	n = clamp(n, 1u, 8u);
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	textures.resize(n);
	glGenTextures(n, &textures[0]);
	for (uint i = 0; i < n; i++) {
		glBindTexture(GL_TEXTURE_2D, textures[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);
	}

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	status = 1;
	GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (e != GL_FRAMEBUFFER_COMPLETE) {
		printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		status = 0;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return e;
}

frameBuffer::~frameBuffer() {
	glDeleteRenderbuffers(1, &rbo);
	glDeleteFramebuffers(1, &fbo);
	if (textures.size())
		glDeleteTextures(textures.size(), &textures[0]);
}
//extern camera* activecamera;
void frameBuffer::bind() {
	//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	// first pass
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	activecamera->aspect = max(float(size.x), 1.0f) / max(float(size.y), 1.0f);
	activecamera->calcMatrix();
	glViewport(0, 0, size.x, size.y);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we’re not using the stencil	buffer now
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void frameBuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



