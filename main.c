#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// Estrutura para representar uma sala
typedef struct {
    int id;
    char nome[50];
    int lotacao;
    int disponivel; // 1 se disponível, 0 se não disponível
    char descricao[100];
} Sala;

// Estrutura para representar uma reserva
typedef struct {
    int id;
    int id_sala;
    char nome_sala[50];
    char dia[11]; // Formato "dd/mm/yyyy"
    int qtd_pessoas;
} Reserva;

// Variáveis globais
Sala *salas = NULL;
Reserva *reservas = NULL;
int num_salas = 0;
int num_reservas = 0;

// Função para adicionar uma nova reserva ao sistema
void adicionar_reserva(int id_sala, char *nome_sala, char *dia, int qtd_pessoas) {
    reservas = realloc(reservas, (num_reservas + 1) * sizeof(Reserva));
    if (reservas != NULL) {
        reservas[num_reservas].id = num_reservas + 1;
        reservas[num_reservas].id_sala = id_sala;
        strcpy(reservas[num_reservas].nome_sala, nome_sala);
        strcpy(reservas[num_reservas].dia, dia);
        reservas[num_reservas].qtd_pessoas = qtd_pessoas;
        num_reservas++;
    } else {
        printf("Erro: Falha ao alocar memória para reservas.\n");
    }
}

// Função para adicionar uma nova sala
void adicionar_sala(char *nome, int lotacao, int disponivel, char *descricao) {
    salas = realloc(salas, (num_salas + 1) * sizeof(Sala));
    if (salas != NULL) {
        salas[num_salas].id = num_salas + 1;
        strcpy(salas[num_salas].nome, nome);
        salas[num_salas].lotacao = lotacao;
        salas[num_salas].disponivel = disponivel;
        strcpy(salas[num_salas].descricao, descricao);
        num_salas++;
    } else {
        printf("Erro: Falha ao alocar memória para salas.\n");
    }
}

// Função para remover uma sala
void remover_sala(int id_sala) {
    int found = 0;
    for (int i = 0; i < num_salas; i++) {
        if (salas[i].id == id_sala) {
            found = 1;
            for (int j = i; j < num_salas - 1; j++) {
                salas[j] = salas[j + 1];
            }
            num_salas--;
            salas = realloc(salas, num_salas * sizeof(Sala));
            break;
        }
    }
    if (!found) {
        printf("Sala com ID %d não encontrada.\n", id_sala);
    } else {
        printf("Sala removida com sucesso.\n");
    }
}

// Função para salvar as salas e reservas em arquivos
void salvar_dados() {
    FILE *arquivo_salas = fopen("salas.txt", "w");
    FILE *arquivo_reservas = fopen("reservas.txt", "w");

    if (arquivo_salas == NULL || arquivo_reservas == NULL) {
        printf("Erro ao abrir arquivo para escrita.\n");
        return;
    }

    // Escreve as salas no arquivo
    fprintf(arquivo_salas, "%d\n", num_salas);
    for (int i = 0; i < num_salas; i++) {
        fprintf(arquivo_salas, "%d;%s;%d;%d;%s\n", salas[i].id, salas[i].nome, salas[i].lotacao, salas[i].disponivel, salas[i].descricao);
    }

    // Escreve as reservas no arquivo
    fprintf(arquivo_reservas, "%d\n", num_reservas);
    for (int i = 0; i < num_reservas; i++) {
        fprintf(arquivo_reservas, "%d;%d;%s;%s;%d\n", reservas[i].id, reservas[i].id_sala, reservas[i].nome_sala, reservas[i].dia, reservas[i].qtd_pessoas);
    }

    fclose(arquivo_salas);
    fclose(arquivo_reservas);
}

// Função para carregar os dados de salas e reservas de arquivos
void carregar_dados() {
    FILE *arquivo_salas = fopen("salas.txt", "r");
    FILE *arquivo_reservas = fopen("reservas.txt", "r");

    if (arquivo_salas == NULL || arquivo_reservas == NULL) {
        printf("Arquivo de dados não encontrado ou erro ao abrir.\n");
        return;
    }

    // Carrega as salas do arquivo
    fscanf(arquivo_salas, "%d\n", &num_salas);
    salas = malloc(num_salas * sizeof(Sala));
    for (int i = 0; i < num_salas; i++) {
        fscanf(arquivo_salas, "%d;%[^;];%d;%d;%[^\n]\n", &salas[i].id, salas[i].nome, &salas[i].lotacao, &salas[i].disponivel, salas[i].descricao);
    }

    // Carrega as reservas do arquivo
    fscanf(arquivo_reservas, "%d\n", &num_reservas);
    reservas = malloc(num_reservas * sizeof(Reserva));
    for (int i = 0; i < num_reservas; i++) {
        fscanf(arquivo_reservas, "%d;%d;%[^;];%[^;];%d\n", &reservas[i].id, &reservas[i].id_sala, reservas[i].nome_sala, reservas[i].dia, &reservas[i].qtd_pessoas);
    }

    fclose(arquivo_salas);
    fclose(arquivo_reservas);
}

// Função para listar todas as salas disponíveis
void listar_salas() {
    printf("\n===========================\n");
    printf("\nLista de Salas Disponíveis:\n");
    printf("\n");
    for (int i = 0; i < num_salas; i++) {
        if (salas[i].disponivel == 1) {
            printf("ID: %d, Nome: %s, Lotação: %d, Descrição: %s\n", salas[i].id, salas[i].nome, salas[i].lotacao, salas[i].descricao);
        }
    }
    printf("\n");
}

// Função para listar todas as reservas
void listar_reservas() {
    printf("\n===========================\n");
    printf("\nLista de Todas as Reservas:\n");
    printf("\n");
    for (int i = 0; i < num_reservas; i++) {
        if (reservas[i].id != -1) { // Verifica se a reserva não foi cancelada
            printf("ID da Reserva: %d, ID da Sala: %d, Nome da Sala: %s, Dia: %s, Quantidade de Pessoas: %d\n", reservas[i].id, reservas[i].id_sala, reservas[i].nome_sala, reservas[i].dia, reservas[i].qtd_pessoas);
        }
    }
    printf("\n");
}

// Função para listar reservas de uma sala específica
void listar_reservas_sala(int id_sala) {
    printf("\n===========================\n");
    printf("\nLista de Reservas da Sala ID %d:\n", id_sala);
    printf("\n");
    for (int i = 0; i < num_reservas; i++) {
        if (reservas[i].id_sala == id_sala && reservas[i].id != -1) {
            printf("ID da Reserva: %d, Dia: %s, Quantidade de Pessoas: %d\n", reservas[i].id, reservas[i].dia, reservas[i].qtd_pessoas);
        }
    }
    printf("\n");
}

int verifica_disponibilidade_sala(int id_sala, char *dia_sala, int lotacao_sala) {
    for (int i = 0; i < num_reservas; i++) {
        if (reservas[i].id_sala == id_sala) {
            // Verifica conflito de datas
            if (strcmp(reservas[i].dia, dia_sala) == 0 || lotacao_sala > salas[i].lotacao) {
                return 0; // Conflito de data encontrada
            }
        }
    }
    return 1; // Data válida
}

// Função para agendar uma sala
void agendar_sala() {
    int id_sala, qtd_pessoas;
    char dia[11];
    printf("Digite o ID da Sala que deseja reservar: ");
    scanf("%d", &id_sala);
    printf("Digite o dia da reserva (formato dd/mm/yyyy): ");
    scanf("%s", dia);
    printf("Digite a quantidade de pessoas: ");
    scanf("%d", &qtd_pessoas);

    // Verifica se a sala está disponível
    int encontrou_sala = 0;
    for (int i = 0; i < num_salas; i++) {
        if (salas[i].id == id_sala) {
            encontrou_sala = 1;
            if (verifica_disponibilidade_sala(id_sala, dia, qtd_pessoas)) {
                adicionar_reserva(id_sala, salas[i].nome, dia, qtd_pessoas);
                printf("Reserva realizada com sucesso.\n");
            } else {
                printf("Sala não está disponível para a data solicitada ou quantidade de pessoas não permitida.\n");
                listar_salas();
                agendar_sala();
            }
            break;
        }
    }
    if (!encontrou_sala) {
        printf("Sala não encontrada.\n");
    }
}

// Função para cancelar uma reserva
void cancelar_reserva(int id_reserva) {
    int encontrou_reserva = 0;
    for (int i = 0; i < num_reservas; i++) {
        if (reservas[i].id == id_reserva) {
            encontrou_reserva = 1;
            // Marca sala como disponível novamente
            for (int j = 0; j < num_salas; j++) {
                if (salas[j].id == reservas[i].id_sala) {
                    salas[j].disponivel = 1;
                    break;
                }
            }
            // Remove a reserva (simplesmente ignorando ela na impressão)
            reservas[i].id = -1;
            printf("Reserva cancelada com sucesso.\n");
            break;
        }
    }
    if (!encontrou_reserva) {
        printf("Reserva não encontrada.\n");
    }
}

// Função para exibir o menu principal e processar as opções
void exibir_menu() {
    int opcao;
    int id_sala, id_reserva;
    do {
        printf("===========================\n");
        printf("\nMenu Principal:\n");
        printf("\n");
        printf("1. Listar salas\n");
        printf("2. Listar todas as reservas\n");
        printf("3. Listar reservas de uma sala\n");
        printf("4. Agendar sala\n");
        printf("5. Cancelar reserva\n");
        printf("6. Adicionar sala\n");
        printf("7. Remover sala\n");
        printf("8. Encerrar o programa\n");
        printf("\n");
        printf("Digite a sua opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer de entrada

        switch (opcao) {
            case 1:
                listar_salas();
                break;
            case 2:
                listar_reservas();
                break;
            case 3:
                printf("Digite o ID da sala: ");
                scanf("%d", &id_sala);
                listar_reservas_sala(id_sala);
                break;
            case 4:
                agendar_sala();
                break;
            case 5:
                printf("Digite o ID da reserva que deseja cancelar: ");
                scanf("%d", &id_reserva);
                cancelar_reserva(id_reserva);
                break;
            case 6: {
                char nome[50], descricao[100];
                int lotacao, disponivel;
                printf("Digite o nome da sala: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0'; // Remove a nova linha final
                printf("Digite a lotação da sala: ");
                scanf("%d", &lotacao);
                printf("Digite a disponibilidade da sala (1 para disponível, 0 para não disponível): ");
                scanf("%d", &disponivel);
                getchar(); // Limpar o buffer de entrada
                printf("Digite a descrição da sala: ");
                fgets(descricao, sizeof(descricao), stdin);
                descricao[strcspn(descricao, "\n")] = '\0'; // Remove a nova linha final
                adicionar_sala(nome, lotacao, disponivel, descricao);
                printf("Sala adicionada com sucesso.\n");
                break;
            }
            case 7:
                printf("Digite o ID da sala que deseja remover: ");
                scanf("%d", &id_sala);
                remover_sala(id_sala);
                break;
            case 8:
                printf("Encerrando o programa. Salvando dados...\n");
                salvar_dados();
                printf("Dados salvos. Programa encerrado.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    } while (opcao != 8);
}

// Função principal
int main() {

    setlocale(LC_ALL, "pt-BR");

    // Carrega os dados previamente salvos, se houver
    carregar_dados();
    // Exibe o menu principal
    exibir_menu();
    // Libera a memória alocada
    free(salas);
    free(reservas);
    return 0;
}
