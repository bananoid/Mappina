#version 120
#extension GL_EXT_gpu_shader4 : enable
varying vec3 worldPos;
varying vec3 worldNormal;
noperspective varying vec3 dist;
uniform vec3 cameraPos;
uniform vec3 lightDir;
uniform vec4 singleColor;
uniform float isSingleColor;

uniform float wireThik;
void main() {
  vec3 dist2 = vec3(dist.x,dist.y,dist.z) * 0.3;
  // determine frag distance to closest edge
  float nearD = min(min(dist2[0],dist2[1]),dist2[2]);
  float edgeIntensity = exp2(-1.0*nearD*nearD);
  
  vec3 L = lightDir;
  vec3 V = normalize(cameraPos - worldPos);
  vec3 N = normalize(worldNormal);
  vec3 H = normalize(L+V);
  vec4 color = isSingleColor*singleColor + (1.0-isSingleColor)*gl_Color;
  float amb = 0.3;
  vec4 ambient = color * amb;
  vec4 diffuse = color * (1.0 - amb) * max(dot(L, N), 0.0);
  //vec4 specular = vec4(0.0);
  //edgeIntensity = 0.01;

  // blend between edge color and normal lighting color
  gl_FragColor = (edgeIntensity * vec4(0.1,0.1,0.1,1.0)) + ((1.0-edgeIntensity));
  dist2 *= 0.04;
  vec4 fadeColor = vec4(0,0.4,0.9,1.0) * dist2.x + vec4(0.0,0.1,0.5,1.0) * dist2.y + vec4(0.0,0.2,0.5,1.0) * dist2.z;
  vec4 wireframeColor = vec4(vec3(edgeIntensity),1.);
  gl_FragColor = vec4(vec3(0.0),1.) + wireframeColor * vec4(0.6,0.5,1.,1.) + fadeColor;

}
