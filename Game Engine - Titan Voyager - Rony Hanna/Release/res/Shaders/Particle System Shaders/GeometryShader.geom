#version 440 core
 
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 vertex_uv;

void DrawQuad(vec4 Pos)
{
	gl_Position = (Pos + vec4(-0.5f, -0.5f, 0.0f, 0.0f)); // Bottom Left       
    vertex_uv = vec2(0.0f, 1.0f);
	EmitVertex();                             
                                              
	gl_Position = (Pos + vec4(0.5f, -0.5f, 0.0f, 0.0f)); // Bottom Right   
	vertex_uv = vec2(1.0f, 1.0f);
	EmitVertex();                             
                                              
	gl_Position = (Pos + vec4(-0.5f, 0.5f, 0.0f, 0.0f)); // Top Left    
	vertex_uv = vec2(0.0f, 0.0f);
	EmitVertex();                             
                                              
	gl_Position = (Pos + vec4(0.5f, 0.5f, 0.0f, 0.0f)); // Top Right
	vertex_uv = vec2(1.0f, 0.0f);
	EmitVertex();
 
    EndPrimitive();
}
 
void main() 
{
	DrawQuad(gl_in[0].gl_Position);
}