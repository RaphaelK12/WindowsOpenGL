#include "pch.h"
#include "shader.h"
#include "texture.h"
#include "material.h"
#include "malha.h"
#include "Utils.h"

std::vector<malha*> g_malha_list;

uint malha::g_count = 0;

vertex::vertex():
position(0,0,0),
normal(0, 0, 0),
tangent(0, 0, 0),
bitangent(0, 0, 0),
uv(0,  0),
color(0, 0, 0,0){}

vertex::vertex(vec3 _position, vec3 _normal, vec3 _tangent, vec3 _bitangent, vec2 _uv, byte4 _color):
position(_position),
normal(_normal),
tangent(_tangent),
bitangent(_bitangent),
uv(_uv),
color(_color){}

vertex::~vertex() {}




malha::malha(void):
	count(0),
	mIndex(0),
	VBO(0),
	VAO(0),
	EBO(0),
	nIndex(0),
	mMaterial(0),
	renderMode(ERenderMode::RenderModeTriangles),
	nVertex(0)/*,
	pBuffers{0,0,0,0,0,0,0,0,0,0}*/
{
	g_count++;
	g_malha_list.push_back(this);
	mIndex = g_malha_list.size() - 1;
	//mMaterial = new material("default");
	//mMaterial->atach();
	//memset(pBuffers, 0, sizeof(pBuffers));
}

malha::~malha(void)
{
	pVertex.clear();
	pIndex.clear();

	pPosition.clear();
	pNormal.clear();
	pUv.clear();
	pColor.clear();

	pTangent.clear();
	pBiTangent.clear();
	pUv1.clear();
	pUv2.clear();
	pColor1.clear();
	pColor2.clear();

	name.clear();
	fileName.clear();

	glDeleteBuffers(sizeof(pBuffers) / sizeof(pBuffers[0]), pBuffers);
	memset(pBuffers, 0, sizeof(pBuffers));

	if (VBO)
		glDeleteBuffers(1, &VBO);
	if (EBO)
		glDeleteBuffers(1, &EBO);
	if (VAO)
		glDeleteVertexArrays(1, &VAO);
	if (mMaterial)
		delete mMaterial;

	nVertex = 0;
	nIndex = 0;
	VBO = 0;
	EBO = 0;
	VAO = 0;
	mMaterial = NULL;

	if (g_malha_list.size() >= mIndex)
		g_malha_list[mIndex] = 0;
	g_count--;
}

void malha::free(void)
{
	pVertex.clear();
	pIndex.clear();

	pPosition.clear();
	pNormal.clear();
	pUv.clear();
	pColor.clear();
	
	pTangent.clear();
	pBiTangent.clear();
	pUv1.clear();
	pUv2.clear();
	pColor1.clear();
	pColor2.clear();
	
	name.clear();
	fileName.clear();

	glDeleteBuffers(sizeof(pBuffers)/sizeof(pBuffers[0]), pBuffers);
	memset(pBuffers, 0, sizeof(pBuffers));

	if (VBO)
		glDeleteBuffers(1, &VBO);
	if (EBO)
		glDeleteBuffers(1, &EBO);
	if (VAO)
		glDeleteVertexArrays(1, &VAO);
	if(mMaterial)
		delete mMaterial;

	nVertex=0;
	nIndex=0;
	VBO = 0;
	EBO = 0;
	VAO = 0;
	renderMode = ERenderMode::RenderModeTriangles;
	mMaterial=NULL;
}

//	Maker basics
void malha::makeTorus2(uint xres, uint yres, float3 size, float internSize){
	// refazer para adicionar um plano distorcido em torus para melhor mapeamento uv, 
	// precisa recalcular normais das bordas para evitar recortes.
	// fazer o mesmo com esfera.
	free();
	xres = max(xres,3);
	yres = max(yres,3);
	nVertex = xres*yres;
	nIndex = xres * yres * 2;

	pVertex.resize(nVertex);
	pIndex.resize(nIndex);

	float vx = F_PI/xres*2.0f, vy = F_PI/yres*2.0f;
	float px=0,py=0;
	// attributes
	uint p = 0, y = 0, x = 0;
	float	xb,// big circle x
			yb,// big circle y
			xs,// small circle x
			z;// small circle z: nedded because the size(/2)
	for (y = 0; y < yres; y++) {
		py = y * vy;
			z = cos(py) * size.z / 2;		// small circle z: nedded because the size(/2)
		for (x = 0; x < xres; x++) {
			px = x * vx;

			xb = sin(px) * size.x / 2;		// big circle x
			yb = cos(px) * size.y / 2;		// big circle y
			xs = internSize + sin(py) * size.x;		// small circle x
			//ys = cos(py) * size.y,		// small circle y
			pVertex[p].position = vec3(xb + (xs * xb), yb + (xs * yb), z);
			p++;
		}
	}


	//center
	p=0;
	y=0;
	x=y+xres;
	uint sx=x, sy=0;
	for(uint a=0 ; a < yres-1; a++){
		uint sx=x, sy=y;
		for(uint b=0 ; b < xres-1; b++, x++, y++){
			pIndex[p] = usvec3(y, x+1, x);
			p++;
			pIndex[p] = usvec3(y, y+1, x+1);
			p++;
		}
		pIndex[p] = usvec3(y, sx, x);
		p++;
		pIndex[p] = usvec3(y, sy, y+1);
		//pIndex[p] = usvec3(y, sx, x+1);
		//p++;
		//pIndex[p] = usvec3(y, sy-1, y+1);
		p++;
		x++;
		y++;
	}

	// booton
	for(x =0, y = nVertex - xres; y < nVertex && p<nIndex;){
		if(x>=xres)
			//pIndex[p] = usvec3(y-yres, x, x+1);
			pIndex[p] = usvec3(0, 0, 0);
		else
			pIndex[p] = usvec3(y, x+1, x);
		//pIndex[p] = usvec3(nVertex-1, y, y-1);
		p++;

		if(x>=xres)
			pIndex[p] = usvec3(0, 0, 0);
			//pIndex[p] = usvec3(y, y+1, x-xres);
		else
			pIndex[p] = usvec3(y, y+1, x+1);
		//pIndex[p] = usvec3(nVertex-1, y+1, y-1);
		p++;
		y++;
		x++;
	}

	pIndex[nIndex-2] = usvec3(xres-1, nVertex-1, 0);
	pIndex[nIndex-1] = usvec3(nVertex-xres, 0, nVertex-1);

	vec3 faceNormal;
	for(p = 0; p < nIndex; p++){
		faceNormal = calcNormal(pVertex[pIndex[p].x].position, pVertex[pIndex[p].y].position, pVertex[pIndex[p].z].position);
		pVertex[pIndex[p].x].normal += faceNormal;
		pVertex[pIndex[p].y].normal += faceNormal;
		pVertex[pIndex[p].z].normal += faceNormal;
	}
	for (p = 0; p < nVertex; p++){
		pVertex[p].normal = normalize(pVertex[p].normal);
	}
	for (p = 0; p < nVertex; p++){
		pVertex[p].uv = vec2(pVertex[p].position.x, pVertex[p].position.y+pVertex[p].position.z);
	}
	p=0;


	name = "Torus 2";

}

void malha::makeTorus(uint xres, uint yres, float3 size, float internSize, float externSize) {
	free();
	xres = min(max(xres, 3), 255);
	yres = min(max(yres, 3), 250);
	nVertex = (xres + 1) * (yres + 1);
	nIndex = (xres) * (yres ) * 2;

	pVertex.resize(nVertex);
	pIndex.resize(nIndex);

	float vx = F_PI / xres * 2.0f, vy = F_PI / yres * 2.0f;
	float px = 0, py = 0;
	// attributes
	uint p = 0, y = 0, x = 0;
	vec3 pos((externSize - internSize) / 2, 0, 0); // first vertex 
	vec3 pos2;
	vec3 pos3;
	for (y = 0; y <= yres; y++) {
		py = y * vy; // linear value angle
		pos2 = rotateYRad(pos, py); // rotate first vertex into y angle to make a circle
		pos2+=vec3((externSize - internSize) / 2 + internSize / 2, 0, 0); // move the circle in y direction
		for (x = 0; x <= xres; x++) {
			px = x * vx; // linear angle value
			pos3 = rotateZRad(pos2, px); // rotate the circle in z angle to make this circle turn a torus
			pVertex[p].position = pos3 * (size / 2.f);
			pVertex[p].uv = vec2(1 - x / float(xres) * 16.f, y / float(yres) * 4.f);
			p++;
		}
	}

	p = 0;
	x = 0;
	y = xres+1;
	for (uint a = 0; a < yres; a++) {
		for (uint b = 0; b < xres; b++, x++, y++) {
			pIndex[p] = usvec3(x, y,     x + 1);
			p++;
			pIndex[p] = usvec3(y, y + 1, x + 1);
			p++;
		}
		x++;
		y++;
	}

	vec3 faceNormal;
	for (p = 0; p < nIndex; p++) {
		faceNormal = faceNormal2(pVertex[pIndex[p].x].position, pVertex[pIndex[p].y].position, pVertex[pIndex[p].z].position);
		pVertex[pIndex[p].x].normal += faceNormal;
		pVertex[pIndex[p].y].normal += faceNormal;
		pVertex[pIndex[p].z].normal += faceNormal;
	}
	for (p = 0; p <= nVertex-xres; p+=xres+1) {
		faceNormal = pVertex[p].normal;
		faceNormal += pVertex[p+xres].normal;
		pVertex[p].normal = faceNormal;
		pVertex[p+xres].normal = faceNormal;
	}
	for (p = 0; p <= xres; p++) {
		faceNormal = pVertex[p].normal;
		faceNormal += pVertex[p+ nVertex - xres-1].normal;
		pVertex[p].normal = faceNormal;
		pVertex[p + nVertex - xres-1].normal = faceNormal;
	}
	for (p = 0; p < nVertex; p++) {
		pVertex[p].normal = normalize(pVertex[p].normal);
	}
	p = 0;
	makeBiTangent();
	name = "Torus";
}

void malha::makeSphere2(uint xres, uint yres, float3 size) {
	xres = min(max(abs(int(xres)), 3), 255);
	yres = min(max(abs(int(yres)) - 2, 1), 256);
	nVertex = xres * yres + 2;
	nIndex = xres * yres * 2;

	pVertex.resize(nVertex);
	//pNormal.resize(nVertex);
	pIndex.resize(nIndex);

	//vec3* v = new vec3[nVertex];
	//vec3* n = new vec3[nVertex];
	//usvec3* i = new usvec3[nIndex];

	float vx = F_PI / (xres + 0) * 2, vy = F_PI / (yres + 1);
	float px = 0, py = 0, pz = 0, spy = 0;
	uint p = 1, x, y;

	// vextex positions
	// top
	pVertex[0].position = vec3(0.0f, 0.0f, size.z);
	pVertex[0].normal = vec3(0.0f, 0.0f, 1.0f);
	// center
	vec3 vec;
	for (y = 1; y <= yres; y++) {
		for (x = 0; x < xres; x++) {
			px = x * vx; py = y * vy;
			spy = sin(py);
			vec = vec3(sin(px) * sin(py), cos(px) * sin(py), cos(py));

			pVertex[p].position = vec * size;
			//n[p] = vec;
			pVertex[p].normal = normalize(vec);
			//v[p] = vec3 (sin(px)*sin(py)*size.x, cos(py)*size.y, cos(px)*sin(py)*size.z);
			p++;
		}
	}
	// booton
	pVertex[p].position = vec3(0.0f, 0.0f, -size.z);
	pVertex[p].normal = vec3(0.0f, 0.0f, -1.0f);


	// indexes
	// top
	p = 0;
	for (y = 1; y < xres; y++) {
		pIndex[p] = usvec3(0, y + 1, y);
		p++;
	}
	pIndex[p] = usvec3(0, 1, y);
	p++;

	//center
	y = 1;
	x = y + xres;
	uint sx = x, sy = 0;
	for (uint a = 0; a < yres - 1; a++) {
		uint sx = x, sy = y;
		for (uint b = 0; b < xres - 1; b++, x++, y++) {
			pIndex[p] = usvec3(x, y, x + 1);
			p++;
			pIndex[p] = usvec3(x + 1, y, y + 1);
			p++;
		}
		pIndex[p] = usvec3(x, y, sx);
		p++;
		pIndex[p] = usvec3(y + 1, y, sy);
		//i[p] = usvec3(y, sx, x+1);
		//p++;
		//i[p] = usvec3(y, sy-1, y+1);

		p++;
		x++;
		y++;
	}

	// booton
	for (y = nVertex - xres; y < nVertex - 1; y++) {
		pIndex[p] = usvec3(nVertex - 1, y - 1, y);
		p++;
	}
	pIndex[p] = usvec3(nVertex - 1, y - 1, nVertex - xres - 1);

	for (p = 0; p < nVertex; p++) {
		pVertex[p].uv = vec2(pVertex[p].position.x, pVertex[p].position.y + pVertex[p].position.z);
	}
	p = 0;
	name = "Esfera 2";
}

void malha::makeSphere(uint xres, uint yres, float3 size) {
	free();
	xres = min(max(xres, 3), 255);
	yres = min(max(yres, 3), 250);
	nVertex = (xres + 1) * (yres );
	nIndex = (xres) * (yres-2 ) *2;

	//nVertex = (xres + 1) * (yres + 1);
	//nIndex = (xres) * (yres) * 2;

	pVertex.resize(nVertex);
	pIndex.resize(nIndex);

	float vx = F_PI / xres * 2, vy = F_PI / (yres - 1);
	float px = 0, py = 0;
	// attributes
	uint p = 0, y = 0, x = 0;
	vec3 pos(0, 0, 1); // first vertex 
	vec3 pos2;
	vec3 pos3;
	//pVertex[p].position = vec3(0, 0, 1.0f) * (size / 2.f);
	//pVertex[p].normal = vec3(0, 0, 1.0f);
	//pVertex[p].uv = vec2(2.f, 1.5f);
	//pVertex[nVertex - 1].position = vec3(0, 0, -1.0f) * (size / 2.f);
	//pVertex[nVertex - 1].normal = vec3(0, 0, -1.0f);
	//pVertex[nVertex - 1].uv = vec2(vx*xres/2, vy * xres / 2);
	//p++;
	for (y = 0; y < yres ; y++) {
		py = y * vy; // linear value angle
		pos2 = rotateYRad(pos, py / 1.0f); // rotate first vertex into y angle to make a circle
		for (x = 0; x <= xres; x++) {
			px = x * vx; // linear angle value
			pos3 = rotateZRad(pos2, px / 1.0f); // rotate the circle in z angle to make this circle turn a torus
			pVertex[p].position = pos3 * (size / 2.f);
			pVertex[p].uv = vec2(1.5f - x / float(xres) * 4.f, y / float(yres) * 3.f);
			p++;
		}
	}

	p = 0;
	x = 0;
	y = xres + 1;

	for (uint b = 0; b < xres; b++) {
		pIndex[p] = usvec3(y, y + 1, x);
		//pIndex[p] = usvec3(x, y, x + 1);
		p++;
		x++;
		y++;
	}
	x++;
	y++;

	for (uint a = 0; a < yres - 3; a++) {
		for (uint b = 0; b < xres; b++, x++, y++) {
			pIndex[p] = usvec3(x, y, x + 1);
			p++;
			pIndex[p] = usvec3(y, y + 1, x + 1);
			p++;
		}
		x++;
		y++;
	}
	for (uint b = 0; b < xres; b++) {
		//pIndex[p] = usvec3(y, y + 1, x);
		pIndex[p] = usvec3(x, y, x + 1);
		p++;
		x++;
		y++;
	}
		x++;
		y++;

	vec3 faceNormal;
	vec3 faceTangent;
	vec3 faceBiTangent;
	makeBiTangent();

	for (p = 0; p < nIndex; p++) {
		faceNormal = calcNormal(pVertex[pIndex[p].x].position, pVertex[pIndex[p].y].position, pVertex[pIndex[p].z].position);
		pVertex[pIndex[p].x].normal += faceNormal;
		pVertex[pIndex[p].y].normal += faceNormal;
		pVertex[pIndex[p].z].normal += faceNormal;
	}
	for (p = xres+1; p < (nVertex - xres)-2; p += xres + 1) {
		faceNormal = pVertex[p].normal;
		faceNormal += pVertex[p + xres].normal;
		faceTangent = pVertex[p].tangent;
		faceTangent += pVertex[p + xres].tangent;
		faceBiTangent = pVertex[p].bitangent;
		faceBiTangent += pVertex[p + xres].bitangent;
		pVertex[p + xres].normal = faceNormal;
		pVertex[p].normal = faceNormal;
		pVertex[p + xres].tangent = faceTangent;
		pVertex[p].tangent = faceTangent;
		pVertex[p + xres].bitangent = faceBiTangent;
		pVertex[p].bitangent = faceBiTangent;
	}
	faceNormal = vec3(0, 0, 1);
	for (p = 0; p <  xres; p ++) {
		pVertex[p].normal = faceNormal;
	}
	faceNormal = vec3(0, 0, -1);
	for (p = nVertex - xres-1; p < nVertex ; p ++) {
		pVertex[p].normal = faceNormal;
	}
	faceTangent=vec3(0);
	faceBiTangent = vec3(0);

	for (p = 0; p <  xres; p ++) {
		faceTangent += pVertex[p].tangent;
		faceBiTangent += pVertex[p].bitangent;
	}
	for (p = nVertex - xres-1; p < nVertex ; p ++) {
		faceTangent += pVertex[p].tangent;
		faceBiTangent += pVertex[p].bitangent;
	}

	for (p = 0; p <  xres; p ++) {
		pVertex[p].tangent = faceTangent;
		pVertex[p].bitangent = faceBiTangent;
	}
	for (p = nVertex - xres-1; p < nVertex ; p ++) {
		pVertex[p].tangent = faceTangent;
		pVertex[p].bitangent = faceBiTangent;
	}

	for (p = 0; p < nVertex; p++) {
		//pVertex[p].normal = normalize(pVertex[p].position);
		pVertex[p].normal = normalize(pVertex[p].normal);
		pVertex[p].tangent = normalize(pVertex[p].tangent);
		pVertex[p].bitangent = normalize(pVertex[p].bitangent);
	}
	p = 0;
	name = "Esfera";
}

void malha::makeSphere3(uint xres, uint yres, float3 size) {
	free();
	xres = min(max(xres, 3), 255);
	yres = min(max(yres, 3), 250);
	nVertex = (xres + 1) * (yres);
	nIndex = (xres) * (yres - 2) * 2;

	//nVertex = (xres + 1) * (yres + 1);
	//nIndex = (xres) * (yres) * 2;

	pVertex.resize(nVertex);
	pIndex.resize(nIndex);

	float vx = F_PI / xres * 2, vy = F_PI / (yres - 1);
	float px = 0, py = 0;
	// attributes
	uint p = 0, y = 0, x = 0;
	vec3 pos(0, 0, 1); // first vertex 
	vec3 pos2;
	vec3 pos3;
	//pVertex[p].position = vec3(0, 0, 1.0f) * (size / 2.f);
	//pVertex[p].normal = vec3(0, 0, 1.0f);
	//pVertex[p].uv = vec2(2.f, 1.5f);
	//pVertex[nVertex - 1].position = vec3(0, 0, -1.0f) * (size / 2.f);
	//pVertex[nVertex - 1].normal = vec3(0, 0, -1.0f);
	//pVertex[nVertex - 1].uv = vec2(vx*xres/2, vy * xres / 2);
	//p++;
	for (y = 0; y < yres; y++) {
		py = y * vy; // linear value angle
		pos2 = rotateYRad(pos, py / 1.0f); // rotate first vertex into y angle to make a circle
		for (x = 0; x <= xres; x++) {
			px = x * vx; // linear angle value
			pos3 = rotateZRad(pos2, px / 1.0f); // rotate the circle in z angle to make this circle turn a torus
			pVertex[p].position = pos3 * (size / 2.f);
			pVertex[p].uv = vec2(1.5f - x / float(xres) * 4.f, y / float(yres) * 3.f);
			p++;
		}
	}

	p = 0;
	x = 0;
	y = xres + 1;

	for (uint b = 0; b < xres; b++) {
		pIndex[p] = usvec3(x, y + 1, y);
		//pIndex[p] = usvec3(x, y, x + 1);
		p++;
		x++;
		y++;
	}
	x++;
	y++;

	for (uint a = 0; a < yres - 3; a++) {
		for (uint b = 0; b < xres; b++, x++, y++) {
			pIndex[p] = usvec3(x, x + 1, y);
			p++;
			pIndex[p] = usvec3(y, x + 1, y + 1);
			p++;
		}
		x++;
		y++;
	}
	for (uint b = 0; b < xres; b++) {
		//pIndex[p] = usvec3(y, y + 1, x);
		pIndex[p] = usvec3(x, x + 1, y);
		p++;
		x++;
		y++;
	}
	x++;
	y++;

	vec3 faceNormal;
	vec3 faceTangent;
	vec3 faceBiTangent;
	makeBiTangent();

	for (p = 0; p < nIndex; p++) {
		faceNormal = faceNormal2(pVertex[pIndex[p].x].position, pVertex[pIndex[p].y].position, pVertex[pIndex[p].z].position);
		pVertex[pIndex[p].x].normal += faceNormal;
		pVertex[pIndex[p].y].normal += faceNormal;
		pVertex[pIndex[p].z].normal += faceNormal;
	}
	for (p = xres + 1; p < (nVertex - xres) - 2; p += xres + 1) {
		faceNormal = pVertex[p].normal;
		faceNormal += pVertex[p + xres].normal;
		faceTangent = pVertex[p].tangent;
		faceTangent += pVertex[p + xres].tangent;
		faceBiTangent = pVertex[p].bitangent;
		faceBiTangent += pVertex[p + xres].bitangent;
		pVertex[p + xres].normal = faceNormal;
		pVertex[p].normal = faceNormal;
		pVertex[p + xres].tangent = faceTangent;
		pVertex[p].tangent = faceTangent;
		pVertex[p + xres].bitangent = faceBiTangent;
		pVertex[p].bitangent = faceBiTangent;
	}
	faceNormal = vec3(0, 0, 1);
	for (p = 0; p < xres; p++) {
		pVertex[p].normal = faceNormal;
	}
	faceNormal = vec3(0, 0, -1);
	for (p = nVertex - xres - 1; p < nVertex; p++) {
		pVertex[p].normal = faceNormal;
	}
	faceTangent = vec3(0);
	faceBiTangent = vec3(0);

	for (p = 0; p < xres; p++) {
		faceTangent += pVertex[p].tangent;
		faceBiTangent += pVertex[p].bitangent;
	}
	for (p = nVertex - xres - 1; p < nVertex; p++) {
		faceTangent += pVertex[p].tangent;
		faceBiTangent += pVertex[p].bitangent;
	}

	for (p = 0; p < xres; p++) {
		pVertex[p].tangent = faceTangent;
		pVertex[p].bitangent = faceBiTangent;
	}
	for (p = nVertex - xres - 1; p < nVertex; p++) {
		pVertex[p].tangent = faceTangent;
		pVertex[p].bitangent = faceBiTangent;
	}

	for (p = 0; p < nVertex; p++) {
		//pVertex[p].normal = normalize(pVertex[p].position);
		pVertex[p].normal = normalize(pVertex[p].normal);
		pVertex[p].tangent = normalize(pVertex[p].tangent);
		pVertex[p].bitangent = normalize(pVertex[p].bitangent);
	}
	p = 0;
	name = "Esfera 3";
}

void malha::makeCone2(uint xres, uint yres, float3 size) {
	// refazer para adicionar um plano distorcido em torus para melhor mapeamento uv, 
	// precisa recalcular normais das bordas para evitar recortes.
	// fazer o mesmo com esfera.
	free();
	xres = max(xres, 3);
	yres = 1;
	nVertex = xres * 3 + 1;
	nIndex = xres * 4 * 2;

	pVertex.resize(nVertex);
	pIndex.resize(nIndex);

	float vx = F_PI / (xres + 0) * 2, vy = F_PI / (yres + 1);
	float px = 0, py = 0, pz = 0, spy = 0;
	uint p = 0, x, y;

	// vextex positions
	// top
	vec3 vec;
	// center
	y = 1;
	{
		for (x = 0; x < xres; x++) {
			vec = vec3(0,0, 1.0f);
			pVertex[p].position = vec * size;
			p++;
		}
	}

	y = 1;
	{
		for (x = 0; x < xres; x++) {
			px = x * vx; py = y * vy;
			spy = sin(py);
			vec = vec3(sin(px) * spy, cos(px) * sin(py), 0.0f);
			pVertex[p].position = vec * size;
			p++;
		}
	}
	// booton
	y = 1;
	{
		for (x = 0; x < xres; x++) {
			px = x * vx; py = y * vy;
			spy = sin(py);
			vec = vec3(sin(px) * spy, cos(px) * sin(py), 0.0f);
			pVertex[p].position = vec * size;
			p++;
		}
	}
	pVertex[p].position = vec3(0.0f, 0.0f, 0.0f);


	// indexes
	// top
	// center
	p = 0;
	y = 0;
	x = y + xres;
	uint sx = x, sy = 0;
	{
		uint sx = x, sy = y;
		for (uint b = 0; b < xres - 1; b++, x++, y++) {
			pIndex[p] = usvec3(y, x + 1, x);
			p++;
			p++;
		}
		pIndex[p] = usvec3(y, x    , sx);
		p++;
		pIndex[p] = usvec3(y, y + 1, sy);
		p++;
		x++;
		y++;
	}


	// booton
	for (y = nVertex - xres; y < nVertex - 1; y++) {
		pIndex[p] = usvec3(nVertex - 1, y, y - 1);
		p++;
	}
	pIndex[p] = usvec3(nVertex - 1, nVertex - xres - 1, y - 1);


	vec3 faceNormal;
	for (p = 0; p < nIndex; p++) {
		faceNormal = calcNormal(pVertex[pIndex[p].x].position, pVertex[pIndex[p].y].position, pVertex[pIndex[p].z].position);
		pVertex[pIndex[p].x].normal += faceNormal;
		pVertex[pIndex[p].y].normal += faceNormal;
		pVertex[pIndex[p].z].normal += faceNormal;
	}
	for (p = 0; p < nVertex; p++) {
		pVertex[p].normal = normalize(pVertex[p].normal);
	}
	for (p = 0; p < nVertex; p++) {
		pVertex[p].uv = vec2(pVertex[p].position.x, pVertex[p].position.y + pVertex[p].position.z);
	}
	p = 0;

	name = "Cone 2";
}

void malha::makeCone(uint xres, uint yres, float3 size) {
	free();
	xres = min(max(abs(int(xres)), 3), 255);
	yres = 1;
	nVertex = (xres+1) * 4 + 1;
	nIndex = (xres+1) * 2 + (xres + 1) * 4;
	nVertex = (xres+1) * 3 + 1;
	nIndex = (xres+1) * 2 ;

	pVertex.resize(nVertex);
	//pNormal.resize(nVertex);
	pIndex.resize(nIndex);

	//vec3* v = new vec3[nVertex];
	//vec3* n = new vec3[nVertex];
	//usvec3* i = new usvec3[nIndex];

	float vx = F_PI / (xres + 0) * 2, vy = F_PI / (yres + 1);
	float px = 0, py = 0, pz = 0, spy = 0;
	uint p = 0, x, y;

	// vextex positions
	// top
	for (x = 0; x <= xres; x++) {
		pVertex[p].position = vec3(0, 0, 1) * (size / 2.f);
		pVertex[p].uv = vec3(0.5);
		p++;
	}
	// center
	y = 1;
	vec3 pos = mix(vec3(0, 0, 1) * (size / 2.f), vec3(0, 1, 0)* (size / 2.f), 0.5f); // first vertex 
	//for (x = 0; x <= xres; x++) {
	//	px = x * vx; // linear angle value
	//	vec3 pos2 = rotateZRad(pos, px / 1.0f); // rotate the circle in z angle to make this circle turn a torus
	//	pVertex[p].position = pos2 ;
	//	p++;
	//}
	pos = vec3(0, 1, 0) ; // first vertex 
	for (x = 0; x <= xres; x++) {
		px = x * vx; // linear angle value
		vec3 pos2 = rotateZRad(pos, px / 1.0f); // rotate the circle in z angle to make this circle turn a torus
		pVertex[p].position = pos2* (size / 2.f) ;
		pVertex[p].uv = vec2(pos2.x,-pos2.y)/2+0.5f ;
		p++;
	}
	pos = vec3(0, 1, 0) ; // first vertex 
	for (x = 0; x <= xres; x++) {
		px = x * vx; // linear angle value
		vec3 pos2 = rotateZRad(pos, px / 1.0f); // rotate the circle in z angle to make this circle turn a torus
		pVertex[p].position = pos2 * (size / 2.f);
		pVertex[p].uv = vec2(pos2.x, pos2.y)/2+0.5f;
		p++;
	}

	// booton
	pVertex[p].position = vec3(0.0f, 0.0f, 0.f) ;
	pVertex[p].normal = vec3(0.0f, 0.0f, -1.0f);
	pVertex[p].uv = vec2(0.5f);


	// indexes
	// top
	p = 0;
	for (y = 0; y <= xres; y++) {
		pIndex[p] = usvec3(y, y+xres, y + xres + 1);
		p++;
	}
	x = y;
	y = y+xres+1;

	//for (uint b = 0; b <= xres; b++, x++, y++) {
	//	pIndex[p] = usvec3(x, y, x + 1);
	//	p++;
	//	pIndex[p] = usvec3(y, y + 1, x + 1);
	//	p++;
	//}



	// booton
	for (y = nVertex - xres; y < nVertex - 1; y++) {
		pIndex[p] = usvec3(nVertex - 1, y, y - 1);
		p++;
	}
	pIndex[p] = usvec3(nVertex - 1, nVertex - xres - 1, y - 1);

	vec3 faceNormal;
	for (p = 0; p < nIndex; p++) {
		faceNormal = faceNormal2(pVertex[pIndex[p].x].position, pVertex[pIndex[p].y].position, pVertex[pIndex[p].z].position);
		pVertex[pIndex[p].x].normal += faceNormal;
		pVertex[pIndex[p].y].normal += faceNormal;
		pVertex[pIndex[p].z].normal += faceNormal;
	}
	faceNormal = pVertex[xres + 1].normal;
	faceNormal += pVertex[xres + xres + 1].normal;
	pVertex[xres + 1].normal = faceNormal;
	pVertex[xres + xres + 1].normal = faceNormal;
	for (p = 0; p < nVertex; p++) {
		pVertex[p].normal = normalize(pVertex[p].normal);
	}

	makeBiTangent();
	p = 0;
	name = "Cone";
}

void malha::makeCilinder2(uint xres, uint yres, float3 size) {
	// refazer para adicionar um plano distorcido em torus para melhor mapeamento uv, 
	// precisa recalcular normais das bordas para evitar recortes.
	// fazer o mesmo com esfera.
	free();
	xres = max(xres, 3);
	yres = 1;
	nVertex = xres * 4 + 2;
	nIndex = xres * 4 * 2;

	pVertex.resize(nVertex);
	pIndex.resize(nIndex);

	float vx = F_PI / (xres + 0) * 2, vy = F_PI / (yres + 1);
	float px = 0, py = 0, pz = 0, spy = 0;
	uint p = 1, x, y;

	// vextex positions
	// top
	pVertex[0].position = vec3(0.0f, 0.0f, 1.0f)*size;
	//pVertex[0].normal = vec3(0.0f, 0.0f, 1.0f);
	vec3 vec;
	y = 1;
	{
		for (x = 0; x < xres; x++) {
			px = x * vx; py = y * vy;
			spy = sin(py);
			vec = vec3(sin(px) * spy, cos(px) * sin(py), 1.0f);
			pVertex[p].position = vec * size;
			p++;
		}
	}
	// center
	y = 1;
	{
		for (x = 0; x < xres; x++) {
			px = x * vx; py = y * vy;
			spy = sin(py);
			vec = vec3(sin(px) * spy, cos(px) * sin(py), 1.0f);
			pVertex[p].position = vec * size;
			p++;
		}
	}

	y = 1;
	{
		for (x = 0; x < xres; x++) {
			px = x * vx; py = y * vy;
			spy = sin(py);
			vec = vec3(sin(px) * spy, cos(px) * sin(py), -1.0f);
			pVertex[p].position = vec * size;
			p++;
		}
	}
	// booton
	y = 1;
	{
		for (x = 0; x < xres; x++) {
			px = x * vx; py = y * vy;
			spy = sin(py);
			vec = vec3(sin(px) * spy, cos(px) * sin(py), -1.0f);
			pVertex[p].position = vec * size;
			p++;
		}
	}
	pVertex[p].position = vec3(0.0f, 0.0f, -1.0f)* size;
	//pVertex[p].normal = vec3(0.0f, 0.0f, -1.0f);


// indexes
// top
	p = 0;
	for (y = 1; y < xres; y++) {
		pIndex[p] = usvec3(0, y + 1, y);
		p++;
	}
	pIndex[p] = usvec3(0, 1, y);
	p++;

	//center
	//p = 0;
	y = 1 + xres;
	x = y + xres;
	uint sx = x, sy = 0;
	{
		uint sx = x, sy = y;
		for (uint b = 0; b < xres - 1; b++, x++, y++) {
			pIndex[p] = usvec3(y, x + 1, x);
			p++;
			pIndex[p] = usvec3(y, y + 1, x + 1);
			p++;
		}
		pIndex[p] = usvec3(y, sx, x);
		p++;
		pIndex[p] = usvec3(y, sy, y + 1);
		//pIndex[p] = usvec3(y, sx, x+1);
		//p++;
		//pIndex[p] = usvec3(y, sy-1, y+1);
		p++;
		x++;
		y++;
	}


	// booton
	for (y = nVertex - xres; y < nVertex - 1; y++) {
		pIndex[p] = usvec3(nVertex - 1, y - 1, y);
		p++;
	}
	pIndex[p] = usvec3(nVertex - 1, y - 1, nVertex - xres - 1);


	vec3 faceNormal;
	for (p = 0; p < nIndex; p++) {
		faceNormal = calcNormal(pVertex[pIndex[p].x].position, pVertex[pIndex[p].y].position, pVertex[pIndex[p].z].position);
		pVertex[pIndex[p].x].normal += faceNormal;
		pVertex[pIndex[p].y].normal += faceNormal;
		pVertex[pIndex[p].z].normal += faceNormal;
	}
	for (p = 0; p < nVertex; p++) {
		pVertex[p].normal = normalize(pVertex[p].normal);
	}
	for (p = 0; p < nVertex; p++) {
		pVertex[p].uv = vec2(pVertex[p].position.x, pVertex[p].position.y + pVertex[p].position.z);
	}
	p = 0;


	name = "Cilinder 2";

}

void malha::makeCilinder(uint xres, uint yres, float3 size) {
	// refazer para adicionar um plano distorcido em torus para melhor mapeamento uv, 
	// precisa recalcular normais das bordas para evitar recortes.
	// fazer o mesmo com esfera.
	free();
	xres = max(xres, 3);
	yres = 1;
	nVertex = (xres + 1) * 2 + xres * 2 + 2;
	nIndex = xres * 4 + 2;

	pVertex.resize(nVertex);
	pIndex.resize(nIndex);

	float vx = F_PI / (xres + 0) * 2, vy = F_PI / (yres + 1);
	float px = 0, py = 0, pz = 0, spy = 0;
	uint p = 0, x, y;

	// vextex positions
	// top
	pVertex[0].position = vec3(0.0f, 0.0f, 1.0f) * size;
	pVertex[0].uv = vec2(0.5);
	//pVertex[0].normal = vec3(0.0f, 0.0f, 1.0f);
	p++;
	vec3 vec;
	y = 1;
	for (x = 0; x < xres; x++) {
		px = x * vx;
		vec = vec3(sin(px), cos(px), 1.0f);
		pVertex[p].position = vec * size;
		pVertex[p].uv = vec2(vec.x, -vec.y)/2+vec2(0.5);
		p++;
	}
	// center
	y = 1;
	for (x = 0; x <= xres; x++) {
		px = x / 1.0f * vx;
		vec = vec3(sin(px), cos(px), 1.0f);
		pVertex[p].position = vec * size;
		pVertex[p].uv = vec2(x / -float(xres)*4, 0);
		p++;
	}

	y = 1;
	for (x = 0; x <= xres; x++) {
		px = x / 1.0f * vx;
		vec = vec3(sin(px), cos(px), -1.0f);
		pVertex[p].position = vec * size;
		pVertex[p].uv = vec2(x / -float(xres)*4, 2);
		p++;
	}
	// booton
	y = 1;
	for (x = 0; x < xres; x++) {
		px = x * vx;
		vec = vec3(sin(px), cos(px), -1.0f);
		pVertex[p].position = vec * size;
		pVertex[p].uv = vec2(-vec.x, vec.y)/2 + vec2(0.5) ;
		p++;
	}
	pVertex[p].position = vec3(0.0f, 0.0f, -1.0f) * size;
	pVertex[p].uv = vec2(0.5);
	//pVertex[p].normal = vec3(0.0f, 0.0f, -1.0f);


// indexes
// top
	p = 0;
	for (y = 1; y < xres; y++) {
		pIndex[p] = usvec3(0, y, y + 1);
		p++;
	}
	pIndex[p] = usvec3(0, y, 1);
	p++;

	//center
	//p = 0;
	x = 1 + xres;
	y = x + xres+1;
	for (uint b = 0; b < xres; b++, x++, y++) {
		pIndex[p] = usvec3(x, y, x + 1);
		p++;
		pIndex[p] = usvec3(y, y + 1, x + 1);
		p++;
	}


	// booton
	for (y = nVertex - xres; y < nVertex - 1; y++) {
		pIndex[p] = usvec3(nVertex - 1, y, y - 1);
		p++;
	}
	pIndex[p] = usvec3(nVertex - 1, nVertex - xres - 1, y - 1);


	vec3 faceNormal;
	for (p = 0; p < nIndex; p++) {
		faceNormal = faceNormal2(pVertex[pIndex[p].x].position, pVertex[pIndex[p].y].position, pVertex[pIndex[p].z].position);
		pVertex[pIndex[p].x].normal += faceNormal;
		pVertex[pIndex[p].y].normal += faceNormal;
		pVertex[pIndex[p].z].normal += faceNormal;
	}
	for (p = 0; p < nVertex; p++) {
		pVertex[p].normal = normalize(pVertex[p].normal);
	}
	//for (p = 0; p < nVertex; p++) {
	//	pVertex[p].uv = vec2(pVertex[p].position.x, pVertex[p].position.y + pVertex[p].position.z);
	//}
	p = 0;

	makeBiTangent();

	name = "Cilinder";

}

void malha::makeBox(uint xres, uint yres, float3 size) {
	free();
	nVertex = 24;
	nIndex = 12;

	pPosition.resize(nVertex);
	pNormal.resize(nVertex);
	pIndex.resize(nIndex);
	pUv.resize(nVertex);

	vec3 sz = vec3(0.5f, 0.5f, 0.5f) * size;


	pPosition[0]  = vec3(-1, -1, -1) * sz;  // frente
	pPosition[1]  = vec3(-1, -1,  1) * sz;  
	pPosition[2]  = vec3( 1, -1, -1) * sz;  
	pPosition[3]  = vec3( 1, -1,  1) * sz;  
										    
	pPosition[4]  = vec3( 1, -1, -1) * sz;  // direita
	pPosition[5]  = vec3( 1,  1, -1) * sz;  
	pPosition[6]  = vec3( 1, -1,  1) * sz;  
	pPosition[7]  = vec3( 1,  1,  1) * sz;  
										    
	pPosition[8]  = vec3( 1,  1, -1) * sz;  // tras
	pPosition[9]  = vec3( 1,  1,  1) * sz;  
	pPosition[10] = vec3(-1,  1, -1) * sz;  
	pPosition[11] = vec3(-1,  1,  1) * sz;  
										    
	pPosition[12] = vec3(-1,  1, -1) * sz;  // esquerda
	pPosition[13] = vec3(-1,  1,  1) * sz;  
	pPosition[14] = vec3(-1, -1, -1) * sz;  
	pPosition[15] = vec3(-1, -1,  1) * sz;  
										    
	pPosition[16] = vec3(-1, -1, -1) * sz;  // baixo
	pPosition[17] = vec3(-1,  1, -1) * sz;  
	pPosition[18] = vec3( 1, -1, -1) * sz;  
	pPosition[19] = vec3( 1,  1, -1) * sz;  
							 			    
	pPosition[20] = vec3(-1,  1,  1) * sz;  // cima
	pPosition[21] = vec3(-1, -1,  1) * sz;
	pPosition[22] = vec3( 1,  1,  1) * sz;
	pPosition[23] = vec3( 1, -1,  1) * sz;

	pUv[0]  = vec2(0, 1 );  // baixo
	pUv[1]  = vec2(0, 0 );  
	pUv[2]  = vec2(1, 1 );  
	pUv[3]  = vec2(1, 0 );  										    
				    	 	
	pUv[4]  = vec2(0, 1 );  // frente
	pUv[5]  = vec2(1, 1 );  
	pUv[6]  = vec2(0, 0 );  
	pUv[7]  = vec2(1, 0 );  										    
				    	 	
	pUv[8]  = vec2(0, 1 );  // cima
	pUv[9]  = vec2(0, 0 );  
	pUv[10] = vec2(1, 1 );  
	pUv[11] = vec2(1, 0 );  										    
				    	
	pUv[12] = vec2(0, 1 );  // tras
	pUv[13] = vec2(0, 0 );
	pUv[14] = vec2(1, 1 );
	pUv[15] = vec2(1, 0 );
					 
	pUv[16] = vec2(1, 0 );  // esquerda
	pUv[17] = vec2(0, 0 ); 
	pUv[18] = vec2(1, 1 ); 
	pUv[19] = vec2(0, 1 ); 
					 	
	pUv[20] = vec2(0, 1 );  // direita
	pUv[21] = vec2(1, 1 );
	pUv[22] = vec2(0, 0 );
	pUv[23] = vec2(1, 0 );


	pIndex[0]  = usvec3( 0,  1,  2); // baixo
	pIndex[1]  = usvec3( 1,  3,  2);
			   
	pIndex[2]  = usvec3( 4,  6,  5); // frente
	pIndex[3]  = usvec3( 5,  6,  7);
			   
	pIndex[4]  = usvec3( 8,  9, 10); // cima
	pIndex[5]  = usvec3( 9, 11, 10);
			   
	pIndex[6]  = usvec3(12, 13, 14); // traz
	pIndex[7]  = usvec3(13, 15, 14);
			   
	pIndex[8]  = usvec3(16, 18, 17); // direita
	pIndex[9]  = usvec3(17, 18, 19);

	pIndex[10] = usvec3(20, 22, 21); // esquerda
	pIndex[11] = usvec3(21, 22, 23);


	vec3 faceNormal;
	for (uint p = 0; p < nIndex; p++) {
		faceNormal = calcNormal(pPosition[pIndex[p].x], pPosition[pIndex[p].y], pPosition[pIndex[p].z]);
		pNormal[pIndex[p].x] += faceNormal;
		pNormal[pIndex[p].y] += faceNormal;
		pNormal[pIndex[p].z] += faceNormal;
	}
	for (uint p = 0; p < nVertex; p++) {
		pNormal[p] = normalize(pNormal[p]);
	}
	makeBiTangent();
	name = "Box";

}

void malha::makeSkyBox(uint xres, uint yres, float3 size) {
	free();
	nVertex = 24;
	nIndex = 12;

	pPosition.resize(nVertex);
	pNormal.resize(nVertex);
	pIndex.resize(nIndex);
	pUv.resize(nVertex);

	vec3 sz = vec3(0.5f, 0.5f, 0.5f) * size;


	pPosition[0]  = vec3(-1, -1, -1) * sz;  // frente
	pPosition[1]  = vec3(-1, -1,  1) * sz;  
	pPosition[2]  = vec3( 1, -1, -1) * sz;  
	pPosition[3]  = vec3( 1, -1,  1) * sz;  
										    
	pPosition[4]  = vec3( 1, -1, -1) * sz;  // direita
	pPosition[5]  = vec3( 1,  1, -1) * sz;  
	pPosition[6]  = vec3( 1, -1,  1) * sz;  
	pPosition[7]  = vec3( 1,  1,  1) * sz;  
										    
	pPosition[8]  = vec3( 1,  1, -1) * sz;  // tras
	pPosition[9]  = vec3( 1,  1,  1) * sz;  
	pPosition[10] = vec3(-1,  1, -1) * sz;  
	pPosition[11] = vec3(-1,  1,  1) * sz;  
										    
	pPosition[12] = vec3(-1,  1, -1) * sz;  // esquerda
	pPosition[13] = vec3(-1,  1,  1) * sz;  
	pPosition[14] = vec3(-1, -1, -1) * sz;  
	pPosition[15] = vec3(-1, -1,  1) * sz;  
										    
	pPosition[16] = vec3(-1, -1, -1) * sz;  // baixo
	pPosition[17] = vec3(-1,  1, -1) * sz;  
	pPosition[18] = vec3( 1, -1, -1) * sz;  
	pPosition[19] = vec3( 1,  1, -1) * sz;  
							 			    
	pPosition[20] = vec3(-1,  1,  1) * sz;  // cima
	pPosition[21] = vec3(-1, -1,  1) * sz;
	pPosition[22] = vec3( 1,  1,  1) * sz;
	pPosition[23] = vec3( 1, -1,  1) * sz;

	pUv[0]  = vec2(0, 1 );  // baixo
	pUv[1]  = vec2(0, 0 );  
	pUv[2]  = vec2(1, 1 );  
	pUv[3]  = vec2(1, 0 );  										    
				    	 	
	pUv[4]  = vec2(0, 1 );  // frente
	pUv[5]  = vec2(1, 1 );  
	pUv[6]  = vec2(0, 0 );  
	pUv[7]  = vec2(1, 0 );  										    
				    	 	
	pUv[8]  = vec2(0, 1 );  // cima
	pUv[9]  = vec2(0, 0 );  
	pUv[10] = vec2(1, 1 );  
	pUv[11] = vec2(1, 0 );  										    
				    	
	pUv[12] = vec2(0, 1 );  // tras
	pUv[13] = vec2(0, 0 );
	pUv[14] = vec2(1, 1 );
	pUv[15] = vec2(1, 0 );
					 
	pUv[16] = vec2(1, 0 );  // esquerda
	pUv[17] = vec2(0, 0 ); 
	pUv[18] = vec2(1, 1 ); 
	pUv[19] = vec2(0, 1 ); 
					 	
	pUv[20] = vec2(0, 1 );  // direita
	pUv[21] = vec2(1, 1 );
	pUv[22] = vec2(0, 0 );
	pUv[23] = vec2(1, 0 );


	pIndex[0]  = usvec3( 0,  2,  1); // baixo
	pIndex[1]  = usvec3( 1,  2,  3);
			   
	pIndex[2]  = usvec3( 4,  5,  6); // frente
	pIndex[3]  = usvec3( 5,  7,  6);
			   
	pIndex[4]  = usvec3( 8, 10,  9); // cima
	pIndex[5]  = usvec3( 9, 10, 11);
			   
	pIndex[6]  = usvec3(12, 14, 13); // traz
	pIndex[7]  = usvec3(13, 14, 15);
			   
	pIndex[8]  = usvec3(16, 17, 18); // direita
	pIndex[9]  = usvec3(17, 19, 18);

	pIndex[10] = usvec3(20, 21, 22); // esquerda
	pIndex[11] = usvec3(21, 23, 22);


	vec3 faceNormal;
	for (uint p = 0; p < nIndex; p++) {
		faceNormal = calcNormal(pPosition[pIndex[p].x], pPosition[pIndex[p].y], pPosition[pIndex[p].z]);
		pNormal[pIndex[p].x] -= faceNormal;
		pNormal[pIndex[p].y] -= faceNormal;
		pNormal[pIndex[p].z] -= faceNormal;
	}
	for (uint p = 0; p < nVertex; p++) {
		pNormal[p] = normalize(pNormal[p]);
	}
	makeBiTangent();
	name = "Sky Box";

}

void malha::makeQuad(uint xres, uint yres, float3 size){
	free();
	nVertex = 4;
	nIndex = 2;

	pVertex.resize(nVertex);
	pIndex.resize(nIndex);
	vec3 sz = vec3(0.5f, 0.5f, 0.5f)*size;

	// vertex
	pVertex[0].position = vec3(-1,-1, 0 ) * sz;
	pVertex[1].position = vec3(-1, 1, 0 ) * sz;
	pVertex[2].position = vec3( 1, 1, 0 ) * sz;
	pVertex[3].position = vec3( 1,-1, 0 ) * sz;

	// index
	pIndex[0] = usvec3(0, 1, 2);
	pIndex[1] = usvec3(2, 3, 0);

	// uv
	pVertex[0].uv = vec2(0, 1);
	pVertex[1].uv = vec2(0, 0);
	pVertex[2].uv = vec2(1, 0);
	pVertex[3].uv = vec2(1, 1);

	// color
	pVertex[0].color = byte4(255, 255, 255, 255);
	pVertex[1].color = byte4(255, 255, 255, 255);
	pVertex[2].color = byte4(255, 255, 255, 255);
	pVertex[3].color = byte4(255, 255, 255, 255);

	// normal
	pVertex[0].normal = calcNormal(pVertex[pIndex[0].x].position, pVertex[pIndex[0].y].position, pVertex[pIndex[0].z].position);

	pVertex[0].normal = normalize(pVertex[0].normal);
	pVertex[1].normal = pVertex[0].normal;
	pVertex[2].normal = pVertex[0].normal;
	pVertex[3].normal = pVertex[0].normal;
	makeBiTangent();

	name = "Quad";

}

void malha::makeTriangle(uint xres, uint yres, float3 size) {
	free();
	nVertex = 3;
	nIndex = 1;

	pVertex.resize(nVertex);
	pIndex.resize(nIndex);
	vec3 sz = vec3(0.5f, 0.5f, 0.5f) * size;

	// vertex
	pVertex[0].position = vec3(-0.5, 0, 0) * sz;
	pVertex[1].position = vec3(   0, 1, 0) * sz;
	pVertex[2].position = vec3( 0.5, 0, 0) * sz;

	// index
	pIndex[0] = usvec3(0, 1, 2);

	// uv
	pVertex[0].uv = vec2(  0, 1);
	pVertex[1].uv = vec2(0.5, 0);
	pVertex[2].uv = vec2(  1, 1);

	// color
	pVertex[0].color = byte4(255, 255, 255, 255);
	pVertex[1].color = byte4(255, 255, 255, 255);
	pVertex[2].color = byte4(255, 255, 255, 255);

	// normal
	pVertex[0].normal = calcNormal(pVertex[pIndex[0].x].position, pVertex[pIndex[0].y].position, pVertex[pIndex[0].z].position);

	pVertex[0].normal = normalize(pVertex[0].normal);
	pVertex[1].normal = pVertex[0].normal;
	pVertex[2].normal = pVertex[0].normal;
	makeBiTangent();

	name = "Triangle";

}

void malha::makeGrid(int xres, int yres, float3 size) {
	free();
	renderMode = ERenderMode::RenderModeLines;
	nVertex = (xres+yres)*4+4;
	nIndex = (xres + yres)*4+2;

	pPosition.clear();
	pColor.clear();
	pIndex2.clear();

	pPosition.reserve(nVertex);
	pColor.reserve(nVertex);
	pIndex2.reserve(nIndex);
	vec3 sz = vec3(0.5f, 0.5f, 0.5f) * size;

	// vertex
	int  i =  - xres;
		//pPosition.push_back( vec3(1, 1, 1) * sz);
	for (; i <= xres; i++) {
		pPosition.push_back( vec3(xres, i, 0) * sz);
		pPosition.push_back( vec3(-xres, i, 0) * sz);
		pColor.push_back(vec4(0.5f, 0.5f, 0.5f, 0.5f));
		pColor.push_back(vec4(0.5f, 0.5f, 0.5f, 0.5f));
	}
	i =  - yres;
	for (; i <= yres; i++) {
		pPosition.push_back( vec3(i, yres, 0) * sz);
		pPosition.push_back( vec3(i, -yres, 0) * sz);
		pColor.push_back(vec4(0.5f, 0.5f, 0.5f, 0.5f));
		pColor.push_back(vec4(0.5f, 0.5f, 0.5f, 0.5f));
	}

	i = 0;
	for (; i <= (int)nIndex; ) {
		pIndex2.push_back(usvec2(i, i+1));
		i += 2;
	}
	name = "Grid";

}

void malha::makeAxis(uint xres, uint yres, float3 size) {
	free();
	renderMode = ERenderMode::RenderModeLines;
	nVertex = 6;
	nIndex = 3;

	pPosition.resize(nVertex);
	pColor.resize(nVertex);
	pIndex2.resize(nIndex);
	vec3 sz = vec3(0.5f, 0.5f, 0.5f) * size;

	// vertex
	pPosition[0] = vec3(0, 0, 0) * sz;
	pPosition[1] = vec3(1, 0, 0) * sz;
	pPosition[2] = vec3(0, 0, 0) * sz;
	pPosition[3] = vec3(0, 1, 0) * sz;
	pPosition[4] = vec3(0, 0, 0) * sz;
	pPosition[5] = vec3(0, 0, 1) * sz;

	// color
	pColor[0] = vec4(1, 0, 0, 1);
	pColor[1] = vec4(1, 0, 0, 1);
	pColor[2] = vec4(0, 1, 0, 1);
	pColor[3] = vec4(0, 1, 0, 1);
	pColor[4] = vec4(0, 0, 1, 1);
	pColor[5] = vec4(0, 0, 1, 1);

	pIndex2[0] = usvec2(0, 1);
	pIndex2[1] = usvec2(2, 3);
	pIndex2[2] = usvec2(4, 5);
	name = "Axis";
}

void malha::makeAxisS(uint xres, uint yres, float3 size) {
	free();
	renderMode = ERenderMode::RenderModeLines;
	nVertex = 6;
	nIndex = 3;

	pPosition.resize(nVertex);
	pColor.resize(nVertex);
	pIndex2.resize(nIndex);
	vec3 sz = vec3(0.5f, 0.5f, 0.5f) * size;

	// vertex
	pPosition[0] = vec3(0, 0, 0) * sz;
	pPosition[1] = vec3(1, 0, 0) * sz;
	pPosition[2] = vec3(0, 0, 0) * sz;
	pPosition[3] = vec3(0, 1, 0) * sz;
	pPosition[4] = vec3(0, 0, 0) * sz;
	pPosition[5] = vec3(0, 0, 1) * sz;

	// color
	pColor[0] = vec4(1, 0, 0, 1);
	pColor[1] = vec4(1, 0, 0, 1);
	pColor[2] = vec4(0, 1, 0, 1);
	pColor[3] = vec4(0, 1, 0, 1);
	pColor[4] = vec4(0, 0, 1, 1);
	pColor[5] = vec4(0, 0, 1, 1);

	pIndex2[0] = usvec2(0, 1);
	pIndex2[1] = usvec2(2, 3);
	pIndex2[2] = usvec2(4, 5);
	name = "Axis S";
}

void malha::makeNormals() {
	uint p = 0;
	if(pVertex.size()>2 && pIndex.size() >= 1 && nVertex == (pVertex.size()-1) && nIndex == pIndex.size() - 1){
		vec3 faceNormal;
		for (p = 0; p < nIndex; p++) {
			faceNormal = faceNormal2(pVertex[pIndex[p].x].position, pVertex[pIndex[p].y].position, pVertex[pIndex[p].z].position);
			pVertex[pIndex[p].x].normal += faceNormal;
			pVertex[pIndex[p].y].normal += faceNormal;
			pVertex[pIndex[p].z].normal += faceNormal;
		}
		for (p = 0; p < nVertex; p++) {
			pVertex[p].normal = normalize(pVertex[p].normal);
		}
	}
	else if (pPosition.size() > 2 && pIndex.size() >= 1 && nVertex == (pPosition.size() - 1) && nIndex == pIndex.size() - 1) {
		if(pNormal.size() != nVertex)
			pNormal.resize(nVertex);
		vec3 faceNormal;
		for (p = 0; p < nIndex; p++) {
			faceNormal = faceNormal2(pPosition[pIndex[p].x], pPosition[pIndex[p].y], pPosition[pIndex[p].z]);
			pNormal[pIndex[p].x] += faceNormal;
			pNormal[pIndex[p].y] += faceNormal;
			pNormal[pIndex[p].z] += faceNormal;
		}
		for (p = 0; p < nVertex; p++) {
			pNormal[p] = normalize(pNormal[p]);
		}
	}
}

void faceBitangent(vec3 pos1, vec3 pos2, vec3 pos3, vec2 uv1, vec2 uv2, vec2 uv3,	vec3 &tangent,vec3 &bitangent) {
			vec3 edge1 = pos1 - pos2;
			vec3 edge2 = pos3 - pos2;
			vec2 deltaUV1 = uv1 - uv2;
			vec2 deltaUV2 = uv3 - uv2;

			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

			bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
}

void malha::makeBiTangent() {
	uint p = 0;
	if (pVertex.size() > 2 && pIndex.size() >= 1 && nVertex == (pVertex.size() ) && nIndex == pIndex.size() ) {
		vec3 tangent;
		vec3 bitangent;
		for (p = 0; p < nIndex; p++) {
			faceBitangent(pVertex[pIndex[p].x].position, pVertex[pIndex[p].y].position, pVertex[pIndex[p].z].position,
						  pVertex[pIndex[p].x].uv, pVertex[pIndex[p].y].uv, pVertex[pIndex[p].z].uv,
						  tangent, bitangent);
			pVertex[pIndex[p].x].tangent += tangent;
			pVertex[pIndex[p].y].tangent += tangent;
			pVertex[pIndex[p].z].tangent += tangent;

			pVertex[pIndex[p].x].bitangent += bitangent;
			pVertex[pIndex[p].y].bitangent += bitangent;
			pVertex[pIndex[p].z].bitangent += bitangent;
		}
		//for (p = 0; p < nVertex; p++) {
		//	pVertex[p].tangent = normalize(pVertex[p].tangent);
		//	pVertex[p].bitangent = normalize(pVertex[p].bitangent);
		//}
	}
	else if (pPosition.size() > 2 && pIndex.size() >= 1 && nVertex == (pPosition.size() ) && nIndex == pIndex.size() ) {
		if (pTangent.size() != nVertex)
			pTangent.resize(nVertex);
			pBiTangent.resize(nVertex);
		vec3 tangent;
		vec3 bitangent;
		for (p = 0; p < nIndex; p++) {
			faceBitangent(pPosition[pIndex[p].x], pPosition[pIndex[p].y], pPosition[pIndex[p].z],
						  pUv[pIndex[p].x], pUv[pIndex[p].y], pUv[pIndex[p].z],
						  tangent, bitangent);
			pTangent[pIndex[p].x] += tangent;
			pTangent[pIndex[p].y] += tangent;
			pTangent[pIndex[p].z] += tangent;

			pBiTangent[pIndex[p].x] += bitangent;
			pBiTangent[pIndex[p].y] += bitangent;
			pBiTangent[pIndex[p].z] += bitangent;
		}
		//for (p = 0; p < nVertex; p++) {
		//	pTangent[p] = normalize(pTangent[p]);
		//	pBiTangent[p] = normalize(pBiTangent[p]);
		//}
	}
}

void malha::CreateBuffer() {
	switch (renderMode) {
		case RenderModePoints:
			break;
		case RenderModeLines:
		{
			// not interleaved atributes, need position and any other atribute is not required, but if available is used
			if (nVertex > 1 && pPosition.size() == nVertex && nIndex) {
				//glGenVertexArrays(1, &VAO);
				//glGenBuffers(1, &VBO);
				//glBindVertexArray(VAO);
				//glBindBuffer(GL_ARRAY_BUFFER, VBO);
				//glBufferData(GL_ARRAY_BUFFER, pPosition.size()  , pPosition.data(), GL_STATIC_DRAW);
				//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
				//glEnableVertexAttribArray(0);
				//glBindBuffer(GL_ARRAY_BUFFER, 0);
				//glBindVertexArray(0);
				//break;

				glGenVertexArrays(1, &VAO);

				//glCreateVertexArrays(1, &VAO);// Create the vertex array object
				glGenBuffers(sizeof(pBuffers) / sizeof(pBuffers[0]), &pBuffers[0]);// Get create then buffers
				//glCreateBuffers(4, &buffer[0]);// Get create two buffers
				glGenBuffers(1, &EBO);
				// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
				glBindVertexArray(VAO);
				//glGenBuffers(1, &VBO);
				//glBindBuffer(GL_ARRAY_BUFFER, VBO);
				//glBufferData(GL_ARRAY_BUFFER, pVertex.size() * sizeof(pVertex[0]), pVertex.data(), GL_STATIC_DRAW);
				uint i = 0;
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, pIndex2.size() * sizeof(pIndex2[0]), pIndex2.data(), GL_STATIC_DRAW);

				glBindBuffer(GL_ARRAY_BUFFER, pBuffers[i]);
				glNamedBufferStorage(pBuffers[i], pPosition.size() * sizeof(pPosition[0]), pPosition.data(), 0);// Initialize the first buffer
				glVertexArrayVertexBuffer(VAO, i, pBuffers[i], 0, sizeof(pPosition[0]));// Bind it to the vertex array - offset zero, stride = sizeof(vec3)
				glVertexArrayAttribFormat(VAO, i, 3, GL_FLOAT, GL_FALSE, 0);// Tell OpenGL what the format of the attribute is
				glVertexArrayAttribBinding(VAO, i, i);// Tell OpenGL which vertex buffer binding to use for this attribute
				glEnableVertexArrayAttrib(VAO, i);// Enable the attribute
				glEnableVertexAttribArray(i);
				i++;
				if (pNormal.size() == nVertex) {
					glBindBuffer(GL_ARRAY_BUFFER, pBuffers[i]);
					glNamedBufferStorage(pBuffers[i], pNormal.size() * sizeof(pNormal[0]), pNormal.data(), 0);// Perform similar initialization for the second buffer
					glVertexArrayVertexBuffer(VAO, i, pBuffers[i], 0, sizeof(pNormal[0]));
					glVertexArrayAttribFormat(VAO, i, 3, GL_FLOAT, GL_FALSE, 0);
					glVertexArrayAttribBinding(VAO, i, i);
					glEnableVertexAttribArray(i);
				}
				i++;
				if (pUv.size() == nVertex) {
					glBindBuffer(GL_ARRAY_BUFFER, pBuffers[i]);
					glNamedBufferStorage(pBuffers[i], pUv.size() * sizeof(pUv[0]), pUv.data(), 0);// Perform similar initialization for the second buffer
					glVertexArrayVertexBuffer(VAO, i, pBuffers[i], 0, sizeof(pUv[0]));
					glVertexArrayAttribFormat(VAO, i, 2, GL_FLOAT, GL_FALSE, 0);
					glVertexArrayAttribBinding(VAO, i, i);
					glEnableVertexAttribArray(i);
				}
				i++;
				if (pColor.size() == nVertex) {
					glBindBuffer(GL_ARRAY_BUFFER, pBuffers[i]);
					glNamedBufferStorage(pBuffers[i], pColor.size() * sizeof(pColor[0]), pColor.data(), 0);// Perform similar initialization for the second buffer
					glVertexArrayVertexBuffer(VAO, i, pBuffers[i], 0, sizeof(pColor[0]));
					glVertexArrayAttribFormat(VAO, i, 4, GL_FLOAT, GL_FALSE, 0);
					glVertexArrayAttribBinding(VAO, i, i);
					glEnableVertexAttribArray(i);
				}
				i++;
				if (pTangent.size() == nVertex) {
					glBindBuffer(GL_ARRAY_BUFFER, pBuffers[i]);
					glNamedBufferStorage(pBuffers[i], pTangent.size() * sizeof(pTangent[0]), pTangent.data(), 0);// Perform similar initialization for the second buffer
					glVertexArrayVertexBuffer(VAO, i, pBuffers[i], 0, sizeof(pTangent[0]));
					glVertexArrayAttribFormat(VAO, i, 3, GL_FLOAT, GL_FALSE, 0);
					glVertexArrayAttribBinding(VAO, i, i);
					glEnableVertexAttribArray(i);
				}
				i++;
				if (pBiTangent.size() == nVertex) {
					glBindBuffer(GL_ARRAY_BUFFER, pBuffers[i]);
					glNamedBufferStorage(pBuffers[i], pBiTangent.size() * sizeof(pBiTangent[0]), pBiTangent.data(), 0);// Perform similar initialization for the second buffer
					glVertexArrayVertexBuffer(VAO, i, pBuffers[i], 0, sizeof(pBiTangent[0]));
					glVertexArrayAttribFormat(VAO, i, 3, GL_FLOAT, GL_FALSE, 0);
					glVertexArrayAttribBinding(VAO, i, i);
					glEnableVertexAttribArray(i);
				}
				i++;
				if (pUv1.size() == nVertex) {
					glBindBuffer(GL_ARRAY_BUFFER, pBuffers[i]);
					glNamedBufferStorage(pBuffers[i], pUv1.size() * sizeof(pUv1[0]), pUv1.data(), 0);// Perform similar initialization for the second buffer
					glVertexArrayVertexBuffer(VAO, i, pBuffers[i], 0, sizeof(pUv1[0]));
					glVertexArrayAttribFormat(VAO, i, 2, GL_FLOAT, GL_FALSE, 0);
					glVertexArrayAttribBinding(VAO, i, i);
					glEnableVertexAttribArray(i);
				}
				i++;
				if (pUv2.size() == nVertex) {
					glBindBuffer(GL_ARRAY_BUFFER, pBuffers[i]);
					glNamedBufferStorage(pBuffers[i], pUv2.size() * sizeof(pUv2[0]), pUv2.data(), 0);// Perform similar initialization for the second buffer
					glVertexArrayVertexBuffer(VAO, i, pBuffers[i], 0, sizeof(pUv2[0]));
					glVertexArrayAttribFormat(VAO, i, 2, GL_FLOAT, GL_FALSE, 0);
					glVertexArrayAttribBinding(VAO, i, i);
					glEnableVertexAttribArray(i);
				}
				i++;
				if (pColor1.size() == nVertex) {
					glBindBuffer(GL_ARRAY_BUFFER, pBuffers[i]);
					glNamedBufferStorage(pBuffers[i], pColor1.size() * sizeof(pColor1[0]), pColor1.data(), 0);// Perform similar initialization for the second buffer
					glVertexArrayVertexBuffer(VAO, i, pBuffers[i], 0, sizeof(pColor1[0]));
					glVertexArrayAttribFormat(VAO, i, 4, GL_BYTE, GL_FALSE, 0);
					glVertexArrayAttribBinding(VAO, i, i);
					glEnableVertexAttribArray(i);
				}
				i++;
				if (pColor2.size() == nVertex) {
					glBindBuffer(GL_ARRAY_BUFFER, pBuffers[i]);
					glNamedBufferStorage(pBuffers[i], pColor2.size() * sizeof(pColor2[0]), pColor2.data(), 0);// Perform similar initialization for the second buffer
					glVertexArrayVertexBuffer(VAO, i, pBuffers[i], 0, sizeof(pColor2[0]));
					glVertexArrayAttribFormat(VAO, i, 4, GL_BYTE, GL_FALSE, 0);
					glVertexArrayAttribBinding(VAO, i, i);
					glEnableVertexAttribArray(i);
				}
				glBindVertexArray(0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			break;
		}
		case RenderModeTriangles:{
			// standard mesh with position, normal, uv and color interleaved
			if (nVertex > 1 && pVertex.size() == nVertex && nIndex) {
				int i = pVertex.size();
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glGenBuffers(1, &EBO);
				// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
				glBindVertexArray(VAO);

				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, pVertex.size() * sizeof(pVertex[0]), pVertex.data(), GL_STATIC_DRAW);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, pIndex.size() * sizeof(pIndex[0]), pIndex.data(), GL_STATIC_DRAW);

				// vertex positions
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0); // (void*)offsetof(vertex, position))
				// vertex normals
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
				// vertex tangent
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, tangent));
				// vertex bitangent
				glEnableVertexAttribArray(3);
				glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, bitangent));
				// vertex texture coords
				glEnableVertexAttribArray(4);
				glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, uv));
				// vertex color
				glEnableVertexAttribArray(5);
				glVertexAttribPointer(5, 4, GL_BYTE, GL_TRUE, sizeof(vertex), (void*)offsetof(vertex, color));

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);

			}
			// not interleaved atributes, need position and any other atribute is not required, but if available is used
			else if (nVertex > 1 && pPosition.size() == nVertex && nIndex) {
				glGenVertexArrays(1, &VAO);
				//glCreateVertexArrays(1, &VAO);// Create the vertex array object
				glGenBuffers(sizeof(pBuffers) / sizeof(pBuffers[0]), &pBuffers[0]);// Get create then buffers
				//glCreateBuffers(4, &buffer[0]);// Get create two buffers
				glGenBuffers(1, &EBO);
				// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
				glBindVertexArray(VAO);
				//glGenBuffers(1, &VBO);
				//glBindBuffer(GL_ARRAY_BUFFER, VBO);
				//glBufferData(GL_ARRAY_BUFFER, pVertex.size() * sizeof(pVertex[0]), pVertex.data(), GL_STATIC_DRAW);
				uint i = 0;
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, pIndex.size() * sizeof(pIndex[0]), pIndex.data(), GL_STATIC_DRAW);

				glBindBuffer(GL_ARRAY_BUFFER, pBuffers[i]);
				glNamedBufferStorage(pBuffers[i], pPosition.size() * sizeof(pPosition[0]), pPosition.data(), 0);// Initialize the first buffer
				glVertexArrayVertexBuffer(VAO, i, pBuffers[i], 0, sizeof(pPosition[0]));// Bind it to the vertex array - offset zero, stride = sizeof(vec3)
				glVertexArrayAttribFormat(VAO, i, 3, GL_FLOAT, GL_FALSE, 0);// Tell OpenGL what the format of the attribute is
				glVertexArrayAttribBinding(VAO, i, i);// Tell OpenGL which vertex buffer binding to use for this attribute
				glEnableVertexArrayAttrib(VAO, i);// Enable the attribute
				glEnableVertexAttribArray(i);
				i++;
				if (pNormal.size() == nVertex) {
					glBindBuffer(GL_ARRAY_BUFFER, pBuffers[i]);
					glNamedBufferStorage(pBuffers[i], pNormal.size() * sizeof(pNormal[0]), pNormal.data(), 0);// Perform similar initialization for the second buffer
					glVertexArrayVertexBuffer(VAO, i, pBuffers[i], 0, sizeof(pNormal[0]));
					glVertexArrayAttribFormat(VAO, i, 3, GL_FLOAT, GL_FALSE, 0);
					glVertexArrayAttribBinding(VAO, i, i);
					glEnableVertexAttribArray(i);
				}
				i++;
				if (pTangent.size() == nVertex) {
					glBindBuffer(GL_ARRAY_BUFFER, pBuffers[i]);
					glNamedBufferStorage(pBuffers[i], pTangent.size() * sizeof(pTangent[0]), pTangent.data(), 0);// Perform similar initialization for the second buffer
					glVertexArrayVertexBuffer(VAO, i, pBuffers[i], 0, sizeof(pTangent[0]));
					glVertexArrayAttribFormat(VAO, i, 3, GL_FLOAT, GL_FALSE, 0);
					glVertexArrayAttribBinding(VAO, i, i);
					glEnableVertexAttribArray(i);
				}
				i++;
				if (pBiTangent.size() == nVertex) {
					glBindBuffer(GL_ARRAY_BUFFER, pBuffers[i]);
					glNamedBufferStorage(pBuffers[i], pBiTangent.size() * sizeof(pBiTangent[0]), pBiTangent.data(), 0);// Perform similar initialization for the second buffer
					glVertexArrayVertexBuffer(VAO, i, pBuffers[i], 0, sizeof(pBiTangent[0]));
					glVertexArrayAttribFormat(VAO, i, 3, GL_FLOAT, GL_FALSE, 0);
					glVertexArrayAttribBinding(VAO, i, i);
					glEnableVertexAttribArray(i);
				}
				i++;
				if (pUv.size() == nVertex) {
					glBindBuffer(GL_ARRAY_BUFFER, pBuffers[i]);
					glNamedBufferStorage(pBuffers[i], pUv.size() * sizeof(pUv[0]), pUv.data(), 0);// Perform similar initialization for the second buffer
					glVertexArrayVertexBuffer(VAO, i, pBuffers[i], 0, sizeof(pUv[0]));
					glVertexArrayAttribFormat(VAO, i, 2, GL_FLOAT, GL_FALSE, 0);
					glVertexArrayAttribBinding(VAO, i, i);
					glEnableVertexAttribArray(i);
				}
				i++;
				if (pColor.size() == nVertex) {
					glBindBuffer(GL_ARRAY_BUFFER, pBuffers[i]);
					glNamedBufferStorage(pBuffers[i], pColor.size() * sizeof(pColor[0]), pColor.data(), 0);// Perform similar initialization for the second buffer
					glVertexArrayVertexBuffer(VAO, i, pBuffers[i], 0, sizeof(pColor[0]));
					glVertexArrayAttribFormat(VAO, i, 4, GL_BYTE, GL_FALSE, 0);
					glVertexArrayAttribBinding(VAO, i, i);
					glEnableVertexAttribArray(i);
				}
				i++;
				if (pUv1.size() == nVertex) {
					glBindBuffer(GL_ARRAY_BUFFER, pBuffers[i]);
					glNamedBufferStorage(pBuffers[i], pUv1.size() * sizeof(pUv1[0]), pUv1.data(), 0);// Perform similar initialization for the second buffer
					glVertexArrayVertexBuffer(VAO, i, pBuffers[i], 0, sizeof(pUv1[0]));
					glVertexArrayAttribFormat(VAO, i, 2, GL_FLOAT, GL_FALSE, 0);
					glVertexArrayAttribBinding(VAO, i, i);
					glEnableVertexAttribArray(i);
				}
				i++;
				if (pUv2.size() == nVertex) {
					glBindBuffer(GL_ARRAY_BUFFER, pBuffers[i]);
					glNamedBufferStorage(pBuffers[i], pUv2.size() * sizeof(pUv2[0]), pUv2.data(), 0);// Perform similar initialization for the second buffer
					glVertexArrayVertexBuffer(VAO, i, pBuffers[i], 0, sizeof(pUv2[0]));
					glVertexArrayAttribFormat(VAO, i, 2, GL_FLOAT, GL_FALSE, 0);
					glVertexArrayAttribBinding(VAO, i, i);
					glEnableVertexAttribArray(i);
				}
				i++;
				if (pColor1.size() == nVertex) {
					glBindBuffer(GL_ARRAY_BUFFER, pBuffers[i]);
					glNamedBufferStorage(pBuffers[i], pColor1.size() * sizeof(pColor1[0]), pColor1.data(), 0);// Perform similar initialization for the second buffer
					glVertexArrayVertexBuffer(VAO, i, pBuffers[i], 0, sizeof(pColor1[0]));
					glVertexArrayAttribFormat(VAO, i, 4, GL_BYTE, GL_FALSE, 0);
					glVertexArrayAttribBinding(VAO, i, i);
					glEnableVertexAttribArray(i);
				}
				i++;
				if (pColor2.size() == nVertex) {
					glBindBuffer(GL_ARRAY_BUFFER, pBuffers[i]);
					glNamedBufferStorage(pBuffers[i], pColor2.size() * sizeof(pColor2[0]), pColor2.data(), 0);// Perform similar initialization for the second buffer
					glVertexArrayVertexBuffer(VAO, i, pBuffers[i], 0, sizeof(pColor2[0]));
					glVertexArrayAttribFormat(VAO, i, 4, GL_BYTE, GL_FALSE, 0);
					glVertexArrayAttribBinding(VAO, i, i);
					glEnableVertexAttribArray(i);
				}
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}
			break;
		}
		default:
			break;
	}
	// example bindlles buffers
	if(0){
		GLuint buffer[2]{0,0};
		GLuint vao;
		static const GLfloat positions[] = { 1 };
		static const GLfloat colors[] = { 1 };
		glCreateVertexArrays(1, &vao);// Create the vertex array object
		glCreateBuffers(2, &buffer[0]);// Get create two buffers
		glNamedBufferStorage(buffer[0], sizeof(positions), positions, 0);// Initialize the first buffer
		glVertexArrayVertexBuffer(vao, 0, buffer[0], 0, sizeof(vec3));// Bind it to the vertex array - offset zero, stride = sizeof(vec3)
		glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);// Tell OpenGL what the format of the attribute is
		glVertexArrayAttribBinding(vao, 0, 0);// Tell OpenGL which vertex buffer binding to use for this attribute
		glEnableVertexArrayAttrib(vao, 0);// Enable the attribute
		glNamedBufferStorage(buffer[1], sizeof(colors), colors, 0);// Perform similar initialization for the second buffer
		glVertexArrayVertexBuffer(vao, 1, buffer[1], 0, sizeof(vec3));
		glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(vao, 1, 1);
		glEnableVertexAttribArray(1);
	}	
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);	   
}

void malha::setMatrix(matrix_block* m) {
	if (mMaterial) {
		mMaterial->active(m);
	}
	else {
		mMaterial = new Material("default");
		//mMaterial->atach();
		mMaterial->active(m);
	}
}

void malha::draw(matrix_block *m){
	if (!VAO)
		CreateBuffer();
	setMatrix(m);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	switch (renderMode) {
		case RenderModePoints:
		{
			break;
		}
		case RenderModeLines:
		{

			glBindVertexArray(VAO);
			//glDrawArrays(GL_LINES, 0, pPosition.size());
			glDrawElements(GL_LINES, (pIndex2.size() * sizeof(pIndex2[0]))/2 , GL_UNSIGNED_SHORT, 0);
			break;
		}
		case RenderModeTriangles:
		{
			glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			glDrawElements(GL_TRIANGLES, pIndex.size() * sizeof(pIndex[0]) / 2, GL_UNSIGNED_SHORT, 0);
			break;
		}
		default:
			break;
	}
	glBindVertexArray(0);
}

int malha::atach() {
	count++;
	return count;
}

void malha::detach() {
	count--;
	if (count <= 0) {
		g_malha_list[mIndex] = NULL;
		delete this;
	}
}





