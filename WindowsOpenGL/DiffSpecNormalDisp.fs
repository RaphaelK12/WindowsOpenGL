#version 410 core
// Input Built-in Variables
// in vec4 gl_FragCoord;
// in bool gl_FrontFacing;
// in vec2 gl_PointCoord;
struct material {
	vec4 diffuse;		
	vec4 color1;		
	vec4 color2;		
	vec4 color3;		

	vec4 ambient;		
	vec4 emission;		
	vec4 translucenci;	
	vec4 sadowsColor;	

	vec4 specular;		
	vec4 reflex;		
	vec4 shinines;		
};
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
uniform material mt;

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
vec3 unpackNormal(vec3 n, float scale){
	vec3 r=n*2-1;
	return normalize(r*vec3(scale,scale,1));
}
vec3 getNormalFromMap(vec3 n, sampler2D tex, vec2 coord, vec3 WorldPos){
    vec3 tangentNormal = unpackNormal(texture(tex, coord).xyz, 0.5) ;
    vec3 N   = (n);
    vec3 T  = normalize(fs_in.tangent);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);
    return normalize(TBN * tangentNormal);
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

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
// Material properties
uniform vec4 diffuse_albedo = vec4(0.5, 0.2, 0.7, 1);
uniform vec3 specular_albedo = vec3(0.7);
uniform float specular_power = 128.0; // 200
uniform vec3 viewPos;

vec3 lightColor = vec3(1.0, 1.0, 1.0);
float lightPower = 18.0; // 40
vec3 light_pos = vec3(5, 0, 0);

// Ray intersect depth map using relaxed cone stepping.    
// Depth value stored in alpha channel (black at object surface)    
// and relaxed cone ratio stored in blue channel.    
void ray_intersect_relaxedcone(sampler2D relief_map, inout vec3 ray_pos, inout vec3 ray_dir){
	const int cone_steps = 15;
	const int binary_steps = 6;
	ray_dir /= ray_dir.z;  // Scale ray_dir    
	float ray_ratio = length(ray_dir.xy);
	vec3 pos = ray_pos;
	for (int i = 0; i < cone_steps; i++) {
		vec4 tex = texture(relief_map, pos.xy);
		float cone_ratio = tex.z;
		float height = saturate(tex.w - pos.z);
		float d = cone_ratio * height / (ray_ratio + cone_ratio);
		pos += ray_dir * d;
	}
	// Binary search initial range and initial position       
	vec3     bs_range = 0.5 * ray_dir * pos.z;
	vec3 bs_position = ray_pos + bs_range;
	for (int i = 0; i < binary_steps; i++) {
		vec4 tex = texture(relief_map, bs_position.xy);
		bs_range *= 0.5;
		if (bs_position.z < tex.w)
			// If outside       
			bs_position += bs_range;
		// Move forward    
		else
			bs_position -= bs_range;
		// Move backward   
	}
}
vec4 depth2relaxedcone(vec2 TexCoord, sampler2D ReliefSampler, vec3 Offset) {
	const int search_steps = 128;
	vec3 src = vec3(TexCoord, 0);
	// Source texel   
	vec3 dst = src + Offset;
	// Destination texel   
	dst.z = texture(ReliefSampler, dst.xy).w;
	// Set dest. depth    
	vec3 vec = dst - src;
	// Ray direction   
	vec /= vec.z;
	// Scale ray direction so that vec.z = 1.0   
	vec *= 1.0 - dst.z;
	// Scale again    
	vec3 step_fwd = vec / search_steps;
	// Length of a forward step    
	// Search until a new point outside the surface    
	vec3 ray_pos = dst + step_fwd;
	for (int i = 1; i < search_steps; i++) {
		float current_depth = texture(ReliefSampler, ray_pos.xy).w;
		if (current_depth <= ray_pos.z)
			ray_pos += step_fwd;
	}   
	// Original texel depth    
	float src_texel_depth = texture(ReliefSampler, TexCoord).w;
	// Compute the cone ratio    
	float cone_ratio = (ray_pos.z >= src_texel_depth) ? 1.0 : length(ray_pos.xy - TexCoord) / (src_texel_depth - ray_pos.z);
	// Check for minimum value with previous pass result    
	float best_ratio = texture(ReliefSampler, TexCoord).x;
	if (cone_ratio > best_ratio)
		cone_ratio = best_ratio;
	return vec4(cone_ratio, cone_ratio, cone_ratio, cone_ratio);
}

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
	// vec3 n1 = unpackNormal(texture(texture3, fs_in.uv1).rgb);
	N =  getNormalFromMap(n, texture3, fs_in.uv1, fs_in.position_M.xyz);
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
	// s.specular=N;
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
	vec3 N =  getNormalFromMap(fs_in.normal, texture3, fs_in.uv1, fs_in.position_M.xyz);
	vec3 I = normalize(fs_in.position_M.xyz - viewPos);
    vec3 R = reflect(I, N);
	vec2 uv = SampleSphericalMap(R);
	
	if (gl_FrontFacing) {
		color = vec4(
					(
					// mt.ambient.rgb + mt.emission.rgb 
					// +
					// texture(texture1, fs_in.uv1).rgb
					// *
					texture(texture4, uv).rgb
					*
					texture(texture2, fs_in.uv1).rgb
					+
					s.specular * mt.specular.rgb
					*
					texture(texture2, fs_in.uv1).rgb
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







