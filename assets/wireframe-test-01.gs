#version 120
#extension GL_EXT_gpu_shader4 : enable
#extension GL_EXT_geometry_shader4 : enable
varying in vec3 vertWorldPos[3];
varying in vec3 vertWorldNormal[3];
varying out vec3 worldNormal;
varying out vec3 worldPos;
uniform vec2 WIN_SCALE;
noperspective varying vec3 dist;

varying float area;
varying float perimeter;

void main(void)
{
  // taken from 'Single-Pass Wireframe Rendering'
  //vec2 p0 = WIN_SCALE * gl_PositionIn[0].xy/gl_PositionIn[0].w;
  //vec2 p1 = WIN_SCALE * gl_PositionIn[1].xy/gl_PositionIn[1].w;
  //vec2 p2 = WIN_SCALE * gl_PositionIn[2].xy/gl_PositionIn[2].w;

  vec4 p0 = gl_PositionIn[0];
  vec4 p1 = gl_PositionIn[1];
  vec4 p2 = gl_PositionIn[2];
	
  vec4 v0 = p2-p1;
  vec4 v1 = p2-p0;
  vec4 v2 = p1-p0;

  float l0 = length(v0);
  float l1 = length(v1);
  float l2 = length(v2);

  perimeter = (l0 + l1 + l2) * .5; 
  
  area = sqrt(perimeter * (perimeter -l0) * (perimeter -l1) * (perimeter -l2));

  dist = vec3(l0,0,0);
  worldPos = vertWorldPos[0];
  worldNormal = vertWorldNormal[0];
  gl_Position = gl_PositionIn[0];
  EmitVertex();

  dist = vec3(0,l1,0);
  worldPos = vertWorldPos[1];
  worldNormal = vertWorldNormal[1];
  gl_Position = gl_PositionIn[1];
  EmitVertex();

  dist = vec3(0,0,l2);
  worldPos = vertWorldPos[2];
  worldNormal = vertWorldNormal[2];
  gl_Position = gl_PositionIn[2];
  EmitVertex();
  EndPrimitive();
}
