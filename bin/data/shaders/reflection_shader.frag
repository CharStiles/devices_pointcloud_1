#version 150

in vec2 textureCoords;

out vec4 outputColor;

uniform vec4 mouseColor;
 
void main()
{
    vec4 reflectionColor = texture(reflectionTexture, textureCoords);

   outputColor = vec4(1)-mouseColor;
}