
#ifdef GL_ES
precision mediump float;
#endif
 
uniform float iGlobalTime;
uniform vec3 iMouse;
uniform vec2 iResolution;

#define time iGlobalTime

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


vec3 drawText1(vec3 color,const in float val)
{
    vec2 fontsize = vec2(13.0, 15.0);
    vec2 position = vec2(15.,10.);
    float prn = PrintValue(gl_FragCoord.xy, position , fontsize, val, 1.0, 6.0);
    return mix( color, vec3(0.7, 0.0, 0.0), prn);
}

float wave(float x)
{
	bool p  = fract(x*.5)<.5;
	x	= fract(x)*2.;
	x 	*= 2.-x;
	x 	*= 1.-abs(1.-x)*.25;
	return  p ? x : -x;
}

#define _2PI 6.2831853071794
void main(void){
	vec2 v = (gl_FragCoord.xy - iResolution/2.0) / min(iResolution.y,iResolution.x) * 20.0 ;//+ iMouse.xy*;
	vec2 vv = v; vec2 vvv = v;
	
	vec2 edge = vec2(0.,_2PI);
	
	//iMouse.x = .58;
    float len= .4;
    float start = iMouse.x;//.68+.32*iMouse.x;
	edge = vec2(_2PI*start,_2PI*start+len);
	float varT = (+1.+cos(time*0.03))*.5*iMouse.y;
	
	float tm = edge.x + abs(edge.x-edge.y)*varT;//time*0.03;
	
	vec2 mspt = (vec2(
			sin(tm)+cos(tm*0.76)+sin(tm*0.5)+cos(tm*-0.4)+sin(tm*1.3),
			cos(tm)+sin(tm*0.1)+cos(tm*0.8)+sin(tm*-1.1)+cos(tm*1.5)
			)+1.0); //5x harmonics, scale back to [0,1]
	float R = 0.0;
	
	float RR = 0.0;
	float RRR = 0.0;
	float a = (.6-mspt.x)*6.2;
	float C = cos(a);
	float S = sin(a);
	vec2 xa=vec2(C, -S);
	vec2 ya=vec2(S, C);
	//vec2 shift = vec2( -.34, 1.62)+(cos(time*.003));
	//vec2 shift = vec2( 0, .6+.1*iMouse.y);//2.+(cos(time*.003)));
	vec2 shift = vec2( 0, .6+((+1.+cos(time*0.03))*.5)*.2);
	float Z = 1.0 + mspt.y*6.0;
	float ZZ = 1.0 + (mspt.y)*1.2;
	float ZZZ = 1.0 + (mspt.y)*1.9;
	
	for ( int i = 0; i < 80; i++ ){
		float r = dot(v,v);
		if ( r > 1.0 )
		{
			r = (1.0)/r ;
			v.x = v.x * r;
			v.y = v.y * r;
		}
		R *= .99;
		R += r;
		if(i < 39){
			RR *= .99;//*(1.+cos(time*.07))*.5;
			RR += r;
			if(i < 38){
				RRR *= .99;//*(1.+sin(time*.01))*.5;
				RRR += r;
			}
		}
		
		v = vec2( dot(v, xa), dot(v, ya)) * Z + shift;
	}
	float c = ((mod(R,2.0)>1.0)?1.0-fract(R):fract(R));
	float cc = ((mod(RR,2.0)>1.0)?1.0-fract(RR):fract(RR));
	float ccc = ((mod(RRR,2.0)>1.0)?1.0-fract(RRR):fract(RRR));
	vec4 newCol = vec4(ccc, cc, c, 1.0);
	
	newCol = vec4(drawText1(newCol.rgb,iMouse.x),newCol.a);
	gl_FragColor = newCol;
	
}
