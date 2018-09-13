#version 150

//these are for the programmable pipeline system
uniform mat4 modelViewProjectionMatrix;
uniform mat4 transformationMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec4 plane;

in vec4 position;
in vec2 texcoord;

in vec4 aVertexColor;

out vec4 vertexColor;

uniform vec2 fogParams;

void main(){
	
	float fogBegin  = fogParams.x;
	float fogEnd = fogParams.y;
    // copy position so we can work with it.
	vec4 pos = vec4(position.x, -position.y, position.z, position.w);
	
	vec4 worldPostion = modelMatrix * pos;
	vec4 VP =  viewMatrix * modelMatrix * vec4(pos.xyz,1);	
	float dist = (fogEnd - abs(VP.z))/(fogEnd - fogBegin);
	float s = 200; // range of light
	float fakeLight = clamp((position.x + (s/2))/s, 0.,1.);
	vertexColor = aVertexColor * dist * fakeLight;
    gl_ClipDistance[0] = dot(plane, worldPostion);
		
	//fogCol = vec4(dist,dist,dist,1);
	gl_Position =  modelViewProjectionMatrix * pos;
}