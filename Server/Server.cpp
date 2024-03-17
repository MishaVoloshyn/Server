#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "WinSock2.h" 
#include <ws2tcpip.h> 
#pragma comment(lib, "Ws2_32.lib") 
#include "Windows.h"
#include "tchar.h"
#include "Resource.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

using namespace std;

HWND hRead, hWrite, hStart, hSend, hEnd, hialog;
const int MAXSTRLEN = 255;
WSADATA wsaData;
SOCKET _socket;
SOCKET acceptSocket;
sockaddr_in addr;

void InitEdit(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    hialog = hDlg;
    hRead = GetDlgItem(hDlg, IDC_EDIT2);
    hWrite = GetDlgItem(hDlg, IDC_EDIT3);

    hStart = GetDlgItem(hDlg, IDC_BUTTON2);
    hEnd = GetDlgItem(hDlg, IDC_BUTTON3);
    hSend = GetDlgItem(hDlg, IDC_BUTTON1);

}



LRESULT CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


    switch (uMsg)
    {
    case WM_INITDIALOG:
        InitEdit(hDlg, uMsg, wParam, lParam);
        break;
	case WM_CLOSE:
		EndDialog(hDlg, NULL);
		break;
    case WM_COMMAND:
		if ((wParam) == IDC_BUTTON2)
		{
            if (_socket != INVALID_SOCKET)
            {
                acceptSocket = accept(_socket, NULL, NULL);
                char buf[MAXSTRLEN];
                int i = recv(acceptSocket, buf, MAXSTRLEN, 0);
                buf[i] = '\0';
                SendMessageA(hWrite, EM_SETSEL, -1, -1);
                SendMessageA(hWrite, EM_REPLACESEL, TRUE, (LPARAM)buf);
                SendMessageA(hWrite, EM_REPLACESEL, TRUE, (LPARAM)"\r\n");

                char buff[MAXSTRLEN];
                GetWindowTextA(hWrite, buff, sizeof(buff));
                SetWindowTextA(hWrite, "");

                send(acceptSocket, buff, strlen(buff), 0);
            }
            else
            {
                WSAStartup(MAKEWORD(2, 2), &wsaData);

			    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			    addr.sin_family = AF_INET;

			    inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);

			    addr.sin_port = htons(20000);
			    bind(_socket, (SOCKADDR*)&addr, sizeof(addr));
			    listen(_socket, 1);
            }

			


		}

		
    break;

    case IDC_BUTTON3:
        PostQuitMessage(0);
        closesocket(acceptSocket);
        closesocket(_socket);
        WSACleanup();
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
		closesocket(acceptSocket);
		closesocket(_socket);
		WSACleanup();
        break;

    }
    return 0;
}

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

