#version 430 core
out vec4 FragColor;

in vec3 TexCoords;

struct Material {
    samplerCube cubemap;
}; 

uniform Material material;

void main()
{    
    FragColor = texture(material.cubemap, TexCoords);
}