#version 450
#extension GL_NV_fragment_shader_barycentric : enable
layout(binding = 0) uniform UniformBufferObject {
	int is_selected;
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;
layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;
void main() 
{
    vec4 textureSample = texture(texSampler, fragTexCoord);
    vec3 wireframeColor = vec3(1.0, 1.0, 1.0);
	
	vec3 barycentric = gl_BaryCoordNV;
	
	// The horizontal and vertical change in barycentric coordinates
	// in screen-space
	const vec3 dBaryCoordX = dFdx(barycentric);
	const vec3 dBaryCoordY = dFdy(barycentric);
	
	const vec3 dBaryCoord  = sqrt(dBaryCoordX*dBaryCoordX + dBaryCoordY*dBaryCoordY);

	const float Thickness = 0.1; // In pixels
	const float Falloff = 0.5;   // In pixels

	const vec3 Remap = smoothstep(
		dBaryCoord * Thickness,
		dBaryCoord * (Thickness + Falloff),
		barycentric
	);
	
	// Get the closest edge
	const float Wireframe = min(Remap.x, min(Remap.y, Remap.z));
	
	if(ubo.is_selected == 1 &&(Wireframe<0.5))
	{
		outColor = vec4(wireframeColor, 1.0);
	}
	else
	{
		outColor = vec4(fragColor * textureSample.rgb, textureSample.a);
	}
}
