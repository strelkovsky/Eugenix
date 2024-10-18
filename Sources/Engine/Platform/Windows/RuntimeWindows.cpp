//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#include "Platform/Platform.h"

#if defined (EUGENIX_PLATFORM_WINDOWS) && defined (EUGENIX_NATIVE_RUNTIME)

#ifdef EUGENIX_OPENGL
#	include <windows.h> // Header File For Windows
#	include "Platform/OpenGL/EugenixGL.h"
#	include "../Deps/gl/wglext.h"
#endif // EUGENIX_OPENGL

#include "Core/Base.h"
#include "Core/Events/Events.h"
#include "Platform/Windows/PlatformWindows.h"
#include "Platform/System.h"
#include "Platform/Runtime.h"

namespace Eugenix
{
	namespace Runtime
	{
		DisplaySetup _setup;

		HWND _windowHandle = nullptr;
		WNDCLASSEX _winClass = { 0 };
		DEVMODE _dmScreenSettings = { 0 };

#ifdef EUGENIX_OPENGL
		PIXELFORMATDESCRIPTOR _pixelFormatDesc = { 0 };
		HDC	_deviceContext = nullptr;
		HGLRC _renderContext = nullptr;
#endif

#ifdef EUGENIX_UNICODE
		static const LPCWSTR _winClassName = L"Eugenix";
#else 
		static const LPCSTR _winClassName = "Eugenix";
#endif // EUGENIX_UNICODE

		Events::KeyCode TranslateKey(uint32_t key, bool shiftOrCaps)
		{
			return Events::KeyCode::A;
		}

		LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			bool shiftDown = false;
			bool capsOn = false;

			switch (msg)
			{
			case WM_LBUTTONDOWN:
				Events::MouseEvent(Events::EventType::MOUSE_PRESS_LEFT_BUTTON, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
				break;

			case WM_LBUTTONUP:
				Events::MouseEvent(Events::EventType::MOUSE_RELEASE_LEFT_BUTTON, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
				break;

			case WM_RBUTTONDOWN:
				Events::MouseEvent(Events::EventType::MOUSE_PRESS_RIGHT_BUTTON, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
				break;

			case WM_RBUTTONUP:
				Events::MouseEvent(Events::EventType::MOUSE_RELEASE_RIGHT_BUTTON, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
				break;

			case WM_MBUTTONDOWN:
				Events::MouseEvent(Events::EventType::MOUSE_PRESS_MIDDLE_BUTTON, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
				break;

			case WM_MBUTTONUP:
				Events::MouseEvent(Events::EventType::MOUSE_RELEASE_MIDDLE_BUTTON, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
				break;

			case WM_MOUSEMOVE:
				Events::MouseEvent(Events::EventType::MOUSE_MOVE, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
				break;

			case WM_MOUSEWHEEL:
				break;

			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:

				if (wparam == VK_SHIFT || wparam == VK_LSHIFT || wparam == VK_RSHIFT)
					shiftDown = true;

				if (wparam == VK_CAPITAL)
					capsOn = true;

				Events::KeyDown(TranslateKey((uint32_t)wparam, shiftDown ^ capsOn));
				break;

			case WM_KEYUP:

				if (wparam == VK_SHIFT || wparam == VK_LSHIFT || wparam == VK_RSHIFT)
					shiftDown = true;

				if (wparam == VK_CAPITAL)
					capsOn = true;

				Events::KeyUp(TranslateKey((uint32_t)wparam, shiftDown ^ capsOn));
				break;

			case WM_SIZE:

				Resize((uint32_t)(short)LOWORD(lparam), (uint32_t)(short)HIWORD(lparam));
				break;

			case WM_CLOSE:
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			}

			return DefWindowProc(hwnd, msg, wparam, lparam);
		}

#ifdef EUGENIX_OPENGL

		bool InitOpenGLContext()
		{
			if (!(_deviceContext = ::GetDC(_windowHandle)))
			{
				Log::Fatal("Failed to create HDC context: %d\n", ::GetLastError());
				return false;
			}

			memset(&_pixelFormatDesc, 0, sizeof(PIXELFORMATDESCRIPTOR));
			_pixelFormatDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			_pixelFormatDesc.nVersion = 1;
			_pixelFormatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			_pixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
			_pixelFormatDesc.cColorBits = 32;
			_pixelFormatDesc.cDepthBits = 24;

			int resultPfd = ::ChoosePixelFormat(_deviceContext, &_pixelFormatDesc);
			if (!resultPfd || !::SetPixelFormat(_deviceContext, resultPfd, &_pixelFormatDesc))
			{
				Log::Fatal("Failed to set pixel format: %d\n", ::GetLastError());
				return false;
			}

			HGLRC tmpContext = ::wglCreateContext(_deviceContext);
			if (!tmpContext || !::wglMakeCurrent(_deviceContext, tmpContext))
			{
				Log::Fatal("wglMakeCurrent failed : %d\n", ::GetLastError());
				return false;
			}

			PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

			::wglMakeCurrent(_deviceContext, NULL);
			::wglDeleteContext(tmpContext);

			if (wglCreateContextAttribsARB != nullptr)
			{
				int attribs[] =
				{
					WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
					WGL_CONTEXT_MINOR_VERSION_ARB, 3,
					WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
					WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
					0
				};

				_renderContext = wglCreateContextAttribsARB(_deviceContext, 0, attribs);

				if (!_renderContext || !::wglMakeCurrent(_deviceContext, _renderContext))
				{
					Log::Fatal("wglMakeCurrent failed : %d\n", ::GetLastError());
					return false;
				}
			}
			else
			{
				Log::Fatal("wglCreateContextAttribsARB not supported\n");
				return false;
			}

			if (!EugenixGL::Init())
			{
				Log::Fatal("EugenixGL init failed!");
				return false;
			}

			return true;
		}

#endif // EUGENIX_OPENGL

		bool Init(const DisplaySetup& setup)
		{
			_setup = setup;

			auto hInstance = static_cast<HINSTANCE>(::GetModuleHandle(0));

			memset(&_winClass, 0, sizeof(_winClass));
			_winClass.cbSize = sizeof(_winClass);
			_winClass.lpszClassName = _winClassName;
			_winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			_winClass.lpfnWndProc = (WNDPROC)wndProc;
			_winClass.hInstance = hInstance;
			_winClass.lpszClassName = _winClassName;
			_winClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			_winClass.hCursor = LoadCursor(NULL, IDC_ARROW);

			if (!::RegisterClassEx(&_winClass))
			{
				Log::Fatal("RegisterClassEx fail : %d\n", GetLastError());
				return false;
			}

			// Window styles
			DWORD dwStyle;
			DWORD dwExStyle;

			if (_setup.fullScreen)
			{
				memset(&_dmScreenSettings, 0, sizeof(_dmScreenSettings));
				_dmScreenSettings.dmSize = sizeof(_dmScreenSettings);
				_dmScreenSettings.dmPelsWidth = _setup.width;
				_dmScreenSettings.dmPelsHeight = _setup.height;
				_dmScreenSettings.dmBitsPerPel = 32;
				_dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

				if (ChangeDisplaySettings(&_dmScreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL)
				{
					dwStyle = WS_POPUP | WS_SYSMENU;
					dwExStyle = WS_EX_APPWINDOW;
				}
				else
				{
					System::Message("Fullscreen mode is not supported");

					dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
					dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

					_setup.fullScreen = false;
				}
			}
			else
			{
				dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
				dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			}

			int x = (GetSystemMetrics(SM_CXSCREEN) - _setup.width) / 2;
			int y = (GetSystemMetrics(SM_CYSCREEN) - _setup.height) / 2;

			RECT windowRect;
			windowRect.left = (long)x;
			windowRect.right = (long)(x + _setup.width);
			windowRect.top = (long)y;
			windowRect.bottom = (long)(y + _setup.height);

			AdjustWindowRectEx(&windowRect, dwStyle, false, dwExStyle);

			_windowHandle = CreateWindowEx(dwExStyle,
				_winClassName,
				"Eugenix",
				dwStyle,
				windowRect.left,
				windowRect.top,
				windowRect.right - windowRect.left,
				windowRect.bottom - windowRect.top,
				NULL,
				NULL,
				hInstance,
				NULL);

			if (!_windowHandle)
			{
				Log::Fatal("CreateWindowEx failed : %d\n", GetLastError());
				UnregisterClass(_winClassName, _winClass.hInstance);
				return false;
			}

			ShowWindow(_windowHandle, SW_SHOW);
			UpdateWindow(_windowHandle);

#ifdef EUGENIX_OPENGL
			if (!InitOpenGLContext())
			{
				return false;
			}

			if (!EugenixGL::Init())
			{
				Log::Fatal("EugenixGL init failed!");
				return false;
			}
#endif // EUGENIX_OPENGL

			return true;
		}

		void Term()
		{
			if (_setup.fullScreen)
			{
				::ChangeDisplaySettings(nullptr, CDS_RESET);
				ShowCursor(true);
			}

			if (_windowHandle)
			{
				if (_renderContext)
				{
					::wglMakeCurrent(nullptr, nullptr);
					::wglDeleteContext(_renderContext);
					_renderContext = nullptr;
				}

				if (_deviceContext)
				{
					::ReleaseDC(_windowHandle, _deviceContext);
					_deviceContext = nullptr;
				}

				::DestroyWindow(_windowHandle);
				_windowHandle = nullptr;
			}

			::UnregisterClass(_winClassName, (HINSTANCE)::GetModuleHandle(0));
		}

		bool RunFrame()
		{
			MSG msg = { 0 };

			bool quit = false;

			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					quit = true;
				}
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			return !quit;
		}

		void CommitFrame()
		{
			::SwapBuffers(_deviceContext);
		}

		void Resize(uint32_t width, uint32_t height)
		{
#ifdef EUGENIX_OPENGL
			glViewport(0, 0, width, height);
#endif // EUGENIX_OPENGL
		}

		const DisplaySetup& Setup()
		{
			return _setup;
		}

	} // namespace Runtime
} // namespace Eugenix

#endif // EUGENIX_WINDOWS
