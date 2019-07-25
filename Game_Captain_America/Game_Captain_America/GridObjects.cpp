#include "GridObjects.h"

void GridObjects::GetPoint(float &left, float &right)
{
	left = this->left;
	right = this->right;
}

vector<LPGAMEOBJECT> GridObjects::GetListObject()
{
	return listObject;
}

void GridObjects::AddObject(LPGAMEOBJECT object)
{
	listObject.push_back(object);
}

void GridObjects::RemoveObject(int i)
{
	listObject.erase(listObject.begin() + i);
}

ListGrids *ListGrids::instance = NULL;

#define GRID_WIDTH	(SCREEN_WIDTH / 2)

void ListGrids::InitList(float mapWidth)
{
	GridObjects *grid;
	int gridCount = ceil(mapWidth / GRID_WIDTH);
	for (int i = 0; i < gridCount; i++)
	{
		float left, right;
		left = i * GRID_WIDTH;
		right = (i + 1) * GRID_WIDTH;
		grid = new GridObjects(left, right);
		listGrids.push_back(grid);
	}
}

void ListGrids::ReleaseList()
{
	vector<LPGAMEOBJECT> listRemoveObjects;

	int listGridSize = listGrids.size();
	for (int i = 0; i < listGridSize; i++)
	{
		int listObjectSize = listGrids[i]->listObject.size();
		GridObjects *grid = listGrids[i];
		for (int j = 0; j < listObjectSize; j++)
		{
			listRemoveObjects.push_back(grid->listObject[j]);
		}
	}

	for (int i = 0; i < listRemoveObjects.size(); i++)
	{
		RemoveObject(listRemoveObjects[i]);
		delete listRemoveObjects[i];
	}

	for (int i = 0; i < listGrids.size(); )
	{
		GridObjects *grid = listGrids[i];
		RemoveGrid(0);
		delete grid;
	}
}

void ListGrids::RemoveGrid(int i)
{
	listGrids.erase(listGrids.begin() + i);
}

void ListGrids::AddObject(LPGAMEOBJECT object)
{
	// Dựa vào vị trí x của object để đặt vật vào grid tương ứng
	float objectX = object->x;
	// Tránh trường hợp vật ra khỏi map
	if (objectX < 0)
		objectX = 0;

	int gridNumber = floor(objectX / GRID_WIDTH);
	// Tránh trường hợp vật ra khỏi map
	if (gridNumber == this->listGrids.size())
		gridNumber = this->listGrids.size() - 1;

	object->gridNumber = gridNumber;
	listGrids[gridNumber]->AddObject(object);
}

void ListGrids::RemoveObject(LPGAMEOBJECT object)
{
	GridObjects *gridObjects = listGrids[object->gridNumber];
	int listObjectSize = gridObjects->listObject.size();
	int objectNumber = -1;

	// Duyệt qua từng phần tử trong gridObjects để lấy chỉ số của object
	for (int i = 0; i < listObjectSize; i++)
	{
		if (gridObjects->listObject[i] == object)
		{
			objectNumber = i;
			break;
		}
	}

	// Nếu tồn tại object trong gridObjects thì chỉ số sẽ khác -1
	if (objectNumber != -1)
	{
		gridObjects->RemoveObject(objectNumber);
	}
}

ListGrids * ListGrids::GetInstance()
{
	if (instance == NULL) instance = new ListGrids();
	return instance;
}

vector<GridObjects*> ListGrids::GetCurrentGrids(float cam_x)
{
	vector<GridObjects*> result;
	int gridNumber = floor(cam_x / GRID_WIDTH);

	// Trường hợp màn hình game nằm giữa 2 grid
	result.push_back(listGrids[gridNumber]);
	if (gridNumber > 0)
		result.push_back(listGrids[gridNumber - 1]);
	if (gridNumber + 1 < listGrids.size())
		result.push_back(listGrids[gridNumber + 1]);

	return result;
}

void ListGrids::UpdateObjectInGrid(LPGAMEOBJECT object)
{
	// Dựa vào vị trí x của object để đặt vật vào grid tương ứng
	int gridNumber = floor(object->x / GRID_WIDTH);

	if (gridNumber < 0)
		gridNumber = 0;
	else if (gridNumber >= listGrids.size())
		gridNumber = listGrids.size() - 1;

	if (gridNumber != object->gridNumber)
	{
		// Xóa object khỏi grid cũ
		RemoveObject(object);

		// Đổi gridNumber hiện tại của object và thêm object vào grid mới
		object->gridNumber = gridNumber;
		listGrids[gridNumber]->AddObject(object);
	}
}
