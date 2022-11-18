#include "Hello.h"
#define PATH _T("C:\\Users\\Administrator\\Desktop\\LSP��װ\\LSPDll.dll")
#define MAXPATH 1024
unsigned int _stdcall Thread(LPVOID lpPara);
SOCKET server_tcp = NULL;
SOCKET server_udp = NULL;
SOCKET client_tcp = NULL;
SOCKET client_udp = NULL;
int main(int argc, char **argv)
{
	system("color 2");

	SetConsoleTitle(_T("LSPЭ���ƽⲹ����װ"));
	ProtocolTraversestheExperiment2 s;
	printf("��װLSPǰ������Э��:\r\n");
	s.ShowAllProtocol();
	installLSP LSP;
	SetConsoleTitle(PATH);
	LSP.InstallProvider(PATH);
	printf("��װLSP�������Э��:\r\n");
	s.ShowAllProtocol();
	int ab = 0;
	scanf("%d", &ab);
	LSP.RemoveProvider();
	printf("���LSP���\r\n");

	return 0;
}