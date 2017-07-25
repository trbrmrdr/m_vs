#ifdef GL_ES
precision mediump float;
#endif
 
uniform vec2 iResolution;
uniform vec2 iMouse;
uniform float iGlobalTime;

float m_LineWidth = 0.3;
float plot(float fx,float uv_y){
  return  smoothstep( fx-m_LineWidth, fx, uv_y) - 
          smoothstep( fx, fx+m_LineWidth, uv_y);
}

vec3 cl = vec3(1.,0.,0.);
vec3 plot_fx(float fx,vec3 oldColor,vec2 uv){
    float pct = plot(fx,uv.y);
    return mix(oldColor,cl,pct);
}
mat2 rotate(float alpha){
    return mat2(cos(alpha),sin(alpha),
               	sin(alpha),-cos(alpha));
}

float sin01(float v){ return .5*(sin(v)+1.);}
float sinab(float a,float b,float v){return a+(b-a)*sin01(v);}

vec3 rgb2hsb( in vec3 c ){
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), 
                 vec4(c.gb, K.xy), 
                 step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), 
                 vec4(c.r, p.yzx), 
                 step(p.x, c.r));
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), 
                d / (q.x + e), 
                q.x);
}

//  Function from Iñigo Quiles 
//  https://www.shadertoy.com/view/MsS3Wc
vec3 hsb2rgb( in vec3 c ){
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                             6.0)-3.0)-1.0, 
                     0.0, 
                     1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix(vec3(1.0), rgb, c.y);
}



vec3 test_img(vec2 uv,float shift){
    
    float alpha =  atan( uv.y, uv.x);
    float r = length(uv);
    
    float sp = sin(100.*( -0.452*sqrt(r)+alpha*(.1) + iGlobalTime*.1) ) ;
    //sp = mod(fract(iGlobalTime*fract(alpha*.001)),sp);
    vec3 color = vec3(sp ,0.,1.);
    
    color = mix(color,
                vec3(
                    smoothstep(.0,1.,shift+.13+.77*sin01(iGlobalTime*.15))
                    ,smoothstep(.0,1.,shift+.6+.4*sin01(iGlobalTime*.05))
                    ,smoothstep(.0,1.,shift+.2+.8*sin01(iGlobalTime*.15))
                )
                ,fract(sp*sin01(iGlobalTime*.4)));
    return color;
}

void main() {
    vec2 uv = gl_FragCoord.xy/iResolution.xy;
    float dx = iResolution.x/iResolution.y;
    uv.x *= dx;

    uv = uv * 2. - 1.;
    vec2 mouse = (iMouse.xy*2.-1.)*100.;
    uv+=vec2(-0.5*dx,0.);
    //uv*=iMouse.xy.x;
    
    //uv+=
    uv *=rotate(iGlobalTime);
#if 1
    float angle =  atan( uv.y, uv.x);
    float r = length(uv);
    
    float sign = mod(r,0.914 * sinab(.8,1.2,iGlobalTime*.5))>=0.448? -1.:+1.;
    //sign = 1.;
    
    float it_sp = 100.*( -1.620 * sqrt(r )+angle*(-0.180 * sign) ) ;
    
    float sp = cos(it_sp) ;
    float sp_2 = sp;
    sp = sp>.4*r?1.:.0;
    vec3 color = vec3(sp);
    
    //color = mix(color, vec3(.1+r, sin(angle), sp), sinab(.0,1.5,iGlobalTime)*sp_2);
    color = mix(color, hsb2rgb(vec3(sign*(r+iGlobalTime*.5) ,sp,sp)), sinab(.0,1.5,iGlobalTime)*sp_2*r);
    //color = hsb2rgb(vec3(iGlobalTime*.5*sign+ ( sign * r *0.1*sp_2),sp,sp));
    //color = mix(color,color,sinab(.0,1.5,iGlobalTime)*sp_2*r);
#else
    //####################
    vec3 color = test_img(uv,0.);
   
    for(float i=0.;i<4.;i++){
    	vec3 color2 = test_img(uv + 
                               vec2(sin(iGlobalTime*.1), sin(iGlobalTime*.34*i)) 
            ,sin01(iGlobalTime*.04)*1.1*i);
    	color = mix(color,color2,sin01(iGlobalTime*.5));
	}
#endif
   
    //color = plot_fx(sp ,color,uv);
    
    
    
    gl_FragColor = vec4(color,1.0);
}
