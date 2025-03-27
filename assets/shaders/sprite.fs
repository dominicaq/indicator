#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D spriteTexture;

void main()
{
    // Sample the texture per fragment
    FragColor = texture(spriteTexture, TexCoord);
}
