
uniform vec3 iMouse;
uniform vec2 iResolution;
uniform float iGlobalTime;


vec3 h2rgb( float hue ){
    
  float h = fract(hue) *5.;//   (4.+floor(6.*((1.+sin(iGlobalTime*.5))*.5)));//5.;
  vec3 c = vec3( 0., 0., 0. );
   
  int f = int(floor( h ));// + floor(5.*((1.+sin(iGlobalTime*.00005))*.5));
    
  if(f==0)
      c=vec3(1.,h,0.);
  else if(f==1)
      c=vec3(2.-h,1.,0.);
  else if(f==2)
      c=vec3(0.,1.,h-2.);
  else if(f==3)
      c=vec3(0.,4.-h,1.);
  else if(f==4)
      c=vec3(h-4.,0.,1.);
  else 
      c=vec3(1.,0.,6.-h);
  return c;
}

float mod2( float x, float y ){
	return fract(x*y)*y;
}

void main()
{
	vec2 q = gl_FragCoord.xy;
    
    float light = mod2( iGlobalTime / 50000. + 
                        //sin( q.y*q.y*q.y / 1e10 ) + 
                        sin( q.y*q.y / 1e10 ) + 
                        cos( q.x * 3. )
                        , iResolution.y );
	gl_FragColor = vec4( h2rgb( q.x / iResolution.x ) / light, 1 );
}