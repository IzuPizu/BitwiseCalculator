#include <stdio.h>

// functie care extrage primii p biti incepand cu pozitia pos
int extraction(unsigned int nr, int p, int pos) {
    unsigned int right_shift = nr >> (pos - 1);
    unsigned new_num = ((1 << p) - 1);
    return new_num & right_shift;
}
// functie pentru adunare
unsigned int op_add(unsigned int nr1, unsigned int nr2) {
    unsigned carry;
    while (nr2 != 0) {
        carry = nr1 & nr2;
        nr1 = nr1 ^ nr2;
        nr2 = carry << 1;
    }
    return nr1;
}
// functie pentru interschimbare
unsigned int op_ichange(unsigned int nr1, unsigned int nr2) {
    unsigned int result;
    // extragem p1 si p2 din NR2
    unsigned p2 = extraction(nr2, 2, 1);
    unsigned p1 = extraction(nr2, 2, 3);
    // fortam numerotarea de la stanga la dreapta
    int final = 0;
    if (final != 1) {
        if ((p1 == 1 && p2 == 3) || (p1 == 3 && p2 == 1)) {
            p1 = 0;
            p2 = 2;
            final = 1;
        }
    }
    if (final != 1) {
        if ((p1 == 0 && p2 == 1) || (p1 == 1 && p2 == 0)) {
            p1 = 3;
            p2 = 2;
            final = 1;
        }
    }
    if (final != 1) {
        if ((p1 == 0 && p2 == 2) || (p1 == 2 && p2 == 0)) {
            p1 = 1;
            p2 = 3;
            final = 1;
        }
    }
    if (final != 1) {
        if ((p1 == 3 && p2 == 2) || (p1 == 2 && p2 == 3)) {
            p1 = 0;
            p2 = 1;
            final = 1;
        }
    }
    // interschimbam bitii de pe pozitiile p1 si p2 din NR1
    unsigned int first_bit = (nr1 >> p1) & 1;
    unsigned int second_bit = (nr1 >> p2) & 1;
    unsigned int x = (first_bit ^ second_bit);
    x = (x << p1) | (x << p2);
    result = nr1 ^ x;

    return result;
}
// functie pentru rotatie la stanga
unsigned int op_lshift(unsigned int nr1, unsigned int nr2) {
    unsigned int shift = nr2 % 4;
    unsigned int result;

    if (shift == 0) {
        return nr1;
    }
    result = (nr1 << shift) | (nr1 >> (4 - shift));
    unsigned int mask = (-1 >> (8 * sizeof(nr1) - 4));
    result = result & mask;
    return result;
}
// functie pentru xor
unsigned int op_xor(unsigned int nr1, unsigned int nr2) {
    return (nr1 & (~nr2)) | ((~nr1) & nr2);
}

int main() {
    // vector de pointeri la functii
    unsigned int (*funct_ptr[4])(unsigned int nr1, unsigned int nr2) = {
        op_add, op_ichange, op_lshift, op_xor}; 

    // numar de operatii;
    int N; 
    do {
        scanf("%d", &N);
    } while (N < 0 || N > 4);

    // numarul din care vor fi extrase operatiile si numerele
    unsigned int M;  
    scanf("%u", &M);

    unsigned int result;
    unsigned int operatie, nr1, nr2;
    // extragem N*6+4 biti de la LSB->MSB si i punem intr-un vector
    unsigned int v[N * 6 + 4];
    int k, i, j;
    for (k = 0, i = 0, j = 4; i < N * 6 + 4; i += 6, j += 6, k += 2) {
        // extragem cate 4 biti incepand de pe poz 0
        v[k] = extraction(M, 4, i + 1);  
         // extragem operatia
        v[k + 1] = extraction(M, 2, j + 1); 
    }

   // op_add =  0 , op_ichange = 1 , op_lshift = 2 , op_xor = 3
    k = 2 * N;
    nr1 = v[k];
    while (k > 0) {
        operatie = v[k - 1];
        nr2 = v[k - 2];
        if (operatie == 0) {  // adunare
            result = (*funct_ptr[0])(nr1, nr2);
            if (result > 15) {
                result = extraction(result, 4, 1);
            }
        }
        if (operatie == 1) {  // interschimbare
            result = (*funct_ptr[1])(nr1, nr2);
            if (result > 15) {
                result = extraction(result, 4, 1);
            }
        }
        if (operatie == 2) {  // rotatie la stanga
            result = (*funct_ptr[2])(nr1, nr2);
            if (result > 15) {
                result = extraction(result, 4, 1);
            }
        }
        if (operatie == 3) {  // xor
            result = (*funct_ptr[3])(nr1, nr2);
            if (result > 15) {
                result = extraction(result, 4, 1);
            }
        }
        nr1 = result;
        k = k - 2;
    }

    printf("%d", nr1);
    return 0;
}