// Author:
// Title:

#ifdef GL_ES
precision mediump float;
#endif


#if 0
uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

#define time u_time
#define mouse (u_mouse/u_resolution)
#define resolution u_resolution

#else

uniform float iGlobalTime;
uniform vec3 iMouse;
uniform vec2 iResolution;

#define time (iGlobalTime*.5)
#define mouse (iMouse.xy)
#define resolution iResolution

#endif

float sin01(float v){return .5+.5*sin(v);}
float cos01(float v){return .5+.5*cos(v);}
float sinab(float a,float b,float v){return a+b*sin01(v);}
    
float hash( float n )
{
    return fract(sin(n)*43758.5453);
}

float hash2(vec2 v){
    return fract(sin(1.+v.x)*cos(v.y)*43758.5453+dot(v.x,v.y));
}

float noise( in vec2 x )
{
    vec2 p = floor(x);
    vec2 f = fract(x);

    f = f*f*(3.0-2.0*f);

    float n = p.x + p.y*57.0;

    return mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
               mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y);
}

const mat2 m = mat2( 0.80,  0.60, -0.60,  0.80 );

float fbm( vec2 p )
{
    float f = 0.0;    
    f += 0.524*noise( p ); p = m*p*2.02;
    f += 0.250*noise( p ); p = m*p*2.03;
    f += 0.125*noise( p ); p = m*p*2.01;
    f += 0.06250*noise( p ); p = m*p*2.04;
    f += 0.03125*noise( p );

    return f/0.984375;
}

#define PI 3.14
#define PI2 1.57
float eye(vec2 pos,vec2 scale,vec2 uv){
 	//vec2 scale = vec2(1.,.5+.5*sin(time));//*10.;//*2.-1.;
    if(uv.x>=(pos.x+PI2*scale.x))return .0;
    if(uv.x<(pos.x-PI2*scale.x))return .0;
    
 	vec2 t_dpos = (uv+(-1.*pos))*
        	vec2(scale.x,1.)+ vec2(-PI2,0);
 	float fx = sin(t_dpos.x)*scale.y;
 	float eye = smoothstep(fx,fx, t_dpos.y) ;
 	fx = cos(t_dpos.x+PI2)*scale.y;
 	eye -= smoothstep(fx,fx, t_dpos.y) ;
    
    return eye;
}

float plot(float fx,float y)
    {
    const float dwl = .04;
    return smoothstep(fx-dwl,fx, y) - smoothstep(fx,fx+dwl,y);
}

vec3 cl = vec3(1.,0.,0.);
vec3 plot_fx(float fx,vec3 oldColor,vec2 uv){
    float pct = plot(fx,uv.y);
    return mix(oldColor,cl,pct);
    //return (1.-pct)*oldColor + pct*vec3(1.,0.,0.);
}

//draw number 

float DigitBin(const in int x)
{
    return x==0?480599.0:x==1?139810.0:x==2?476951.0:x==3?476999.0:x==4?350020.0:x==5?464711.0:x==6?464727.0:x==7?476228.0:x==8?481111.0:x==9?481095.0:0.0;
}

float PrintValue(const in vec2 fragCoord, const in vec2 vPixelCoords, const in vec2 vFontSize, const in float fValue, const in float fMaxDigits, const in float fDecimalPlaces)
{
    vec2 vStringCharCoords = (fragCoord.xy - vPixelCoords) / vFontSize;
    if ((vStringCharCoords.y < 0.0) || (vStringCharCoords.y >= 1.0)) return 0.0;
	float fLog10Value = log2(abs(fValue)) / log2(10.0);
	float fBiggestIndex = max(floor(fLog10Value), 0.0);
	float fDigitIndex = fMaxDigits - floor(vStringCharCoords.x);
	float fCharBin = 0.0;
	if(fDigitIndex > (-fDecimalPlaces - 1.01)) {
		if(fDigitIndex > fBiggestIndex) {
			if((fValue < 0.0) && (fDigitIndex < (fBiggestIndex+1.5))) fCharBin = 1792.0;
		} else {		
			if(fDigitIndex == -1.0) {
				if(fDecimalPlaces > 0.0) fCharBin = 2.0;
			} else {
				if(fDigitIndex < 0.0) fDigitIndex += 1.0;
				float fDigitValue = (abs(fValue / (pow(10.0, fDigitIndex))));
                float kFix = 0.0001;
                fCharBin = DigitBin(int(floor(mod(kFix+fDigitValue, 10.0))));
			}		
		}
	}
    return floor(mod((fCharBin / pow(2.0, floor(fract(vStringCharCoords.x) * 4.0) + (floor(vStringCharCoords.y * 5.0) * 4.0))), 2.0));
}


float drawText1 = .0;
float drawText2 = .0;
vec3 drawText(vec2 position,vec3 color,const in float val)
{
    vec2 fontsize = vec2(23.0, 25.0);
    float prn = PrintValue(gl_FragCoord.xy, position , fontsize, val, 1.0, 6.0);
    return mix( color, vec3(0.693,0.995,0.466), prn);
}
//###################


void main() {
    vec2 uv = gl_FragCoord.xy/resolution.xy;
    //uv+=vec2(-.5);
    
    uv.x *= resolution.x/resolution.y; 
    float scaleUV = 400.*.05;//*sinab(.6,1.,time*.3);//mouse.x;
    //uv.x+=time*10.;
    //scaleUV=400.*mouse.x;
    uv*=scaleUV;
    vec3 color = vec3(0.);
#if 0
    //OS XY
    cl =vec3(0,0,1);
    color = plot_fx(.0,color,uv);
    color = plot_fx(0.,color,uv.yx);    
#endif
    
    const vec2 eye_s = vec2((3.428),(2.108));
    vec2 it_st = floor(uv/eye_s);
    vec2 f_st = vec2(-eye_s/2.);
    //if(false)
    	{
            //float rand_d = 10.*hash(it_st.x*it_st.y);
            float rand_d = 10.*hash2(it_st+vec2(1000));
            
            vec2 scale = vec2(1.,sinab(.2,.7,time+rand_d));//*10.;//*2.-1.;
            vec2 pos = eye_s*it_st - f_st;
        	float feye = eye(pos,scale,uv);
        	color += mix(vec3(0,0,0),vec3(1),feye);
            
            //if(false)
            float colorR = 0.;
            if(feye>.0)
            {
                float mix_ceye = 1.-floor(length(uv-pos));
                color = mix(color,vec3(0),mix_ceye);
            	
                float cof2 = .7*(sin01(time+rand_d));
                float r2 = sinab(.2,.3,time+rand_d);
                
                //vec2 pos_2 = vec2(0.200,0.290);
                vec2 dst =  (pos - mouse*scaleUV)*-1.;
                vec2 dm = normalize(dst);
                vec2 pos_2 = dm;//vec2(0.080,0.000);//dm;//vec2(sin01(time+rand_d),cos01(time+rand_d));
                float dr2 = min(1.,length(dst));//sin01(time+rand_d);
                vec2 pos2 = PI*(+vec2(-.5+(-1.+.5+.5*pos_2.x),.5+.5*pos_2.y));
                float coof = length(uv-pos+
                              vec2(sin(pos2.x),cos(pos2.y))*(dr2-r2*.5)
                                   );
                if(coof<=r2 && mix_ceye>.0){
                    float mix_v = smoothstep(0.,1.,cof2);
                    color = mix(color,vec3(1),1.);
                    
                    //colorR = mix_v;
                    float l_eye_s = length(eye_s);
                    if(length(dst)<=l_eye_s)
                    	colorR = smoothstep(.0,l_eye_s,length(dst));
                    //color = mix(color,vec3(1,0,0),mix_v);
                }
                
            }
            
            float inv = floor(fract(rand_d+time*.003)/.5);
            color = vec3(abs(color.x - inv));
            color = mix(color,vec3(1,0,0),colorR);
        
        }    
    
    //vec2 dm = normalize(vec2(.5) - mouse)*-1.;
    //drawText1= dm.x;
    //drawText2= dm.y;
    
    //float feye = eye(mouse,vec2(1.),uvo);
	//color += mix(vec3(0,0,0),vec3(1),feye);
    	//color = plot_fx(i_st,color,uv);
    	//color = plot_fx(j_st,color,uv.yx);
    //color = mix(color,color)
	//color = drawText(resolution * vec2(0.010,0.120),color,drawText1);
    //color = drawText(resolution * vec2(0.500,0.050),color,drawText2);
gl_FragColor = vec4(color,1.);
return;
  
    //_________________
    float fx = smoothstep(0.,1.,uv.x)+cos(uv.x+1.);
    //fx = 1. - pow(fx,10.*mouse.x);
    fx = 1. - fx;
    cl =vec3(0.785,0.751,0.138);color = plot_fx(fx,color,uv);
    
    fx = sin(uv.x);
    cl =vec3(0.784,0.767,0.785);color = plot_fx(fx,color,uv);
#if 0
    //if(false)
    vec3 color_t  = vec3(
        //fbm( uv*fbm(vec2(sin(time),cos(time))+time))
        smoothstep(.3,.7,fbm( uv*fbm(uv+time) ))
        ,fbm(uv.yx*.1*fbm(uv+time*1.3))
        ,fbm(vec2(sin(time),cos(time))*fbm(uv+time*.3))
        );
    
    
    float f = fbm(10.*uv+vec2(time));
    color = mix(vec3(0.145,0.595,0.269),vec3(0.320,0.048,0.057),f);
    
    vec2 p = uv*2.-1.;
    float r = length(p);///fract(time*0.045);//0.336+time*.5;
    float a ;//= pow(length(uv),2.)*fbm(uv)+time*.3;//
    //float 
        a = atan(p.x,p.y);//+time*.3;
     a += 0.05*fbm( 20.0*p );

    f = smoothstep( 0.3, 1.0, fbm( vec2(18.360*a,(6.016*r*(.8+.1*(.5+.5*sin(time)))) )));
    color = mix( color,
                color_t
                //vec3(1.0,1.0,1.0)
                , f );
    if(false)
    color = mix(
        vec3(0.),color,
        1.-floor(r*2.)
    );
#endif
        
    gl_FragColor = vec4(color,1.0);
    }