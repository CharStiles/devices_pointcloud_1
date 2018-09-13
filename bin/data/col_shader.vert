#version 150

//these are for the programmable pipeline system
uniform mat4 modelViewProjectionMatrix;
uniform mat4 transformationMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec4 plane;
uniform sampler2D tex0;

in vec4 position;
in vec2 texcoord;

in vec4 aVertexColor;

out vec4 vertexColor;
out vec2 texCoordVarying;
out vec4 fogCol;
out float fakeLight;

uniform vec2 fogParams;

void main(){
	
	float fogBegin  = fogParams.x;
	float fogEnd = fogParams.y;

	vec4 worldPostion = modelMatrix * position;
    gl_ClipDistance[0] = dot(plane, worldPostion);

	float s = 20; // range of light
	fakeLight = clamp((position.y + (s/2))/s, 0.,1.);
	float f = clamp((position.z + (20/2))/20, 0.,1.);

	vec4 VP =  viewMatrix*modelMatrix* vec4(position.xyz,1);	

	float dist = (fogEnd - abs(VP.z))/(fogEnd - fogBegin);

	fogCol = vec4(dist,dist,dist,f) ;

	vertexColor = (aVertexColor);
	texCoordVarying = texcoord;
	gl_Position =  modelViewProjectionMatrix * position;
}