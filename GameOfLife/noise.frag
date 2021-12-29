#version 450

out vec4 fragment;
uniform vec2 displacement;

highp float rand(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt= dot(co.xy ,vec2(a,b));
    highp float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}

void main() {
    float r = rand(gl_FragCoord.xy + displacement);
	float val = round(r);

    fragment = vec4( vec3(val), 1.0 );
}