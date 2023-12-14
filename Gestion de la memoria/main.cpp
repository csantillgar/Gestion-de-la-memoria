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

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("Child reads: %s\n", shared_memory);
        munmap(shared_memory, SIZE);
        exit(EXIT_SUCCESS);
    } else {
        strcpy(shared_memory, "Hello, child process!");
        wait(NULL);
        munmap(shared_memory, SIZE);
    }

    return 0;
}