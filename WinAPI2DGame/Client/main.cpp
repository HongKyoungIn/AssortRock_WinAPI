// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Client.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

struct _tagArea
{
	bool bStart;
	POINT ptStart;
	POINT ptEnd;
};

_tagArea g_tArea;

// SAL 주석(소스코드 주석 언어)
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, /*실행 된 프로세스의 시작 주소(가상 주소)*/
					  _In_opt_ HINSTANCE hPrevInstance, /*이전에 실행 된 프로세스의 시작 주소, 지금은 의미없음*/
					  _In_ LPWSTR    lpCmdLine, /*프로그램 시작 시 넣는 문자열을 넣을 시 문자열을 저장하는 변수*/
					  _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); // 프로젝트의 이름을 문자열 값으로 사용. 프로젝트 타이틀 창 이름을 설정
	LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING); // 

	// 윈도우 정보 등록
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	// 윈도우 생성
	if(!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	MSG msg;
	/*
	HWND        hwnd;
	UINT        message;
	WPARAM      wParam;
	LPARAM      lParam;
	DWORD       time;
	POINT       pt;
	*/

	// GetMessage
	// 메세지 큐에서 메세지 확인할 때까지 대기
	// msg.message == WM_QUIT일경우 false를 반환 -> 프로그램 종료
	while(GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CLIENT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	// 윈도우에서 제공해주는 함수.
	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	// 윈도우 창 이름 넣는 함수
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
							  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if(!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// 메뉴 선택을 구문 분석합니다:
			switch(wmId)
			{
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;

		// 마우스 왼쪽 버튼을 눌렀을 때 들어온다.
		case WM_LBUTTONDOWN:
		{
			// 마우스 위치는 lParam에 들어오게 되는데 16비트로 쪼개서 x, y 값이 32비트 변수에 들어오게 된다.
			// LOWORD, HIWORD 매크로를 이용해서 하위, 상위 16비트의 값을 얻어올 수 있다.
			if(!g_tArea.bStart)
			{
				g_tArea.bStart = true;
				g_tArea.ptStart.x = lParam & 0x0000ffff;
				g_tArea.ptStart.y = lParam >> 16;

				// InvalidateRect 함수는 강제로 WM_PAINT 메시지를 호출해주는 함수이다.
				// 1번 인자는 윈도우 핸들이 들어간다.
				// 2번 인자는 초기화할 영역이 들어간다.
				// 2번 인자에 NULL을 넣어줄 경우 전체 화면을 대상으로 갱신한다.
				// 3번 인자는 True일 경우 현재 화면을 지우고 갱신한다.
				// Flase일 경우 안 지우고 갱신한다.
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
			// 마우스가 움직일 때 들어오는 메시지이다.
		case WM_MOUSEMOVE:
			if(g_tArea.bStart)
			{
				g_tArea.ptEnd.x = lParam & 0x0000ffff;
				g_tArea.ptEnd.y = lParam >> 16;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}

		// 마우스 왼쪽 버튼을 누르다가 뗐을 때 발생하는 메세지이다.
		case WM_LBUTTONUP:
			if(g_tArea.bStart)
			{
				g_tArea.bStart = false;
				g_tArea.ptEnd.x = lParam & 0x0000ffff;
				g_tArea.ptEnd.y = lParam >> 16;
				InvalidateRect(hWnd, NULL, TRUE);
			}
		// 키가 눌러졌을 때 들어오는 메시지
		case WM_KEYDOWN:
		{
			// 이 메세지가 들어올 경우 wParam에 어떤 키를 눌렀는지 들어온다.
			switch(wParam)
			{
				case VK_ESCAPE:
					DestroyWindow(hWnd);
					break;
			}
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			// 유니코드 문자열을 "" 앞에 L을 붙여서 L""로 해줄 수 있다.
			// 아니면 TEXT 매크로를 이용한다.
			TextOut(hdc, 50, 50, TEXT("win32"), 5);
			TextOut(hdc, 150, 50, TEXT("win32"), 5);
			TextOut(hdc, 250, 50, TEXT("win32"), 5);

			TCHAR strMouse[64] = {};
			// wsprintf : 유니코드 문자열을 만들어주는 함수이다.
			wsprintf(strMouse, TEXT("Start = x : %d y : %d"), g_tArea.ptStart.x, g_tArea.ptStart.y);

			// lstrlen : 유니코드 문자열의 길이를 구해주는 함수이다.
			TextOut(hdc, 600, 30, strMouse, lstrlen(strMouse));

			// 사각형
			Rectangle(hdc, 100, 100, 200, 200);

			// 원
			Ellipse(hdc, 100, 100, 200, 200);

			// 라인
			MoveToEx(hdc, 300, 100, NULL);
			LineTo(hdc, 400, 150);
			LineTo(hdc, 500, 100);
			
			MoveToEx(hdc, 100, 400, NULL);
			LineTo(hdc, 200, 500);

			if(g_tArea.bStart)
			{
				Rectangle(hdc, g_tArea.ptStart.x, g_tArea.ptStart.y, g_tArea.ptEnd.x, g_tArea.ptEnd.y);
			}

			EndPaint(hWnd, &ps);
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch(message)
	{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
	}
	return (INT_PTR)FALSE;
}
