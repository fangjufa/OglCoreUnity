#include<assert.h>
#include"GLEW\glew.h"
#include"Debug.h"

using namespace dxlib;

class TextureQuad {
public:
	GLuint m_VAO;
	GLuint m_VertexShader;
	GLuint m_FragmentShader;

	//顶点缓存，还要看一下在哪里用
	GLuint m_VertexBuffer;
	//索引缓存
	GLuint m_IndexBuffer;
	//shader对象
	GLuint m_Program;

	GLuint kVertexInputPosition = 0;
	GLuint kVertexInputTexcoord = 1;

	bool CreateShader(const char* vertexShaderText,const char* fragmentShaderText)
	{
		if (m_Program != 0)
		{
			return true;
		}
		m_VertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderText);
		//if (isDrawLR == FALSE)
		//	m_FragmentShader = CompileShader(GL_FRAGMENT_SHADER, kGlesFSingleShaderTextGLCore);
		//else
		m_FragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderText);

		// Link shaders into a program and find uniform locations
		m_Program = glCreateProgram();
		if (m_Program != 0)
			Debug::GetInst()->Log("Shader program not null.\r\n");
		else
			return false;

		//将变量kVertexInputPosition与shader中的pos变量绑定。
		glBindAttribLocation(m_Program, kVertexInputPosition, "pos");
		glBindAttribLocation(m_Program, kVertexInputTexcoord, "texcoord");

		glAttachShader(m_Program, m_VertexShader);
		glAttachShader(m_Program, m_FragmentShader);

		//加上这句话之后，glBindAttribLocation的语句才开始生效。
		glLinkProgram(m_Program);

		GLint status = 0;
		glGetProgramiv(m_Program, GL_LINK_STATUS, &status);
		//Debug::GetInst()->Log("status:%d .\r\n", status);
		if (status != GL_TRUE)
		{
			Debug::GetInst()->Log("status:%d .\r\n", status);
			return false;
		}

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			Debug::GetInst()->Log("Create shader error:%d.\r\n", error);
			return false;
		}
		return true;
	}

	///创建mesh信息，参数包括顶点位置及其索引等。
	///由于动态数组的大小无法获取，所以要以参数的形式加进来。
	void CreateResource(float* m_vertexArray,int m_vertexLength,int* m_indexArray,int m_indexLength) {
		vertexArray = m_vertexArray;
		indexArray = m_indexArray;
		vertexLength = m_vertexLength;
		indexLength = m_indexLength;

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertexLength * sizeof(float), vertexArray, GL_STATIC_DRAW);

		//创建索引缓存
		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexLength * sizeof(int), indexArray, GL_STATIC_DRAW);

		//创建VAO。
		glGenVertexArrays(1, &m_VAO);

		assert(glGetError() == GL_NO_ERROR);
		Debug::GetInst()->Log("Create resource success.\r\n");
	}

	///每帧更新绘制
	void Draw(GLuint lTexture, GLuint rTexture,bool isDrawHeader,int headerW,int headerH,void* headerData) {
		glUseProgram(m_Program);

		glBindVertexArray(m_VAO);
		//下面的几句话，操作的数组是在glBindBuffer中设置GL_ARRAY_BUFFER设定的。

		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertexLength, vertexArray);
		//开启GPU读取顶点着色器的数据，如果不写这句，GPU是无法读取数据的，因为CPU与GPU之间的通道默认是关闭的。
		glEnableVertexAttribArray(kVertexInputPosition);

		//给shader中传递Position。
		//最后一个参数是指偏移量，因为只有这一个数组，所以偏移量为0.
		glVertexAttribPointer(kVertexInputPosition, 3/*每个顶点属性的组件数量，比如每个顶点由3个float值组成*/, GL_FLOAT, GL_FALSE, vertexLength/*步长，连续两个顶点之间跨越的值*/, (char*)0);

		glEnableVertexAttribArray(kVertexInputTexcoord);
		glVertexAttribPointer(kVertexInputTexcoord, 2, GL_FLOAT, GL_FALSE, vertexLength, (char*)NULL + 12);
		//索引数据
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);


		//glEnum = glGetError();
		//if (glEnum != GL_NO_ERROR)
		//	Debug::GetInst()->Log("Bind vertex data error:%d.\r\n", glEnum);

		//贴图数据
		if (rTexture == 0)
		{
			GLuint texLocation = glGetUniformLocation(m_Program, "mSampler");
			glUniform1i(texLocation, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, lTexture);
			if (isDrawHeader)
			{
				//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, headerH-1, headerW, headerH, GL_RGBA, GL_UNSIGNED_BYTE, headerData);
				//这里1079其实是窗口的高1080-1得到的。
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 1079, headerW, headerH, GL_RGBA, GL_UNSIGNED_BYTE, headerData);
			}
		}
		else
		{
			//Debug::GetInst()->Log("isDrawLR TRUE.\r\n");
			GLuint lTex = glGetUniformLocation(m_Program, "leftSampler");
			GLuint rTex = glGetUniformLocation(m_Program, "rightSampler");
			glUniform1i(lTex, 0);
			glUniform1i(rTex, 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, lTexture);
			//该方法是修改图像的指定像素值，当图像被伸缩时，它也会被伸缩，我们需要寻求一个直接在窗口上改变的方法。
			//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 690, headerWidth, headerHeight, GL_RGBA, GL_UNSIGNED_BYTE, headerData);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, rTexture);

		}

		//第一个参数：三角形的形态，第二个参数，
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(kVertexInputPosition);
		glDisableVertexAttribArray(kVertexInputTexcoord);
		glBindTexture(GL_TEXTURE_2D, 0);
		//在绘制完之后清空一下VAO。
		glBindVertexArray(0);
	}

	void Clean()
	{
		m_Program = 0;
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VertexBuffer);
		glDeleteBuffers(1, &m_IndexBuffer);
	}

private:
	float* vertexArray;
	int*   indexArray;
	int    vertexLength;
	int    indexLength;

	GLuint CompileShader(GLenum type, const char* sourceText)
	{
		GLuint ret = glCreateShader(type);
		glShaderSource(ret, 1, &sourceText, NULL);
		glCompileShader(ret);
		if (ret != GL_ZERO)
			Debug::GetInst()->Log("Compile shader success.\r\n");
		else
			Debug::GetInst()->Log("Compile shader failed.Type:%d;error:%d.\r\n", type, ret);
		return ret;
	}
};