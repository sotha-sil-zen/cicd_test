#version 450

layout(set = 0, binding = 0) uniform ViewUniforms 
{
    float grid_width;
    float grid_space;
    mat4 view;
    mat4 proj;
} gubo;
layout(location = 1) in vec3 nearPoint;
layout(location = 2) in vec3 farPoint;
layout(location = 0) out vec4 outColor;

vec4 grid(vec3 fragPos3D, float scale, bool drawAxis) 
{
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    if( mod(fragPos3D.x , gubo.grid_space) < gubo.grid_width)
    {
    	color.xyz = vec3(0.2);
    	color.w = 0.5;
        if(fragPos3D.x>=-gubo.grid_width&&fragPos3D.x<=gubo.grid_width)
        {
            color.xyz = vec3(0.0, 0.0, 1.0);
        }
    }
    if( mod(fragPos3D.z , gubo.grid_space) < gubo.grid_width)
    {
    	color.xyz = vec3(0.2);
    	color.w = 0.5;
        if(fragPos3D.z>=-gubo.grid_width&&fragPos3D.z<=gubo.grid_width)
        {
            color.xyz = vec3(1.0, 0.0, 0.0);
        }
    }
    
    return color;
}
float computeDepth(vec3 pos) 
{
    vec4 clip_space_pos = gubo.proj * gubo.view * vec4(pos.xyz, 1.0);
    return (clip_space_pos.z / clip_space_pos.w);
}
void main() {
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);
    gl_FragDepth = computeDepth(fragPos3D);
    outColor = grid(fragPos3D, 10, true) * float(t > 0);
}
