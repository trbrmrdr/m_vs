
#ifdef GL_ES
precision mediump float;
#endif
 
#define SWAP(x,y) {float T=x;x=y;y=T;}
uniform float lowFreq;
uniform float midFreq;
uniform float highFreq;

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
 
void main(void){
	vec2 v = (gl_FragCoord.xy - resolution/2.0) / min(resolution.y,resolution.x) * 20.0;
	vec2 vv = v; vec2 vvv = v;
	float tm = time*.0026*mouse.x;//*(mouse.x/resolution.x);//time*0.03;
	tm = 1255.+30.*.0026*mouse.x;
	vec4 t1 = vec4(.2,.5,.4,20.7);//*tm*.01;
	vec4 t2 = vec4(.2,.8,1.1,1.5);
	vec2 mspt = (vec2(
			sin(tm)+cos(tm*t1.r)+sin(tm*0.5)+cos(tm*t1.b)+sin(tm*t1.a),//-sin(12.*tm),
			cos(tm)+sin(tm*0.1)+cos(tm*0.8)+sin(tm*-1.1)+cos(tm*t2.a)
			)+1.0)*0.1; //5x harmonics, scale back to [0,1]
	float R = 0.0;////+sin(time)*cos(time);
	float RR = 0.0+3.*cos(time);
	float RRR = 0.0+1.*sin(time);
	float a = (5.-mspt.x)*3.2;
	float C = cos(a);
	float S = sin(a);
	vec2 xa=vec2(C, -S);
	vec2 ya=vec2(S, C);
	vec2 shift = vec2( 0, 1.62);
	float Z = 1.0 + mspt.y*6.0;
	float ZZ = 1.0 + (mspt.y)*6.2;
	float ZZZ = 1.0 + (mspt.y)*6.9;
	
	for ( int i = 0; i < 120; i++ ){
		float r = dot(v,v);
		if ( r > 1.0 )
		{
			r = (1.0)/r ;
			v.x = v.x * r;
			v.y = v.y * r;
		}
		R *= .99*mouse.x;
		R += r;
		if(i < 39){
			RR *= .99*mouse.x;
			RR += r;
			if(i < 18*sin(1-lowFreq*.001)){
				RRR *= .99;
				RRR += r;
			}
		}
		
		v = vec2( dot(v, xa)* ZZ, dot(v, ya)* ZZ)  + shift;
	}
	//SWAP(RRR,RR);
	//SWAP(RR,R);
	
	float c = ((mod(R,2.0)>1.0)?1.0-fract(R):fract(R));
	float cc = ((mod(RR,2.0)>1.0)?1.0-fract(RR):fract(RR));
	float ccc = ((mod(RRR,2.0)>1.0)?1.0-fract(RRR):fract(RRR));
	gl_FragColor = vec4(ccc, cc, c, 1.0); 
}