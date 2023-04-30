#version 460 core

in vec3 normal;
in vec2 uv;
in vec3 fragpos;

uniform sampler2D diffuse_texture;
uniform vec3 LightPosition;

out vec4 FragColour;

void main()
{
    vec3 d = texture(diffuse_texture, uv).xyz;
    vec3 lightColour = vec3(1.0);

    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * lightColour;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(LightPosition - fragpos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColour * diff;

    vec3 result = (ambient + diffuse) * d;
    FragColour = vec4(result, texture(diffuse_texture, uv).a);
}