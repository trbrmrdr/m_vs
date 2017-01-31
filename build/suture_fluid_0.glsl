#ifdef GL_ES
precision mediump float;
#endif
 
uniform float iGlobalTime;
uniform vec3 iMouse;
uniform vec2 iResolution;
uniform int iFrame;

uniform sampler2D backbuffer;
uniform sampler2D iChannel0;
uniform sampler2D optTex;
uniform int press;


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


vec4 drawText(vec4 color,const in vec2 pos,const in float val)
{
    vec2 fontsize = vec2(13.0, 15.0);
    //vec2 pos = vec2(15.,10.);
    float prn = PrintValue(gl_FragCoord, pos , fontsize, val, 1.0, 6.0);
    return vec4(mix( color, vec3(0.7, 0.0, 0.0), prn),1.0);
}

bool reset() {
    //return texture2D(iChannel3, vec2(32.5/256.0, 0.5) ).x > 0.5;
	return press==1;
}

vec2 normz(vec2 x) {
	return x == vec2(0.0, 0.0) ? vec2(0.0, 0.0) : normalize(x);
}

// reverse advection
vec3 advect(vec2 ab, vec2 vUv, vec2 step, float sc) {

    vec2 aUv = vUv - ab * sc * step;

    const float _G0 = 0.25; // center weight
    const float _G1 = 0.125; // edge-neighbors
    const float _G2 = 0.0625; // vertex-neighbors

    // 3x3 neighborhood coordinates
    float step_x = step.x;
    float step_y = step.y;
    vec2 n  = vec2(0.0, step_y);
    vec2 ne = vec2(step_x, step_y);
    vec2 e  = vec2(step_x, 0.0);
    vec2 se = vec2(step_x, -step_y);
    vec2 s  = vec2(0.0, -step_y);
    vec2 sw = vec2(-step_x, -step_y);
    vec2 w  = vec2(-step_x, 0.0);
    vec2 nw = vec2(-step_x, step_y);

    vec3 uv =    texture2D(backbuffer, fract(aUv)).xyz;
    vec3 uv_n =  texture2D(backbuffer, fract(aUv+n)).xyz;
    vec3 uv_e =  texture2D(backbuffer, fract(aUv+e)).xyz;
    vec3 uv_s =  texture2D(backbuffer, fract(aUv+s)).xyz;
    vec3 uv_w =  texture2D(backbuffer, fract(aUv+w)).xyz;
    vec3 uv_nw = texture2D(backbuffer, fract(aUv+nw)).xyz;
    vec3 uv_sw = texture2D(backbuffer, fract(aUv+sw)).xyz;
    vec3 uv_ne = texture2D(backbuffer, fract(aUv+ne)).xyz;
    vec3 uv_se = texture2D(backbuffer, fract(aUv+se)).xyz;

    return _G0*uv + _G1*(uv_n + uv_e + uv_w + uv_s) + _G2*(uv_nw + uv_sw + uv_ne + uv_se);
}

void main(void){
	vec2 v = (gl_FragCoord.xy - iResolution/2.0) / min(iResolution.y,iResolution.x) * 20.0;
	
    const float _K0 = -20.0/6.0; // center weight
    const float _K1 = 4.0/6.0;   // edge-neighbors
    const float _K2 = 1.0/6.0;   // vertex-neighbors
    const float cs = -0.6;  // curl scale
    const float ls = 0.05;  // laplacian scale
    const float ps = -0.8;  // laplacian of divergence scale
    const float ds = -0.05; // divergence scale
    const float dp = -0.04; // divergence update scale
    const float pl = 0.3;   // divergence smoothing
    const float ad = 6.0;   // advection distance scale
    const float pwr = 1.0;  // power when deriving rotation angle from curl
    const float amp = 1.0;  // self-amplification
    const float upd = 0.8;  // update smoothing
    const float sq2 = 0.6;  // diagonal weight

    vec2 vUv = gl_FragCoord.xy / iResolution.xy;
    vec2 texel = 1. / iResolution.xy;

	float a1 = 1.;//sin(iGlobalTime*.005);
	float a2 = 1.;//cos(iGlobalTime);
    // 3x3 neighborhood coordinates
    float step_x = texel.x*a1;
    float step_y = texel.y*a2;
    vec2 n  = vec2(0.0, step_y);
    vec2 ne = vec2(step_x, step_y);
    vec2 e  = vec2(step_x, 0.0);
    vec2 se = vec2(step_x, -step_y);
    vec2 s  = vec2(0.0, -step_y);
    vec2 sw = vec2(-step_x, -step_y);
    vec2 w  = vec2(-step_x, 0.0);
    vec2 nw = vec2(-step_x, step_y);

    vec3 uv =    texture2D(backbuffer, fract(vUv)).xyz;
    vec3 uv_n =  texture2D(backbuffer, fract(vUv+n)).xyz;
    vec3 uv_e =  texture2D(backbuffer, fract(vUv+e)).xyz;
    vec3 uv_s =  texture2D(backbuffer, fract(vUv+s)).xyz;
    vec3 uv_w =  texture2D(backbuffer, fract(vUv+w)).xyz;
    vec3 uv_nw = texture2D(backbuffer, fract(vUv+nw)).xyz;
    vec3 uv_sw = texture2D(backbuffer, fract(vUv+sw)).xyz;
    vec3 uv_ne = texture2D(backbuffer, fract(vUv+ne)).xyz;
    vec3 uv_se = texture2D(backbuffer, fract(vUv+se)).xyz;

    // uv.x and uv.y are the x and y components, uv.z is divergence

    // laplacian of all components
    vec3 lapl  = _K0*uv + _K1*(uv_n + uv_e + uv_w + uv_s) + _K2*(uv_nw + uv_sw + uv_ne + uv_se);
    float sp = ps * lapl.z;

    // calculate curl
    // vectors point clockwise about the center point
    float curl = uv_n.x - uv_s.x - uv_e.y + uv_w.y + sq2 * (uv_nw.x + uv_nw.y + uv_ne.x - uv_ne.y + uv_sw.y - uv_sw.x - uv_se.y - uv_se.x);

    // compute angle of rotation from curl
    float sc = cs * sign(curl) * pow(abs(curl), pwr);

    // calculate divergence
    // vectors point inwards towards the center point
    float div  = uv_s.y - uv_n.y - uv_e.x + uv_w.x + sq2 * (uv_nw.x - uv_nw.y - uv_ne.x - uv_ne.y + uv_sw.x + uv_sw.y + uv_se.y - uv_se.x);
    float sd = uv.z + dp * div + pl * lapl.z;

    vec2 norm = normz(uv.xy);

    vec3 ab = advect(vec2(uv.x, uv.y), vUv, texel, ad);

    // temp values for the update rule
    float ta = amp * ab.x + ls * lapl.x + norm.x * sp + uv.x * ds * sd;
    float tb = amp * ab.y + ls * lapl.y + norm.y * sp + uv.y * ds * sd;

    // rotate
    float a = ta * cos(sc) - tb * sin(sc);
    float b = ta * sin(sc) + tb * cos(sc);

    vec3 abd = upd * uv + (1.0 - upd) * vec3(a,b,sd);

    if (iMouse.z > 0.0) {
    	vec2 d = gl_FragCoord.xy -iResolution.xy*iMouse.xy;
        float m = exp(-length(d) / 10.0);
        abd.xy += m * normz(d);
    }

	vec4 newCol;
    // initialize with noise
    if(iFrame<10 || reset()) {
		newCol = vec4(
			(1.+sin(iGlobalTime))*.5,
			(1.+cos(iGlobalTime))*.5,
			1.,
			1.);
        newCol = -0.5 + texture2D(iChannel0, gl_FragCoord.xy / iResolution.xy);
    } else {
        //fragColor = clamp(vec4(abd,0.0), -1., 1.);
        abd.z = clamp(abd.z, -1.0, 1.0);
        abd.xy = clamp(length(abd.xy) > 1.0 ? normz(abd.xy) : abd.xy, -1.0, 1.0);
        newCol = vec4(abd, 0.0);
    }
	
	//newCol = texture2D(iChannel0, gl_FragCoord.xy / iResolution.xy);
	//newCol = drawText(newCol,vec2(15.,10.),iMouse.y);
	gl_FragColor = newCol;
}