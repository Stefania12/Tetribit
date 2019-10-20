// Copyright 2018 Damian Stefania-Dorina 312CD
#include <stdio.h>
#include <math.h>

// determinarea bitului de pe pozitia i
int bitul_i(unsigned long long n, int i) {
    unsigned long long mask = 1;
    mask = mask << i;

    if ((n & mask) == 0) {
        return 0;
    } else {
    return 1;
    }
}



// afisarea hartii cu # si . conform cerintei
void afisare_harta(unsigned long long h) {
    int i = 63, j, k;
    for (j = 1; j <= 8; j++) {
        for (k = 1; k <= 8; k++) {
            if (bitul_i(h, i) == 1) {
                printf("#");
            } else {
                printf(".");
            }

        i--;
        }

        printf("\n");
    }
}




// deplasam la stanga piesa avand ca reper linia inferioara
unsigned long long deplasare_stanga(unsigned long long h,
                                    unsigned long long piesa,
                                    unsigned long long piesa2, int distanta,
                                    int linie) {
    int bit1_l1 = -1, bit1_l2 = -1, i, d;

    // cautam pozitiile primilor biti de 1 de pe liniile l1 (sus) si l2 (jos)
    // ale piesei
    i = 15;
    while (bitul_i(piesa, i) == 0 && i >= 8) {
        i--;
    }

    if (i >= 8) {
        bit1_l1 = i - 8;
    }


    i = 7;
    while (bitul_i(piesa, i) == 0 && i >= 0) {
        i--;
    }

    if (i >= 0) {
        bit1_l2 = i;
    }


    // aflam cu cate pozitii se poate deplasa piesa daca linia ar fi goala
    if (bit1_l1 > bit1_l2) {
        d = 7 - bit1_l1;
    } else {
        d = 7 - bit1_l2;
    }

    if (d < distanta) {
        distanta = d;
    }



    // efectuam deplasarea la stanga a piesei
    int nr_deplasari = 0;
    piesa2 = piesa << (linie * 8);

    while (distanta != 0 && ((piesa2 << 1) & h) == 0) {
        nr_deplasari++;
        piesa2 = piesa2 << 1;
        distanta--;
    }

    piesa = piesa << nr_deplasari;
    return piesa;
}// deplasare_stanga


// deplasam la dreapta piesa avand ca reper linia inferioara
unsigned long long deplasare_dreapta(unsigned long long h,
                                    unsigned long long piesa,
                                    unsigned long long piesa2,
                                    int distanta, int linie) {
    int bit1_l1 = 16, bit1_l2 = 16, i, d;

    // cautam pozitiile ultimilor biti de 1 de pe liniile l1 (sus) si l2 (jos)
    // ale piesei
    i = 0;
    while (bitul_i(piesa, i) == 0 && i <= 7) {
        i++;
    }

    if (i <= 7) {
        bit1_l2 = i;
    }


    i = 8;
    while (bitul_i(piesa, i) == 0 && i <= 15) {
        i++;
    }

    if (i <= 15) {
        bit1_l1 = i - 8;
    }



    // aflam cu cate pozitii se poate deplasa piesa daca linia ar fi goala
    if (bit1_l1 < bit1_l2) {
        d = bit1_l1;
    } else {
        d = bit1_l2;
    }

    if (d < distanta) {
        distanta = d;
    }



    // efectuam deplasarea la dreapta a piesei
    int nr_deplasari = 0;
    piesa2 = piesa << (linie * 8);

    while (distanta != 0 && ((piesa2 >> 1) & h) == 0) {
        nr_deplasari++;
        piesa2 = piesa2 >> 1;
        distanta--;
    }

    piesa = piesa >> nr_deplasari;

    return piesa;
}// deplasare_dreapta



// verificam daca avem coliziune
int coliziune(unsigned long long h, unsigned long long piesa,
            unsigned long long piesa2, int linie) {
        piesa2 = piesa << (linie * 8);
        if ((h & piesa2) != 0) {
            return 1;
        } else {
            return 0;
        }
}


// coborarea piesei pe linia urmatoare
unsigned long long coborare(unsigned long long piesa,
                            unsigned long long piesa2, int linie) {
        piesa2 = piesa;
        piesa2 = piesa2 << (linie * 8);

        return piesa2;
}



// formam o masca la care inseram numai 1 pe o linie
unsigned long long adaugare_linie1(unsigned long long x, int linie) {
    unsigned long long m, n;
    m = 0;
    n = 1;
    int i;

    for (i = 0; i <= 7; i++) {
        m = m << 1;
        m = m | n;
    }

    m = m << (linie * 8);
    x = x | m;

    return x;
}

// verificam daca harta are o linie numai cu 1
int linie1(unsigned long long h) {
    int linie = -1, i, j, nr1;

    for (i = 0; i < 8 && linie == -1; i++) {
        nr1 = 0;
        for (j = 0; j < 8; j++) {
            if (bitul_i(h, i * 8 + j) == 1) {
                nr1++;
            }
        }

        if (nr1 == 8) {
            linie = i;
        }
    }

    return linie;
}



// eliminam linia din harta care contine numai 1
unsigned long long eliminare_linie(unsigned long long h, int linie) {
    unsigned long long mask1, mask2;
    int i;

    mask1 = 0;
    mask2 = 0;

    for (i = 0; i < linie; i++) {
        mask2 = adaugare_linie1(mask2, i);
    }


    for (i = linie + 1; i < 8; i++) {
        mask1 = adaugare_linie1(mask1, i);
    }


    mask2 = h & mask2;
    mask1 = h & mask1;
    mask1 = mask1 >> 8;

    h = mask1 | mask2;


    return h;
}



int main() {
    unsigned long long H, h, piesa_1, piesa_2 = 0;
    int M, transf, i, linie_de_1, completedLines = 0,
        zerosNumber = 0, coliz, endgame = 0;
    float score;


    scanf("%llu", &H);
    afisare_harta(H);

    scanf("%d", &M);

    while (M > 0 && endgame == 0) {
        M--;
        scanf("%llu", &piesa_1);

        endgame = 0;
        coliz = 0;


        // efectuam coborarile si transformarile piesei

        for (i = 7; i >= 0 && endgame == 0; i--) {
            scanf("%d", &transf);

            if (coliziune(H, piesa_1, piesa_2, i) == 1) {
                coliz = 1;
            }


            if (coliz == 0) {
                piesa_2 = coborare(piesa_1, piesa_2, i);

                if (transf < 0) {
                    piesa_1 = deplasare_stanga(H, piesa_1, piesa_2,
                                                transf * (-1), i);
                } else {
                    piesa_1 = deplasare_dreapta(H, piesa_1, piesa_2,
                                                transf, i);
                }

                piesa_2 = piesa_1 << (i * 8);
                h = H | piesa_2;
                printf("\n");
                afisare_harta(h);
            } else {
                /* verificam	daca este indeplinita conditia
                nr 2 de final de joc */

                piesa_2 = piesa_1 << (56);
                piesa_2 = piesa_2 >> (56);

                if (i == 7 || (i == 6 && piesa_1 != piesa_2)) {
                    endgame = 1;
                }
            }
        }// coborari si transformari

        H = h;


        if (endgame == 1) {
            printf("\n");
            afisare_harta(H);
        } else {
            // eliminare linii de 1

            linie_de_1 = linie1(H);

            if (linie_de_1 >= 0) {
                while (linie_de_1 >= 0) {
                    H = eliminare_linie(H, linie_de_1);
                    completedLines++;
                    linie_de_1 = linie1(H);
                }

                printf("\n");
                afisare_harta(H);
            }
        }
    }// while (M > 0)



    // numaram bitii de 0 ramasi in harta finala
    for (i = 0; i <= 63; i++) {
        if (bitul_i(H, i) == 0) {
            zerosNumber++;
        }
    }

    // scor rotunjit
    score = sqrt(zerosNumber) + pow(1.25, completedLines);
    score = round(score * 100) / 100;

    printf("\nGAME OVER!\nScore:%.2f", score);

    return 0;
}
