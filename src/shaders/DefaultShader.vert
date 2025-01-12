#version 450

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec3 fragColor;

void main()
{
    gl_Position = vec4(inPosition[gl_VertexIndex], 0.0, 1.0);
    fragColor = colors[gl_VertexIndex];
}