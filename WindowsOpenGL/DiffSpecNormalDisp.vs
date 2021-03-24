#version 410 core

// Per-vertex inputs
layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inTangent;
layout (location = 3) in vec3 inBiTangent;
layout (location = 4) in vec2 inUv1;
layout (location = 5) in vec4 inColor1;

layout (location = 6) in vec2 inUv2;
layout (location = 7) in vec2 inUv3;
layout (location = 8) in vec4 inColor2;
layout (location = 9) in vec4 inColor3;

// Matrices we'll need
uniform mat4 mt_model;
uniform mat4 mt_view;
uniform mat4 mt_projection;
uniform mat4 mt_modelView;
uniform mat4 mt_modelViewProjection;
uniform vec3 viewPos;
// Outputs to fragment shader
out VS_OUT
{
	vec4 position;
	vec4 position_M;
	vec4 position_V;
	vec4 position_P;
	vec4 position_MV;
	vec4 position_MVP;

	vec3 normal;	// normal
	vec3 tangent;	// tangent
	vec3 bitangent;	// bitangent
	vec3 normalV;	// normal
	vec3 tangentV;	// tangent
	vec3 bitangentV;	// bitangent

	vec3 lightDir;	// Light
	vec3 viewPos;	// Light
	vec2 uv1;		// first uv
	
	// rarely used, not necessary
	vec2 uv2;		// Second uv possible not used
	vec4 color1;		// vertex color 
	vec4 color2;	// vertex color 
} vs_out;

vec3 light_pos = vec3(0, 0, 2);

void main(void){
	// Calculate normal in view-space
	// vs_out.N = mat3(mt_modelView) * inNormal;
	mat3 normalMatrix = mat3(transpose(inverse(mt_model)));
	vec3 normal 	= normalize(vec3( normalMatrix * inNormal		));
	vec3 tangent 	= normalize(vec3( normalMatrix * inTangent		));
	vec3 bitangent 	= normalize(vec3( normalMatrix * inBiTangent	));
	
	vs_out.normal 		= normal ;
	vs_out.tangent 		= tangent ;
	vs_out.bitangent 	= bitangent;
	
	mat3 local2WorldTranspose = (mat3(    tangent,	bitangent, normal));
	vec3 worldNormal = normalize(local2WorldTranspose*inNormal );
	vec3 viewNormal = normalize(mat3(mt_modelView)*worldNormal);

	
	mat3 normalMatrix2 = mat3(((mt_modelView)));
	vs_out.normalV = 	normalize(vec3( mat3(mt_modelView) * vec3(inNormal	)));
	vs_out.tangentV = 	normalize(vec3( normalMatrix2 * vec3(inTangent	)));
	vs_out.bitangentV = normalize(vec3( normalMatrix2 * vec3(inBiTangent)));

	vec4 P = mt_model * inPosition;

	vs_out.position		= inPosition; // local
	vs_out.position_M	= mt_model * inPosition; // global
	vs_out.position_V	= mt_view * inPosition; // global
	vs_out.position_P	= mt_projection * inPosition;
	vs_out.position_MV	= mt_modelView * inPosition;  // global view
	vs_out.position_MVP = mt_modelViewProjection * inPosition; // global projection
	vs_out.uv1=inUv1;
	vs_out.color1=inColor1;
	// Calculate light vector
	vs_out.lightDir = light_pos - P.xyz;
	vs_out.viewPos = viewPos - P.xyz;

	// Calculate the clip-space position of each vertex
	gl_Position = mt_modelViewProjection * inPosition;
}
