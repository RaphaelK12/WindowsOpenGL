#pragma once
class frameBuffer {
public:
	vector<GLuint> textures;
	uivec2 size;
	uint fbo;
	uint rbo;
	uint status;

	~frameBuffer();
	frameBuffer();
	frameBuffer(uint n, uivec2 sz);

	void bind();
	void resize(uivec2 sz);
	void unbind();

private:
	int gen(uint n, uivec2 sz);
};

