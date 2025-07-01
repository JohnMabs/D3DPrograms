#include <windows.h>  
#include <d3d11.h>  
#include <d3dcompiler.h>  

#pragma comment(lib, "d3d11.lib")  
#pragma comment(lib, "d3dcompiler.lib")  

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);  

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {  
    // Register the window class  
    const char CLASS_NAME[] = "Direct3D Window Class";  

    WNDCLASS wc = {};  
    wc.lpfnWndProc = WindowProc;  
    wc.hInstance = hInstance;  
    wc.lpszClassName = CLASS_NAME;  

    RegisterClass(&wc);  

    // Create the window  
    HWND hwnd = CreateWindowEx(  
        0,                              // Optional window styles  
        CLASS_NAME,                     // Window class  
        "Direct3D Window",              // Window title  
        WS_OVERLAPPEDWINDOW,            // Window style  

        // Size and position  
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,  

        NULL,       // Parent window  
        NULL,       // Menu  
        hInstance,  // Instance handle  
        NULL        // Additional application data  
    );  

    if (hwnd == NULL) {  
        return 0;  
    }  

    ShowWindow(hwnd, nCmdShow);  

    // Initialize Direct3D 11  
    ID3D11Device* device = nullptr;  
    ID3D11DeviceContext* deviceContext = nullptr;  
    IDXGISwapChain* swapChain = nullptr;  
    ID3D11RenderTargetView* renderTargetView = nullptr;  

    DXGI_SWAP_CHAIN_DESC scd;  
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));  
    scd.BufferCount = 1;  
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  
    scd.OutputWindow = hwnd;  
    scd.SampleDesc.Count = 1;  
    scd.Windowed = TRUE;  

    // Define the dimensions of the swap chain buffers  
    scd.BufferDesc.Width = 800;  
    scd.BufferDesc.Height = 600;  

    // Create the device and swap chain  
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };  
    D3D_FEATURE_LEVEL featureLevel;  

    HRESULT hr = D3D11CreateDeviceAndSwapChain(  
        NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 1,  
        D3D11_SDK_VERSION, &scd, &swapChain, &device, &featureLevel,  
        &deviceContext);  

    if (FAILED(hr)) {  
        MessageBox(hwnd, "Failed to create Direct3D device and swap chain", "Error", MB_OK);  
        return 0;  
    }  

    // Create a render target view  
    ID3D11Texture2D* backBuffer = nullptr;
    hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);  
    if (FAILED(hr)) {  
        MessageBox(hwnd, "Failed to get swap chain back buffer", "Error", MB_OK);  
        return 0;  
    }  

    hr = device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);  
    backBuffer->Release();  
    if (FAILED(hr)) {  
        MessageBox(hwnd, "Failed to create render target view", "Error", MB_OK);  
        return 0;  
    }

    deviceContext->OMSetRenderTargets(1, &renderTargetView, NULL);  

    // Main message loop  
    MSG msg = { };  
    while (true) {  
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {  
            if (msg.message == WM_QUIT)  
                break;  
            TranslateMessage(&msg);  
            DispatchMessage(&msg);  
        }  

        // Clear the back buffer  
        float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f }; // RGBA  
        deviceContext->ClearRenderTargetView(renderTargetView, clearColor);  

        // Présente le rendu à l'écran  
        swapChain->Present(1, 0);  
    }  

    // Clean up Direct3D 11  
    if (renderTargetView) renderTargetView->Release();  
    if (swapChain) swapChain->Release();  
    if (deviceContext) deviceContext->Release();  
    if (device) device->Release();  

    return 0;  
}  

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {  
    switch (message) {  
    case WM_DESTROY:  
        PostQuitMessage(0);  
        return 0;  
    }  
    return DefWindowProc(hWnd, message, wParam, lParam);  
}
