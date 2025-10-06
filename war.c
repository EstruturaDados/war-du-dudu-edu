// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define MAX_TERRITORIOS 6
#define MAX_NOME 30


const char *NOMES_TERRITORIOS[MAX_TERRITORIOS] = {
    "America", "Oceania", "Europa", "Asia",
    "Africa", "Antartida"
};


const char *CORES_POSSIVEIS[] = {
    "Vermelho", "Verde", "Azul", "Amarelo", "Ciano", "Preto"
};
const int NUM_CORES = sizeof(CORES_POSSIVEIS) / sizeof(CORES_POSSIVEIS[0]);
// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[MAX_NOME];
    char dono[MAX_NOME];   // "Jogador" ou "Inimigo"
    int tropas;
    char cor[MAX_NOME];    // cor textual atribuída aleatoriamente
} Territorio;

typedef struct {
    int idMissao;
    char descricao[200];
    int targetNumber;                 // número alvo (ex: 3 territórios / 12 tropas / etc)
    char targetColor[MAX_NOME];       // cor alvo (para missões de cor)
    char targetTerritorio[MAX_NOME];  // território alvo (para missões específicas)
} Missao;


// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:
void limparBufferEntrada(void);

Territorio* alocarMapa(void);
void liberarMemoria(Territorio* mapa);

void inicializarTerritorios(Territorio* mapa);

void exibirMenuPrincipal(void);
void exibirMapa(const Territorio* mapa);

Missao sortearMissao(void);
void exibirMissao(const Missao* m);
int verificarVitoria(const Territorio* mapa, const Missao* m);

void simularAtaque(Territorio* atacante, Territorio* defensor);
void faseDeAtaque(Territorio* mapa);

// Funções auxiliares internas
const char* gerarCorAleatoria(void);
Territorio* buscarTerritorioPorNome(Territorio* mapa, const char* nome);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.

int main(void) {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    
    srand((unsigned int) time(NULL));

    // alocar e inicializar mapa
    Territorio* mapa = alocarMapa();
    if (mapa == NULL) return 1;
    inicializarTerritorios(mapa);

    // sortear a primeira missão
    Missao missao = sortearMissao();

    int opcao = -1;
    do {
        exibirMenuPrincipal();
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            limparBufferEntrada();
            printf("Entrada invalida. Tente novamente.\n");
            opcao = -1;
            continue;
        }
        limparBufferEntrada();
// 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.



        switch (opcao) {
            case 1:
                exibirMapa(mapa);
                break;
            case 2:
                faseDeAtaque(mapa);
                break;
            case 3:
                exibirMissao(&missao);
                break;
            case 4:
                if (verificarVitoria(mapa, &missao))
                    printf("\n>>> Parabens! Missao cumprida! <<<\n");
                else
                    printf("\nMissao ainda nao cumprida. Continue jogando.\n");
                break;
            case 5:
                missao = sortearMissao();
                printf("Nova missao sorteada!\n");
                exibirMissao(&missao);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);
    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    liberarMemoria(mapa);
    return 0;
}

    



// --- Implementação das Funções ---
// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

Territorio* alocarMapa(void) {
    Territorio* mapa = (Territorio*) calloc(MAX_TERRITORIOS, sizeof(Territorio));
    if (mapa == NULL) {
        fprintf(stderr, "Erro: falha na alocacao de memoria para o mapa.\n");
        return NULL;
    }
    return mapa;
}

// liberarMemoria():
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

// gerarCorAleatoria(): retorna ponteiro para string literal de cor
const char* gerarCorAleatoria(void) {
    int idx = rand() % NUM_CORES;
    return CORES_POSSIVEIS[idx];
}

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void inicializarTerritorios(Territorio* mapa) {
    int contadorJogador = 0;
    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        strncpy(mapa[i].nome, NOMES_TERRITORIOS[i], MAX_NOME-1);
        mapa[i].nome[MAX_NOME-1] = '\0';

        // dono aleatório (chance ~50%): "Jogador" ou "Inimigo"
        if (rand() % 100 < 50) {
            strcpy(mapa[i].dono, "Jogador");
            contadorJogador++;
        } else {
            strcpy(mapa[i].dono, "Inimigo");
        }

        // tropas: aleatórias entre 1 e 8
        mapa[i].tropas = rand() % 8 + 1;

        // cor: aleatória (string copiada)
        const char* cor = gerarCorAleatoria();
        strncpy(mapa[i].cor, cor, MAX_NOME-1);
        mapa[i].cor[MAX_NOME-1] = '\0';
    }

    // garantir que o jogador tenha ao menos 1 território
    if (contadorJogador == 0) {
        strcpy(mapa[0].dono, "Jogador");
    }
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
void exibirMenuPrincipal(void) {
    printf("\n=== MENU PRINCIPAL ===\n");
    printf("1. Exibir mapa\n");
    printf("2. Atacar território\n");
    printf("3. Exibir missão atual\n");
    printf("4. Verificar vitória (status da missão)\n");
    printf("5. Sortear nova missão\n");
    printf("0. Sair\n");
    printf("======================\n");
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const Territorio* mapa) {
    printf("\n--- ESTADO ATUAL DO MAPA ---\n");
    printf("%-12s | %-9s | %-6s | %-8s\n", "Território", "Dono", "Tropas", "Cor");
    printf("------------------------------------------------\n");
    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        printf("%-12s | %-9s | %-6d | %-8s\n",
               mapa[i].nome, mapa[i].dono, mapa[i].tropas, mapa[i].cor);
    }
}

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.
Missao sortearMissao(void) {
    Missao m;
    memset(&m, 0, sizeof(Missao));
    m.idMissao = rand() % 5; // 0..4

    switch (m.idMissao) {
        case 0:
            m.targetNumber = 3;
            snprintf(m.descricao, sizeof(m.descricao),
                     "Conquistar %d territorios (posse total).", m.targetNumber);
            break;
        case 1:
            m.targetNumber = 12;
            snprintf(m.descricao, sizeof(m.descricao),
                     "Controlar pelo menos %d tropas no total.", m.targetNumber);
            break;
        case 2:
            snprintf(m.descricao, sizeof(m.descricao),
                     "Destruir todo o exército inimigo (nenhum territorio do inimigo).");
            break;
        case 3: {
            // missão: controlar um território específico (escolhido aleatoriamente)
            int idx = rand() % MAX_TERRITORIOS;
            strncpy(m.targetTerritorio, NOMES_TERRITORIOS[idx], MAX_NOME-1);
            m.targetTerritorio[MAX_NOME-1] = '\0';
            snprintf(m.descricao, sizeof(m.descricao),
                     "Conquistar o territorio '%s'.", m.targetTerritorio);
            break;
        }
        case 4: {
            // missão: controlar N territorios de cor X
            int colorIdx = rand() % NUM_CORES;
            int need = 2 + (rand() % 2); // 2 ou 3
            strncpy(m.targetColor, CORES_POSSIVEIS[colorIdx], MAX_NOME-1);
            m.targetColor[MAX_NOME-1] = '\0';
            m.targetNumber = need;
            snprintf(m.descricao, sizeof(m.descricao),
                     "Controlar %d territorios da cor %s.", m.targetNumber, m.targetColor);
            break;
        }
        default:
            strcpy(m.descricao, "Missao desconhecida.");
    }
    return m;
}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.
void exibirMissao(const Missao* m) {
    printf("\n--- MISSÃO ATUAL ---\n");
    printf("%s\n", m->descricao);

    // Dados extras (úteis para debug/visual)
    if (m->idMissao == 0 || m->idMissao == 1 || m->idMissao == 4) {
        if (m->targetNumber > 0)
            printf("Alvo numerico: %d\n", m->targetNumber);
    }
    if (m->idMissao == 3) {
        printf("Territorio alvo: %s\n", m->targetTerritorio);
    }
    if (m->idMissao == 4) {
        printf("Cor alvo: %s\n", m->targetColor);
    }
}


// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.
int verificarVitoria(const Territorio* mapa, const Missao* m) {
    if (m == NULL || mapa == NULL) return 0;

    switch (m->idMissao) {
        case 0: { // conquistar N territorios (posse)
            int cont = 0;
            for (int i = 0; i < MAX_TERRITORIOS; i++)
                if (strcmp(mapa[i].dono, "Jogador") == 0) cont++;
            return cont >= m->targetNumber;
        }
        case 1: { // controlar pelo menos X tropas no total
            int total = 0;
            for (int i = 0; i < MAX_TERRITORIOS; i++)
                if (strcmp(mapa[i].dono, "Jogador") == 0) total += mapa[i].tropas;
            return total >= m->targetNumber;
        }
        case 2: { // destruir todo o exército inimigo (nenhum territorio do inimigo)
            for (int i = 0; i < MAX_TERRITORIOS; i++)
                if (strcmp(mapa[i].dono, "Inimigo") == 0) return 0;
            return 1;
        }
        case 3: { // conquistar território específico
            for (int i = 0; i < MAX_TERRITORIOS; i++)
                if (strcmp(mapa[i].nome, m->targetTerritorio) == 0)
                    return strcmp(mapa[i].dono, "Jogador") == 0;
            return 0; // não encontrou o território (não deveria ocorrer)
        }
        case 4: { // controlar N territorios da cor X
            int cont = 0;
            for (int i = 0; i < MAX_TERRITORIOS; i++) {
                if (strcmp(mapa[i].dono, "Jogador") == 0 &&
                    strcmp(mapa[i].cor, m->targetColor) == 0) {
                    cont++;
                }
            }
            return cont >= m->targetNumber;
        }
        default:
            return 0;
    }
}

// buscarTerritorioPorNome(): retorna ponteiro (modificável) ou NULL
Territorio* buscarTerritorioPorNome(Territorio* mapa, const char* nome) {
    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        if (strcmp(mapa[i].nome, nome) == 0)
            return &mapa[i];
    }
    return NULL;
}

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void simularAtaque(Territorio* atacante, Territorio* defensor) {
    if (atacante == NULL || defensor == NULL) {
        printf("Erro: territorios invalidos.\n");
        return;
    }
    if (strcmp(atacante->dono, defensor->dono) == 0) {
        printf("Ataque invalido: mesmo dono.\n");
        return;
    }
    if (atacante->tropas < 2) {
        printf("Ataque invalido: territorio atacante precisa ter ao menos 2 tropas.\n");
        return;
    }

    printf("\n--- BATALHA ---\n");
    printf("%s (%d tropas) ataca %s (%d tropas)\n",
           atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);

    int dadoA = rand() % 6 + 1;
    int dadoD = rand() % 6 + 1;
    printf("Dado atacante: %d | Dado defensor: %d\n", dadoA, dadoD);

    if (dadoA > dadoD) {
        // defensor perde 1 tropa
        defensor->tropas -= 1;
        printf("Resultado: defensor perde 1 tropa (agora %d).\n", defensor->tropas);
        if (defensor->tropas <= 0) {
            // conquista:
            printf("Território %s conquistado por %s!\n", defensor->nome, atacante->dono);
            strncpy(defensor->dono, atacante->dono, MAX_NOME-1);
            defensor->dono[MAX_NOME-1] = '\0';
            // mover 1 tropa do atacante para o novo território (padrão simples)
            defensor->tropas = 1;
            atacante->tropas -= 1;
        }
    } else {
        // atacante perde 1 tropa
        atacante->tropas -= 1;
        printf("Resultado: atacante perde 1 tropa (agora %d).\n", atacante->tropas);
    }
}

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
void faseDeAtaque(Territorio* mapa) {
    char origem[MAX_NOME];
    char destino[MAX_NOME];

    printf("\nDigite o nome do territorio de origem: ");
    if (scanf("%29s", origem) != 1) {
        limparBufferEntrada();
        printf("Entrada invalida.\n");
        return;
    }
    printf("Digite o nome do territorio de destino: ");
    if (scanf("%29s", destino) != 1) {
        limparBufferEntrada();
        printf("Entrada invalida.\n");
        return;
    }
    limparBufferEntrada();

    Territorio* tOrigem = buscarTerritorioPorNome(mapa, origem);
    Territorio* tDestino = buscarTerritorioPorNome(mapa, destino);

    if (tOrigem == NULL || tDestino == NULL) {
        printf("Um ou ambos os territorios nao encontrados.\n");
        return;
    }

    simularAtaque(tOrigem, tDestino);
}


















