#version 150

in vec2 texCoordVarying;
in vec4 vertexColor;
uniform sampler2D tex0;
out vec4 fragColor;
in vec4 fogCol;
in float fakeLight;

void main(){
	vec3 base = vec3(0);
	vec3 blend = texture(tex0,texCoordVarying).xyz;
	fragColor = vec4(mix(base,blend,0.9)* fakeLight,1) * fogCol; 
}
