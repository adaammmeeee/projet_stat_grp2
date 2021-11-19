#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

struct individu
{
    char nom[20];
    int courage;
    int loyaute;
    int sagesse;
    int malice;
    char maison[20];
};

int main()
{
    FILE *F = fopen("choixpeauMagique.csv", "r");
    if (!F)
    {
        perror("fopen");
        exit(1);
    } 

    struct stat sb;
    if (stat("choixpeauMagique.csv", &sb) == -1)
    {
        perror("choixpeauMagique");
        exit(EXIT_FAILURE);
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
    ligne--;
    fseek(F, cpt, SEEK_SET);

    struct individu ind[ligne];

    int indice = 0;
    int info = 1;
    int recule = 0;
    int indice_individu = 0;
    int parcours = 1;
    int a;
    for (int i=0; i<ligne-1; i++)
    {
    fscanf(F, "%[^;];%d;%d;%d;%d;%[^;\n]", ind[i].nom, &ind[i].courage, &ind[i].loyaute, &ind[i].sagesse, &ind[i].malice, ind[i].maison);
    }

    printf("%s\n", ind[1].nom);
    printf("%d\n", ind[1].courage);
    printf("%d\n", ind[1].loyaute);
    printf("%d\n", ind[1].sagesse);
    printf("%d\n", ind[1].malice);
    printf("%s\n", ind[1].maison);

    fclose(F);
}