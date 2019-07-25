#pragma once

#include <d3dx9.h>
#include <d3d9.h>

class Camera
{
public:

	Camera(int x, int y, int width, int height);
	int getX();
	int getY();
	void setX(float x);
	void setY(float y);
	int getW();
	int getH();
	//center of camera
	~Camera();

private:
	float x_cam, y_cam;
	int             mWidth,
		mHeight;

	D3DXVECTOR3     mPosition;
};

