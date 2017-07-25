//Lignettes
//https://www.shadertoy.com/view/4dXBD2
// Leon 04 / 07 / 2017
// using lines of code from iq, mercury, lj, koltes, duke

uniform float iGlobalTime;
uniform vec3 iMouse;
uniform vec2 iResolution;
varying vec2 surfacePosition;


#define PI 3.14159
#define TAU 2.*PI
#define t iGlobalTime*.3
//#define DITHER
//#define STEPS 50.
#define STEPS 50.

float rand(in vec2 co) { return fract(sin(dot(co*0.123,vec2(12.9898,78.233))) * 43758.5453); }
float cyl (in vec2 p, float r) { return length(p)-r; }
mat2 rot (in float a) { float c=cos(a),s=sin(a);return mat2(c,-s,s,c);}
vec3 moda (in vec2 p,in  float count) {
    float an = TAU/count;
    float a = atan(p.y,p.x)+an/2.;
    float c = floor(a/an);
    c = mix(c,abs(c),step(count/2., abs(c)));
    a = mod(a,an)-an/2.;
    return vec3(vec2(cos(a),sin(a))*length(p),c);
}
float smin (in float a,in  float b,in  float r) {
    float h = clamp(.5+.5*(b-a)/r, 0., 1.);
    return mix(b, a, h) - r*h*(1.-h);
}

vec3 camera (in vec3 p) {
    p.xz *= rot((PI*(iMouse.x/iResolution.x-.5)*step(0.5,iMouse.z)));
    p.yz *= rot((PI*(iMouse.y/iResolution.y-.5)*step(0.5,iMouse.z)));
    p.yz *= rot(PI/2.);
    return p;
}

float curve (in float x) {
   	return sin(x*4.-t*2.)*.1 + sin(x*2.+t*4.)*0.2;
}

// lines tunnel
vec3 displace (in vec3 p,in  float radius,in  float count) {
    vec3 p1 = moda(p.xz, count);
    p1.x -= radius;
    p.xz = p1.xy;
    p.z -= curve(p.y+p1.z)*.2;
    return p;
}

// circles stuff
vec3 displace2 (in vec3 p,in float radius,in float count) {
    float a = atan(p.y,p.x);
    float l = length(p.xy);
    p.x = l - radius;
    return p;
}

float map (in vec3 p) {
    float scene = 1.;
    vec3 p0 = p;
    
    // tunnel distortion
    p.xy *= rot(sin(length(p)+t*4.)*.1);
    p.yz *= rot(sin(length(p*.5)+t)*.2);
    p.xz *= rot(sin(length(p*2.)+t*2.)*.1);
    float radius = 2.;
    float size = 0.02;
    float count = 9.;
    vec2 front = normalize(p.xz)*.1;
    vec2 right = vec2(front.y,-front.x);
    vec3 p2 = p;
    p.y -= t*4.;
    const float repeat = 2.;//8.;
    for (float i = 0.; i < repeat; ++i) {
        
        // lines tunnel
        p.xz -= front*1.5;
        p.xz -= right*.5;
    	scene = min(scene,cyl(displace(p, radius, count).xz,size));
        
        // circle stuff
        p2.xz *= rot(.3*(i/repeat)+t*.5);
        p2.yz *= rot(.2*i/repeat+t);
        p2.yx *= rot(.4*i/repeat+t*2.);
    	scene = smin(scene,cyl(displace2(p2, radius*.5-i*.05, count).xz,size),.1);
    }
    return scene;
}

const float e = 0.01;
vec3 getNormal (in vec3 p) {
    return normalize(vec3(map(p+vec3(e,0,0))-map(p-vec3(e,0,0)),
                          map(p+vec3(0,e,0))-map(p-vec3(0,e,0)),
                          map(p+vec3(0,0,e))-map(p-vec3(0,0,e))));
}

void main()
{
    vec2 coord = gl_FragCoord;
	vec2 uv = (coord.xy-.5*iResolution.xy)/iResolution.y;
    #ifdef DITHER
	vec2 dpos = ( coord.xy / iResolution.xy );
	vec2 seed = dpos + fract(iGlobalTime);
	#endif 
    vec3 eye = camera(vec3(uv,-3.));
    vec3 ray = normalize(camera(vec3(uv,1.0)));
    vec3 pos = eye;
    float shade = 0.;
    for (float i = 0.; i < STEPS; ++i) {
        float dist = map(pos);
        if (dist < 0.001) {
            break;
        }
        #ifdef DITHER
        dist=abs(dist)*(.8+0.2*rand(seed*vec2(i)));
        #endif 
        pos += ray*dist;
		shade = i;
    }
    vec3 n = getNormal(pos);
    vec4 color = vec4(1);
    color.rgb = n*.5+.5;
    color.rgb *= 1.-shade/(STEPS-1.);
    gl_FragColor = color;
}