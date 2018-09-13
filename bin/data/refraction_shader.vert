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

void main(){
	
	vec4 pos = vec4(position.x, -position.y, position.z, position.w);
	
	vec4 worldPostion = modelMatrix * pos;
	vertexColor = aVertexColor;
    gl_ClipDistance[0] = dot(plane, worldPostion);
	
	gl_Position =  modelViewProjectionMatrix * pos;
}