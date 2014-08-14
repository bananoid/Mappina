#version 120
#extension GL_EXT_gpu_shader4 : enable
uniform vec4 color;

varying vec3 vertWorldPos;
varying vec3 vertWorldNormal;
uniform mat4 objToWorld;
uniform mat4 cameraPV;
uniform mat4 normalToWorld;

void main() {
  vec3 normal = gl_Normal;
  vec3 vertex = (gl_ModelViewProjectionMatrix * gl_Vertex).xyz * 0.000004 - 0.008;

  vertWorldPos = vertex;
  vertWorldNormal = (normalToWorld * vec4(normal,1.0)).xyz;
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  gl_FrontColor = color;
}
