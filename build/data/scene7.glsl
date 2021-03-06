// uninitialized variables... yuck

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 iResolution;
uniform float iGlobalTime;
#define time iGlobalTime
uniform vec3 iMouse;

#define STEPS 16
#define PRECISION 0.001
#define DEPTH 35.0
#define M_PI 3.14159265359
#define M_PI2 1.57079632679

vec3 eye = vec3(0,0.5,-1)*3.0;
vec3 light = vec3(0,1,-1);

const float lineWidth = 0.02;
const float border = 0.5;
const float scale = 0.7;

float bounding=0., ground=0., letters=0.;
const float groundPosition = -0.5;
const vec3 boundingSize = vec3(30,12,0.8)*scale;

float t = time;
float scene(vec3);

// Utilities
float udBox(vec3 p, vec3 s) { return length(max(abs(p)-s,0.0)); }

/*
    mat4 rotX = mat4(      vec4(1,0,0,0),
                           vec4(0,c,-s,0),
                           vec4(0,s,c,0),
                           vec4(0,0,0,1) );
    
    mat4 rotY = mat4(      vec4(c,0,-s,0),
                           vec4(0,1,0,0),
                           vec4(s,0,c,0),
                           vec4(0,0,0,1) );
    
    mat4 rotZ = mat4(      vec4(c,s,0,0),
                           vec4(-s,c,0,0),
                           vec4(0,0,1,0),
                           vec4(0,0,0,1) );
    
    mat4 pos = mat4(       vec4(1,0,0,s),
                           vec4(0,1,0,0),
                           vec4(0,0,1,c),
                           vec4(0,0,0,1) );
*/
mat3 rotX(float a) {float s=sin(a); float c=cos(a); return mat3(1,0,0,0,c,-s,0,s,c);}
mat3 rotY(float a) {float s=sin(a); float c=cos(a); return mat3(c,0,-s,0,1,0,s,0,c);}

// Letter code (https://dl.dropboxusercontent.com/u/14645664/files/glsl-text.txt)
float line(vec2 p, vec2 s, vec2 e) {s*=scale;e*=scale;float l=length(s-e);vec2 d=vec2(e-s)/l;p-=vec2(s.x,-s.y);p=vec2(p.x*d.x+p.y*-d.y,p.x*d.y+p.y*d.x);return length(max(abs(p-vec2(l/2.0,0))-vec2(l/2.0,lineWidth/2.0),0.0))-border;}


// Marching
vec3 getNormal(vec3 p){vec2 e=vec2(PRECISION,0);return(normalize(vec3(scene(p+e.xyy)-scene(p-e.xyy),scene(p+e.yxy)-scene(p-e.yxy),scene(p+e.yyx)-scene(p-e.yyx))));}
vec3 march(vec3 ro,vec3 rd){float t=0.0,d;for(int i=0;i<STEPS;i++){d=scene(ro+rd*t);if(d<PRECISION||t>DEPTH){break;}t+=d;}return(ro+rd*t);}
vec3 lookAt(vec3 o,vec3 t){vec2 uv=(2.0*gl_FragCoord.xy-iResolution.xy)/iResolution.xx;vec3 d=normalize(t-o),u=vec3(0,1,0),r=cross(u,d);return(normalize(r*uv.x+cross(d,r)*uv.y+d));}

vec3 processColor(vec3 p)
{
	float d = 0.000000000001;//1e65;
	
	vec3 n = getNormal(p);
	vec3 l = normalize(light-p);
	vec3 col=vec3(0.1);
	
	float dist = length(light-p);
	float diff = max(dot(n, normalize(light-p)),0.0);
	float spec = pow(diff, 100.0);
	
	if (ground<d) { col = vec3(diff+spec*0.3)*vec3(0.8,0.3,0.6); d = ground; }
	if (letters<d) { col = vec3(0,p.y*0.5+0.5,.1)+diff+spec; }
		
	col *= min(1.0, 1.0/dist);
	
	return col;
}

float scene(vec3 p)
{	
	return 0.0;
}

void glow(float d) {
	float br =10.601;
    //float br =0.005 * iResolution.y;
    if(d>1.0)return;
    br=1.0;
	gl_FragColor.rgb += vec3(0.3, 0.15, 0.45) * br / d;
}

void glow(float d,float br,vec3 color) {
    glow(d);
    return;
    //gl_FragColor.rgb += color * br / d;
    //return;
    //if(d>40.)
      //  return;
    if(d>30.)// && d<30.)
    {
        //d-=30;
        //d = smoothstep(0.,1.,d);//11.1;//smoothstep(20.,30.,d);
        //d *=0.4;
        //d = cos(d)*10.;
        //br = br*(d*10.);
        
        //d=cos(d-.5)*100.;
        //d = br+d;
        
    }
    
    //if(d>br)
    //    return;
    float sc = br / d;
    if(d/br>M_PI2)return;
    //if(sc>M_PI)
        //return;
    sc = sin(d/br+M_PI2)*(br*1000./abs(d*d*d));
    vec3 newC = color * sc;
    if(false)
    if(newC.r<.1
    || newC.g<.005
    || newC.b<.005 
    )
    return;
	gl_FragColor.rgb += newC;
}

void line( vec2 a, vec2 l ) {
	l.x *= iResolution.y/iResolution.x;
	l += 0.5;
	l *= iResolution;
	
	vec2 P = gl_FragCoord.xy;
	a.x *= iResolution.y/iResolution.x;
	a += 0.5;
	a *= iResolution;
	
	vec2 aP = P-a;
	vec2 al = l-a;
	vec3 al3 = vec3(al, 0.0);
	vec3 aP3 = vec3(aP, 0.0);
	//float q = length(dot(aP,al))/length(al);
	float q = length(cross(aP3,al3))/length(al3);
	
	float d = q;
	if ( dot(al, aP) <= 0.0 ) { // before start
               d = distance(P, a);
	}
        else if ( dot(al, al) <= dot(al, aP) ) { // after end
               d = distance(P, l);
	}
	glow(d);
}

void point(vec2 a) {
	a.x *= iResolution.y/iResolution.x;
	a += 0.5;
	a *= iResolution;

	vec2 P = gl_FragCoord.xy;
	float d = distance(P, a);
	glow(d);
}

void point2(vec2 a) {
	//a.x *= iResolution.y/iResolution.x;
	//a += 0.5;
	//a *= iResolution;
	vec2 P = gl_FragCoord.xy;
	float d = distance(P, a);
	//glow(d);
    //if(d<20.)
    glow(d,20.1,vec3(0.13, 0.15, 0.15));
}

float rand(int seed) {
	return fract(sin(float(seed)*15.234234) + sin(float(seed)*4.3456342) * 372.4532);
}

	
void main()
{	
	gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);

    vec2 tm = iMouse;
	eye *= rotY(tm.x - 0.5 );//+ 0.5*sin(time);
	eye *= rotX(tm.y - 0.45);
	light.x = sin(t);

    eye = vec3(.0,iMouse.y,iMouse.x);
	vec3 p = march(eye,lookAt(eye,vec3(0)));
    //p = vec3(1.,iMouse.x,iMouse.y);
	vec3 col = processColor(p);
	// Horizontal grid lines
	float y = 0.0;
    float dT = 0.1;
	for (int l=-4; l<4; l++) {
		y = -1.0/(0.6 * sin(time * dT) + float(l)*1.2) + 0.25;
		line(vec2(-1.0, l * -0.25 ), vec2(1.0, l * -0.25) );

        {
            float x = float(l) + fract(time * dT);
            line(vec2(x * 0.25, 1.0), vec2(x * 0.25, -1.0));
        }        
	}
	
	
	// Starfield
    float tmp = iResolution.y/iResolution.x;
	for (int l=0; l<70; l++) {
		float sx = (fract(rand(l+342) + time * (0.002 + 0.01*rand(l)))-0.5) * 3.0;
		float sy = y + 0.4 * rand(l+324);
       
        sx = fract(rand(l+342) + time * (0.002 + 0.01*rand(l))) * 3000.0;
		sy = rand(l)*iResolution.y;
       
        if(false){
            sx = iMouse.x*iResolution.x;//*2. - 1.;
            sy = iMouse.y*iResolution.y;//;gl_FragCoord.y*tmp;
        }       
        //sx = .5;
        //sy = .0;
        //sy =100.5;
		point2(vec2(sx,sy));
        //break;
	}
   
	
	gl_FragColor += vec4(col.yyx,1.0);
}