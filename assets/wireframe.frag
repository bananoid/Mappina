#version 120
#extension GL_EXT_gpu_shader4 : enable
varying vec3 worldPos;
varying vec3 worldNormal;

varying vec3 perlinNoise;

noperspective varying vec3 dist;
noperspective varying vec3 activeVetex;
uniform vec3 cameraPos;
uniform vec3 lightDir;
uniform vec4 singleColor;
uniform float isSingleColor;

uniform float zDepthMult;
uniform float zDepthAdd;

uniform float wireAmp = 1.0;
uniform float fillAmp = 0.01;

uniform float perlinNoiseTime;
uniform float insetWFMix = 0;

vec3 wfSideFade(float wlGradient, float wlPos){
  // wlGradient = 2;
  // wlPos = 0.9;
  float mapWlPos = wlPos * (2+1/wlGradient) -1;
  vec3 wireLenght = ((vec3(activeVetex)-0.5) * 2) * wlGradient + mapWlPos * wlGradient;
  wireLenght = clamp(wireLenght,0,1);
  return wireLenght;
}

void main() {
  vec3 dist2 = vec3(dist.x,dist.y,dist.z) * 0.3;
  // determine frag distance to closest edge
  float nearD = min(min(dist2[0],dist2[1]),dist2[2]);
  float edgeIntensity = exp2(-1.0*nearD*nearD);

  // vec3 L = lightDir;
  // vec3 V = normalize(cameraPos - worldPos);
  // vec3 N = normalize(worldNormal);
  // vec3 H = normalize(L+V);
  // vec4 color = isSingleColor*singleColor + (1.0-isSingleColor)*gl_Color;
  // float amb = 0.3;
  // vec4 ambient = color * amb;
  // vec4 diffuse = color * (1.0 - amb) * max(dot(L, N), 0.0);
  //vec4 specular = vec4(0.0);
  //edgeIntensity = 0.01;

  vec4 perlinNoiseColor = vec4( vec3(perlinNoise.x) ,1.);

  vec4 zDepthColor = vec4(vec3(1.0-(worldPos.z * 400 * zDepthMult) + 0.8 + zDepthAdd)  ,1.0);
  //gl_FragColor = zDepthColor;

  // blend between edge color and normal lighting color
  gl_FragColor = (edgeIntensity * vec4(0.1,0.1,0.1,1.0)) + ((1.0-edgeIntensity));
  dist2 *= 0.04;
  vec4 fadeColor = vec4(0.3,0.4,0.9,1.0) * dist2.x + vec4(0.01,0.1,0.5,1.0) * dist2.y + vec4(0.01,0.2,0.5,1.0) * dist2.z;
  vec4 wireframeColor = vec4(vec3(edgeIntensity),1.);

  vec3 insetWF = vec3( cos(nearD * 1.0 + perlinNoiseTime*2) * 1.5);
  vec4 insetWFColor = mix( vec4(insetWF,1.0) , vec4(1.0) , 1-insetWFMix);

  // wireLenght = vec3(activeVetex);
  // wireLenght = wireLenght/wlGradient;

  vec3 wfSideFade = wfSideFade(5, perlinNoise.x);
  float wfSideFadeF = (wfSideFade.x + wfSideFade.y + wfSideFade.z );

  vec4 wireLenghtColor = vec4(vec3(wfSideFadeF),1);

  gl_FragColor = zDepthColor * (vec4(vec3(0.0),1.) + wireAmp  * wireframeColor * wfSideFadeF * vec4(0.3,0.5,1.,1.) + fadeColor *perlinNoise.x* fillAmp * max(insetWFColor,0) );
  //gl_FragColor = zDepthColor;

  // gl_FragColor *= perlinNoiseColor ;

  // gl_FragColor = wireLenghtColor;
}
