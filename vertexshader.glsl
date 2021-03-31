#version 400 core
layout (location = 0) in vec3 aPos;

void main(){
	gl_Position = vect4(aPos.x, aPos.y, aPox.z, 1.0);
	// forth element = perspective division
	// whatever we set gl_Position to will be used as the output of the vertex shader
}
