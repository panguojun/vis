attribute float y;
varying float vy;
void main() 
{
	gl_FrontColor = gl_Color;
	gl_Position = ftransform();
	vy = y;
}