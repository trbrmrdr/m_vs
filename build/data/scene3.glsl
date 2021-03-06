// Blobs

#ifdef GL_ES
precision highp float;
#endif

uniform float time;
//uniform float lowFreq;
uniform float midFreq;
uniform vec3 iMouse;
uniform vec2 iResolution;

uniform vec2 val_1;

uniform sampler2D backbuffer;
//varying vec2 v_uv;

float makePoint(float x,float y,float fx,float fy,float sx,float sy,float t){
    float xx=x+sin(t*fx)*sx;
    float yy=y+cos(t*fy)*sy;
    //float xx=x+sin(fx)*sx;
    //float yy=y+cos(fy)*sy;
    return 1.0/sqrt(xx*xx+yy*yy);
}



void fill(float d){
    float br = 10.;
    if(d>1.)return;
    float tVal = br / d;
    tVal = sqrt(br - d*d);
    gl_FragColor.rgb += vec3(0.3, 0.15, 0.45) * tVal;
}

void point(vec2 pos){
    vec2 P = gl_FragCoord.xy;
    float d = distance(P, pos);
    //glow(d);
    //if(d<20.)
    //glow(d,20.1,vec3(0.13, 0.15, 0.15));
    fill(d);
}

void main( void ) {
    float weight = 10.1;//50.0 * lowFreq;// * lowFreq;
    vec2 p=(gl_FragCoord.xy/iResolution.x)*2.0-vec2(1.0,iResolution.y/iResolution.x);

    p=p*1.5;


    float percent = iMouse.x/iResolution.x;
    //percent = (1.+midFreq*.00000001);
    //p = iMouse.xy*2.-vec2(1.,iResolution.y/iResolution.x);
    vec2 m = vec2(1.,10.001);//iMouse/iResolution;
    p *= cos(time);
    float x=p.x;
    float y=p.y;

    //time = 0.05*time;
    //time = 1.;
    float a=makePoint(x,y,m.x,m.y,1.3,0.3,time);
    //float a=makePoint(x,y,3.3,2.9,0.3,0.3,time);
    //a=a+makePoint(x,y,1.9,2.0,0.4,0.4,time);
    //a=a+makePoint(x,y,0.8,0.7,0.4,0.5,time);

    //a=a+makePoint(x,y,1.4,1.7,0.4,0.5,time);
    //a=a+makePoint(x,y,1.3,2.1,0.6,0.3,time);
    //a=a+makePoint(x,y,1.8,1.7,0.5,0.4,time);   

    float b=makePoint(x,y,1.2,.9,.2,0.3,time);
    //b=b+makePoint(x,y,0.7,2.7,0.4,0.4,time);
    //b=b+makePoint(x,y,1.4,0.6,0.4,0.5,time);

    //b=b+makePoint(x,y,1.4,0.9,0.6,0.3,time);
    //b=b+makePoint(x,y,0.7,1.3,0.5,0.4,time);

    float c=makePoint(x,y,3.7,0.3,0.3,0.3,time);
    //c=c+makePoint(x,y,1.9,1.3,0.4,0.4,time);

    //c=c+makePoint(x,y,0.2,0.6,0.6,0.3,time);
    //c=c+makePoint(x,y,1.3,0.5,0.5,0.4,time);

    //vec3 d=vec3(a,b,c)*.012;
    vec3 d=vec3(a,0,0)*.032;

    gl_FragColor = weight * vec4(d.x,d.y,d.z,1.0);
    //point(iMouse*iResolution);
    if(false){
        float a=iMouse.y*10.0;
        float b= cos(iMouse.y)*32.;
        float c=sin(iMouse.x)*13.0;
        float d= .23;
        vec2 ttt = gl_FragCoord.xy/iResolution.xy;
        
        float Xn = ttt.x;
        float Yn = ttt.y;
        float v1 = sin(a * Yn) - cos(b * Xn);
        float  v2 = sin(c * Xn) - cos(d * Yn);
        gl_FragColor = vec4(v1,v2,1.,1.);
    }

    point(val_1*iResolution);
}
