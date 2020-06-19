#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 256) out;

out vec3 TexCoord;

uniform float radius;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform samplerCube bump;

//Division of a equilateral Triangle into 4 sub equilateral triangles
//Array S will store 12 vertices of the 4 equilateral triangles formed
void CreateSubTriangles(vec4 P1, vec4 P2, vec4 P3, out vec4 S[12]) 
{
	S[0] = (P1 + P2)/2;
	S[1] = (P2 + P3)/2;
	S[2] = (P3 + P1)/2;
	S[3] = P1;
	S[4] = S[0];
	S[5] = S[2];
	S[6] = S[0];
	S[7] = P2;
	S[8] = S[1];
	S[9] = P3;
	S[10] = S[2];
	S[11] = S[1]; 
}

void main()
{ 
	float bumpv;                            //to provide terrain

	vec4 P1 = gl_in[0].gl_Position;		//Vertices of main triangle
	vec4 P2 = gl_in[1].gl_Position;
	vec4 P3 = gl_in[2].gl_Position;

	vec4 S1[12], S2[12], S3[12], S4[12];	//Division level 1,2,3,4 respectively

	CreateSubTriangles(P1, P2, P3, S1);
	for(int i=0;i<4;i++)
	{
	CreateSubTriangles(S1[3*i], S1[3*i+1], S1[3*i+2], S2);
	for(int j=0;j<4;j++)
	{
	CreateSubTriangles(S2[3*j], S2[3*j+1], S2[3*j+2], S3);
	for(int k=0;k<4;k++)
	{
	TexCoord = radius*normalize(vec3(S3[3*k]));
	bumpv = length(texture(bump, TexCoord))/50;
	gl_Position = projection*view*model*vec4(radius*(1+bumpv)*normalize(vec3(S3[3*k])),1.0f);
	EmitVertex();
	TexCoord = radius*normalize(vec3(S3[3*k+1]));
	bumpv = length(texture(bump, TexCoord))/50;
	gl_Position = projection*view*model*vec4(radius*(1+bumpv)*normalize(vec3(S3[3*k+1])),1.0f);	
	EmitVertex();
	TexCoord = radius*normalize(vec3(S3[3*k+2]));
	bumpv = length(texture(bump, TexCoord))/50;
	gl_Position = projection*view*model*vec4(radius*(1+bumpv)*normalize(vec3(S3[3*k+2])),1.0f);
	EmitVertex();
	EndPrimitive();	
	}
	}
	}
}