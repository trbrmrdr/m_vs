// Fractals: MRS
// by Nikos Papadopoulos, 4rknova / 2015
// Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
//
// Adapted from  by J.

// best on 1 resolution

#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

uniform sampler2D fft;//1024 1
uniform float lowFreq;
uniform float midFreq;
uniform float highFreq;

float edge(float e1,float e2,float x,float v1,float v2){
	if(x>=e1 && x<=e2)
	//if(x<=e2)
		return v1;
	return v2;
}

bool edge_2(float e1,float e2,float c)
{
	if( c>=e1 && c<=e2)
		return true;
	return false;
}

void main( void ) {

    vec2 uv = .187 * gl_FragCoord.xy / resolution.y;
    //vec2 uv =  gl_FragCoord.xy / resolution.y + vec2(sin(time*0.003)*7.13,cos(time*0.005)*7.513);
    float t = time*.01125, k = cos(t), l = sin(t);        
    
    //float s = .456;
    float s = .1956;
    for(int i=0; i<97; ++i) {
        uv  = abs(uv) - s;    // Mirror
        uv *= mat2(k,-l,l,k); // Rotate
        //s  *= .959;         // Scale
    	s  *= .959;
    }
    
    float x = .5 + .5*cos(9.8318*(537.*length(uv)));
    vec3 rgb = vec3(length(uv)*37.0,0.755*x, 0.155/x);
	
    rgb.r = edge(.1,.3,rgb.r,1.,.0);
    rgb.g = edge(.3,.5,rgb.g,1.,.0);
    //rgb.b = smoothstep(1.5,2.,rgb.b);
    //rgb.b = edge(.6,.9,rgb.b,1.,.0);

	vec3 edge = vec3(.5,.5,.5);
	if(true){
	if(
		edge_2(.3,.7,rgb.r)
	      ||edge_2(.3,.7,rgb.g)
	      ||edge_2(.3,.7,rgb.b)
	   )
		rgb = vec3(1.);
		else
			rgb = vec3(.0);
	}
    if(false){
	
	if(rgb.r>=edge.r
	    //|| rgb.g>=edge.g
	    //|| rgb.b>=edge.b
	  )
		rgb = vec3(1.);
		else
		rgb = vec3(0.);
	}

    gl_FragColor = vec4(rgb,.65);
}