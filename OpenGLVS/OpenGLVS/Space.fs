#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube SpaceBox;

void main()
{    
    FragColor = texture(SpaceBox, TexCoords);
}