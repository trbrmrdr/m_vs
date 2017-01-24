// uninitialized variables... yuck

#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec3 iMouse;
uniform vec2 iResolution;
uniform sampler2D optTex;


// print helper
// https://www.shadertoy.com/view/4sBSWW

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
    float prn = PrintValue(gl_FragCoord, position , fontsize, val, 1.0, 6.0);
    return mix( color, vec3(0.7, 0.0, 0.0), prn);
}
//###########################################

void main(void)
{
    vec2 pos = gl_FragCoord/iResolution;
    vec2 pp = vec2(.5,.5);
    
    vec2 cc = length(gl_FragCoord-pp*iResolution)/iResolution;
    float r = 10.*atan(cc.y*2,cc.x*2);
    r = smoothstep(.0,.25,r);
    //nPos*=length(pos - pp);
    //nPos=smoothstep(.0,.4,length(gl_FragCoord-pp*iResolution)/iResolution);
    vec3 color  = vec3(.4,.3,.7);
    color *= r;
    color = drawText1(color,123.333);
    
    gl_FragColor = vec4(color,1.);
}
