in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec4 uTint;

uniform float uProgress;

const float PI = 3.1415;
const float TAU = 2.0 * PI;
const vec2 CENTER = vec2(0.5);
const float START_ANGLE = -PI / 2.0;

void main()
{
		float realProgress = clamp(uProgress, 0.0, 1.0);

		vec4 base = texture(uTexture, vec2(vUV.x * 0.5, vUV.y));
		vec4 overlay = texture(uTexture, vec2((vUV.x * 0.5) + 0.5, vUV.y));

		vec2 dir = vUV - CENTER;

		float angle = atan(dir.y, dir.x) - START_ANGLE;
		angle = mod(angle, TAU);

		float pixelProgress = angle / TAU;
		float mask = step(pixelProgress, realProgress);

		FragColor = mix(base, overlay, mask) * uTint;
}
