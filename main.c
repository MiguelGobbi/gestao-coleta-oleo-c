#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pss.h"

#define MAX 1000
#define ARQUIVO_DADOS "dados.bin"
#define ARQUIVO_BACKUP "dados_backup.bin"

typedef struct {
    char data[11];        
    float quantidade;
    char lote[11];       
} Registro;

Registro registros[MAX];
int total = 0;

void clear() {
#ifdef __linux__
    system("clear");
#elif _WIN32
    system("cls");
#endif
}

void salvar_binario() {
    FILE *f = fopen(ARQUIVO_DADOS, "ab+");
    if (f) {
        fwrite(&total, sizeof(int), 1, f);
        fwrite(registros, sizeof(Registro), total, f);
        fclose(f);
    }
}

void carregar_binario() {
    FILE *f = fopen(ARQUIVO_DADOS, "rb");
    if (f) {
        fread(&total, sizeof(int), 1, f);
        fread(registros, sizeof(Registro), total, f);
        fclose(f);
    }
}

void backup() {
    FILE *f = fopen(ARQUIVO_BACKUP, "wb");
    if (f) {
        fwrite(&total, sizeof(int), 1, f);
        fwrite(registros, sizeof(Registro), total, f);
        fclose(f);
    }
}

int validar_data(const char *data) {
    unsigned int i;
	if (strlen(data) != 10) return 0;
    if (data[2] != '/' || data[5] != '/') return 0;

    for (i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && !isdigit(data[i]))
            return 0;
    }

    int dia, mes, ano;
    sscanf(data, "%2d/%2d/%4d", &dia, &mes, &ano);

    if (mes < 1 || mes > 12 || dia < 1 || ano < 1)
        return 0;

    int dias_no_mes[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
    if (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0))
        dias_no_mes[2] = 29;

    if (dia > dias_no_mes[mes]) return 0;

    return 1;
}

int importar_txt(const char *arquivo_nome) {
    FILE *f = fopen(arquivo_nome, "r");
    if (!f) {
        printf("Erro ao abrir o arquivo '%s'\n", arquivo_nome);
        return 0;
    }

    char linha[100];
    long int adicionados = 0;

    while (fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\n")] = '\0';

        char *data = strtok(linha, " ");
        char *litros = strtok(NULL, "\n");

        if (data == NULL || litros == NULL) 
			continue;

        if (!validar_data(data)) {
            printf("Data invalida: '%s'\n", data);
            continue;
        }

        float qtd;
        if (sscanf(litros, "%f", &qtd) != 1) {
            printf("Erro na quantidade: '%s'\n", litros);
            continue;
        }

        if (total >= MAX) {
            printf("Limite de registros atingido (%d).\n", MAX);
            break;
        }

        Registro r;
        strncpy(r.data, data, 10);
        r.data[10] = '\0';
        r.quantidade = qtd;
        strncpy(r.lote, arquivo_nome, sizeof(r.lote));
        r.lote[sizeof(r.lote) - 1] = '\0';

        registros[total++] = r;
        adicionados++;
    }

    fclose(f);
    salvar_binario();
    printf("Importacao concluida: %ld registros adicionados.\n", adicionados);
    return 1;
}

void gerar_geral_csv() {
    unsigned int i;
	FILE *f = fopen("relatorio_geral.csv", "w");
    if (!f) {
        printf("Erro ao criar relatorio_geral.csv\n");
        return;
    }
	printf("\n+========================================+\n");
	printf("|    Data    | Total litros |    Lote    |\n");
	printf("+========================================+\n");
    fprintf(f, "Data;Quantidade;Lote\n");
    for (i = 0; i < total; i++) {
        fprintf(f, "%s;%.2f;%s\n", registros[i].data, registros[i].quantidade, registros[i].lote);
		printf("| %10s |    %6.2f    | %10s |\n", registros[i].data, registros[i].quantidade, registros[i].lote);
    }
	printf("+========================================+\n");
    
	fclose(f);
    printf("Relatorio geral gerado com sucesso.\n");
}

void gerar_mensal_csv() {
    unsigned int i;
	float soma[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

    for (i = 0; i < total; i++) {
        int mes;
        sscanf(&registros[i].data[3], "%2d", &mes);
        if (mes >= 1 && mes <= 12) {
            soma[mes] += registros[i].quantidade;
        }
    }

    FILE *f = fopen("relatorio_mensal.csv", "w");
    if (!f) {
        printf("Erro ao criar relatorio_mensal.csv\n");
        return;
    }
	printf("\n+============================+\n");
	printf("|   Mes   |   Total litros   |\n");
	printf("+============================+\n");
	
    fprintf(f, "Mes;Total Litros\n");
    for (i = 1; i <= 12; i++) {
        fprintf(f, "%02d;%.2f\n", i, soma[i]);
        printf("|    %02d   |      %6.2f      |\n", i, soma[i]);    
    }
    printf("+============================+\n");

    fclose(f);
    printf("Relatorio mensal gerado com sucesso.\n");
}

void excluir_lote() {
    unsigned int i;
	char nome_lote[11];
    input_s("Informe o nome do lote (arquivo .txt): ", nome_lote, sizeof(nome_lote));

    int novoTotal = 0;
    long int excluidos = 0;

    for (i = 0; i < total; i++) {
        if (strcmp(registros[i].lote, nome_lote) != 0) {
            registros[novoTotal++] = registros[i];
        } else {
            excluidos++;
        }
    }

    if (excluidos == 0) {
        printf("Nenhum registro encontrado com o lote '%s'.\n", nome_lote);
    } else {
        backup();
        total = novoTotal;
        salvar_binario();
        printf("%ld registro(s) excluido(s) do lote '%s'. Backup salvo.\n", excluidos, nome_lote);
    }
}

void menu() {
    int opcao;
    do {
        clear();
        printf("+==========================================+\n");
		printf("|--------- MENU DE COLETA DE OLEO ---------|\n");
		printf("+==========================================+\n");
        printf("|  1. Inserir lote (importar arquivo .txt) |\n");
        printf("|  2. Eliminar lote (por nome do arquivo)  |\n");
        printf("|  3. Gerar relatorio mensal (CSV)         |\n");
        printf("|  4. Gerar listagem geral (CSV)           |\n");
        printf("|  5. Encerrar                             |\n");
        printf("+==========================================+\n");

        opcao = input_d("Escolha uma opcao: ");

        switch (opcao) {
            case 1: {
				char nomeArquivo[100];
                input_s("Informe o nome do arquivo .txt: ", nomeArquivo, sizeof(nomeArquivo));
                importar_txt(nomeArquivo);
                break;
            }
            case 2:
            	if(total == 0)
            		printf("Insira um lote primeiro!");
            	else
                	excluir_lote();
                break;
            case 3:
            	if(total == 0)
            		printf("Insira um lote primeiro!");
            	else
                	gerar_mensal_csv();
                break;
            case 4:
            	if(total == 0)
            		printf("Insira um lote primeiro!");
            	else
               		gerar_geral_csv();
                break;
            case 5:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opcão inválida.\n");
        }

        printf("\nPressione enter para continuar...");
        setbuf(stdin, NULL);
        getchar();

    } while (opcao != 5);
}

int main() {
    carregar_binario();
    menu();
    return 0;
}


