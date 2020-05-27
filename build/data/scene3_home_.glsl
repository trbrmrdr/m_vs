
#ifdef GL_ES
precision mediump float;
#endif
 
#define _PI  3.14159265358979323846
#define _2PI 6.2831853071794  
#define _PI2 1.57079632679489661923

#define SWAP(x,y) {float T=x;x=y;y=T;}

uniform float lowFreq;
//uniform float midFreq;
uniform float highFreq;

   
uniform float iGlobalTime;
uniform vec3 iMouse;
uniform vec2 iResolution;

#define time iGlobalTime
#define mouse iMouse
#define resolution iResolution

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


vec3 drawText(vec2 position,vec3 color,const in float val)
{
    vec2 fontsize = vec2(13.0, 15.0);
    //vec2 position = vec2(15.,10.);
    float prn = PrintValue(gl_FragCoord.xy, position , fontsize, val, 1.0, 6.0);
    return mix( color, vec3(1., 1.0, 0.5), prn);
}

 
#define _2PI 6.2831853071794
#define shiftI 100.

#define SIN1(x)((+1.+sin(x))*.5)
//#define TEXT SIN1(time*0.03)
//#define TEXT (.5*mouse.x)
#define TEXT1 (mouse.x)
#define TEXT2 (mouse.y)


vec4 fract1(in vec2 uv)
{
	vec2 v = uv;
	vec2 vv = v; 
	vec2 vvv = v;
	vec2 var = vec2(mouse.x,mouse.y*.5);//vec2(.25+.035,.021);//mouse;
	vec2 edge = vec2(0.,_2PI);
	edge = vec2(_2PI*.0,_2PI*.0101)+_2PI*var.x;
	float speed = (0.03*var.y);
	float timeS = time*speed;
	float timeS2 = time*var.y;
	float varT = SIN1(timeS);
	
	float tm = edge.x + abs(edge.x-edge.y)*varT;//time*0.03;
	
	//float tm = 6.28*mouse.x;//0.25+.01*time;//*(mouse.x/resolution.x);//time*0.03;
	vec4 t1 = vec4(.2,.3,.4,20.7);//*time.001;
	vec4 t2 = vec4(.2,.8,.4,1.5);
	vec2 mspt = (vec2(
			sin(tm)+cos(tm*t1.r)+sin(tm*t1.g)+cos(tm*t1.b)+sin(tm*t1.a),//-sin(12.*tm),
			cos(tm)+sin(tm*t2.r)+cos(tm*t2.g)+sin(tm*t2.b)+cos(tm*t2.a)
			)+1.0)*0.1; //5x harmonics, scale back to [0,1]
	float R = 0.0;//+sin(time)*cos(time);
	float RR = 0.0+3.*cos(timeS2);
	float RRR = 0.0+1.;//*sin(timeS2);
	float a = (5.-mspt.x)*3.2;
	float C = cos(a);
	float S = sin(a);
	vec2 xa=vec2(C, -S);
	vec2 ya=vec2(S, C);
	vec2 shift = vec2( 0, 1.62);//+.42*mouse.x;
	float Z = 1.0 + mspt.y*6.0;
	float ZZ = 1.0 + (mspt.y)*6.2;
	float ZZZ = 1.0 + (mspt.y)*6.9;
	
	for ( int i = 0; i < 30; i++ ){
		float r = dot(v,v);
		if ( r > 1.0 )
		{
			r = (1.0)/r ;
			v.x = v.x * r;
			v.y = v.y * r;
		}
		R *= .99;
		R += r;
		if(i < 59){
			RR *= 1.09;-.0021*sin(timeS);//*mouse.x;
			RR += r;
			if(i < 18){//*sin(1-lowFreq*.001)){
				RRR *= .99;
				RRR += r;
			}
		}
		
		v = vec2( dot(v, xa)* ZZ, dot(v, ya)* ZZ)  + shift;
	}
	
	float c = ((mod(R,2.0)>1.0)?1.0-fract(R):fract(R));
	float cc = ((mod(RR,2.0)>1.0)?1.0-fract(RR):fract(RR));
	float ccc = ((mod(RRR,2.0)>1.0)?1.0-fract(RRR):fract(RRR));
	return vec4(ccc, cc, c, 1.0);
}

void main(void){
	
	vec2 coord = gl_FragCoord.xy;
	//vec2 oxy = vec2(.5)*mouse.x;
	//coord =  oxy+oxy * coord * sign(coord,oxy);
	
	vec2 res =  resolution*.5 ;//* oxy;
	//coord = coord - oxy;
	vec2 v = (coord.xy - res/2.0) / min(res.y,res.x) * 20.0;
	//v = vec2(.37,.23);
	vec4 col = fract1(v);
	col = vec4(drawText(vec2(15.,35.),col.rgb,TEXT1),col.a);
	col = vec4(drawText(vec2(15.,10.),col.rgb,TEXT2),col.a);
	gl_FragColor = col; 
}
