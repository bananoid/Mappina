#version 120
#extension GL_EXT_gpu_shader4 : enable
uniform vec4 color;

varying vec3 vertWorldPos;
varying vec3 vertWorldNormal;
varying vec3 vertPerlinNoise;

varying vec3 perlinNoise;
uniform float perlinNoiseTime;
uniform float perlinNoiseSize;
uniform float perlinNoiseAmp;

uniform mat4 objToWorld;
uniform mat4 cameraPV;
uniform mat4 normalToWorld;

void calcPerlinNoise(){
  vec3 vertex = (gl_ModelViewProjectionMatrix * gl_Vertex).xyz;
  float perlinNoiseSize = 0.001 * perlinNoiseSize;
  vec3 baseFreq = perlinNoiseSize * vertex;

  vertPerlinNoise.x = sin( baseFreq.x + perlinNoiseTime * 0.9032414) + sin( baseFreq.x * 0.53625 + perlinNoiseTime);
  vertPerlinNoise.x += cos( baseFreq.y + perlinNoiseTime * 0.9837) + cos( baseFreq.y * 0.51345 + perlinNoiseTime);
  vertPerlinNoise.x += sin( baseFreq.z + perlinNoiseTime * 0.89234238) * sin( baseFreq.z * 0.5345892 + perlinNoiseTime);

  vertPerlinNoise.x  = vertPerlinNoise.x + 0.0;
  vertPerlinNoise.x = max(vertPerlinNoise.x, 0.1);
  vertPerlinNoise.x = min(vertPerlinNoise.x, 1.3);
  vertPerlinNoise.x = mix(1 , vertPerlinNoise.x , perlinNoiseAmp);

  //vertPerlinNoise.y = sin(vertex.y + perlinNoiseTime)*.5 + .5;
  //vertPerlinNoise.z = sin(vertex.z + perlinNoiseTime)*.5 + .5;

  // vertPerlinNoise = vec3(1,0,0);
}

void main() {
  vec3 normal = gl_Normal;
  vec3 vertex = (gl_ModelViewProjectionMatrix * gl_Vertex).xyz * 0.000004 - 0.008;

  calcPerlinNoise();

  vertWorldPos = vertex;
  vertWorldNormal = (normalToWorld * vec4(normal,1.0)).xyz;
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  gl_FrontColor = color;

}
