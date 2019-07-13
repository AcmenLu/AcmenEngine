#include "BaseApplication.h"

int Acmen::BaseApplication::Initialize()
{
	m_bQuit = false;
	return 0;
}

void Acmen::BaseApplication::Finalize()
{
	
}

void Acmen::BaseApplication::Tick()
{

}

bool Acmen::BaseApplication::IsQuit()
{
	return m_bQuit;
}