#include "EvryThng.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define REQUIRED_LINES 10

int main(int argc, char* argv[]) {
    if (argc != 2) {
        ReportError(_T("Необходим аргумент: Имя_файла\n"), 1, FALSE);
        return 1;
    }

    HANDLE hFile = CreateFileA(
        argv[1],
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        ReportError(_T("Ошибка открытия файла\n"), 2, FALSE);
        return 2;
    }

    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(hFile, &fileSize)) {
        ReportError(_T("Ошибка получения размера файла\n"), 2, FALSE);
        CloseHandle(hFile);
        return 1;
    }

    if (fileSize.QuadPart == 0) {
        CloseHandle(hFile);
        return 0;
    }

    LARGE_INTEGER currentPosition;
    currentPosition.QuadPart = fileSize.QuadPart - 1;

    int lineCount = 0;
    BYTE buffer;
    DWORD bytesRead;

    while (currentPosition.QuadPart >= 0 && lineCount < REQUIRED_LINES) {
        if (!SetFilePointerEx(hFile, currentPosition, NULL, FILE_BEGIN)) {
            ReportError(_T("SetFilePointerEx error\n"), 3, FALSE);
            break;
        }

        if (!ReadFile(hFile, &buffer, 1, &bytesRead, NULL) || bytesRead != 1) {
            ReportError(_T("ReadFile error\n"), 3, FALSE);
            break;
        }

        if (buffer == '\n') {
            lineCount++;
            if (lineCount == REQUIRED_LINES) {
                currentPosition.QuadPart++; // Переходим на позицию после '\n'
                break;
            }
        }

        currentPosition.QuadPart--;
    }

    if (lineCount < REQUIRED_LINES) {
        currentPosition.QuadPart = 0;
    }
    else if (currentPosition.QuadPart > fileSize.QuadPart) {
        currentPosition.QuadPart = 0;
    }

    SetFilePointerEx(hFile, currentPosition, NULL, FILE_BEGIN);

    DWORD bytesToRead = (DWORD)(fileSize.QuadPart - currentPosition.QuadPart);
    BYTE* output = (BYTE*)malloc(bytesToRead + 1);
    if (!output) {
        ReportError(_T("Memory allocation error\n"), 3, FALSE);
        CloseHandle(hFile);
        return 1;
    }

    if (!ReadFile(hFile, output, bytesToRead, &bytesRead, NULL)) {
        ReportError(_T("ReadFile error\n"), 3, FALSE);
        free(output);
        CloseHandle(hFile);
        return 1;
    }

    output[bytesRead] = '\0';
    printf("%s", output);

    free(output);
    CloseHandle(hFile);

    return 0;
}