# DISCLAIMER
**Hidden-Call-API is for education/research purposes only. The author takes NO responsibility and/or liability for how you choose to use any of the tools/source code/any files provided.
The author and anyone affiliated with will not be liable for any losses and/or damages in connection with use of ANY files provided with Hidden-Call-API.
By using Hidden-Call-API or any files included, you understand that you are AGREEING TO USE AT YOUR OWN RISK. Once again Hidden-Call-API and ALL files included are for EDUCATION and/or RESEARCH purposes ONLY.
Hidden-Call-API is ONLY intended to be used on your own pentesting labs, or with explicit consent from the owner of the property being tested.** 

# Use
```c
#include "api.h"

typedef int(WINAPI* MSG2)(HWND, LPCWSTR, LPCWSTR, UINT);

int main()
{
  MSG2 msg = (MSG2)call_api(TEXT("USER32.DLL"), "MessageBoxW");

  msg(
    NULL,
    (LPCWSTR)L"Resource not available\nDo you want to try again?",
    (LPCWSTR)L"Account Details",
    MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
  );

}
```
