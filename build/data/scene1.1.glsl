#ifdef GL_ES
//precision mediump float;
precision highp float;
//precision lowp float;
#endif

uniform sampler2D fft;//1024 1
uniform sampler2D optTex;
uniform sampler2D backbuffer;

uniform vec2 iResolution;
uniform vec3 iMouse;
uniform float lowFreq;
uniform float midFreq;
uniform float highFreq;
uniform float iGlobalTime;
#define time iGlobalTime

//varying vec2 v_uv;
/*
void main() {
  vec2 pos = vec2(0.5, 0.5) - gl_FragCoord.xy / iResolution.y;
  
  float d = length(pos);
  float a = atan(pos.y, pos.x);
  
//  vec2 tex = vec2(time + 2.0 / (6.0 * d + 3.0 * pos.x), 3.0 * a / 3.14);
  vec2 tex = vec2(time + 2.0 / (6.0 * d + 3.0 * pos.x), 4.0 * d / 3.14 + 3.0 * a / 3.14);
  
  float f = min(1.0, d/0.3);
  
  gl_FragColor = texture2D(optTex, tex) * f;
}
*/


void main( void ) {
//return;
//
    float iMouse_pos = iMouse.x/iResolution.x;
    vec2 uv = .987 * gl_FragCoord.xy / iResolution.y;
    float t = time*.01;//.01125;
    float k = cos(t); 
    float l = sin(t);        
    
    float s = .456+midFreq;
    float tmp = .963 +lowFreq*.003;
    for(int i=0; i<96; ++i) {
        uv  = abs(uv) - s;    // Mirror
        uv *= mat2(k,-l,l,k); // Rotate
        s  *= tmp;//0.963;// * tmp;         // Scale
    }
    
    float x = .5 + .5*cos((3.28318+(2.-midFreq*4.))*337.*length(uv));
    //x = texture2D(backbuffer,gl_FragCoord);
    //x.r = texture2D(fft,vec2(0,0)).r;
    gl_FragColor = vec4(
        //vec3(length(uv)*37.0,0.755*x, 0.955/x),
        vec3(length(uv)*37*(lowFreq),
        (.5+lowFreq)*x,
        1*(1-lowFreq)/x),
    0.65);
    
    
    int fix_size = 1000;
    vec2 uv_t = gl_FragCoord / fix_size;
    vec4 newCol = gl_FragColor;
    if(gl_FragCoord.x<=fix_size)
    {
        float lc = texture2D(fft,vec2(uv_t.x,0)).r;
        //if(uv_t.y<=lc)
        //   newCol =vec4(1.);
        
        float rc = texture2D(fft,vec2(uv_t.x,0)).g;
        //if(uv_t.y<=rc)
        //    newCol = vec4(.0);
    } 
    if(uv_t.x<=.1 && uv_t.y<=lowFreq)
        newCol = vec4(vec3(1.),.1);
    
    if(uv_t.x >=.15 && uv_t.x<=.3 && uv_t.y<=midFreq)
       newCol = vec4(vec3(.7),.1);
    
    if(uv_t.x >=.45 && uv_t.x<=.7 && uv_t.y<=highFreq)
       newCol = vec4(vec3(.3),.1);

    /*
    if(gl_FragCoord.x> fix_size && (gl_FragCoord.x-fix_size)<=fix_size)
    {
        float rc = texture2D(fft,vec2(uv_t.x,0)).g;
        if(uv_t.y<=rc)
            newCol = vec4(1.);
    }
    */
    gl_FragColor = newCol;
    //gl_FragColor = texture2D(backbuffer,gl_FragCoord);
}
