#include"Helper.h"
#include<atlimage.h>

GLuint readTextureFromFile(char* file)
{
	GLuint glTex;
	CImage img;
	CString path(file);
	HRESULT hr = img.Load(path);// (LPCTSTR)file);

	if (FAILED(hr))
	{
		printf("加载%s图片文件失败。",file);
		return -1;
	}

	//图片加载完之后，
	BITMAP bm;
	HBITMAP hbmp = img;
	if (!GetObject(hbmp, sizeof(bm), &bm))
		return -1;

	glGenTextures(1, &glTex);
	if (glTex)
	{
		glBindTexture(GL_TEXTURE_2D, glTex);
		//设置图片采样的样式。
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glPixelStoref(GL_PACK_ALIGNMENT, 1);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, bm.bmWidth, bm.bmHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bm.bmBits); //这里不是GL_RGB  
	}
	return glTex;
}