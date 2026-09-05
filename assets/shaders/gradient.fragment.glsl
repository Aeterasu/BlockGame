in vec2 vUV;
out vec4 FragColor;

void main()
{
		FragColor = mix(vec4(1.0, 1.0, 1.0, 1.0), vec4(0.0, 0.0, 0.0, 1.0), vUV.y);
}
