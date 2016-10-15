#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

void main( void ) {

	vec2 p = ( gl_FragCoord.xy - 0.5 * resolution.xy ) / resolution.y;
	p /= dot(p, p);
	float tmp = sin(time*.2)*8.;//*cos(time);
	float t = time*.5;
	float tmp2 = sin(t)*cos(t)*5.;
	p.x = sin(p.x*(1.*mouse.x*tmp))*time*.0001*mouse.x;
	p.y = cos(p.y*(1.*mouse.y*tmp2))*time*.1*mouse.y;
	gl_FragColor = vec4( vec3(atan(p.x, p.y)), 1.0 );

}