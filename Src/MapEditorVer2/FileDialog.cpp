#include "FileDialog.h"
#include <windows.h>
#include <shobjidl.h>

namespace Platform
{
    // ファイルオープンダイアログを表示し、選択されたファイルのパスをUTF-8で返す
    std::string OpenFileDialog(const wchar_t* filter)
    {
        std::string result;

        IFileOpenDialog* pFileOpen = nullptr;

        HRESULT hr = CoCreateInstance(
            CLSID_FileOpenDialog,
            nullptr,
            CLSCTX_ALL,
            IID_IFileOpenDialog,
            reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            if (filter)
            {
                COMDLG_FILTERSPEC spec[] =
                {
                    { L"Files", filter }
                };
                pFileOpen->SetFileTypes(1, spec);
            }

            hr = pFileOpen->Show(nullptr);

            if (SUCCEEDED(hr))
            {
                IShellItem* pItem = nullptr;
                hr = pFileOpen->GetResult(&pItem);

                if (SUCCEEDED(hr))
                {
                    PWSTR filePath = nullptr;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);

                    if (SUCCEEDED(hr))
                    {
                        char buffer[MAX_PATH];
                        WideCharToMultiByte(
                            CP_UTF8, 0,
                            filePath, -1,
                            buffer, MAX_PATH,
                            nullptr, nullptr);

                        result = buffer;
                        CoTaskMemFree(filePath);
                    }

                    pItem->Release();
                }
            }

            pFileOpen->Release();
        }

        return result;
    }
}