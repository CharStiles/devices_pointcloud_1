#version 150

//these are for the programmable pipeline system
uniform mat4 modelViewProjectionMatrix;
uniform mat4 transformationMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec4 plane;

in vec4 position;
in vec2 texcoord;
in float time;

in vec4 aVertexColor;

out vec4 vertexColor;

uniform vec2 fogParams;

void main(){
	
	float fogBegin  = fogParams.x;
	float fogEnd = fogParams.y;
    // copy position so we can work with it.
	vec4 pos = vec4(position.x, -position.y, position.z, position.w);
	
	//apply distortion
    float size = 1;
    vec3 distortMid = vec3(0,0,0);
    float fromMid = length(pos.xyz - distortMid);
    float t = time/10000;
    float inside = cos(5.+t/5.);
    float distort = size / abs(fromMid*( cos(t+fromMid* (cos(2.+t/2.+fromMid*inside)+1) +2 ) ) );
	//pos.z = pos.z + (distort/10000000000.);

	vec4 worldPostion = modelMatrix * pos;
	vec4 VP =  viewMatrix * modelMatrix * vec4(pos.xyz,1);	
	float dist = (fogEnd - abs(VP.z))/(fogEnd - fogBegin);
	float s = 200; // range of light
	float fakeLight = clamp((position.x + (s/2))/s, 0.,1.);
	vertexColor = aVertexColor * dist * fakeLight * distort + vec4(0,0,0,1);
    gl_ClipDistance[0] = dot(plane, worldPostion);
		
	//fogCol = vec4(dist,dist,dist,1);
	gl_Position =  modelViewProjectionMatrix * pos;
}

// #version 150
// #ifdef GL_ES
// precision mediump float;
// #endif


// //bringbuggyCircles
// uniform vec2  resolution;
// uniform float time;
// uniform vec3 spectrum;
// out vec4 fragColor;

// vec3 hsv2rgb( vec3 c )
// {
//     return c.z * (1. - c.y * smoothstep(2.,1., abs( mod( c.x*6.+vec3(0,4,2), 6.) -3.) ));
// }

// void main() {
//     float x = resolution.x / 2.0;
//     float y = resolution.y / 2.0;
//     float size = 1;
//     vec2  pos = vec2(x, y);
//     float dist = length(gl_FragCoord.xy - pos) ;
//     float t = time/10000;
//     float inside = cos(5.+t/5.);
//     float color = size / abs(dist*( cos(t+dist* (cos(2.+t/2.+dist*inside)+1) +2 ) ) );
//     float color2 = size*20 / abs(dist*(cos(t+dist*(sin(2.+t/2.+dist*inside) ))));
//     fragColor = vec4(hsv2rgb(vec3(color,1,color2)),1);

// }