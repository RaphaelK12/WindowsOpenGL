#version 410 core
// Input Built-in Variables
// in vec4 gl_FragCoord;
// in bool gl_FrontFacing;
// in vec2 gl_PointCoord;
float saturate(float value) {
	return clamp(value, 0.0, 1.0);
}
vec2 saturate(vec2 value) {
	return clamp(value, 0.0, 1.0);
}
vec3 saturate(vec3 value) {
	return clamp(value, 0.0, 1.0);
}
vec4 saturate(vec4 value) {
	return clamp(value, 0.0, 1.0);
}
vec3 pow(in float p, in vec3 v) {
	return pow(vec3(p), v);
}
vec3 pow(in vec3 p, in float v) {
	return pow(p, vec3(v));
}
vec2 SampleSphericalMap(vec3 v){
	const vec2 invAtan = vec2(0.1591, 0.3183);
	vec2 uv = vec2(atan(v.x, v.y), asin(-v.z));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}

// Output
layout(location = 0) out vec4 color;
layout(location = 1) out vec4 nr;
// Input from vertex shader
in VS_OUT{
	// Vertex positions
	vec4 position;		// position in local space
	vec4 position_M;	// position in global space
	vec4 position_V;	// position in view space
	vec4 position_P;	// position in projection space
	vec4 position_MV;	// position in model view space
	vec4 position_MVP;	// position in model view projection space
	vec3 normal;	// normal
	vec3 tangent;	// tangent
	vec3 bitangent;	// bitangent
	
	vec3 lightDir;	// Light
	vec3 viewPos;	// Light
	vec2 uv1;		// first uv
	vec2 uv2;		// Second uv possible not used
	vec4 color1;		// vertex color 
	vec4 color2;	// vertex color 
} fs_in;
uniform sampler2D texture1;
// Material properties
uniform vec4 diffuse_albedo = vec4(0.5, 0.2, 0.7, 1);
uniform vec3 specular_albedo = vec3(0.7);
uniform float specular_power = 128.0; // 200

vec3 lightColor = vec3(1.0, 1.0, 1.0);
float lightPower = 18.0; // 40
vec3 light_pos = vec3(5, 0, 0);

struct material {
	vec4 diffuse;		// color passed to program
	vec4 color1;		// color passed to program
	vec4 color2;		// color passed to program
	vec4 color3;		// color passed to program

	vec4 ambient;		// color passed to program
	vec4 emission;		// color passed to program
	vec4 translucenci;	// color passed to program
	vec4 sadowsColor;	// color passed to program

	vec4 specular;		// color passed to program
	vec4 reflex;		// color passed to program
	vec4 shinines;		// color passed to program
};

uniform material mt;

struct light {
	vec3 direction;
	vec3 color;
	float power;
	vec3 specPower;
};
struct shading {
	vec3 diffuse;
	vec3 specular;
	float rim;
	float distance;
};

shading lambert(in vec3 n, in vec3 v, in light l) {
	shading s;
	vec3 N = normalize(n);	// Normalize the incoming N, L and V vectors
	vec3 L = normalize(l.direction);
	s.diffuse = l.color * saturate(dot(N, L)) ; // Compute the diffuse component for each fragment
	s.specular = vec3(0); // Compute the specular component for each fragment
	s.rim = 0;
	return s;	// Write final color to the framebuffer
}
shading blinPhong1(in vec3 n, in vec3 v, in light l) {
	shading s;
	vec3 N = normalize(n);
	vec3 L = normalize(l.direction);
	vec3 V = normalize(v);
	vec3 H = normalize(L + V);
	float distance = length(l.direction);
	s.distance = distance * distance;
	float diff = dot(L, N);
	s.diffuse = l.color * saturate(diff);

	s.specular = l.color * pow(vec3(saturate(dot(H, N))), l.specPower)
	* smoothstep(0, 1, saturate(diff + 0.01) * 4)
		;
	s.rim = 0;
	return s;
}
shading blinPhong2(in vec3 n, in vec3 v, in light l) {
	shading s;
	vec3 N = normalize(n);
	vec3 L = normalize(l.direction);
	vec3 V = normalize(v);
	vec3 H = normalize(L + V); // Half Dir
	float distance = length(l.direction);
	s.distance = distance * distance;
	float diff = dot(L, N);
	s.diffuse = l.color * saturate(diff);

	vec3 rf = normalize(reflect(-L, N));
	s.specular = l.color * vec3(pow(saturate(dot(rf, V)), l.specPower))
	* smoothstep(0, 1, saturate(diff + 0.01) * 4)
	;
	s.rim = 0;
	return s;	
}
shading blinPhong3(in vec3 n, in vec3 v, in light l) {
	shading s;
	vec3 N = normalize(n);
	vec3 L = normalize(l.direction);
	vec3 V = normalize(v);
	vec3 H = normalize(L + V); // Half Dir
	float distance = length(l.direction);
	s.distance = distance * distance;
	float diff = dot(L, N);
	s.diffuse = l.color * saturate(diff);

	float specAngle = saturate(dot(reflect(-L, N), V));
	s.specular = l.color * pow(vec3(specAngle), l.specPower )
	* smoothstep(0, 1, saturate(diff + 0.01) * 4)
	;
	s.rim = 0;
	return s;
}

void main(void) {
	light l;
	l.direction = fs_in.lightDir;
	l.color = lightColor;
	l.power = lightPower;
	l.specPower = mt.shinines.rgb;
	// color = blinPhong_2(fs_in.normal, fs_in.lightDir, viewPos, 
	// specular_power, specular_albedo,	diffuse_albedo,	fs_in.color1);
	shading s = blinPhong2(fs_in.normal, fs_in.viewPos, l);
	if (gl_FrontFacing) {
		color = vec4(
					(
					// mt.ambient.rgb + mt.emission.rgb 
					// +
					// texture(texture1, fs_in.uv1).rgb
					// *
					s.specular * mt.specular.rgb
					+
					s.diffuse * mt.diffuse.rgb
					*
					texture(texture1, fs_in.uv1).rgb
					),
					mt.diffuse.a);
	}
	else {
		color = vec4(1, 0, 1, 1);
	}


}







