
#ifdef _WINDOWS

    #if defined(DEBUG) || defined(_DEBUG)
    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>
    #else
    #endif

    #include <memory>
    #include <string>
    #include <Windows.h>

    #include "supervisor.h"


    int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
    {
    #if defined(DEBUG) | defined(_DEBUG)
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif

        //initialize all systems
        auto _supervisor = std::make_unique<Supervisor>(instance);
        _supervisor->PassControl();
        return 0;
    }


#endif // _WINDOWS