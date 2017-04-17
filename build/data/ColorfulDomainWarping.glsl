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


float random (in vec2 st) { 
  return fract(sin(dot(st.xy,
                       vec2(12.9898,78.233)))* 
               43758.5453123);
}

// Based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 st) {
  vec2 i = floor(st);
  vec2 f = fract(st);

  // Four corners in 2D of a tile
  float a = random(i);
  float b = random(i + vec2(1.0, 0.0));
  float c = random(i + vec2(0.0, 1.0));
  float d = random(i + vec2(1.0, 1.0));

  vec2 u = f * f * (3.0 - 2.0 * f);

  return mix(a, b, u.x) + 
      (c - a)* u.y * (1.0 - u.x) + 
      (d - b) * u.x * u.y;
}

#define OCTAVES 10
float fbm (in vec2 st) {
  // Initial values
  float value = 0.0;
  float amplitude = 1.4;
  float frequency = 2.;
  //
  // Loop of octaves
  for (int i = 0; i < OCTAVES; i++) {
    value += amplitude * noise(st);
    st *= 3.;
    amplitude *= .4;
  }
  return value;
}

float fbmWarp2(in vec2 st, out vec2 q, out vec2 r)  {
  q.x = fbm(st + vec2(0.0,0.0));
  q.y = fbm(st + vec2(5.2,1.3));

  r.x = fbm( st + 4.0*q + vec2(1.7,9.2) + 0.7*iGlobalTime);
  r.y = fbm( st + 4.0*q + vec2(8.3,2.8) + 0.7*iGlobalTime);

  return fbm( st + 4.0*r);
}

vec3 hsb2rgb( in vec3 c ){
  vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                           6.0)-3.0)-1.0,
                   0.0,
                   1.0 );
  rgb = rgb*rgb*(3.0-2.0*rgb);
  return c.z * mix(vec3(1.0), rgb, c.y);
}

void main() {
  vec2 st = gl_FragCoord.xy/iResolution.xy;
  st.x *= iResolution.x/iResolution.y;

  vec3 color = vec3(0.0);
  vec2 q = vec2(0.);
  vec2 r = vec2(0.);
  float height = fbmWarp2(st*10., q, r);

  //color += hsb2rgb(vec3(0.3,1.0 - (0.5*sin(iGlobalTime) + 0.5),height));
  color = mix(color, hsb2rgb(vec3(0.0,q.x,0.2 + (0.2*sin(0.7*iGlobalTime) + 0.2))), length(q));
  color = mix(color, hsb2rgb(vec3(0.58,r.x,0.0 + (0.25*sin(0.3*iGlobalTime) + 0.25))), r.y);

  gl_FragColor = vec4(color,1.0);
}
