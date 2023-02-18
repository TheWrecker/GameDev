
#ifdef _WINDOWS

    #if defined(DEBUG) || defined(_DEBUG)
        #define _CRTDBG_MAP_ALLOC
        #include <stdlib.h>
        #include <crtdbg.h>
    #else
    #endif

    #include <d3d11.h>
    #include <memory>
    #include <string>
    #include <Windows.h>

    #include "core/supervisor.h"


    int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
    {
    #if defined(DEBUG) | defined(_DEBUG)
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        //ID3D11Debug* _debug = nullptr;
    #endif
        
        //initialize all systems
        {
            auto _supervisor = std::make_unique<Supervisor>(instance);
            //_debug = _supervisor->GetDebugQuery();
            _supervisor->PassControl();
        }

    #if defined(DEBUG) | defined(_DEBUG)
        //_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
        //_debug->Release();
    #endif

        return 0;
    }


#endif // _WINDOWS