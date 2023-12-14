#include <windows.h>
#include <stdio.h>
#include <string.h>

#define SIZE 4096

int main() {
    HANDLE hMapFile;
    LPCTSTR pBuf;

    // Crear un mapeo de archivo
    hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE,   // Handle del archivo mapeado
            NULL,                   // Atributos de seguridad
            PAGE_READWRITE,         // Permisos de lectura/escritura
            0,                      // Tamaño máximo de la memoria (alto)
            SIZE,                   // Tamaño máximo de la memoria (bajo)
            L"MemoriaCompartida");  // Nombre del objeto de mapeo compartido

    if (hMapFile == NULL) {
        perror("CreateFileMapping");
        return 1;
    }

    // Mapear la vista de archivo a la memoria del proceso actual
    pBuf = (LPCTSTR)MapViewOfFile(
            hMapFile,               // Handle del archivo mapeado
            FILE_MAP_ALL_ACCESS,    // Permisos de acceso
            0,
            0,
            SIZE);

    if (pBuf == NULL) {
        perror("MapViewOfFile");
        CloseHandle(hMapFile);
        return 1;
    }

    // Crear un nuevo proceso
    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(STARTUPINFO);

    if (!CreateProcess(
            NULL,                 // Nombre de la aplicación a ejecutar
            "child_process.exe",  // Nombre del ejecutable del proceso hijo
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi)) {
        perror("CreateProcess");
        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
        return 1;
    }

    // Esperar a que el proceso hijo termine
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Imprimir el contenido leído desde la memoria compartida
    printf("Child reads: %s\n", pBuf);

    // Liberar recursos
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
