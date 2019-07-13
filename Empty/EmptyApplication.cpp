#include "BaseApplication.h"

namespace Acmen
{
    BaseApplication g_App;
    IApplication* g_App = &g_App;
}