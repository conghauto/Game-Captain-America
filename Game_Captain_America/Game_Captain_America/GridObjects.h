#pragma once
#include "GameObject.h"
#include "define.h"
#include <math.h>

class GridObjects
{
	float left, right;

public:
	vector<LPGAMEOBJECT> listObject;

	GridObjects(float left, float right)
	{
		this->left = left;
		this->right = right;
	}

	void GetPoint(float &left, float &right);
	vector<LPGAMEOBJECT> GetListObject();

	void AddObject(LPGAMEOBJECT object);
	void RemoveObject(int i);
};

class ListGrids
{
	static ListGrids *instance;
	vector<GridObjects*> listGrids;

public:
	void InitList(float mapWidth);
	void ReleaseList();

	void RemoveGrid(int i);

	void AddObject(LPGAMEOBJECT object);
	void RemoveObject(LPGAMEOBJECT object);

	static ListGrids *GetInstance();
	vector<GridObjects*> GetCurrentGrids(float cam_x);
	void UpdateObjectInGrid(LPGAMEOBJECT object);
};