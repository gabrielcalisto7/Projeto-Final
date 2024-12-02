#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Informações predefinidas de login:
 * professor | admin
 * aluno     | aluno
 *
 * */

// Definindo as constantes
#define MAX_ALUNOS 100
#define MAX_USUARIOS 2
#define MAX_NOTAS 4

// Estrutura Alunos
typedef struct {
    int numero;
    char matricula[4];
    char nome[50];
    char curso[20];
    float notas[4];
    float media;
    char status[20];
} Aluno;

// Estrutura Usuarios
typedef struct {
    char usuario[20];
    char senha[20];
} Usuario;


//Variaveis globais
Aluno alunos[MAX_ALUNOS];
int total_alunos = 0;
Usuario usuarios[MAX_USUARIOS] = {
        {"professor", "admin"},
        {"aluno", "aluno"}
};
int contador_matricula = 1;

// Protótipos das funções
void login(char *tipo_usuario);
void cadastrar_aluno();
void listar_alunos();
void editar_aluno();
void menu_professor();
void menu_aluno();
void lancar_notas();
void salvar_dados();
void carregar_dados();
void remover_aluno();

// Função para realizar login
void login(char *tipo_usuario) {
    char usuario[20];
    char senha[20];
    int login_sucesso = 0;

    while (!login_sucesso) {
        printf("\n=== Sistema Gerenciador de Alunos ===\n\n");
        printf("Login\n");
        printf("Usuario:");
        scanf("%s", usuario);
        printf("Senha:");
        scanf("%s", senha);

        for (int i = 0; i < MAX_USUARIOS; i++) {
            if (strcmp(usuarios[i].usuario, usuario) == 0 && strcmp(usuarios[i].senha, senha) == 0) {
                login_sucesso = 1;
                strcpy(tipo_usuario, usuario);
                break;
            }
        }

        if (!login_sucesso) {
            printf("Usuario ou senha incorretos. Tente novamente.\n\n");
        }
    }

    printf("Login Bem-sucedido!\n\n");
}


// Função para cadastrar aluno
void cadastrar_aluno(Aluno alunos[], int *qtd_alunos) {
    if (*qtd_alunos >= MAX_ALUNOS) {
        printf("Erro: Limite de alunos atingido!\n");
        return;
    }

    // Gera um número único para o aluno
    int novo_numero = (*qtd_alunos > 0) ? alunos[*qtd_alunos - 1].numero + 1 : 1;

    printf("Cadastrando novo aluno com numero %03d...\n", novo_numero);
    alunos[*qtd_alunos].numero = novo_numero; // Atribui o número único

    // Gera matrícula automaticamente
    snprintf(alunos[*qtd_alunos].matricula, sizeof(alunos[*qtd_alunos].matricula), "%03d", *qtd_alunos + 1);

    printf("Digite o nome do aluno: ");
    scanf(" %[^\n]", alunos[*qtd_alunos].nome);

    printf("Digite o curso do aluno: ");
    scanf(" %[^\n]", alunos[*qtd_alunos].curso);

    // Inicializa as notas e o status
    for (int i = 0; i < MAX_NOTAS; i++) {
        alunos[*qtd_alunos].notas[i] = 0.0;
    }
    alunos[*qtd_alunos].media = 0.0;
    strcpy(alunos[*qtd_alunos].status, "Indefinido");

    (*qtd_alunos)++; // Incrementa a quantidade de alunos
    printf("Aluno cadastrado com sucesso!\n");
}


// Função para lançar notas
void lancar_notas() {
    char matricula[4];
    printf("=== Lancamento de Notas ===\n");
    printf("Digite a matricula do aluno: ");
    scanf("%s", matricula);

    for (int i = 0; i < total_alunos; i++) {
        if (strcmp(alunos[i].matricula, matricula) == 0) {
            printf("Aluno encontrado: %s\n", alunos[i].nome);

            // Lançando as notas
            for (int j = 0; j < 4; j++) {
                do {
                    printf("Digite a %d nota: ", j + 1);
                    scanf("%f", &alunos[i].notas[j]);

                    // Verifica se a nota está dentro do intervalo válido
                    if (alunos[i].notas[j] < 0 || alunos[i].notas[j] > 10) {
                        printf("Nota invalida. A nota deve ser entre 0 e 10. Tente novamente.\n");
                    }
                } while (alunos[i].notas[j] < 0 || alunos[i].notas[j] > 10); // Repete até uma nota válida ser inserida
            }

            // Calculando a média
            alunos[i].media = 0;
            for (int j = 0; j < 4; j++) {
                alunos[i].media += alunos[i].notas[j];
            }
            alunos[i].media /= 4; // Média simples

            printf("Notas lancadas com sucesso! Verifique o status do aluno em 'Listar Alunos'\n\n");
            return;
        }
    }

    printf("Aluno nao encontrado. Verifique e tente novamente.\n\n");
}

// Função para listar alunos
void listar_alunos() {
    printf("=== Lista de Alunos ===\n");
    if (total_alunos == 0) {
        printf("Nenhum aluno cadastrado.\n\n");
        return;
    }

    for (int i = 0; i < total_alunos; i++) {
        // Calcular o status de aprovação diretamente ao listar
        printf("Matricula: %s, Nome: %s, Curso: %s, Status: %s, Media: %.2f\n",
               alunos[i].matricula, alunos[i].nome, alunos[i].curso,
               (alunos[i].media >= 6.0 ? "Aprovado" : "Reprovado"), alunos[i].media);
    }
    printf("\n");
}

// Função para editar aluno
void editar_aluno() {
    char matricula[20];
    printf("=== Editar Aluno ===\n");
    printf("Digite a matricula do aluno a ser editado: ");
    scanf("%s", matricula);
    getchar(); // Limpa o buffer do '\n' deixado pelo scanf.

    for (int i = 0; i < total_alunos; i++) {
        if (strcmp(alunos[i].matricula, matricula) == 0) {
            printf("Aluno encontrado: %s\n", alunos[i].nome);

            printf("Novo nome: ");
            fgets(alunos[i].nome, sizeof(alunos[i].nome), stdin);
            alunos[i].nome[strcspn(alunos[i].nome, "\n")] = '\0'; // Remove o '\n' do final.

            printf("Novo curso: ");
            fgets(alunos[i].curso, sizeof(alunos[i].curso), stdin);
            alunos[i].curso[strcspn(alunos[i].curso, "\n")] = '\0';


            printf("Aluno atualizado com sucesso!\n\n");
            return;
        }
    }

    printf("Aluno nao encontrado.\n\n");
}

// Função para remover aluno
void remover_aluno() {
    char matricula[4];
    printf("=== Remover Aluno ===\n");
    printf("Digite a matricula do aluno a ser removido: ");
    scanf("%s", matricula);

    for (int i = 0; i < total_alunos; i++) {
        if (strcmp(alunos[i].matricula, matricula) == 0) {
            printf("Aluno encontrado: %s\n", alunos[i].nome);
            printf("Removendo aluno...\n");

            // Ajusta o array, deslocando os elementos seguintes
            for (int j = i; j < total_alunos - 1; j++) {
                alunos[j] = alunos[j + 1];
            }
            total_alunos--;

            printf("Aluno removido com sucesso!\n\n");
            return;
        }
    }

    printf("Aluno nao encontrado.\n\n");
}


// Menu do professor
void menu_professor() {
    int opcao;

    do {
        printf("=== Bem Vindo Professor ===\n");
        printf("1. Listar alunos\n");
        printf("2. Cadastrar aluno\n");
        printf("3. Editar aluno\n");
        printf("4. Lancar notas\n");
        printf("5. Remover aluno\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                listar_alunos();
                break;
            case 2:
                cadastrar_aluno(alunos, &total_alunos);
                break;
            case 3:
                editar_aluno();
                break;
            case 4:
                lancar_notas();
                break;
            case 5:
                remover_aluno();
                break;
            case 6:
                printf("Encerrando o sistema");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n\n");
        }
    } while (opcao != 5);
}

void menu_aluno() {
    int opcao;

    do {
        printf("=== Bem Vindo Aluno ===\n");
        printf("1. Listar alunos\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                listar_alunos();
                break;
            case 0:
                printf("Saindo do sistema...\n\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n\n");
        }
    } while (opcao != 0);
}

void salvar_dados() {
    FILE *arquivo = fopen("dados_alunos.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para salvar os dados!\n");
        return;
    }

    for (int i = 0; i < total_alunos; i++) {
        fprintf(arquivo, "Matricula: %s\n", alunos[i].matricula);
        fprintf(arquivo, "Nome: %s\n", alunos[i].nome);
        fprintf(arquivo, "Curso: %s\n", alunos[i].curso);
        fprintf(arquivo, "Notas: %.2f %.2f %.2f %.2f\n", alunos[i].notas[0], alunos[i].notas[1], alunos[i].notas[2], alunos[i].notas[3]);
        fprintf(arquivo, "Média: %.2f\n", alunos[i].media);
        fprintf(arquivo, "Status: %s\n\n", alunos[i].status);
    }

    fclose(arquivo);
}

void carregar_dados() {
    FILE *arquivo = fopen("dados_alunos.txt", "r");
    if (arquivo == NULL) {
        printf("Nenhum dado encontrado para carregar.\n");
        return;
    }

    // Leitura dos dados dos alunos
    while (fscanf(arquivo, "Matricula: %s\n", alunos[total_alunos].matricula) != EOF) {
        if (total_alunos >= MAX_ALUNOS) break;  // Limite de alunos alcançado

        fscanf(arquivo, "Nome: %[^\n]\n", alunos[total_alunos].nome);
        fscanf(arquivo, "Curso: %[^\n]\n", alunos[total_alunos].curso);
        fscanf(arquivo, "Notas: %f %f %f %f\n",
               &alunos[total_alunos].notas[0],
               &alunos[total_alunos].notas[1],
               &alunos[total_alunos].notas[2],
               &alunos[total_alunos].notas[3]);
        fscanf(arquivo, "Média: %f\n", &alunos[total_alunos].media);
        fscanf(arquivo, "Status: %[^\n]\n\n", alunos[total_alunos].status);

        total_alunos++;
    }

    fclose(arquivo);
}


// Função principal
int main() {
    char tipo_usuario[20];

    carregar_dados();

    login(tipo_usuario);

    if (strcmp(tipo_usuario, "professor") == 0) {
        menu_professor();
    } else if (strcmp(tipo_usuario, "aluno") == 0) {
        menu_aluno();
    }

    salvar_dados();

    return 0;
}
