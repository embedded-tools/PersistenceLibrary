#include "stdafx.h"
#include "FileToCarray.h"
#include "WinApiHelper.h"
#include "TFileStream.h"
#include "TFilePath.h"
#include "TLZ77Streamed.h"
#include <CommCtrl.h>
#include <CommDlg.h>
#include <Windows.h>
#include <string.h>

HWND hMainDialog;
BOOL bOpenDialogRunning = FALSE;

enum eOutputLanguage
{
    eolC,
    eolCplusplus,
};

INT_PTR CALLBACK	DlgProc(HWND, UINT, WPARAM, LPARAM);
void DoConversion(const char* fileName, bool useDataCompression, eOutputLanguage language);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    BOOL ret;
    MSG msg;  
    RECT rect;

    InitCommonControls();
    hMainDialog = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, DlgProc, 0);        
    GetWindowRect(hMainDialog, &rect);
    MoveWindow(hMainDialog, 128, 128, rect.right-rect.left, rect.bottom-rect.top, FALSE);
    ShowWindow(hMainDialog, SW_SHOW);

    while((ret = GetMessage(&msg, 0, 0, 0)) != 0) 
    {
        if (ret == -1)
        { 
            return -1;
        }

        if(!IsDialogMessage(hMainDialog, &msg)) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg); 
        }
    }
    return 0;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
INT_PTR CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
        case WM_INITDIALOG:
        {
            ComboBox_AddItem(hWnd, IDC_COMBO_COMPRESSION, "No data compression");
            ComboBox_AddItem(hWnd, IDC_COMBO_COMPRESSION, "LZ77 data compression");
            ComboBox_SetItemIndex(hWnd, IDC_COMBO_COMPRESSION, 0); 
            ComboBox_SetDropdownListHeight(hWnd, IDC_COMBO_COMPRESSION, 128);

            ComboBox_AddItem(hWnd, IDC_COMBO_LANGUAGE, "C language");
            ComboBox_AddItem(hWnd, IDC_COMBO_LANGUAGE, "C++ language");
            ComboBox_SetItemIndex(hWnd, IDC_COMBO_LANGUAGE, 0); 
            ComboBox_SetDropdownListHeight(hWnd, IDC_COMBO_LANGUAGE, 128);
            

        }
        break;

        case WM_COMMAND:
        {
            wmId    = LOWORD(wParam);
            wmEvent = HIWORD(wParam);
            // Parse the menu selections:
            switch (wmEvent)
            {
                //dialog init
                case WM_INITDIALOG: return 0;

                //combobox drop down list shown
                case CBN_DROPDOWN: return 0;

                //combobox selection changed
                case CBN_SELCHANGE: return 0;

                //button clicked
                case BN_CLICKED:
                {
                    switch(wmId)
                    {
                        case IDC_BUTTON_CLOSE: 
                            {
                            SendMessage(hWnd, WM_CLOSE, 0, 0); 
                        }
                        break;

                        case IDC_BUTTON_CHOOSE_FILE:
                        {                         
                                static char filename[512];

                                OPENFILENAME ofn;                            
                                memset(&ofn, 0, sizeof(ofn));

                                ofn.lStructSize = sizeof ( ofn );
                                ofn.hwndOwner = hWnd  ;
                                ofn.lpstrFile = filename ;
                                ofn.lpstrFile[0] = '\0';
                                ofn.nMaxFile = sizeof( filename );
                                ofn.lpstrFilter = "All\0*.*\0";
                                ofn.nFilterIndex = 0;

                                bOpenDialogRunning = TRUE;
                                if (GetOpenFileNameA(&ofn))
                                {
                                    SetText(hWnd, IDC_EDIT1, filename);                                
                                }
                                bOpenDialogRunning = FALSE;
                        }
                        break;

                        case IDC_BUTTON_CONVERT: 
                        {
                            char filename[256];
                            filename[0] = 0;
                            GetText(hWnd, IDC_EDIT1, filename, sizeof(filename) );   
                            bool useCompression = ComboBox_GetItemIndex(hWnd, IDC_COMBO_COMPRESSION)>0;
                            eOutputLanguage outputLanguage = (eOutputLanguage) ComboBox_GetItemIndex(hWnd, IDC_COMBO_LANGUAGE); 
                            DoConversion(filename, useCompression, outputLanguage ); break;
                        }
                        break;
                    }
                    return 0;
                }
                default: return DefWindowProc(hWnd, message, wParam, lParam);
            }            
        }
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code here...
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
       if (!bOpenDialogRunning) // bug workaround
       {
           return DefWindowProc(hWnd, message, wParam, lParam);
       }
    }
    return 0;
}

void DoConversion(const char* fileName, bool useDataCompression, eOutputLanguage language)
{
    TFileStream inputFile(fileName, efmOpenRead);
    if (inputFile.GetSize()==0)
    {

        return;
    }

    TFilePath headerFileName = fileName;
    headerFileName.ChangeFileExt(".h");

    TFilePath sourceFileName = fileName;
    sourceFileName.ChangeFileExt(".cpp");


    TFileStream* sourceFile = &inputFile;
    if (useDataCompression)
    {
        TFilePath tempFile = fileName;
        tempFile.ChangeFileExt(".tmp");

        TFileStream compressedFile(tempFile, efmCreate);
        if (!compressedFile.CanWrite())
        {
            return;
        }
        TLZ77Streamed compressor(&compressedFile);
        unsigned char readingBuffer[256];
        long bytesRead;
        while(!inputFile.Eof())
        {
            bytesRead = inputFile.ReadBuffer(readingBuffer, sizeof(readingBuffer));
            compressor.WriteBuffer(readingBuffer, bytesRead);
        }
        compressor.Close();
        compressedFile.Close();

        sourceFile = new TFileStream(tempFile, efmOpenRead);
    }

    int size = sourceFile->GetSize();
    unsigned char b;

    TFileStream outputHeaderFile(headerFileName, efmCreate);
    if (!outputHeaderFile.CanWrite())
    {
        return;
    }    

    TString name = headerFileName.ExtractFileName();
    int n = name.IndexOf('.');
    if (n!=-1)
    {
        name.SetLength(n);
    }
    TString define = name;
    for(int i = 0; i<define.Length(); i++)
    {
        if ((define[i]>='A') && (define[i]<='Z')) continue;
        if ((define[i]>='a') && (define[i]<='z')) continue;
        if ((define[i]>='0') && (define[i]<='9')) continue;
        if (define[i]=='_') continue;
        define[i] = '_';
    }
    define.UpperCase();
    define+="___H";

    outputHeaderFile.WritePlainText("#ifndef ");
    outputHeaderFile.WritePlainText(define);
    outputHeaderFile.WritePlainText("\r\n");
    outputHeaderFile.WritePlainText("#define ");
    outputHeaderFile.WritePlainText(define);
    outputHeaderFile.WritePlainText("\r\n");
    outputHeaderFile.WritePlainText("\r\n");
    outputHeaderFile.WritePlainText("extern const unsigned char ");
    outputHeaderFile.WritePlainText(name);
    outputHeaderFile.WriteByte('[');
    outputHeaderFile.WriteULongAsText(size);
    outputHeaderFile.WritePlainText("];\r\n");
    outputHeaderFile.WritePlainText("\r\n");
    outputHeaderFile.WritePlainText("#endif\r\n");   
    outputHeaderFile.Close();

    TFileStream outputSourceFile(sourceFileName, efmCreate);    
    outputSourceFile.WritePlainText("#include \"");
    outputSourceFile.WritePlainText(name);
    outputSourceFile.WritePlainText(".h\r\n");
    outputSourceFile.WritePlainText("\r\n");
    outputSourceFile.WritePlainText("const unsigned char ");
    outputSourceFile.WritePlainText(name);
    outputSourceFile.WriteByte('[');
    outputSourceFile.WriteULongAsText(size);
    outputSourceFile.WritePlainText("] =\r\n{");


    bool firstLine = true;
    while(size>0)
    {        
        char hexNumber[5];
        outputSourceFile.WritePlainText("\r\n\t");

        for(int j = 0; j<16; j++)
        {
            sourceFile->ReadByte(b);  

            hexNumber[0] = '0';
            hexNumber[1] = 'x';
            hexNumber[2] = (b>>4) + '0';
            if (hexNumber[2]>'9') hexNumber[2]+=7;

            hexNumber[3] = (b & 15) + '0';
            if (hexNumber[3]>'9') hexNumber[3]+=7;

            hexNumber[4] = 0;

            outputSourceFile.WritePlainText(hexNumber);

            size--;
            if (size>0)
            {
                outputSourceFile.WriteByte(',');
            } else break;           
        }
        firstLine = false;
    }    
    outputSourceFile.WritePlainText("\r\n};\r\n");
    outputSourceFile.Close();

}

