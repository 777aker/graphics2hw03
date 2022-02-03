#version 120

// model and light from vertex shader

varying vec3 View;
varying vec3 Light;
varying vec3 Normal;
varying vec4 Ambient;

varying vec3 ModelPos;

uniform float time;

uniform sampler2D tex;

vec4 blinn()
{
   //  N is the object normal
   vec3 N = normalize(Normal);
   //  L is the light vector
   vec3 L = normalize(Light);
   //  V is the view vector
   vec3 V = normalize(View);

   //  Emission and ambient color
   vec4 color = Ambient;

   //  Diffuse light is cosine of light and normal vectors
   float Id = dot(L,N);
   if (Id>0.0)
   {
      //  Add diffuse
      color += Id*gl_FrontLightProduct[0].diffuse;
      //  The half vectors
      vec3 H = normalize(V+L);
      //  Specular is cosine of reflected and view vectors
      float Is = dot(H,N);
      if (Is>0.0) color += pow(Is,gl_FrontMaterial.shininess)*gl_FrontLightProduct[0].specular;
   }

   //  Return sum of color components
   return color;
}

void main() {
	// build our noise 
	// add time to the y so we get the snow falling effect
	float noise = fract(sin(dot(floor((gl_TexCoord[0].xy+vec2(0,time*.25))*10), vec2(12.9898, 78.233))) * 43758.5453);
	// clamp it so it looks cool
	noise = clamp(step(.8, noise) * (ModelPos.y+1), 0, 1)*5;
	// construct our base color + the snow
	vec4 color = vec4(noise+.2, noise+.2, noise+.4, 1);
	// make the color fade as you go down
	color *= ModelPos.y+1;
	// make the top a cloud
	color += step(1-ModelPos.y, .2)*4;
	// make the snow build up on the ground
	if(gl_TexCoord[0].y < .5)
		color += step((1-abs(ModelPos.y)*time*.25), .2)*4;
	// make the bottom invisible because it's dark and otherwise bottom face looks silly
	color *= step(-ModelPos.y-.8, 0);
	if(ModelPos.y <= -.8)
		discard;
	// actually do our stuff
	gl_FragColor = blinn() * color;
}