#include "UIContext.h"

#include <windowsx.h>
#include <CommCtrl.h>
#include <sstream>

#define MAX_COMBOSTR 1024
#define MAX_EDITSTR 10240

UIContext::UIContext(HWND hWnd) :
_hWnd(hWnd) {

}

bool UIContext::GetCheck(int chkId) {
    return IsDlgButtonChecked(_hWnd, chkId) == BST_CHECKED;
}

bool UIContext::SetCheck(int chkId, bool checked) {
    return CheckDlgButton(
        _hWnd, chkId, checked ? BST_CHECKED : BST_UNCHECKED) == TRUE;
}

void UIContext::AddComboItem(int cmbId, std::wstring item) {
    SendDlgItemMessage(_hWnd, cmbId, CB_ADDSTRING, NULL, (LPARAM) item.c_str());
}

int UIContext::SelectComboItem(int cmbId, std::wstring item) {
    HWND cmbWnd = GetDlgItem(_hWnd, cmbId);
    return ComboBox_SelectString(cmbWnd, -1, item.c_str());
}

void UIContext::SelectComboItem(int cmbId, int itemIdx) {
    HWND cmbWnd = GetDlgItem(_hWnd, cmbId);
    ComboBox_SetCurSel(cmbWnd, itemIdx);
}

std::wstring UIContext::GetComboSelection(int cmbId) {
    HWND cmbWnd = GetDlgItem(_hWnd, cmbId);
    wchar_t text[MAX_COMBOSTR];
    ComboBox_GetText(cmbWnd, text, MAX_COMBOSTR);
    return std::wstring(text);
}

int UIContext::GetComboSelectionIndex(int cmbId) {
    HWND cmbWnd = GetDlgItem(_hWnd, cmbId);
    return ComboBox_GetCurSel(cmbWnd);
}

void UIContext::SetSpinRange(int spId, int lo, int hi) {
    HWND spWnd = GetDlgItem(_hWnd, spId);
    SendMessage(spWnd, UDM_SETRANGE32, lo, hi);
}

void UIContext::Enable(int id) {
    HWND wnd = GetDlgItem(_hWnd, id);
    EnableWindow(wnd, TRUE);
}

void UIContext::Disable(int id) {
    HWND wnd = GetDlgItem(_hWnd, id);
    EnableWindow(wnd, FALSE);
}

void UIContext::SetEnabled(int id, bool enabled) {
    if (enabled == true) {
        Enable(id);
    } else {
        Disable(id);
    }
}

bool UIContext::SetText(int id, std::wstring text) {
    return SetDlgItemText(_hWnd, id, text.c_str()) == TRUE;
}

bool UIContext::SetText(int id, int value) {
    return SetText(id, std::to_wstring(value));
}

std::wstring UIContext::GetText(int id) {
    wchar_t text[MAX_EDITSTR];
    GetDlgItemText(_hWnd, id, text, MAX_EDITSTR);
    return std::wstring(text);
}

int UIContext::GetTextAsInt(int id) {
    std::wstring str = GetText(id);
    int num;
    std::wistringstream wistr(str);
    wistr >> num;
    return num;
}