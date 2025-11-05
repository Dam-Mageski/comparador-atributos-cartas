#include <stdio.h>
#include <string.h>

#define MAX_STR 64

typedef struct {
    char estado[MAX_STR];
    char codigo[MAX_STR];
    char nome[MAX_STR];

    unsigned long populacao; // grande
    double area;             // km²
    double pib;              // mesma unidade nas duas cartas
    int pontos;              // pontos turísticos

    // derivados
    double densidade;        // hab/km²
    double pib_percapita;    // pib / populacao
    float super_poder;       // opcional: maior vence
} Carta;

// ----------------- Cálculos derivados -----------------
static void calcular_derivados(Carta *c) {
    c->densidade = (c->area > 0.0) ? (double)c->populacao / c->area : 0.0;
    c->pib_percapita = (c->populacao > 0UL) ? c->pib / (double)c->populacao : 0.0;

    double inv_dens = (c->densidade > 0.0) ? 1.0 / c->densidade : 0.0;
    double soma = (double)c->populacao + c->area + c->pib
                + (double)c->pontos + c->pib_percapita + inv_dens;
    c->super_poder = (float)soma;
}

// ----------------- Enum de atributos ------------------
typedef enum {
    A_POP = 1,
    A_AREA,
    A_PIB,
    A_PONTOS,
    A_DENS,
    A_PIBPC,
    A_SUPERPODER
} Atributo;

// nome do atributo para prints
const char* nome_attr(int a) {
    switch (a) {
        case A_POP:       return "Populacao";
        case A_AREA:      return "Area";
        case A_PIB:       return "PIB";
        case A_PONTOS:    return "Pontos Turisticos";
        case A_DENS:      return "Densidade Demografica";
        case A_PIBPC:     return "PIB per Capita";
        case A_SUPERPODER:return "Super Poder";
        default:          return "Atributo Invalido";
    }
}

// pega o valor do atributo como double para somar/comparar
double valor_attr(const Carta* c, int a) {
    switch (a) {
        case A_POP:        return (double)c->populacao;
        case A_AREA:       return c->area;
        case A_PIB:        return c->pib;
        case A_PONTOS:     return (double)c->pontos;
        case A_DENS:       return c->densidade;
        case A_PIBPC:      return c->pib_percapita;
        case A_SUPERPODER: return (double)c->super_poder;
        default:           return 0.0;
    }
}

// regra: para TODOS os atributos MAIOR vence, EXCETO DENSIDADE (menor vence)
int carta1_vence_attr(double v1, double v2, int atributo) {
    return (atributo == A_DENS) ? (v1 < v2) : (v1 > v2);
}

// ----------------- Menus dinâmicos --------------------
int menu_primeiro_atributo(void) {
    int op = 0;
    while (1) {
        printf("\nEscolha o PRIMEIRO atributo:\n");
        printf("1) Populacao\n2) Area\n3) PIB\n4) Pontos Turisticos\n");
        printf("5) Densidade Demografica (menor vence)\n6) PIB per Capita\n");
        printf("7) Super Poder\n> ");
        if (scanf("%d", &op) != 1) { // entrada inválida
            while (getchar() != '\n'); // limpa buffer
            printf("Opcao invalida. Tente novamente.\n");
            continue;
        }
        switch (op) {
            case A_POP: case A_AREA: case A_PIB: case A_PONTOS:
            case A_DENS: case A_PIBPC: case A_SUPERPODER:
                return op;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    }
}

int menu_segundo_atributo(int escolhido1) {
    int op = 0;
    while (1) {
        printf("\nEscolha o SEGUNDO atributo (diferente do primeiro):\n");
        for (int a = A_POP; a <= A_SUPERPODER; a++) {
            if (a == escolhido1) continue; // dinâmico: esconde o já escolhido
            printf("%d) %s\n", a, nome_attr(a));
        }
        printf("> ");
        if (scanf("%d", &op) != 1) {
            while (getchar() != '\n');
            printf("Opcao invalida. Tente novamente.\n");
            continue;
        }
        // valida: precisa ser um atributo valido e diferente do primeiro
        switch (op) {
            case A_POP: case A_AREA: case A_PIB: case A_PONTOS:
            case A_DENS: case A_PIBPC: case A_SUPERPODER:
                if (op == escolhido1) {
                    printf("Nao pode repetir o mesmo atributo. Escolha outro.\n");
                } else {
                    return op;
                }
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    }
}

// ----------------- Impressão dos resultados -----------------
void imprime_resultados(const Carta *c1, const Carta *c2, int a1, int a2) {
    double v1a = valor_attr(c1, a1), v2a = valor_attr(c2, a1);
    double v1b = valor_attr(c1, a2), v2b = valor_attr(c2, a2);

    int rA = carta1_vence_attr(v1a, v2a, a1);
    int rB = carta1_vence_attr(v1b, v2b, a2);

    double soma1 = v1a + v1b;
    double soma2 = v2a + v2b;

    // vencedor por soma usando operador ternário
    const char* vencedorSoma =
        (soma1 > soma2) ? "Carta 1 venceu"
                        : (soma2 > soma1) ? "Carta 2 venceu"
                                          : "Empate!";

    printf("\n================= RESULTADO =================\n");
    printf("Carta 1: %s\nCarta 2: %s\n\n", c1->nome, c2->nome);

    printf("Atributo 1: %s\n", nome_attr(a1));
    printf(" - %s: %.6f\n - %s: %.6f\n", c1->nome, v1a, c2->nome, v2a);
    printf(" -> %s\n\n", rA ? "Carta 1 venceu" : "Carta 2 venceu");

    printf("Atributo 2: %s\n", nome_attr(a2));
    printf(" - %s: %.6f\n - %s: %.6f\n", c1->nome, v1b, c2->nome, v2b);
    printf(" -> %s\n\n", rB ? "Carta 1 venceu" : "Carta 2 venceu");

    printf("SOMA dos atributos:\n");
    printf(" - %s: %.6f\n - %s: %.6f\n", c1->nome, soma1, c2->nome, soma2);
    printf(" => %s\n", vencedorSoma);
    printf("=============================================\n");
}

// ----------------- Programa principal -----------------
int main(void) {
    // Cartas pré-cadastradas (exemplo)
    Carta c1 = { "BR", "B01", "Brasil", 214000000UL, 8515767.0, 1.92e12, 45 };
    Carta c2 = { "AR", "A01", "Argentina", 45380000UL, 2780400.0, 4.88e11, 28 };

    calcular_derivados(&c1);
    calcular_derivados(&c2);

    printf("Comparador de Cartas – Escolha 2 atributos diferentes.\n");
    int primeiro = menu_primeiro_atributo();
    int segundo  = menu_segundo_atributo(primeiro);

    imprime_resultados(&c1, &c2, primeiro, segundo);
    return 0;
}
