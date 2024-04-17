#version 450

layout(binding = 0) uniform UniformBufferObject {
	int is_selected;
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
void main() 
{
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    fragColor = ubo.is_selected == 1 ? vec3(1.0, 1.0, 1.0) : vec3(0.8, 0.8, 0.8);
    fragTexCoord = inTexCoord;
}
