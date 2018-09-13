#version 150

// these are for the programmable pipeline system
uniform mat4 modelViewProjectionMatrix;
uniform mat4 transformationMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec4 position;
in vec2 texcoord;

uniform float mouseRange;
uniform vec2 mousePos;
uniform vec4 mouseColor;
uniform sampler2D reflectionTexture;

//uniform sampler2D reflection;

out vec2 textureCoords;

uniform vec4 plane;

void main()
{
    // copy position so we can work with it.
    vec4 pos = position;

	vec4 worldPostion = modelMatrix * pos;

    //gl_ClipDistance[0] = dot(plane, worldPostion);
    // direction vector from mouse position to vertex position.
	vec2 dir = pos.xy - mousePos;
    
    // distance between the mouse position and vertex position.
	float dist =  sqrt(dir.x * dir.x + dir.y * dir.y);
    
    // check vertex is within mouse range.
	if(dist > 0.0 && dist < mouseRange) {
		
		// normalise distance between 0 and 1.
		float distNorm = dist / mouseRange;
        
		// flip it so the closer we are the greater the repulsion.
		distNorm = 1.0 - distNorm;
		
        // make the direction vector magnitude fade out the further it gets from mouse position.
        dir *= distNorm;
        
		// add the direction vector to the vertex position.
		pos.x += dir.x;
		pos.y += dir.y;
	}

	// finally set the pos to be that actual position rendered
	textureCoords = texcoord;
	gl_Position = modelViewProjectionMatrix * pos;
}