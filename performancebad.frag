//  Per-pixel Blinn-Phong lighting
//  Fragment shader
#version 120

varying vec3 Light;

uniform float time;

varying vec3 ModelPos;

//http://www.science-and-fiction.org/rendering/noise.html
float rand3D(in vec3 co) {
    return fract(sin(dot(co.xyz, vec3(12.9898, 78.233, 144.7272))) * 43758.5453);
}

void main()
{
   //gl_FragColor = blinn() * texture2D(tex,gl_TexCoord[0].xy);
   float dis = clamp(1-distance(Light,ModelPos)*11/75, 0, 5)*2.5;
   vec4 color = vec4(dis, dis, dis, 1);
   color += (fract(sin(dot(floor((ModelPos-time/5)*10), vec3(12.9898, 78.233, 144.7272))) * 43758.5453)<= 0.2) ? 1 : 0;
   gl_FragColor = color;

   /* sorry, just ignore this please. it's some cool stuff I was messing with
   float dis = distance(Light,ModelPos);
   vec4 color = vec4(clamp(1-(dis/6+dis/5)/2.5, 0, 1), clamp((1-dis/6)/2, 0, 1), 0, 1);
   
   if(length(color) <= 1.01) {
       if(rand3D(floor((ModelPos-time/5)*10)) <= 0.2) {
           gl_FragColor = vec4(1,1,1,1);
       } else {
           gl_FragColor = vec4(0,0,0,1);
       }
   } else {
       gl_FragColor = color;
   }
   */
}
