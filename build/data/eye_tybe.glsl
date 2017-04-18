
#if 0 //original

#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
varying vec2 surfacePosition;
float saturate(float x){return clamp(x,0.,1.);}
vec4 tex(vec2 u){
	vec2 p=fract(fract(u)+(sin(u.y+time)/4.));
	float f=1.-saturate((max(length(p-vec2(0.25,0.5)),length(p-vec2(0.75,0.5)))-.5)*50.);
	f-=1.-saturate((length(p-.5)-(((sin(time+u.x)+2.)/3.)*.25))*50.);
	return vec4(f,f,f,0.);
}

vec4 tex2( vec2 g )
{
    g /= 10.;
    float color = sign((mod(g.x, 0.1) - 0.05) * (mod(g.y, 0.1) - 0.05));
    
    return sqrt(vec4(color));
}

void main()
{
    vec2 uv = surfacePosition * 2.;
    
    float t = time * .5;
    uv.y += sin(t) * .5;
    uv.x += cos(t) * .5;
    float a = atan(uv.x,uv.y)/1.57;
    float d = max(max(abs(uv.x),abs(uv.y)), min(abs(uv.x)+uv.y, length(uv)));
   
    vec2 k = vec2(a,.8/d + t);
    
    vec4 tx = tex(k*6.);
    vec4 tx2 = tex2(k*2.);
    
    // ground
    gl_FragColor = tx2;
    
    // wall
    if (d<=abs(uv.x)+0.05||d<=abs(uv.x)+uv.y)
        gl_FragColor = tx;
    
    gl_FragColor *= d;
	gl_FragColor.a = 1.;
	
}

#endif


#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable


 
uniform float iGlobalTime;
uniform vec3 iMouse;
uniform vec2 iResolution;
uniform float time;

//#define time iGlobalTime

#define time (iGlobalTime)



float saturate(float x){return clamp(x,0.,3.);}


vec4 tex0(vec2 u){
	vec2 p=fract(fract(u)+(sin(u.y+time)/4.));
	float f=1.-saturate((max(length(p-vec2(0.25,0.5)),length(p-vec2(0.75,0.5)))-.5)*50.);
	f-=1.-saturate((length(p-.5)-(((sin(time+u.x)+2.)/3.)*.25))*50.);
	
	float val = .5+.5*sin(time);
	float val2 = .5+.5*sin(time+3.14);
	
	
	return vec4(f,f,f,.0);
	
	return vec4(f , //* (mod(u.y,.4)),
		    f * 1.4*val,
		    
		    f*.5,
		    0.);
}

#define time2 (time*.5)
vec4 tex(vec2 u){
	vec2 u1 = smoothstep(vec2(-1),vec2(1),vec2(sin(time2+3.14),cos(time2+1.45))) *.3 ;
	vec2 u2 = smoothstep(vec2(-1),vec2(1),vec2(sin(time2+2.78),cos(time2))) * .3;
	
	float mixv = distance(u1,u2) *.3;
	
	vec4 c1 = tex0(u+u1);
	vec4 c2 = tex0(u+u2);
	float r = mix(c1.r, c2.r, mixv);
	float g = mix(c1.g, c2.g, 1.-mixv);
	float b = mix(c1.b, c2.b, mixv);
	return vec4(r, g, b, .0);
}

vec4 tex2( vec2 g )
{
    g /= 10.;
    float color = sign((mod(g.x, 0.1) - 0.05) * (mod(g.y, 0.1) - 0.05));
    
    return sqrt(vec4(color));
}


void main(void){
	//vec2 v = (gl_FragCoord.xy - iResolution/2.0) / min(iResolution.y,iResolution.x) * 20.0 ;//+ iMouse.xy*;
	
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    
    uv.x *=iResolution.x/iResolution.y;
    uv = uv*2.-1.;
    uv +=vec2(-.7,.15);
	//fragColor = vec4(uv,0.5+0.5*sin(iGlobalTime),1.0);
    float t = time * .5;
    //uv.x += cos(t) * .5;
	//uv.y += cos(t) * .5;
    float a = atan(uv.x,uv.y)/1.57;
    //float d = max(max(abs(uv.x),abs(uv.y)), min(abs(uv.x)+uv.y, length(uv)));
	float d =  length(uv);
   
    vec2 k = vec2(a,.8/d + t);
    
    vec4 tx = tex(k*6.);
        
	gl_FragColor = tx;
	
}
