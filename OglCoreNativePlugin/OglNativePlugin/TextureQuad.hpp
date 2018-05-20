#include<assert.h>
#include"GLEW\glew.h"
#include"Debug.h"

using namespace dxlib;

class TextureQuad {
public:
	GLuint m_VAO;
	GLuint m_VertexShader;
	GLuint m_FragmentShader;

	//���㻺�棬��Ҫ��һ����������
	GLuint m_VertexBuffer;
	//��������
	GLuint m_IndexBuffer;
	//shader����
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

		//������kVertexInputPosition��shader�е�pos�����󶨡�
		glBindAttribLocation(m_Program, kVertexInputPosition, "pos");
		glBindAttribLocation(m_Program, kVertexInputTexcoord, "texcoord");

		glAttachShader(m_Program, m_VertexShader);
		glAttachShader(m_Program, m_FragmentShader);

		//������仰֮��glBindAttribLocation�����ſ�ʼ��Ч��
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

	///����mesh��Ϣ��������������λ�ü��������ȡ�
	///���ڶ�̬����Ĵ�С�޷���ȡ������Ҫ�Բ�������ʽ�ӽ�����
	void CreateResource(float* m_vertexArray,int m_vertexLength,int* m_indexArray,int m_indexLength) {
		vertexArray = m_vertexArray;
		indexArray = m_indexArray;
		vertexLength = m_vertexLength;
		indexLength = m_indexLength;

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertexLength * sizeof(float), vertexArray, GL_STATIC_DRAW);

		//������������
		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexLength * sizeof(int), indexArray, GL_STATIC_DRAW);

		//����VAO��
		glGenVertexArrays(1, &m_VAO);

		assert(glGetError() == GL_NO_ERROR);
		Debug::GetInst()->Log("Create resource success.\r\n");
	}

	///ÿ֡���»���
	void Draw(GLuint lTexture, GLuint rTexture,bool isDrawHeader,int headerW,int headerH,void* headerData) {
		glUseProgram(m_Program);

		glBindVertexArray(m_VAO);
		//����ļ��仰����������������glBindBuffer������GL_ARRAY_BUFFER�趨�ġ�

		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertexLength, vertexArray);
		//����GPU��ȡ������ɫ�������ݣ������д��䣬GPU���޷���ȡ���ݵģ���ΪCPU��GPU֮���ͨ��Ĭ���ǹرյġ�
		glEnableVertexAttribArray(kVertexInputPosition);

		//��shader�д���Position��
		//���һ��������ָƫ��������Ϊֻ����һ�����飬����ƫ����Ϊ0.
		glVertexAttribPointer(kVertexInputPosition, 3/*ÿ���������Ե��������������ÿ��������3��floatֵ���*/, GL_FLOAT, GL_FALSE, vertexLength/*������������������֮���Խ��ֵ*/, (char*)0);

		glEnableVertexAttribArray(kVertexInputTexcoord);
		glVertexAttribPointer(kVertexInputTexcoord, 2, GL_FLOAT, GL_FALSE, vertexLength, (char*)NULL + 12);
		//��������
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);


		//glEnum = glGetError();
		//if (glEnum != GL_NO_ERROR)
		//	Debug::GetInst()->Log("Bind vertex data error:%d.\r\n", glEnum);

		//��ͼ����
		if (rTexture == 0)
		{
			GLuint texLocation = glGetUniformLocation(m_Program, "mSampler");
			glUniform1i(texLocation, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, lTexture);
			if (isDrawHeader)
			{
				//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, headerH-1, headerW, headerH, GL_RGBA, GL_UNSIGNED_BYTE, headerData);
				//����1079��ʵ�Ǵ��ڵĸ�1080-1�õ��ġ�
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
			//�÷������޸�ͼ���ָ������ֵ����ͼ������ʱ����Ҳ�ᱻ������������ҪѰ��һ��ֱ���ڴ����ϸı�ķ�����
			//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 690, headerWidth, headerHeight, GL_RGBA, GL_UNSIGNED_BYTE, headerData);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, rTexture);

		}

		//��һ�������������ε���̬���ڶ���������
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(kVertexInputPosition);
		glDisableVertexAttribArray(kVertexInputTexcoord);
		glBindTexture(GL_TEXTURE_2D, 0);
		//�ڻ�����֮�����һ��VAO��
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