/*
图灵机器人 C++实现

极简版
几乎没有界面（废话，毕竟是控制台）

代码参考自微软提供的例子，见：
https://msdn.microsoft.com/en-us/library/windows/desktop/aa384104(v=vs.85).aspx
*/

#include <iostream>
//#include <Windows.h> 
//#include <winhttp.h> 
//#pragma comment(lib,"winhttp.lib")

#define TULING_URL  L"www.tuling123.com/openapi/api?key=23771317366d42ca8cbbc978a6a87364&info=%s"
static wchar_t String[1024];

//编码转换
char *UnicodeToANSI(const wchar_t *str)
{
    static char result[1024];
    int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
    WideCharToMultiByte(CP_ACP, 0, str, -1, result, len, NULL, NULL);
    result[len] = '\0';
    return result;
}
wchar_t *UTF8ToUnicode(const char *str)
{
    static wchar_t result[1024];
    int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    MultiByteToWideChar(CP_UTF8, 0, str, -1, result, len);
    result[len] = L'\0';
    return result;
}
wchar_t *ANSIToUnicode(const char* str)
{
    int textlen;
    static wchar_t result[1024];
    textlen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    memset(result, 0, sizeof(char) * (textlen + 1));
    MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)result, textlen);
    return result;
}

bool GetHttpPage(void)
{
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer = NULL;
    static HINTERNET hSession = WinHttpOpen(L"A Tuling API Example Program/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    static HINTERNET hConnect = NULL, hRequest = NULL;
    BOOL  bResults = FALSE;

    //从控制台读出一行文字，注意读出来的内容是ANSI编码的，我们需要转换成 Unicode编码
    static char uin[1024]; gets_s(uin);
    wsprintf(String, TULING_URL, ANSIToUnicode(uin));

    //建立一个http的连接会话，给出主机名就行，可以域名，也可以是IP地址，不需要http;前缀
    if (hSession)
    {
        if (!hConnect)
            hConnect = WinHttpConnect(hSession, L"www.tuling123.com", INTERNET_DEFAULT_HTTP_PORT, 0);
    }

    //创建一个HTTP请求句柄
    if (hConnect)
        hRequest = WinHttpOpenRequest(hConnect, L"GET", String, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_ESCAPE_PERCENT | WINHTTP_FLAG_REFRESH);

    //发送请求数据
    if (hRequest)
        bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);

    // 请求结束，接收数据
    if (bResults)
        bResults = WinHttpReceiveResponse(hRequest, NULL);
    else
        printf("Error %d has occurred.\n", GetLastError());
    //如果返回值为false，可以使用getlasterror来得到错误信息，下同
    //返回值的详细信息可以看微软网页，或者看这里翻译好的中文接口说明
    //http://blog.csdn.net/fengsh998/article/details/8201591

    // 内部使用的一个循环来确保能接受到所有数据
    if (bResults)
    {
        do
        {
            //检查是否还有数据需要接收
            dwSize = 0;
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
            {
                printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());
                break;
            }

            if (!dwSize)
                break;

            //为缓冲分配内存并读取
            pszOutBuffer = new char[dwSize + 1];

            if (!pszOutBuffer)
            {
                printf("Out of memory\n");
                break;
            }

            ZeroMemory(pszOutBuffer, dwSize + 1);

            if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
            {
                printf("Error %u in WinHttpReadData.\n", GetLastError());
            }
            else
            {
                //图灵api返回来的内容使用的是UTF-8编码，我们需要把它转换回ANSI才能在控制台显示
                //printf("return:%s\n", UnicodeToANSI(UTF8ToUnicode(pszOutBuffer)) );

                //因为没有使用JSON库，所以我暴力拆了这字符串。
                pszOutBuffer[strlen(pszOutBuffer)-2] = '\0';
                printf("小灵:%s\n\n", UnicodeToANSI(UTF8ToUnicode(pszOutBuffer)) + 23);
                return true;
            }

            delete[] pszOutBuffer; 
            if (!dwDownloaded)
                break;

        } while (dwSize > 0);
    }
    
    //收尾，关闭被打开的句柄
    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);

    return false;
}

int main(void)
{
    system("color F0");
    system("title 会聊天的图灵机器人 ●﹏●");
    printf("\n  我是小灵，快来和我聊天吧! ●▽●\n\n");

    do{ printf("我："); } while (GetHttpPage());

    system("pause");
    return 0;
}