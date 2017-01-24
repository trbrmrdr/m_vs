// Visualization of the system in Buffer A

uniform sampler2D iChannel0;
uniform vec2 iResolution;

void main(void)
{
    vec2 texel = 1. / iResolution.xy;
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    vec3 c = texture2D(iChannel0, uv).xyz;
    vec3 norm = normalize(c);
    
    vec3 div = vec3(0.1) * norm.z;    
    vec3 rbcol = 0.5 + 0.6 * cross(norm.xyz, vec3(0.5, -0.4, 0.5));
    
    gl_FragColor = vec4(rbcol + div, 0.0);
}