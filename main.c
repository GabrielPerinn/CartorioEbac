#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define SEPARADOR ","

// Função para adicionar dados ao arquivo
void adicionar_dados_arquivo(FILE *file, const char *dado) {
    fprintf(file, "%s%s", dado, SEPARADOR);
}

// Função para criar e abrir arquivo
FILE* criar_arquivo(const char *nome_arquivo) {
    FILE *file = fopen(nome_arquivo, "w");
    if (file == NULL) {
        printf("Erro ao criar o arquivo.\n");
    }
    return file;
}

// Função para abrir arquivo no modo de adição (append)
FILE* abrir_arquivo_append(const char *nome_arquivo) {
    FILE *file = fopen(nome_arquivo, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
    }
    return file;
}

// Função para confirmar ação com opções claras
int confirmar_acao(const char *acao) {
    int resposta;
    printf("\nVocê deseja %s?\n", acao);
    printf("Digite 1 para 'Sim' ou 2 para 'Não': ");
    
    while (scanf("%d", &resposta) != 1 || (resposta != 1 && resposta != 2)) {
        printf("Entrada inválida. Por favor, digite 1 para 'Sim' ou 2 para 'Não': ");
        while (getchar() != '\n');  // Limpa o buffer
    }
    
    return resposta == 1;
}

// Função para limpar o buffer de entrada
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para registrar os dados
int registro() {
    char arquivo[40];
    char cpf[40];
    char nome[40];
    char sobrenome[40];
    char cargo[40];

    printf("### Registro de Usuário ###\n\n");
    printf("Para iniciar, insira o CPF do usuário que deseja cadastrar:\n");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;  // Remove a nova linha capturada pelo fgets

    if (!confirmar_acao("continuar com o cadastro")) {
        printf("\nCadastro cancelado.\n");
        return 0;
    }

    // Copiando o CPF para o nome do arquivo
    strcpy(arquivo, cpf);

    // Criação do arquivo
    FILE *file = criar_arquivo(arquivo);
    if (file == NULL) return 1;  // Retorna se houver erro

    // Adiciona o CPF ao arquivo
    adicionar_dados_arquivo(file, cpf);
    fclose(file);

    // Entrada e registro do nome
    printf("Digite o primeiro nome do usuário:\n");
    limpar_buffer();  // Limpa o buffer antes de usar fgets
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;  // Remove a nova linha
    file = abrir_arquivo_append(arquivo);
    if (file == NULL) return 1;
    adicionar_dados_arquivo(file, nome);
    fclose(file);

    // Entrada e registro do sobrenome
    printf("Agora, digite o sobrenome do usuário:\n");
    fgets(sobrenome, sizeof(sobrenome), stdin);
    sobrenome[strcspn(sobrenome, "\n")] = 0;
    file = abrir_arquivo_append(arquivo);
    if (file == NULL) return 1;
    adicionar_dados_arquivo(file, sobrenome);
    fclose(file);

    // Entrada e registro do cargo
    printf("Por fim, digite o cargo do usuário:\n");
    fgets(cargo, sizeof(cargo), stdin);
    cargo[strcspn(cargo, "\n")] = 0;
    file = abrir_arquivo_append(arquivo);
    if (file == NULL) return 1;
    adicionar_dados_arquivo(file, cargo);
    fclose(file);

    printf("\nCadastro concluído com sucesso!\n");
    system("read -n1");  // Pausa no Mac

    return 0;
}

// Função para consultar os dados
int consulta() {
    setlocale(LC_ALL, "Portuguese");
    char cpf[40];
    char conteudo[200];

    printf("### Consulta de Cadastro ###\n\n");
    printf("Digite o CPF que deseja consultar:\n");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;

    FILE *file = fopen(cpf, "r");
    if (file == NULL) {
        printf("Erro: CPF não localizado. Verifique se o CPF está correto.\n");
        return 1;
    }

    printf("\n### Informações do Usuário ###\n");
    while (fgets(conteudo, sizeof(conteudo), file) != NULL) {
        printf("%s", conteudo);  // Exibe o conteúdo do arquivo
    }
    fclose(file);

    printf("\nConsulta finalizada.\n");
    system("read -p 'Pressione qualquer tecla para continuar...' var");
    return 0;
}

// Função para deletar dados com confirmação
int deletar() {
    char cpf[40];
    printf("### Deletar Cadastro ###\n\n");
    printf("Digite o CPF do registro que deseja deletar:\n");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;

    if (!confirmar_acao("deletar o registro")) {
        printf("\nAção cancelada.\n");
        return 0;
    }

    if (remove(cpf) == 0) {
        printf("Registro deletado com sucesso.\n");
    } else {
        printf("Erro ao tentar deletar o registro.\n");
    }

    system("read -n1");
    return 0;
}

// Função principal
int main() {
    int opcao = 0;

    while (1) {
        system("clear");
        setlocale(LC_ALL, "Portuguese");

        printf("### Cartório da EBAC ###\n\n");
        printf("Escolha uma opção do menu:\n");
        printf("\t1 - Registrar usuário\n");
        printf("\t2 - Consultar cadastro\n");
        printf("\t3 - Deletar cadastro\n");
        printf("\t4 - Sair\n");
        printf("Opção: ");

        while (scanf("%d", &opcao) != 1 || opcao < 1 || opcao > 4) {
            printf("Opção inválida. Por favor, selecione uma opção de 1 a 4: ");
            while (getchar() != '\n');  // Limpa o buffer de entrada
        }
        getchar();  // Limpa o buffer de entrada
        system("clear");

        switch (opcao) {
            case 1:
                registro();
                break;
            case 2:
                consulta();
                break;
            case 3:
                deletar();
                break;
            case 4:
                if (confirmar_acao("sair do sistema")) {
                    printf("Saindo do sistema...\n");
                    exit(0);
                } else {
                    printf("Ação cancelada. Voltando ao menu principal.\n");
                }
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                system("read -n1");
        }
    }

    return 0;
}
