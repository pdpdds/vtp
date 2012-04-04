#ifdef _DEBUG

#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

void _trace(char *fmt, ...)
{
    char out[1024];
	WCHAR wFileName[MAX_PATH];          

	va_list body;
	va_start(body, fmt);
	vsprintf(out, fmt, body);
	va_end(body);

	MultiByteToWideChar( CP_ACP, 0, out, -1,                          
									wFileName, MAX_PATH );
	OutputDebugString(wFileName);
}//_trace()

#endif // #ifdef _DEBUG