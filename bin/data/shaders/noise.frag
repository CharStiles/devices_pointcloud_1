#version 410

uniform float u_time;
uniform vec2 iResolution;
uniform sampler2DRect iChannel0;
uniform vec4 globalColor;

float iTime = u_time;

// void main(){
// 	//this is the fragment shader
// 	//this is where the pixel level drawing happens
// 	//gl_FragCoord gives us the x and y of the current pixel its drawing

// 	//we grab the x and y and store them in an int
// 	float xVal = gl_FragCoord.x;
// 	float yVal = gl_FragCoord.y;
// 	float r = mod((255.*gl_Color.r),255.)/255.;
// 	gl_FragColor = vec4(r,gl_Color.g,gl_Color.b,gl_Color.a);


// }
#define THRESHOLD 0.005
#define MAX_DISTANCE 2.0

#define RAY_STEPS 30


// iq's LUT based 3d value noise
float noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
	f = f*f*(3.0-2.0*f);

	vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;
	vec2 rg = texture2DRect(iChannel0, (mod(uv,256.0) + 0.5)/256.0, -100.0 ).yx;
	//vec2 rg = vec2(mod(iTime/77.,1.),f.x);
	return mix( rg.x, rg.y, f.z );
}

float map( in vec3 p )
{
	vec3 q = p + 0.2*vec3(-1.0, 1.0, 2.2)*iTime;
	float f;
    f = 0.500*noise( q ); q = q*2.0;
    f += 0.25*noise( q ); q = q*2.0;
    f += 0.125*noise( q ); q = q*2.0;
//    f += 0.0625*noise( q ); q = q*2.0;
//    f += 0.03125*noise( q ); q = q*2.0;
//    f += 0.015625*noise( q );
	return f;
}


float scene(vec3 p)
{
	return min(length(p-vec3(0.0, 0.9, 0.0))-0.7, (pow(map(p), 6.0)*-2.0+0.2+0.5+p.y));
}

vec3 normal(vec3 p, float d)
{
	float e = 0.05;
	float dx = scene(vec3(e, 0.0, 0.0) + p) - d;
	float dy = scene(vec3(0.0, e, 0.0) + p) - d;
	float dz = scene(vec3(0.0, 0.0, e) + p) - d;
	return normalize(vec3(dx, dy, dz));
}

vec3 shadeBg(vec3 nml, vec2 fragCoord)
{
	vec2 aspect = vec2(iResolution.x/iResolution.y, 1.0);
	vec2 uv = (2.0 * fragCoord.xy / iResolution.xy - 1.0) * aspect;
    float t = 144.57;
	vec3 bgLight = normalize(vec3(
		cos(t*0.2/0.954929658551372)*4.0,
		sin(t/1.1936620731892151)*3.0 - 4.0,
		sin(t*0.2/0.954929658551372)*8.0
	));
	vec3 sun = vec3(3.0, 1.0, 0.5);
	float bgDiff = dot(nml, vec3(0.0, -1.0, 0.0));
	float sunPow = dot(nml, bgLight);
	float sp = max(sunPow, 0.0);
	vec3 bgCol = max(0.0, bgDiff)*2.0*vec3(0.4, 0.7, 1.0);
	bgCol += max(0.0, -bgDiff)*vec3(0.4, 0.55, 0.7);
	bgCol += vec3(0.2, 0.5, 0.7)*((0.5*pow(1.0-abs(bgDiff), 5.0)*(5.0-dot(uv,uv))));
	bgCol += sun*(0.5*pow( sp, 3.0)+pow( sp, 256.0));
	bgCol += vec3(0.5, 0.2, 0.15)*(pow( sp, 8.0) + pow( sp, abs(bgLight.y)*128.0));
    bgCol += vec3(1.9,0.,0.9) * ((1.0-pow(abs(bgDiff), 0.6)) * 1.0 * map(-nml) * map(-nml*nml.y));
	return pow(max(vec3(0.0), bgCol), vec3(1.9));
}

void main()
{

		vec2 aspect = vec2(iResolution.x/iResolution.y, 1.0);
		if (iResolution.x < iResolution.y) {
			aspect = vec2(1.0, iResolution.y / iResolution.x);
		}
		vec2 uv = (2.0 * gl_FragCoord.xy / iResolution.xy - 1.0) * aspect;
		vec3 d = normalize(vec3(uv, 1.0));
		vec3 p = vec3(uv*-2.0, -6.5) + d*3.6;
	    for (int i=0; i<RAY_STEPS; i++) {
	        float dist = scene(p);
	        if (dist < THRESHOLD ||gl_Color.r>0.01 ) {
	            vec3 nml = normal(p, dist);
	            d = reflect(d, nml);
	            p += (23.0*THRESHOLD) * d;
	        }
	        if (dist > MAX_DISTANCE) {
	            break;
	        }
					// if (gl_Color.r>0.01){
					// 	d = gl_Color.rgb;
					// }
	        p += dist * d;
	    }
	// if (gl_Color.r>0.01){
	// 	gl_FragColor = gl_Color;
	// }
	//else{
		vec4 rg = texture2D(iChannel0, (mod(uv,256.0) + 0.5)/256.0, -100.0 );
		gl_FragColor = rg;
		//gl_FragColor = vec4( 1.0 - exp(-1.3 * shadeBg(-d, gl_FragCoord.xy)), 1.0 );
	//}
}

// // Author @patriciogv - 2015
// // http://patriciogonzalezvivo.com

// #ifdef GL_ES
// precision mediump float;
// #endif

// uniform float u_time;

// float random (in vec2 _st) {
//     return fract(sin(dot(_st.xy,
//                          vec2(12.9898,78.233)))*
//         43758.5453123);
// }

// // Based on Morgan McGuire @morgan3d
// // https://www.shadertoy.com/view/4dS3Wd
// float noise (in vec2 _st) {
//     vec2 i = floor(_st);
//     vec2 f = fract(_st);

//     // Four corners in 2D of a tile
//     float a = random(i);
//     float b = random(i + vec2(1.0, 0.0));
//     float c = random(i + vec2(0.0, 1.0));
//     float d = random(i + vec2(1.0, 1.0));

//     vec2 u = f * f * (3.0 - 2.0 * f);

//     return mix(a, b, u.x) +
//             (c - a)* u.y * (1.0 - u.x) +
//             (d - b) * u.x * u.y;
// }

// #define NUM_OCTAVES 5

// float fbm ( in vec2 _st) {
//     float v = 0.0;
//     float a = 0.5;
//     vec2 shift = vec2(100.0);
//     // Rotate to reduce axial bias
//     mat2 rot = mat2(cos(0.5), sin(0.5),
//                     -sin(0.5), cos(0.50));
//     for (int i = 0; i < NUM_OCTAVES; ++i) {
//         v += a * noise(_st);
//         _st = rot * _st * 2.0 + shift;
//         a *= 0.5;
//     }
//     return v;
// }

// void main() {
//     vec2 st = gl_FragCoord.xy;
//     // st += st * abs(sin(u_time*0.1)*3.0);
//     // vec3 color = vec3(0.0);

//     vec2 q = vec2(0.);
//     q.x = fbm( st + 0.00*u_time);
//     q.y = fbm( st + vec2(1.0));

//     vec2 r = vec2(0.);
//     r.x = fbm( st + 1.0*q + vec2(1.7,9.2)+ 0.15*u_time );
//     r.y = fbm( st + 1.0*q + vec2(8.3,2.8)+ 0.126*u_time);

//     float f = fbm(st+r);

//     // color = mix(vec3(0.101961,0.619608,0.666667),
//     //             vec3(0.666667,0.666667,0.498039),
//     //             clamp((f*f)*4.0,0.0,1.0));

//     // color = mix(color,
//     //             vec3(0,0,0.164706),
//     //             clamp(length(q),0.0,1.0));

//     // color = mix(color,
//     //             vec3(0.666667,1,1),
//     //             clamp(length(r.x),0.0,1.0));

//     gl_FragColor = vec4((f*f*f+.6*f*f+.5*f)*gl_FragColor.rgb,1.);
// }
