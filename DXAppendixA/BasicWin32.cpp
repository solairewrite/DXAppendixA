#include <windows.h> // Win32 API结构体,数据类型,函数声明

HWND ghMainWnd = 0; // 主窗口的句柄

// 初始化Windows应用程序
bool InitWindowsApp(HINSTANCE instanceHandle, int show);

// 消息循环
int Run();

// 窗口过程会处理窗口所接受到的消息
LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 在Windows应用程序中WinMain相当于大多数语言的main()
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	// 创建并初始化应用程序主窗口
	if (!InitWindowsApp(hInstance, nCmdShow))
	{
		return 0;
	}
	// 开消息循环.直到接收到消息WM_QUIT(应用程序关闭)
	return Run();
}

bool InitWindowsApp(HINSTANCE instanceHandle, int show)
{
	// 填写WNDCLASS结构体,根据其中描述的特征来创建一个窗口
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instanceHandle;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"BasicWndClass";

	// 在Windows系统中,为上述WNDCLASS注册一个实例
	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass FAILED", 0, 0);
		return false;
	}

	// 调用CreateWindow()创建窗口,返回创建窗口的句柄(HWND类型),创建失败返回0.
	// 窗口句柄是一种窗口的引用方式
	ghMainWnd = CreateWindow(
		L"BasicWndClass",	// 采用前面注册的WNDCLASS实例
		L"Win32Basic",		// 窗口标题
		WS_OVERLAPPEDWINDOW,// 窗口的样式标志
		CW_USEDEFAULT,		// x坐标
		CW_USEDEFAULT,		// y坐标
		CW_USEDEFAULT,		// 窗口宽度
		CW_USEDEFAULT,		// 窗口高度
		0,					// 父窗口
		0,					// 菜单句柄
		instanceHandle,		// 应用程序实例句柄
		0					// 其他参数
	);

	if (ghMainWnd == 0)
	{
		MessageBox(0, L"CreateWindow FAILED", 0, 0);
		return false;
	}

	// 显示并更新窗口
	ShowWindow(ghMainWnd, show);
	UpdateWindow(ghMainWnd);

	return true;
}

int Run()
{
	MSG msg = { 0 };

	// 在获取WM_QUIT消息之前,该函数会一直保持循环.
	// GetMessage函数只有在收到WM_QUIT消息时才会返回0,若发生错误,返回-1
	// 没有信息到来之时,GetMessage会令此应用程序线程进入休眠状态
	BOOL bRet = 1; // boolReturn
	while ((bRet = GetMessage(&msg, 0, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			MessageBox(0, L"GetMessage FAILED", L"Error", MB_OK);
			break;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// 处理一些特定的消息.处理完一个消息后,应返回0
	switch (msg)
	{
		// 按下鼠标左键,弹出消息框
	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hello, World", L"Hello", MB_OK);
		return 0;
		// 按下Esc后,销毁应用程序主窗口
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(ghMainWnd);
		return 0;

		// 处理销毁消息的方法是发送退出消息,这样会终止消息循环
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	// 将上面没有处理的消息转发给默认的窗口过程
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
