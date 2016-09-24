#include "MainObject.h"
#include "ClassManagerA.h"

ClassManagerA MainObject::MainManager;  

MainObject::MainObject()
{
	MainManager.ManagerName = "UserSettings";
	//MainManager.ElementName = "Global";
	MainManager.ManagerB.ManagerName = "Users";
	MainManager.ManagerB.ElementName = "User";
	MainManager.ManagerC.ManagerName = "Groups";
	MainManager.ManagerC.ElementName = "Group";
	MainManager.ManagerD.ManagerName = "Keypads";
	MainManager.ManagerD.ElementName = "Keypad";
	
	registeredManagers.Add(&MainManager);
}

MainObject::~MainObject()
{
	

}

void MainObject::Clear()
{
	MainManager.Clear();
}

bool MainObject::Serialize(unsigned short version)
{
	MainManager.Serialize(version);
	return true;
}

unsigned short MainObject::GetVersion()
{
	return 1; //MainObject version
}