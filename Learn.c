#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int Position;
typedef struct {
    int coef;
    int expon;
}Element;
typedef Element* ElementType;
typedef struct PolyNode* Polynomial;
struct PolyNode {
    ElementType Data;
    Position Last;
};
Polynomial MakeEmpty_PolyNode(void)
{
    Polynomial P;

    P = (Polynomial)malloc(sizeof(struct PolyNode));
    if (P != NULL)
        P->Last = -1;

    return P;
}
void Read_Polynomial(Polynomial P) {
    Position N;
    scanf("%d", &N);
    if (P != NULL)P->Last = N - 1;
    else return;
    if (N == 0) {
        N = 1; P->Last = 0;
        P->Data = (ElementType)malloc(sizeof(Element) * N);
        P->Data[P->Last].coef = 0; P->Data[P->Last].expon = 0;
        return;

    }
    P->Data = (ElementType)malloc(sizeof(Element) * N);
    if (P == NULL)return;
    for (int i = 0; i < N; i++)
        scanf("%d %d", &(P->Data[i].coef), &(P->Data[i].expon));
    return;
}
void print_Polynomial(Polynomial P) {
    Position i;
    if (P == NULL)return;
    if (P->Last == -1) { P->Last = 0; printf("0 0"); return; }
    for (i = 0; i < P->Last; i++)
        printf("%d %d ", P->Data[i].coef, P->Data[i].expon);
    printf("%d %d", P->Data[P->Last].coef, P->Data[P->Last].expon);
    return;
}

Polynomial Polynomial_Merge(Polynomial Poly_A, Polynomial Poly_B) {
    Polynomial Poly_Sum;

    if (Poly_A == NULL || Poly_B == NULL)return NULL;
    Poly_Sum = MakeEmpty_PolyNode();
    Poly_Sum->Data = (ElementType)malloc(sizeof(Element) * (Poly_A->Last + Poly_B->Last + 2));
    if (Poly_Sum->Data == NULL)return NULL;
    int A = 0, B = 0;
    if (Poly_A->Data->coef == 0)A++;
    if (Poly_B->Data->coef == 0)B++;
    for (; A <= Poly_A->Last && B <= Poly_B->Last;) {
        if (Poly_A->Data[A].expon > Poly_B->Data[B].expon) {
            Poly_Sum->Last++;
            Poly_Sum->Data[Poly_Sum->Last].expon = Poly_A->Data[A].expon;
            Poly_Sum->Data[Poly_Sum->Last].coef = Poly_A->Data[A].coef;
            A++;
        }
        else if (Poly_A->Data[A].expon == Poly_B->Data[B].expon) {
            Poly_Sum->Last++;
            Poly_Sum->Data[Poly_Sum->Last].expon = Poly_A->Data[A].expon;
            Poly_Sum->Data[Poly_Sum->Last].coef = Poly_A->Data[A].coef + Poly_B->Data[B].coef;
            if (Poly_Sum->Data[Poly_Sum->Last].coef == 0) {
                Poly_Sum->Last--;
            }
            A++; B++;
        }
        else {
            Poly_Sum->Last++;
            Poly_Sum->Data[Poly_Sum->Last].expon = Poly_B->Data[B].expon;
            Poly_Sum->Data[Poly_Sum->Last].coef = Poly_B->Data[B].coef;
            B++;
        }
    }
    if (A > Poly_A->Last) { //说明多项式A已经计算完毕，只需要把多项式B接在多项式Sum的末尾即可
        memcpy(&Poly_Sum->Data[Poly_Sum->Last + 1], &Poly_B->Data[B], (Poly_B->Last - B + 1) * sizeof(Element));
        Poly_Sum->Last += (Poly_B->Last - B + 1);
    }
    else if (B > Poly_B->Last) {
        memcpy(&Poly_Sum->Data[Poly_Sum->Last + 1], &Poly_A->Data[A], (Poly_A->Last - A + 1) * sizeof(Element));
        Poly_Sum->Last += (Poly_A->Last - A + 1);
    }
    if (A > Poly_A->Last && B > Poly_B->Last && Poly_A->Data[0].coef == 0 && Poly_B->Data[0].coef == 0) {
        Poly_Sum->Last = 0; Poly_Sum->Data[0].coef = 0; Poly_Sum->Data[0].expon = 0;
    }
    int temp_data_size = Poly_Sum->Last + 1;
    Poly_Sum->Data = realloc(Poly_Sum->Data, temp_data_size * sizeof(Element));
    if (Poly_Sum == NULL) {
        return NULL;
    }
    return Poly_Sum;
}
Polynomial Polynomial_Product(Polynomial Poly_A, Polynomial Poly_B) {
    Polynomial Poly_Product, Poly_temp, Poly_temp_for_free;

    if (Poly_A == NULL || Poly_B == NULL)return NULL;
    Poly_Product = MakeEmpty_PolyNode();

    Poly_Product->Data = (ElementType)malloc(sizeof(Element) * (Poly_A->Last + 1) * (Poly_B->Last + 1));
    if (Poly_Product == NULL || Poly_Product->Data == NULL)
        return NULL;

    if (Poly_A->Data->coef == 0 || Poly_B->Data->coef == 0) {
        Poly_Product->Last = 0;
        //
        Poly_Product->Data[0].coef = 0;
        Poly_Product->Data[0].expon = 0;

        return Poly_Product;
    }
    Poly_temp = MakeEmpty_PolyNode();    //存放一次相乘的结果
    Poly_temp->Data = (ElementType)malloc(sizeof(Element) * (Poly_B->Last + 1));
    Poly_temp->Last = Poly_B->Last;
    if (Poly_temp == NULL || Poly_temp->Data == NULL)
        return NULL;
    int A = 0, B = 0;
    for (; A <= Poly_A->Last; A++) {
        for (B = 0; B <= Poly_B->Last; B++) {
            Poly_temp->Data[B].coef = Poly_A->Data[A].coef * Poly_B->Data[B].coef;
            Poly_temp->Data[B].expon = Poly_A->Data[A].expon + Poly_B->Data[B].expon;

        }
        Poly_temp_for_free = Poly_Product;
        Poly_Product = Polynomial_Merge(Poly_Product, Poly_temp);
        free(Poly_temp_for_free->Data); free(Poly_temp_for_free);

    }
    free(Poly_temp->Data); Poly_temp->Data = NULL;
    free(Poly_temp); Poly_temp = NULL;
    Poly_Product->Data = realloc(Poly_Product->Data, (Poly_Product->Last + 1) * sizeof(Element));
    return Poly_Product;
}

int main()
{
    Polynomial Poly_A = NULL, Poly_B = NULL;
    Poly_A = MakeEmpty_PolyNode();
    Poly_B = MakeEmpty_PolyNode();
    Read_Polynomial(Poly_A);
    Read_Polynomial(Poly_B);
    print_Polynomial(Polynomial_Product(Poly_A, Poly_B)); printf("\n");
    print_Polynomial(Polynomial_Merge(Poly_A, Poly_B));


}
