#version 150

in vec2 texCoordVarying;
in vec4 vertexColor;

out vec4 fragColor;

void main(){
	if(abs(sin(vertexColor.y*3)) < 0.5){
		fragColor = vec4(0,0,0,0.4);
		return;
	}
	
	fragColor = vec4(0,0,0,1); 
}
