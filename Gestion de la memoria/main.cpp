#include <windows.h>
#include <stdio.h>
#include <string.h>

#define SIZE 4096

int main() {
    HANDLE hMapFile;
    LPCTSTR pBuf;
    hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE,   // Handle del archivo mapeado
            NULL,                   // Atributos de seguridad
            PAGE_READWRITE,         // Permisos de lectura/escritura
            0,                      // Tamaño máximo de la memoria (alto)
            SIZE,                   // Tamaño máximo de la memoria (bajo)
            L"MemoriaCompartida");  // Nombre del archivo mapeado
    if (hMapFile == NULL) {
        perror("CreateFileMapping");
        return 1;
    }
    pBuf = (LPTSTR)MapViewOfFile(
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
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        printf("Child reads: %s\n",pBuf);
        munmap(shared_memory, SIZE);
        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
        exit(EXIT_SUCCESS);
        return 0;
    } else {
        strcpy(shared_memory, "Hello, child process!");
        wait(NULL);
        munmap(shared_memory, SIZE);
    }

    return 0;
}