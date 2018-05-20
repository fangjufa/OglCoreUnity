#pragma once
// Simple vertex shader source
#define VERTEX_SHADER_SRC()											\
	"#version 150\n"												\
	"in highp vec3 pos;\n"											\
	"in highp vec2 texcoord;\n"										\
	"out highp vec2 otexcoord;\n"									\
	"void main()\n"													\
	"{\n"															\
	"	gl_Position = vec4(pos,1);\n"								\
	"	otexcoord = texcoord;\n"									\
	"}\n"															\

// Simple single texture fragment shader source
#define FRAGMENT_SINGLE_TEXTURE_SHADER_SRC()										\
	"#version 150\n"												\
	"uniform sampler2D mSampler;\n"								    \
	"in lowp vec2 otexcoord;\n"										\
	"out lowp vec4 fragColor;\n"									\
	"void main()\n"													\
	"{\n"															\
	"   \n"															\
	"	fragColor = texture2D(mSampler,otexcoord);\n"			    \
	"}\n"															\

// Simple sidebyside texture fragment shader source
#define FRAGMENT__DOUBLE_TEXTURE_SHADER_SRC()						\
	"#version 150\n"												\
	"uniform sampler2D leftSampler;\n"								\
	"uniform sampler2D rightSampler;\n"								\
	"in lowp vec2 otexcoord;\n"										\
	"out lowp vec4 fragColor;\n"									\
	"void main()\n"													\
	"{\n"															\
	"  float uOffset = otexcoord.x <= 0.5 ? 0.0 : 0.5;\n"           \
	"  vec2 modifiedUv = vec2((otexcoord.x - uOffset) * 2.0, otexcoord.y);\n"\
	"if (otexcoord.x <= 0.5)\n"											\
	"	fragColor = texture2D(leftSampler, modifiedUv);\n"		\
	"else \n"														\
	"	fragColor = texture2D(rightSampler, modifiedUv);\n"		\
	"}\n"															\
//本来提示未申明的标记，加了一行空行就可以了，神奇。