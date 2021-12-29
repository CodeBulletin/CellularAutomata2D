#version 450

out vec4 frag;

uniform sampler2D tex;
uniform float Width;
uniform float Height;

vec2 wrapAround(vec2 coord) {
	return mod(coord + vec2(1, 1), vec2(1, 1));
}

float getNeighbourValue(vec2 coord, vec2 neighbour) {
	vec2 mapSize = vec2(Width, Height);
	return texture2D(tex, wrapAround((coord + neighbour) / mapSize)).x;
}

float getNeighbourSums(vec2 coord) {
	return 
		getNeighbourValue(coord, vec2(-1, -1)) +
		getNeighbourValue(coord, vec2(1, 1)) +
		getNeighbourValue(coord, vec2(1, -1)) +
		getNeighbourValue(coord, vec2(-1, 1)) +
		getNeighbourValue(coord, vec2(0, 1)) +
		getNeighbourValue(coord, vec2(0, -1)) +
		getNeighbourValue(coord, vec2(-1, 0)) +
		getNeighbourValue(coord, vec2(1, 0));
}

float getState(vec2 coord, float CurrentState) {
	float nSum = getNeighbourSums(coord);
	float nState;
	if (CurrentState == 1.0) {
		if (nSum == 2.0 || nSum == 3.0)
			nState = 1.0;
		else
			nState = 0.0;
	}
	else if (CurrentState == 0.0) {
		if (nSum == 3.0)
			nState = 1.0;
		else
			nState = 0;
	}
	return nState;
}

void main() {
	vec4 col = texture2D(tex, gl_FragCoord.xy / vec2(Width, Height));
	col.y = getState(gl_FragCoord.xy, col.x);
	frag = col;
}