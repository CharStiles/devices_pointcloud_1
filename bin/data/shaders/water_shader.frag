#version 150

in vec2 textureCoords;

out vec4 outputColor;

uniform vec4 mouseColor;
uniform sampler2D reflectionTexture;
 
void main()
{
    vec4 reflectionColor = texture(reflectionTexture, textureCoords);

   outputColor = mix(mouseColor, reflectionColor, 0.5);
}