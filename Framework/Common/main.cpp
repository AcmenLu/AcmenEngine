#include <stdio.h>
#include "IApplication.h"

using namespace Acmen;

namespace Acmen
{
    extern IApplication* g_App;
}

int main(int argc, char** argv)
{
    int ret;
    if(ret = g_App->Initialize() != 0)
    {
        printf("App Initialize failed, willExit now.");
        return ret;
    }

    while (!g_App->IsQuit())
    {
        g_App->Tick();
    }
    g_App->Finalize();
    return 0;
}