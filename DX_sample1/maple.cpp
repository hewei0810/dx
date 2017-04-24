//*****************************************************************************************
//
//【Visual C++】游戏开发笔记系列配套源码 三十三 浅墨DirectX提高班之二 化腐朽为神奇：Direct3D初始化四部曲
//		 VS2010版
// 2012年 11月18日  Create by 浅墨 
//图标素材： 古剑奇谭 百里屠苏
//源码配套博文链接：  http://blog.csdn.net/zhmxy555/article/details/8197792
//更多内容请访问我的博客： http://blog.csdn.net/zhmxy555 
//此刻心情：多年以后，当你回忆往昔，唯一让你觉得真实，和骄傲的，是你现在昂首挺胸、用力走过的人生。
//
//***************************************************************************************** 
#include "stdafx.h"



//*****************************************************************************************
// Desc: 头文件定义部分  
//*****************************************************************************************                                                                                       
#include <d3d9.h>



//*****************************************************************************************
// Desc: 库文件定义部分  
//***************************************************************************************** 
#pragma comment(lib,"d3d9.lib")
//#pragma comment(lib,"d3dx9.lib")



//*****************************************************************************************
// Desc: 宏定义部分   
//*****************************************************************************************
#define SCREEN_WIDTH	800							//为窗口宽度定义的宏，以方便在此处修改窗口宽度
#define SCREEN_LEIGHT	600							//为窗口高度定义的宏，以方便在此处修改窗口高度
#define WINDOW_TITLE	L"【Visual C++游戏开发笔记】博文配套demo之三十三 浅墨DirectX提高班之二 化腐朽为神奇：Direct3D初始化四部曲"		//为窗口标题定义的宏
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }      //自定义一个SAFE_RELEASE()宏,便于资源的释放



//*****************************************************************************************
// Desc: 全局变量声明部分  
//
//*****************************************************************************************
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; //Direct3D设备对象



//*****************************************************************************************
// Desc: 全局函数声明部分 
//
//***************************************************************************************** 
LRESULT CALLBACK	WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
HRESULT				Direct3D_Init(HWND hwnd);
void				Direct3D_Render( );
void				Direct3D_CleanUp( );



//*****************************************************************************************
// Name: WinMain( )
// Desc: Windows应用程序入口函数
//*****************************************************************************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nShowCmd)
{

	//开始设计一个完整的窗口类
	WNDCLASSEX wndClass = { 0 };				//用WINDCLASSEX定义了一个窗口类，即用wndClass实例化了WINDCLASSEX，用于之后窗口的各项初始化    
	wndClass.cbSize = sizeof( WNDCLASSEX ) ;	//设置结构体的字节数大小
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//设置窗口的样式
	wndClass.lpfnWndProc = WndProc;				//设置指向窗口过程函数的指针
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance = hInstance;				//指定包含窗口过程的程序的实例句柄。
	wndClass.hIcon=(HICON)::LoadImage(NULL,L"icon.ico",IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE); //从全局的::LoadImage函数从本地加载自定义ico图标
	wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );    //指定窗口类的光标句柄。
	wndClass.hbrBackground=(HBRUSH)GetStockObject(GRAY_BRUSH);  //为hbrBackground成员指定一个灰色画刷句柄
	wndClass.lpszMenuName = NULL;						//用一个以空终止的字符串，指定菜单资源的名字。
	wndClass.lpszClassName = L"ForTheDreamOfGameDevelop";		//用一个以空终止的字符串，指定窗口类的名字。

	if( !RegisterClassEx( &wndClass ) )				//设计完窗口后，需要对窗口类进行注册，这样才能创建该类型的窗口
		return -1;		

	HWND hwnd = CreateWindow( L"ForTheDreamOfGameDevelop",WINDOW_TITLE,			//喜闻乐见的创建窗口函数CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH,
		SCREEN_LEIGHT, NULL, NULL, hInstance, NULL );


	MoveWindow(hwnd,200,50,SCREEN_WIDTH,SCREEN_LEIGHT,true);   //调整窗口显示时的位置，窗口左上角位于屏幕坐标（200，50）处
	ShowWindow( hwnd, nShowCmd );    //调用Win32函数ShowWindow来显示窗口
	UpdateWindow(hwnd);  //对窗口进行更新，就像我们买了新房子要装修一样

	//Direct3D资源的初始化，成功或者失败都用messagebox予以显示
	if (S_OK==Direct3D_Init (hwnd))
	{
		MessageBox(hwnd, L"Direct3D初始化完成~！", L"浅墨的消息窗口", 0); //使用MessageBox函数，创建一个消息窗口  
	}
	else
	{
		MessageBox(hwnd, L"Direct3D初始化失败~！", L"浅墨的消息窗口", 0); //使用MessageBox函数，创建一个消息窗口 
	}

	

	//消息循环过程
	MSG msg = { 0 };  //初始化msg
	while( msg.message != WM_QUIT )			//使用while循环
	{
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //查看应用程序消息队列，有消息时将队列中的消息派发出去。
		{
			TranslateMessage( &msg );		//将虚拟键消息转换为字符消息
			DispatchMessage( &msg );		//该函数分发一个消息给窗口程序。
		}
		else
		{
			Direct3D_Render();
		}
	}

	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);
	return 0;  
}



//*****************************************************************************************
// Name: WndProc()
// Desc: 对窗口消息进行处理
//*****************************************************************************************
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )   //窗口过程函数WndProc
{
	switch( message )				//switch语句开始
	{
	case WM_PAINT:					 // 客户区重绘消息
		Direct3D_Render();          //调用Direct3D_Render函数，进行画面的绘制
		ValidateRect(hwnd, NULL);   // 更新客户区的显示
		break;									//跳出该switch语句

	case WM_KEYDOWN:                // 键盘按下消息
		if (wParam == VK_ESCAPE)    // ESC键
			DestroyWindow(hwnd);    // 销毁窗口, 并发送一条WM_DESTROY消息
		break;
	case WM_DESTROY:				//窗口销毁消息
		Direct3D_CleanUp();     //调用Direct3D_CleanUp函数，清理COM接口对象
		PostQuitMessage( 0 );		//向系统表明有个线程有终止请求。用来响应WM_DESTROY消息
		break;						//跳出该switch语句

	default:						//若上述case条件都不符合，则执行该default语句
		return DefWindowProc( hwnd, message, wParam, lParam );		//调用缺省的窗口过程来为应用程序没有处理的窗口消息提供缺省的处理。
	}

	return 0;					//正常退出
}


//*****************************************************************************************
// Name: Direct3D_Init( )
// Desc: 初始化Direct3D
// Point:【Direct3D初始化三部曲】
//		1.初始化三部曲之一，创建Direct3D接口对象
//		2.初始化三部曲之二，获取硬件设备信息
//		3.初始化三部曲之三，填充结构体
//		4.初始化三部曲之四，创建Direct3D设备接口
//*****************************************************************************************

HRESULT Direct3D_Init(HWND hwnd)
{

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之一，创接口】：创建Direct3D接口对象, 以便用该Direct3D对象创建Direct3D设备对象
	//--------------------------------------------------------------------------------------
	LPDIRECT3D9  pD3D = NULL; //Direct3D接口对象的创建
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) ) //初始化Direct3D接口对象，并进行DirectX版本协商
 			return E_FAIL;

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之二,取信息】：获取硬件设备信息
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if( FAILED( pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps ) ) )
		{
			return E_FAIL;
		}
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //支持硬件顶点运算，我们就采用硬件顶点运算，妥妥的
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //不支持硬件顶点运算，无奈只好采用软件顶点运算

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之三，填内容】：填充D3DPRESENT_PARAMETERS结构体
	//--------------------------------------------------------------------------------------
	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth            = SCREEN_WIDTH;
	d3dpp.BackBufferHeight           = SCREEN_LEIGHT;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = true;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之四，创设备】：创建Direct3D设备接口
	//--------------------------------------------------------------------------------------
	if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		hwnd, vp, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;



	SAFE_RELEASE(pD3D) //LPDIRECT3D9接口对象的使命完成，我们将其释放掉

	return S_OK;
}



//*****************************************************************************************
// Name: Direct3D_Render()
// Desc: 使用Direct3D进行渲染
//*****************************************************************************************
void Direct3D_Render()
{
	 
}


//*****************************************************************************************
// Name: Direct3D_CleanUp()
// Desc: 对Direct3D的资源进行清理
//*****************************************************************************************
void Direct3D_CleanUp()
{
	//释放Direct3D设备对象
	SAFE_RELEASE(g_pd3dDevice)	
}

