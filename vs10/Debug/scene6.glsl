//demo - after edit

#ifdef GL_ES
precision mediump float;
#endif

//varying vec2 surfacePosition;
uniform vec2 resolution;
uniform float time;
uniform vec2 mouse;


const float color_intensity = 0.45;
const float Pi = 3.14159;


float adnan (float scale) {
  return 5. * (1. - sin(time*scale) ) / 2.;

}

void main()
{
  vec2 surfacePosition = gl_FragCoord/resolution;
  float speed  =mouse.x/resolution.x*100;
  vec2 var2 = vec2(.1,.1);//time*0.1);//mouse;//vec2(0.5,0.5);
  float var0 = 0.5;
  vec2 var1 = mouse;
  var1 = vec2(1.500,1.00)*.5;
	//r1 = vec2(var1.x,var2.y);
  //vec2 p=(mouse.x * 10. *surfacePosition);
  vec2 p=(var1.x * 10. *surfacePosition);
  for(int i=1;i<300;i++)
  {
    vec2 newp=p;
    newp.x+=var2.x/float(i)*sin(float(i)*Pi*p.y+time*.1*speed);
    newp.y+=var2.y/float(i)*cos(float(i)*Pi*p.x+time*.1*speed)-1.;
  if (mod(float(i),2.) == 0.)
     p=newp;
  else
     p = 1. - newp;
  }
	float t1 = adnan(cos(time/surfacePosition.x)*.000001);
	float t0 = adnan(sin(time/surfacePosition.y)*.000001);//5.; 
 vec3 col=vec3(sin(p.x+p.y + t0)*.5+.5,
  		sin(p.x+ t1)*.5+.5,
  		sin(p.x+p.y+ var1.y * 10.)*.5+.5);
	
  //vec3 col=vec3(sin(p.x+p.y + t0)*.5+.5,
  //		sin(p.x+p.y+ t1)*.5+.5,
  //		sin(p.x+p.y+ var1.y * 10.)*.5+.5);
  gl_FragColor=vec4(col* col, 1.);
}
