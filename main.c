#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

struct individu
{
    char nom[20];
    int courage;
    int loyaute;
    int sagesse;
    int malice;
    char maison[20];
    int appartenance;
    int representant;
};

struct individurep
{
    char nom[20];
    int courage;
    int loyaute;
    int sagesse;
    int malice;
    char maison[20];
    int appartenance;
    int indice;
};

// -- Initialisation de la struct à partir du fichier choixpeauMagique
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
        ind[i].appartenance = 0;
        ind[i].representant = 0;
    }
    fclose(F);
    return ind;
}

// -- Initalisation des représentant de chaque cluster, ils sont choisis aléatoirement
struct individurep *representation_initiale(struct individu *ind, int k)
{
    struct individurep *ind_rep = malloc(k * sizeof(struct individurep));

    int cpt = 1;
    srand(getpid());
    for (int i = 0; i < k; i++)
    {
        int alea = rand() % 50;
        if (ind[alea].representant == 0)
        {
            ind[alea].appartenance = cpt;
            ind[alea].representant = 1;

            ind_rep[i].loyaute = ind[alea].loyaute;
            ind_rep[i].sagesse = ind[alea].sagesse;
            ind_rep[i].appartenance = ind[alea].appartenance;
            ind_rep[i].indice = alea;
            ind_rep[i].malice = ind[alea].malice;
            strcpy(ind_rep[i].maison, ind[alea].maison);

            cpt++;
        }
        else
        {
            while (ind[alea].representant != 0)
            {
                alea = rand() % 50;
            }
            ind[alea].appartenance = cpt;
            ind[alea].representant = 1;
            ind_rep[i].loyaute = ind[alea].loyaute;
            ind_rep[i].sagesse = ind[alea].sagesse;
            ind_rep[i].appartenance = ind[alea].appartenance;

            ind_rep[i].indice = alea;
            ind_rep[i].malice = ind[alea].malice;
            strcpy(ind_rep[i].maison, ind[alea].maison);

            cpt++;
        }
    }

    return ind_rep;
}

// -- ASSIGNATION AU PLUS PROCHE

void assignation(struct individu *ind, struct individurep *ind_rep, float **matrice_distance, int nb_individu, int nb_cluster)
{
    float buffer = 40.0;
    // Pour chaque individu
    for (int i = 0; i < 50; i++)
    {
        int cluster = 0;
        // Pour chaque cluster
        for (int j = 0; j < nb_cluster; j++)
        {
            // On test chaque distance vers un cluster pour trouver la plus petite
            if (buffer > matrice_distance[i][ind_rep[j].indice])
            {
                // On garde la meilleure valeur
                buffer = matrice_distance[i][ind_rep[j].indice];
                // On garde la valeur du meilleur cluster
                cluster = ind_rep[j].appartenance;
            }
        }
        buffer = 40.0;           // On change d'individu, on réinitialise le buffer
        ind[i].appartenance = cluster; // On assigne l'appartenance d'un individu au meilleur cluster
    }
}

float calcul_cout_cluster_k(struct individu *ind, struct individurep ind_rep, float **matrice)
{
    float buffer = 0;
    for (int i = 0; i < 50; i++)
    {
        if (ind[i].appartenance == ind_rep.appartenance)
        {
            buffer += matrice[ind_rep.indice][i];
        }
    }

    return buffer;
}

float calcul_cout(struct individu *ind, struct individurep *ind_rep, float **matrice, int nbcluster)
{
    float buffer = 0.0;
    for (int i = 0; i < nbcluster; i++)
    {
        buffer += calcul_cout_cluster_k(ind, ind_rep[i], matrice);
    }
    return buffer;
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
    for (int i = 0; i < 50; i++)
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

// ----CALCUL DISTANCE Manhattan
float distance_m(struct individu a, struct individu b)
{

    float r;
    r = abs(b.courage - a.courage) + abs(b.loyaute - a.loyaute) + abs(b.malice - a.malice) + abs(b.sagesse - a.sagesse);

    return r;
}

// --- CALCUL MATRICE DISTANCE Manhattan

float **matrice_distance_m(struct individu ind[])
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
            if (i == j)
            {
                matrice[i][j] = 0;
            }
            else
            {
                matrice[i][j] = distance_m(ind[i], ind[j]);
            }
        }
    }
    return matrice;
}

// --- ECHANGE un individu i avec un individu representatif j

int echange(struct individu *ind, struct individurep *ind_rep, float **matrice, int nbcluster)
{
    // Gain est un booléen, il vaut 0 ou 1 en fonction de si une meilleure valeur a été trouvé.
    int gain = 0;
    int gain_cout = 0;
    struct individurep old_representant;
    struct individu new_representant;
    int indice_new_representant = 0;
    int indice_old_representant = 0;
    int cluster = 0;
    // E est le cout actuel du partitionnement
    float E = calcul_cout(ind, ind_rep, matrice, nbcluster);
    //pour chaque cluster
    for (int i = 0; i < nbcluster; i++)
    {
        

        // pour chaque individu
        for (int j = 0; j < 50; j++)
        {
            // non représentant
            if (ind[j].representant == 0)
            {
                struct individu copy = ind[j];
                struct individurep copy_rep = ind_rep[i];
                // On remplace temporairement l'individu représentant du cluster i par l'individu j
                ind_rep[i].indice = j;
                ind[j].representant = 1;
                ind[copy_rep.indice].representant = 0;

                // Cout du partitionnement si l'individu j devient le représentant du cluster i
                float S = calcul_cout(ind, ind_rep, matrice, nbcluster);

                if (E - S > gain_cout)
                {
                    printf("Cout du clusters: %f \n", S);
                    gain = 1;
                    gain_cout = E - S;
                    new_representant = ind[j];
                    old_representant = ind_rep[i];
                    indice_new_representant = j;
                    indice_old_representant = i;
                    cluster = old_representant.appartenance;
                    printf("nouveau représenant \n");
                }

                // On réassigne l'individu représentant comme au début
                ind_rep[i].indice = copy_rep.indice;
                ind[copy_rep.indice].representant = 1;
                ind[j].representant = 0;
                ind[j].appartenance = copy.appartenance;
            }
        }
    }
    // On a traité tout les individus et tous les clusters, on peut maintenant procéder à l'échange
    if (gain)
    {

        // Le représentant n'est plus
        ind[indice_old_representant].representant = 0;
        ind[indice_new_representant].representant = 1;
        ind[indice_new_representant].appartenance = cluster;
        // On assigne le nouveau représentant
        ind_rep[indice_old_representant].indice = indice_new_representant;
        printf("FIN ECHANGE\n");
    }
    assignation(ind, ind_rep, matrice, 50, nbcluster);
    // On réassigne avec notre nouveau représentant (ou pas nouveaux si fin d'algo)
    return 0;
}

void test_representation(struct individu *ind)
{
    int cpt = 0;
    printf("--------------------------------------------------------\n");
    for (int i = 0; i < 50; i++)
    {
        printf("Individu %3d  représentant : %d   \n", i, ind[i].representant);
        if (ind[i].appartenance != 0)
        {
            cpt++;
        }
    }
    printf("On a donc %2d individus représentatif au début \n", cpt);
    printf("--------------------------------------------------------\n");
}

void test_representation_graine(struct individurep *ind_rep, int nbcluster)
{
    printf("--------------------------------------------------------\n");
    for (int i = 0; i < nbcluster; i++)
    {
        printf("Graine %3d  représentant : %d indice : %d \n", i, ind_rep[i].appartenance, ind_rep[i].indice);
    }
    printf("--------------------------------------------------------\n");
}

void test_cluster(struct individu *ind, struct individurep *ind_rep, int nbcluster)
{
    int cpt = 0;
    for (int i = 0; i < nbcluster; i++)
    {
        for (int j = 0; j < 50; j++)
        {
            if (ind_rep[i].appartenance == ind[j].appartenance)
            {
                printf("Cluster %d : individu %d\n", ind_rep[i].appartenance, j);
                cpt++;
            }
        }
    }
    printf("Individus total : %d\n", cpt);
}

int main()
{
    int n = 4;
    struct individu *ind = pick_up();

    float **matrice = matrice_distance_m(ind);                     // OK
    struct individurep *ind_rep = representation_initiale(ind, n); // OK
    assignation(ind, ind_rep, matrice, 50, n);                     // OK
    float cout_total = calcul_cout(ind, ind_rep, matrice, n);
    printf("Voici le cout total : %f\n", cout_total);

    float cout_total_ancien = 0.0;
    int cpt = 0;
    cout_total = calcul_cout(ind, ind_rep, matrice, n);

    test_cluster(ind, ind_rep, n);

    while (cpt < 10)
    {
        echange(ind, ind_rep, matrice, n);
        cpt++;
    }
    //test_representation_graine(ind_rep,n);
    printf("--------------------------------\n");
    printf("Cout initial     : %f\n", cout_total);
    printf("===>\n");
    printf("Cout total final : %f \n", calcul_cout(ind, ind_rep, matrice, n));
    test_representation_graine(ind_rep,n);
    for (int i = 0; i < 50; i++)
    {
        free(matrice[i]);
    }

    free(matrice);
    free(ind);
    free(ind_rep);
}
