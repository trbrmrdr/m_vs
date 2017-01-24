#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable

#define PI_2 1.57
#define PI_180

uniform float iGlobalTime;
#define time iGlobalTime
//#define time 10.*iMouse.x

uniform vec3 iMouse;
uniform vec2 iResolution;

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

float TEXT1=.0;
float TEXT2=.0;
float TEXT3 = .0;
vec3 drawText(in vec2 pos,in vec3 color,const in float val)
{
    vec2 fontsize = vec2(13.0, 15.0);
    float prn = PrintValue(gl_FragCoord.xy, pos , fontsize, val, 1.0, 6.0);
    return mix( color, vec3(0.7, 0.0, 0.0), prn);
}


float rand(int seed) {
	return fract(sin(float(seed)*15.234234) + sin(float(seed)*4.3456342) * 372.4532);
}

float hash(float p)
{
    mat2 m = mat2(  13.85, 47.77,
                    99.41, 88.48
                );
    return fract(sin(m*vec2(p)) * 46738.29).x;
}


vec2 hash(vec2 p)
{
    mat2 m = mat2(  13.85, 47.77,
                    99.41, 88.48
                );

    return fract(sin(m*p) * 46738.29);
}

float voronoi(vec2 p)
{
    vec2 g = floor(p);
    vec2 f = fract(p);

    float distanceToClosestFeaturePoint =1.;
    for(int y = -1; y <= 1; y++)
    {
        for(int x = -1; x <= 1; x++)
        {
            vec2 latticePoint = vec2(x, y);
            float currentDistance = distance(latticePoint + hash(g+latticePoint), f);
            distanceToClosestFeaturePoint = min(distanceToClosestFeaturePoint, currentDistance);
	    //return ((latticePoint + hash(g+latticePoint))- f).x;
	    //distanceToClosestFeaturePoint = min(distanceToClosestFeaturePoint, ((latticePoint + hash(g+latticePoint))- f).x);
        }
    }

    return distanceToClosestFeaturePoint;
}


float voronoi(float p)
{
	return voronoi(vec2(p,0));
    float g = floor(p);
    float f = fract(p);
    return f - hash(g);
}

float lerp(float v,float min,float max){return (min+v*(max-min));}

float serp(float v,float min,float max){if (v >= 1.)return max;else if (v <= .0)return min;return min + sin(v * PI_2) * ( max - min );}

float getOffset(in float i,in float vTime,float min=0.,float max=1.)
{
    float pos_i = hash(i);
    float sig = fract(pos_i)<=.5?-1.:+1.;
        
    //float speed_i = hash(sin(i)*10.);
    //float vTime = time*.05*speed_i;//iMouse.x*10.*hash2_i;
	float offset = pos_i+vTime*sig;
        
    float len = abs(max-min);
    offset = offset-floor(offset/len);
    offset = min+len*offset;
    //offset = offset < min ? max+offset : offset;
    //offset = serp(min,max,offset);
    //offset = smoothstep(min,max,offset);
    return offset;
}
void main( void ) {

	vec2 pos = ( gl_FragCoord.xy / iResolution.xy );

	vec2 tx = 1./iResolution;
	
	vec4 newCol = vec4(.0);
	
	for(float i=1.;i<=1.;++i)
	{
        float speed_l = .25*hash(10.+i);
        speed_l *= time;//iMouse.x*10.*hash2_i;
        float offset_l = getOffset(i,speed_l);
        
        {
            float speed_P = hash(20.+i);
            speed_P *= time;//iMouse.x*10.*hash2_i;
            
            float rd = max(tx.x,tx.y)*200.;
            float offset_p = getOffset(30.+i,speed_P,-rd,rd);
            
            vec2 d = vec2(offset_l,offset_p) - pos;
            float ld = length(d);
            if( ld<=rd.x )
            {
                float sc=ld/rd.x;
                float tv = 1.-sin(sc*200.*time)*.5;
                newCol = vec4(tv,1.-tv,tv,
                .0
                );
                newCol = mix(newCol,vec4(.0,.0,.0,.0),sc);
                //break;
            }
        }
        
        if(abs(offset_l - pos.x)<tx.x){newCol=vec4(1.,.0,i,1.);}
		//if(abs(offset - pos.y)<tx.y){t=vec3(t.x,1.,i);}
        //if(t.z!=.0)break;
	}
	
	//TEXT1 = iMouse.x*1000.;
	//if(TEXT1!=.0)newCol.rgb = drawText(vec2(505.,60.),newCol.rgb,TEXT1);
	//if(TEXT2!=.0)newCol = drawText(vec2(505.,36.),newCol,TEXT2);
	//if(TEXT3!=.0)newCol = drawText(vec2(505.,10.),newCol,TEXT3);
	gl_FragColor = vec4( newCol );

}