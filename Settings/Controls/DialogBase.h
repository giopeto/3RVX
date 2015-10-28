#pragma once

#include <Windows.h>
#include <unordered_map>

class Control;

class DialogBase {
public:
    void AddControl(Control *control);
    HWND DialogHandle();

protected:
    HWND _dlgHwnd;

    /// <summary>Maps control IDs to their respective instances.</summary>
    std::unordered_map<int, Control *> _controlMap;

    static INT_PTR CALLBACK StaticDialogProc(HWND hwndDlg, UINT uMsg,
        WPARAM wParam, LPARAM lParam);

    virtual INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg,
        WPARAM wParam, LPARAM lParam);
};