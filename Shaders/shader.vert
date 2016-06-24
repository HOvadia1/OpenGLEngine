#version 330

in vec2 vertexPosition;

void main()
{
	gl_Position.xy = vertexPosition;
	gl_Position.z = 0.0; //z = 0 since 2D
	gl_Position.w = 1.0; //normalize coordinates
}