//extern "C" _declspec(dllexport) bool SetTexture(void* pTexture)
//{
//	texture_ptr = pTexture;
//	loadImage(NULL);
//	return true;
//}
//
//void setImageInfo(int width,int height,void* pTexture,char* fileName)
//{
//	texture_w = width;
//	texture_h = height;
//	texture_ptr = pTexture;
//
//	dxlib::Debug::GetInst()->isLog = true; //设置是否输出日志
//	dxlib::Debug::GetInst()->CreatLogFile("another.txt");
//
//	imageDC = CreateCompatibleDC(NULL);     // create an offscreen DC
//											//loadImage("aa.bmp");
//	loadImage(fileName);
//}
//
//// Function to load the image into our DC so we can draw it to the screen
//void loadImage(const char* pathname)
//{
//	imageBmp = CreateBitmap(texture_w, texture_h, 1, 32, texture_ptr);
//
//	//imageBmp = (HBITMAP)LoadImage(         // load the bitmap from a file
//	//	NULL,                           // not loading from a module, so this is NULL
//	//	(LPCWSTR)pathname,                       // the path we're loading from
//	//	IMAGE_BITMAP,                   // we are loading a bitmap
//	//	0, 0,                            // don't need to specify width/height
//	//	LR_DEFAULTSIZE | LR_LOADFROMFILE// use the default bitmap size (whatever the file is), and load it from a file
//	//);
//	if (imageBmp)
//		FileDebug::Log("File exist");
//	else
//		FileDebug::Log("File nonexist");
//	imageBmpOld = (HBITMAP)SelectObject(imageDC, imageBmp);  // put the loaded image into our DC
//}
//
//// Function to clean up
//void cleanUpImage()
//{
//	SelectObject(imageDC, imageBmpOld);      // put the old bmp back in our DC
//	DeleteObject(imageBmp);                 // delete the bmp we loaded
//	DeleteDC(imageDC);                      // delete the DC we created
//}
//// The function to draw our image to the display (the given DC is the screen DC)
//void drawImage(HDC screen)
//{
//	BitBlt(
//		screen,         // tell it we want to draw to the screen
//		0, 0,            // as position 0,0 (upper-left corner)
//		texture_w,   // width of the rect to draw
//		texture_h,   // height of the rect
//		imageDC,        // the DC to get the rect from (our image DC)
//		0, 0,            // take it from position 0,0 in the image DC
//		SRCCOPY         // tell it to do a pixel-by-pixel copy
//	);
//	DWORD key = GetLastError();
//	dxlib::Debug::Log("BitBlt Error = %d \r\n", key);
//}