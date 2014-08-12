/*
 Built-in variables:
 - vec4  gl_FragCoord
 - bool  gl_FrontFacing
 - vec4  gl_FragColor
 - vec4  gl_FragData[gl_MaxDrawBuffers]
 - float gl_FragDepth
 
 Varying variables:
 - vec4  gl_Color
 - vec4  gl_SecondaryColor
 - vec4  gl_TexCoord[]
 - float gl_FogFragCoord
 - vec2  gl_PointCoord
 */

//Declare a 2D texture as a uniform variable
uniform sampler2D texture;
varying vec4 Color;
varying vec4 pos;
varying vec3 relPos;

uniform float lineOff;
uniform float lineOff2;

void main()
{
	vec4 textColor = gl_Color * texture2D(texture, gl_TexCoord[0].xy);
	
	float x = sin(gl_TexCoord[0].x*100.0 + lineOff)+.3;
	float y = sin(relPos.x*100.0 + lineOff)+.3;
	vec4 zebraColor = vec4(y,y,y,1.);
	
	vec4 color2 = gl_FragCoord;
	
	vec4 uvColor = vec4(gl_TexCoord[0].x,0.,gl_TexCoord[0].y,1.);
	
	float cR = 100.0;
	float circle = sin(uvColor.r*cR) * cos(uvColor.g*cR) * 4. + 0.5;
	vec4 circleColor = vec4(circle,circle,circle,1.);
	//gl_FragColor = zebraColor;
	gl_FragColor = uvColor + zebraColor;
	//gl_FragColor = circleColor;
	gl_FragColor = vec4(pos.x,pos.y,pos.z,0.);
	gl_FragColor = vec4(relPos.x,relPos.y,relPos.z,0.)+zebraColor;
	
  
}
