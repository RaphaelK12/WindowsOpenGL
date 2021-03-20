#pragma once
class texture
{
public:
	texture(void);
	texture(std::string &txt);
	~texture(void);
	void atach();
	void detach();
	int read();
	int bind();
	uint globj;
	uint mIndex;
	static uint g_count;	// total number of shaders actual in the program

	uint type;
	std::string fileName;
	usvec2 res;
	uint format;
	uint dataType;
};

