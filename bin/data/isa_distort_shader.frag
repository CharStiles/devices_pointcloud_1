#version 150

in vec2 texCoordVarying;
in vec4 vertexColor;
out vec4 fragColor;

void main(){
	if (vertexColor.r ==0){
		discard;
	}
	fragColor = vertexColor; 
}
