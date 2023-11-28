#include <stdio.h>
#include <dirent.h>

int processFile(const char *filePath) {
    FILE *arquivo = fopen(filePath, "r");
    if (arquivo == NULL) {
        printf("Não foi possível abrir o arquivo %s.\n", filePath);
        return -1;
    }

    int N;
    fscanf(arquivo, "%d", &N);

    int ti, di;
    int momentoAtual = 0;
    int momentoFinal_direcaoAtual = 0;
    int momentoInicio_Esperar = 0;
    int direcaoesperar = -1;
    int direcaoAtual = -1;
    int esperar = 0;

    for (int i = 0; i < N; i++) {
        fscanf(arquivo, "%d %d", &ti, &di);

        if (esperar && ti >= momentoInicio_Esperar) {
            momentoAtual = momentoFinal_direcaoAtual;
            momentoFinal_direcaoAtual = momentoAtual + 10;
            direcaoAtual = direcaoesperar;
            esperar = 0;
        }

        if (direcaoAtual == -1) {
            momentoAtual = ti;
            momentoFinal_direcaoAtual = ti + 10;
            direcaoAtual = di;
        } else if (di != direcaoAtual) {
            if (ti > momentoFinal_direcaoAtual) {
                momentoAtual = ti;
                momentoFinal_direcaoAtual = ti + 10;
                direcaoAtual = di;
            } else {
                if (!esperar) {
                    momentoInicio_Esperar = momentoFinal_direcaoAtual;
                    direcaoesperar = di;
                    esperar = 1;
                } else {
                    momentoInicio_Esperar = momentoFinal_direcaoAtual;
                }
            }
        } else if (ti <= momentoFinal_direcaoAtual) {
            momentoFinal_direcaoAtual = ti + 10;
            momentoInicio_Esperar = momentoFinal_direcaoAtual;
        } else {
            momentoAtual = ti;
            momentoFinal_direcaoAtual = ti + 10;
        }
    }

    if (esperar) {
        momentoFinal_direcaoAtual += 10;
    }

    fclose(arquivo);
    return momentoFinal_direcaoAtual;
}

int main() {
    DIR *d;
    struct dirent *dir;
    d = opendir("./input");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) { 
                char filePath[256];
                snprintf(filePath, sizeof(filePath), "./input/%s", dir->d_name);
                int result = processFile(filePath);
                printf("\n %s: %d\n", dir->d_name, result);
            }
        }
        closedir(d);
    } else {
        printf("Não foi possível abrir o diretório 'input'.\n");
        return 1;
    }
    return 0;
}
