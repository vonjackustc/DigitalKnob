#include "DK/stdafx.h"
#include "DK/DKFile.h"
#include "DKScreenRecorder.h"

#ifdef WIN32
#define sleep Sleep
#include <WS2tcpip.h>
#endif

#ifdef LINUX
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
Display* DKScreenRecorder::disp;
Window DKScreenRecorder::root;
XImage* DKScreenRecorder::image;
#endif

#ifdef MAC
CGImageRef DKScreenRecorder::image_ref;
CGDataProviderRef DKScreenRecorder::provider;
CFDataRef DKScreenRecorder::dataref;
#endif

#ifdef __IRIX__
#include <netdb.h>
#endif

static int bpp = 4;
DKString DKScreenRecorder::capture;
char* DKScreenRecorder::frameBuffer;
int DKScreenRecorder::desktopWidth;
int DKScreenRecorder::desktopHeight;


/////////////////////////////
bool DKScreenRecorder::Init()
{
	DKFile::GetSetting(DKFile::local_assets + "settings.txt", "[CAPTURE]", capture);
	if(capture.empty()){ capture = "GDI"; } //or DIRECTX

	DKUtil::GetScreenWidth(desktopWidth);
	DKUtil::GetScreenHeight(desktopHeight);

/*
#ifdef MAC
	image_ref = CGDisplayCreateImage(CGMainDisplayID());
	provider = CGImageGetDataProvider(image_ref);
	dataref = CGDataProviderCopyData(provider);
#endif
*/
#ifdef LINUX
	disp = XOpenDisplay(NULL);
	root = XDefaultRootWindow(disp);
#endif
 
	frameBuffer = (char*)malloc(desktopHeight * desktopWidth * bpp);
 
	DKApp::AppendLoopFunc(&DKScreenRecorder::Loop, this);
	return true;
}

////////////////////////////
bool DKScreenRecorder::End()
{
	return true;
}


/////////////////////////////
void DKScreenRecorder::Loop()
{
	//DrawBuffer();
	//TODO: the framebuffer holds a current image of the desktop
	//TODO: we need to use OpenCV or FFMPEG to record this to a video file.
}

///////////////////////////////////
void DKScreenRecorder::DrawBuffer()
{  
#ifdef WIN32
	//Capture Desktop with DirectX
	if(capture == "DIRECTX"){
		//DKLog("DIRECTX\n",DKINFO);
		//https://stackoverflow.com/questions/30021274/capture-screen-using-directx

		HRESULT hr = S_OK;
		IDirect3D9 *d3d = nullptr;
		IDirect3DDevice9 *device = nullptr;
		IDirect3DSurface9 *surface = nullptr;
		D3DPRESENT_PARAMETERS parameters = { 0 };
		D3DDISPLAYMODE mode;
		D3DLOCKED_RECT rc;
		UINT pitch;
		UINT adapter = D3DADAPTER_DEFAULT;

		// init D3D and get screen size
		d3d = Direct3DCreate9(D3D_SDK_VERSION);
		if(!d3d){
			DKLog("DKScreenRecorder::DrawBuffer(): Direct3DCreate9() failed\n", DKERROR);
			return;
		}
		HRCHECK(d3d->GetAdapterDisplayMode(adapter, &mode));

		parameters.Windowed = TRUE;
		parameters.BackBufferCount = 1;
		parameters.BackBufferHeight = mode.Height;
		parameters.BackBufferWidth = mode.Width;
		parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		parameters.hDeviceWindow = NULL;

		// create device & capture surface
		//HRCHECK(d3d->CreateDevice(adapter, D3DDEVTYPE_HAL, NULL, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &parameters, &device));
		HRCHECK(d3d->CreateDevice(adapter, D3DDEVTYPE_REF, NULL, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &parameters, &device));
		HRCHECK(device->CreateOffscreenPlainSurface(mode.Width, mode.Height, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &surface, nullptr));

		// compute the required buffer size
		HRCHECK(surface->LockRect(&rc, NULL, 0));
		pitch = rc.Pitch;
		HRCHECK(surface->UnlockRect());
		HRCHECK(device->GetFrontBufferData(0, surface));

		// copy it into our buffers
		HRCHECK(surface->LockRect(&rc, NULL, 0));
		CopyMemory(frameBuffer, rc.pBits, mode.Height * mode.Width * bpp);
		HRCHECK(surface->UnlockRect());


		RELEASE(surface);
		RELEASE(device);
		RELEASE(d3d);
	}
	//Capture Desktop with GDI
	if(capture == "GDI"){
		//https://pastebin.com/r3CZpWDs
		HWND desktop = GetDesktopWindow();
		BITMAPINFO info = {0};
		info.bmiHeader.biSize = sizeof(info.bmiHeader);
		info.bmiHeader.biWidth = desktopWidth;
		info.bmiHeader.biHeight = desktopHeight* -1;
		info.bmiHeader.biPlanes = 1;
		info.bmiHeader.biBitCount = 32;
		info.bmiHeader.biCompression = BI_RGB;
		BYTE* pbBitmap;

		HDC src_dc = GetDC(desktop);
		HDC buffer_dc = CreateCompatibleDC(src_dc);
		HBITMAP dest_dc = CreateDIBSection(buffer_dc, &info, DIB_RGB_COLORS, (void**)&pbBitmap, NULL, 0);
		BITMAP bmp;
		SelectObject(buffer_dc, dest_dc);
		GetObject(dest_dc, sizeof(BITMAP), &bmp);
		int res = BitBlt(buffer_dc, 0, 0, desktopWidth, desktopHeight, src_dc, 0, 0, SRCCOPY);
		int go = GetObject(dest_dc, sizeof(BITMAP), &bmp);
		//Invert
		size_t n = desktopWidth * desktopHeight * bpp;
		int* buffer = (int*)malloc(n);
		int* dest = (int*)frameBuffer;
		int* src = ((int*)bmp.bmBits);
		while(src != ((int*)bmp.bmBits) + (desktopWidth * desktopHeight - 1)){
			char* c_dest = (char*)dest;
			char* c_src = (char*)src;
			c_dest[0] = c_src[0];
			c_dest[1] = c_src[1];
			c_dest[2] = c_src[2];
			c_dest[3] = 0;
			src++;
			dest++;
		}

		ReleaseDC(desktop, src_dc);
		DeleteDC(src_dc);
		DeleteDC(buffer_dc);
		DeleteObject(dest_dc);
		delete buffer;
	}
#endif

#ifdef MAC
	image_ref = CGDisplayCreateImage(CGMainDisplayID());
	provider = CGImageGetDataProvider(image_ref);
	dataref = CGDataProviderCopyData(provider);
	size_t width, height;
    width = CGImageGetWidth(image_ref);
	height = CGImageGetHeight(image_ref); 
	size_t bpp = CGImageGetBitsPerPixel(image_ref) / 8;
	memcpy(frameBuffer, CFDataGetBytePtr(dataref), width * height * bpp);
	//CFRelease(dataref); 
	//CGImageRelease(image_ref); 
#endif

#ifdef LINUX
	image = XGetImage(disp, root, 0, 0, desktopWidth, desktopHeight, AllPlanes, ZPixmap);
	int w,h;
	for(h=0; h<desktopHeight; ++h) {
		for(w=0; w<desktopWidth; ++w) {
			unsigned long xpixel = XGetPixel(image, w, h);
			unsigned int red   = (xpixel & 0x00ff0000) >> 16;
			unsigned int green = (xpixel & 0x0000ff00) >> 8;
			unsigned int blue  = (xpixel & 0x000000ff);

			frameBuffer[(h*desktopWidth+w)*bpp+0]=blue;
			frameBuffer[(h*desktopWidth+w)*bpp+1]=green;
			frameBuffer[(h*desktopWidth+w)*bpp+2]=red;
		}
		frameBuffer[h*desktopWidthh*bpp+0]=0xff;
		frameBuffer[h*desktopWidth*bpp+1]=0xff;
		frameBuffer[h*desktopWidth*bpp+2]=0xff;
		frameBuffer[h*desktopWidth*bpp+3]=0xff;
	}

	//XDestroyImage(image);
	//image = NULL;
#endif

}