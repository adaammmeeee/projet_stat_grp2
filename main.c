#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

struct individu
{
    char nom[20];
    int courage;
    int loyaute;
    int sagesse;
    int malice;
    char maison[20];
    int rep_initial;
};

// -- PIKK EUP
struct individu *pick_up()
{
    FILE *F = fopen("choixpeauMagique.csv", "r");
    if (!F)
    {
        perror("fopen");
        exit(1);
    }

    int caractere = 0;
    int ligne = 0;
    int cpt = 0;

    while ((caractere = fgetc(F)) != EOF)
    {
        if (ligne == 0)
            cpt++;
        if (caractere == '\n')
            ligne++;
    }
    fseek(F, cpt, SEEK_SET);

    struct individu *ind = malloc(ligne * sizeof(struct individu));
    for (int i = 0; i < ligne; i++)
    {
        fscanf(F, "%[^;];%d;%d;%d;%d;%[^;\n]", ind[i].nom, &ind[i].courage, &ind[i].loyaute, &ind[i].sagesse, &ind[i].malice, ind[i].maison);
        ind[i].rep_initial = 0;
    }
    fclose(F);
    return ind;
}

// -- REPRESENTATION INIIALE

void representation_initiale(struct individu *ind, int k)
{
    int cpt = 1;
    srand(time(NULL));
    for (int i = 0; i < k; i++)
    {
        int alea = rand() % 50;
        if (ind[alea].rep_initial == 0)
        {
            ind[alea].rep_initial = cpt;
            cpt ++;
        }
        else
        {
            while (ind[alea].rep_initial != 0)
            {
                alea = rand() % 50;
            }
            ind[alea].rep_initial = cpt;
            cpt ++;
        }
    }
}

// -- CALCUL ETENDU
int calcul_etendu_sagesse(struct individu a[])
{
    int max = 0;
    int min = 1000;
    for (int i = 0; i < 50; i++)
    {
        if (a[i].sagesse > max)
        {
            max = a[i].sagesse;
        }
        if (a[i].sagesse < min)
        {
            min = a[i].sagesse;
        }
    }

    return (max - min);
}

int calcul_etendu_loyaute(struct individu a[])
{
    int max = 0;
    int min = 1000;
    for (int i = 0; i < 50; i++)
    {
        if (a[i].loyaute > max)
        {
            max = a[i].loyaute;
        }
        if (a[i].loyaute < min)
        {
            min = a[i].loyaute;
        }
    }

    return (max - min);
}

int calcul_etendu_malice(struct individu a[])
{
    int max = 0;
    int min = 1000;
    for (int i = 0; i < 50; i++)
    {
        if (a[i].malice > max)
        {
            max = a[i].malice;
        }
        if (a[i].malice < min)
        {
            min = a[i].malice;
        }
    }

    return (max - min);
}

int calcul_etendu_courage(struct individu *a)
{
    int max = 0;
    int min = 1000;
    for (int i = 0; i < 49; i++)
    {
        if (a[i].courage > max)
        {
            max = a[i].courage;
        }
        if (a[i].courage < min)
        {
            min = a[i].courage;
        }
    }

    return (max - min);
}

// --  CALCUL DISTANCE

float calcul_distance_sagesse(struct individu a, struct individu b, int etendu)
{
    return abs((float)a.sagesse - (float)b.sagesse) / (float)etendu;
}

float calcul_distance_loyaute(struct individu a, struct individu b, int etendu)
{

    return abs((float)a.loyaute - (float)b.loyaute) / (float)etendu;
}

float calcul_distance_courage(struct individu a, struct individu b, int etendu)
{

    return abs((float)a.courage - (float)b.courage) / (float)etendu;
}

float calcul_distance_malice(struct individu a, struct individu b, int etendu)
{

    return abs((float)a.malice - (float)b.malice) / (float)etendu;
}

int calcul_distance_maison(struct individu a, struct individu b)
{

    if (strcmp(a.maison, b.maison))
    {
        return 1;
    }
    else
        return 0;
}

// --- CALCUL GOWER

float distance(struct individu a, struct individu b, struct individu ind[])
{
    float malice = calcul_distance_malice(a, b, calcul_etendu_malice(ind));
    float loyaute = calcul_distance_loyaute(a, b, calcul_etendu_loyaute(ind));
    float courage = calcul_distance_courage(a, b, calcul_etendu_courage(ind));
    float sagesse = calcul_distance_sagesse(a, b, calcul_etendu_sagesse(ind));
    float maison = (float)calcul_distance_maison(a, b);
    return ((malice + loyaute + courage + sagesse + maison) / (5.0));
}

// --- CALCUL MATRICE DISTANCE

float **matrice_distance(struct individu ind[])
{
    float **matrice = malloc(50 * sizeof(*matrice));
    for (int k = 0; k < 50; k++)
    {
        matrice[k] = malloc(50 * sizeof(**matrice));
    }

    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 50; j++)
        {
            matrice[i][j] = distance(ind[i], ind[j], ind);
        }
    }
    return matrice;
}

int main()
{

    struct individu *ind = pick_up();

    float **matrice = matrice_distance(ind);
    for (int i = 0; i < 50; i++)
    {
       printf("%d : %d\n",i, ind[i].rep_initial );
    }

    printf("--------------------------------------------------------\n");

    representation_initiale(ind, 10);

     for (int i = 0; i < 50; i++)
    {
       printf("%d : %d\n",i, ind[i].rep_initial );
    }


    for (int i = 0; i < 50; i++)
    {
        free(matrice[i]);
    }
    free(matrice);
    free(ind);
}
