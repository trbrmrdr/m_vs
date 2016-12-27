#ifdef GL_ES
precision highp float;
#endif

// Simple Mandelbulb - by @SyntopiaDK
// (The camera code and lightning was taken from an example by @mrdoob)

uniform float lowFreq;
uniform vec2 resolution;
uniform float time;
uniform vec2 mouse;

uniform sampler2D backbuffer;


#define _PI 3.1415
#define _PI2 1.57
#define _2PI 6.283
#define IP 0.15915

void main(void){
    vec2 pos = ( gl_FragCoord.xy / resolution.xy );
    //pos = pos - vec2(.5);
    //pos*=resolution;
    if(mouse.x<=.1 && mouse.y<=.1)
    {
        gl_FragColor = vec4(.0);
        return;
    }
    
    vec2 it = 1./resolution;
    vec2 uv = gl_FragCoord.xy/resolution;
    vec4 pcol = vec4(vec3(1.),.1);
    bool skip = uv.x <= it.x && uv.y<=it.y;
    if(!skip)
    {
        if(uv.x <= it.x )//&& uv.y>.0)
            uv = vec2(1.- it.x,uv.y - it.y);
        else
            uv.x -= it.x;
        pcol = texture2D(backbuffer,uv);
    }
    
    if(pcol.a>.0)
    {
        //pcol += vec4(.001,.0,.0012,.01)*10.;
        ///*
        float nx;
        float ny;
        
        float x = (pcol.r*4.)-2.;
        float y = (pcol.g*4.)-2.;
        float id = pcol.b;
        
        float var_x = mouse.x*0.1;
        float var_y = mouse.y*0.1;
        //var_y = var_x = mouse.x;
        float A=-20.+40.;//*var_x;
        float B=-3.+9.*var_x;
        float C = -5.+10.;//*var_y;
        float D = -3.+6.*var_y;
        //############
        A=-2.7;
        B=-.09;
        C=-.86;
        D=-2.2;
        //############
        vec3 color = vec3(.0);
        float sc = 1.;
        //for(int i=1;i<2;++i)
        {
            nx=sin(A*y)-cos(B*x); 
            ny=sin(C*x)-cos(D*y);
            
            nx =(2.+nx)/4.;
            ny =(2.+ny)/4.;
            id=id+.1;
            id = id>=.5 ? .0: id;
            /*
            float v = sqrt(pow((x-nx),2.)+pow((y-ny),2.))*atan(ny,nx)*IP;
            color.r=(color.r+abs(sin(v*_PI*3.2-_PI*.2)))*.5;
            color.g=(color.g+abs(sin(v*_PI*12.3+_PI*1.5)*.5+.5))*.5;
            color.b=(color.b+(sin(v*_PI*3.84+_PI2-.6)*.5+.5))*.5;
            x=nx;
            y=ny;
            //nx=nx*sc+512.; 
            //ny=ny*sc+512.;
            */
        }
        //pcol = vec4(color,1.);
        pcol = vec4(nx,ny,id,1.);
    }
    else
    {
        pcol = vec4(.0);
    }
    gl_FragColor = pcol;
}