#ifndef __SPLASHSCREEN__
#define __SPLASHSCREEN__

#include"State.h"

class SplashScreen:public State {
	Texture texture;
public:
	SplashScreen();
	void CreateSplachScreen();
	void Draw();

	void Text1();
};


#endif // !__SPLASHSCREEN__

