#ifdef MAC
#pragma once
#ifndef DKMac_H
#define DKMac_H
#define DESKTOP

//#ifdef USE_Boost_System
//#include <boost/asio/io_service.hpp>
//#endif

////////////
class DKMac
{
public:
	static bool GetMousePos(int& x, int& y){
		//NSPoint mouseLoc;
		//mouseLoc = [NSEvent mouseLocation]; //get current mouse position
		//NSLog(@"Mouse location: %f %f", mouseLoc.x, mouseLoc.y);
		x = 0;//mouseLoc.x;
		y = 0;//mouseLoc.y;
		return true;
	}
};

#endif //DKMac_H
#endif //MAC