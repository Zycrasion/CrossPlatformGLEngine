#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;

out vec3 normal;
out vec2 uv;
out vec3 fragpos;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main()
{
    normal = mat3(transpose(inverse(model))) * aNormal;
    fragpos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(fragpos, 1.0);
    uv = aUV;
}