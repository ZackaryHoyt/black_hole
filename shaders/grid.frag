#version 330 core
out vec4 FragColor;

void main()
{
	// Semi-transparent cyan-ish grid line (blends over the scene)
	FragColor = vec4(0.2, 0.9, 1.0, 0.35);
}
