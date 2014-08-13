#version 120
#extension GL_EXT_gpu_shader4 : enable
in vec3 vertex;
in vec4 color;
in vec3 normal;
varying vec3 vertWorldPos;
varying vec3 vertWorldNormal;
uniform mat4 objToWorld;
uniform mat4 cameraPV;
uniform mat4 normalToWorld;

void main() {
  vertWorldPos = (objToWorld * vec4(vertex,1.0)).xyz;
  vertWorldNormal = (normalToWorld * vec4(normal,1.0)).xyz;
  gl_Position = cameraPV * objToWorld * vec4(vertex,1.0);
  gl_FrontColor = color;
}
