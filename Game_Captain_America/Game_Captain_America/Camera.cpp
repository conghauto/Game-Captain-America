#include "Camera.h"


Camera::Camera(int x, int y, int width, int height)
{
	x_cam = x;
	y_cam = y;
	mWidth = width;
	mHeight = height;
}


Camera::~Camera()
{

}

int Camera::getX() {
	return x_cam;
}
int Camera::getY() {
	return y_cam;
}
void Camera::setX(float x) {
	x_cam = x;
}
void Camera::setY(float y) {
	y_cam = y;
}
int Camera::getW() {
	return mWidth;
}
int Camera::getH() {
	return mHeight;
}

