#version 150

in vec4 position;
in vec2 texcoord;

out vec4 clipSpace;
out vec3 toCameraVector;
out vec3 fromLightVector;
out vec2 textureCoords;
out vec4 fogCol;

uniform float moveFactor;
uniform sampler2D dudvMap;
uniform vec3 cameraPosition;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 lightPosition;

float waveStrength = 0.005;

uniform vec2 fogParams;

void main(){
	
	float fogBegin  = fogParams.x;
	float fogEnd = fogParams.y;

    vec2 modTexCoords = texcoord * 7;
    float moveModY = modTexCoords.y + moveFactor;
    float moveModX = modTexCoords.x + moveFactor; // these are to remidy still water at times

    vec2 distortedTexCoords = texture(dudvMap, vec2(moveModX, modTexCoords.y)).rg*0.1;
	distortedTexCoords = modTexCoords + vec2(distortedTexCoords.x, moveModY);
	vec2 distort = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength;

	vec4 pos = vec4(position.x , (position.y ) , position.z, position.w );

	vec4 worldPosition = modelMatrix *  vec4(pos.x, distort.x * -2000., pos.y ,1);
	vec4 VP =  viewMatrix*modelMatrix* vec4(pos.xyz,1);

	textureCoords = texcoord;
	clipSpace = projectionMatrix * viewMatrix * worldPosition;
	toCameraVector = cameraPosition - worldPosition.xyz;

	//finally set the pos to be that actual position rendered
	gl_Position = clipSpace;
	float dist = (fogEnd - abs(VP.z))/(fogEnd - fogBegin);
	fogCol = vec4(dist,dist,dist,1);
	fromLightVector = worldPosition.xyz - lightPosition;
}