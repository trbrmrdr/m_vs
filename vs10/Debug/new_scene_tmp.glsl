#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable

uniform float time;
uniform vec3 iMouse;
uniform vec2 iResolution;

void main( void ) {

	vec2 p = ( gl_FragCoord.xy - 0.5 * iResolution.xy ) / iResolution.y;
	p /= dot(p, p);
	float tmp = sin(time*.2)*8.;//*cos(time);
	float t = time*.5;
	float tmp2 = sin(t)*cos(t)*5.;
	p.x = sin(p.x*(1.*iMouse.x*tmp))*time*.0001*iMouse.x;
	p.y = cos(p.y*(1.*iMouse.y*tmp2))*time*.1*iMouse.y;
	gl_FragColor = vec4( vec3(atan(p.x, p.y)), 1.0 );

}


//#########################
μμμμμ
#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable

uniform float time;
uniform vec3 iMouse;
uniform vec2 iResolution;

float hash(vec2 p) {
	return fract(sin(p.x * 15.67 + p.y * 37.28) * 43758.26);
}

void main( void ) {

	vec2 p = ( gl_FragCoord.xy - 0.5 * iResolution.xy ) / iResolution.y;
	
	float theta = 3.141592 / 4.0;
	float cn = cos(theta);
	float sn = sin(theta);
	vec2 o = p;
	mat2 m = mat2(cn, -sn, sn, cn);
	p = m * p;
	p = p * 10.0;
	p.x += 0.1 * sin(p.y * 3.14 * 1.0);
	//p.x += 0.5 * sin(o.y * 3.14 * 10.0);
	vec2 q = p;
	float k = hash(floor(q));
	p = mod(p, 1.0) - 0.5;
	float d = length(p);
	float c = 0.5 + 0.5 * sin(d * 3.14 * (5.0 + 10.0 * k) + time);
	c = 0.8 * c + 0.6 * k;
	vec3 ca = vec3(1.0, sin(time), 0.0);
	vec3 cb = vec3(0.4, 0.0, 0.7);
	vec3 cc = vec3(0.3, 0.8, 0.2);
	vec3 cd = vec3(0.7, 0.74, 0.2);
	vec3 ce = vec3(0.32, 0.0, 0.2);
	
	vec3 col = mix(ca, cb, smoothstep(0.0, 0.25, c));
	col = mix(col, cc, smoothstep(0.25, 0.5, k));
	col = mix(col, cd, smoothstep(0.5, 0.75, c));
	col = mix(col, ce, smoothstep(0.75, 1.0, c));

	gl_FragColor = vec4( col, 1.0 );

}
//###################################


smoothstep
#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable

uniform float time;
uniform vec3 iMouse;
uniform vec2 iResolution;

float circle(vec2 pos, float s){
	return smoothstep(s+0.02,s+0.01,length(pos))-smoothstep(s+0.01,s,length(pos));
}

void main( void ) {

	vec2 p =  gl_FragCoord.xy/iResolution.xy ;
	
	float ratio =min(iResolution.x/iResolution.y,iResolution.y/iResolution.x);
	p-=0.5;
	p.x /= ratio;
	//p-=vec2(0.5,0.5ratio);
		//( gl_FragCoord.xy -0.5* iResolution.xy)/iResolution.y;
	
	float col = smoothstep(0.14,0.5,sin(length(p)*20.0+time)-(length(p*1.5)));
	
	
	gl_FragColor = vec4( vec3(0.5,0.0,0.5)*circle(p,sin(p.y*10.0+time)+sin(p.x*10.0))+vec3(0.0,1.0,0.5)*circle(p,0.43), 1.0 );

}
//########################