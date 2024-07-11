#Shader:vertex
#version 330 core
layout(location = 0) in vec3 position;
out vec2 FragUV;
void main()
{
	gl_Position = vec4(position,1);

	//Bring from range [-1:1] to [0:1]
	FragUV = (position.xy + 1.)/2.;
}

#Shader:fragment
#version 330 core

out vec4 FragColor;
in vec2 FragUV;

uniform sampler2D u_framebufferTexture;

void main()
{
	FragColor = vec4(texture(u_framebufferTexture,FragUV));
}


