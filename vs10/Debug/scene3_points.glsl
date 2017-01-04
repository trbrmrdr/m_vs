//
// Blobs

#ifdef GL_ES
precision highp float;
#endif
uniform float time;
uniform float lowFreq;
uniform vec3 iMouse;
uniform vec2 iResolution;

float makePoint(float x,float y,float fx,float fy,float sx,float sy,float t){
   float xx=x+sin(t*fx)*sx;
   float yy=y+cos(t*fy)*sy;
   return 1.0/sqrt(xx*xx+yy*yy);
}


void main( void ) {
   float weight = 1.1;//50.0 * lowFreq;// * lowFreq;
   vec2 p=(gl_FragCoord.xy/iResolution.x)*2.0-vec2(1.0,iResolution.y/iResolution.x);

   p=p*1.5;
   
   float x=p.x;
   float y=p.y;

   time = 1.0;
   float a=makePoint(x,y,3.3,2.9,0.3,0.3,time);
   a=a+makePoint(x,y,1.9,2.0,0.4,0.4,time);
   a=a+makePoint(x,y,0.8,0.7,0.4,0.5,time);

   a=a+makePoint(x,y,1.4,1.7,0.4,0.5,time);
   a=a+makePoint(x,y,1.3,2.1,0.6,0.3,time);
   a=a+makePoint(x,y,1.8,1.7,0.5,0.4,time);   
   
   float b=makePoint(x,y,1.2,1.9,10000.0,0.3,time);
   b=b+makePoint(x,y,0.7,2.7,0.4,0.4,time);
   b=b+makePoint(x,y,1.4,0.6,0.4,0.5,time);
 
   b=b+makePoint(x,y,1.4,0.9,0.6,0.3,time);
   b=b+makePoint(x,y,0.7,1.3,0.5,0.4,time);

   float c=makePoint(x,y,3.7,0.3,0.3,0.3,time);
   c=c+makePoint(x,y,1.9,1.3,0.4,0.4,time);
  
   c=c+makePoint(x,y,0.2,0.6,0.6,0.3,time);
   c=c+makePoint(x,y,1.3,0.5,0.5,0.4,time);
   
   vec3 d=vec3(a,b,c)/32.0;
   
   gl_FragColor = weight * vec4(d.x,d.y,d.z,1.0);
}
