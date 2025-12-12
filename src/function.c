#include <stdio.h>

int lireEntier(const char *s, int *i) {
    int val = 0;
    while (s[*i] >= '0' && s[*i] <= '9') {
        val = val * 10 + (s[*i] - '0');
        (*i)++;
    }
    return val;
}


void lireChamp(const char *s, int *i, char *dest) {
    int k = 0;
    while (s[*i] != ',' && s[*i] != '\0') {
        dest[k++] = s[*i];
        (*i)++;
    }
    dest[k] = '\0';
    if (s[*i] == ',') (*i)++; // passer la virgule
}


Trajet lireTrajet(const char *ligne) {
    Trajet t;
    int i = 0;

    // numéro bus
    t.numBus = lireEntier(ligne, &i);
    i++; // virgule

    // ville départ
    lireChamp(ligne, &i, t.villeDepart);

    // ville arrivée
    lireChamp(ligne, &i, t.villeArrivee);

    // date JJ/MM/AAAA
    t.jour = lireEntier(ligne, &i); i++;
    t.mois = lireEntier(ligne, &i); i++;
    t.annee = lireEntier(ligne, &i); i++; // virgule

    // horaire départ
    t.heureDepart = lireEntier(ligne, &i);
    i++; // virgule

    // horaire arrivée
    t.heureArrivee = lireEntier(ligne, &i);
    // Le reste de la ligne = passagers (ignoré ici)

    return t;
}

int chargerTrajets(const char *nom, Trajet tab[], int max) {
    FILE *f = fopen(nom, "r");
    if (!f) return 0;

    char ligne[500];
    int n = 0;

    while (fgets(ligne, 500, f) && n < max) {
        // enlever \n
        int len = 0;
        while (ligne[len] != '\0') {
            if (ligne[len] == '\n') ligne[len] = '\0';
            len++;
        }

        tab[n] = lireTrajet(ligne);
        n++;
    }

    fclose(f);
    return n;
}


void afficherTrajet(const Trajet *t) {
    printf(
        "Bus %d : %s -> %s | %02d/%02d/%04d | %04d -> %04d\n",
        (*t).numBus,
        (*t).villeDepart,
        (*t).villeArrivee,
        (*t).jour, (*t).mois, (*t).annee,
        (*t).heureDepart,
        (*t).heureArrivee
    );
}


void afficherTous(const Trajet tab[], int n) {
    for (int i = 0; i < n; i++) {
        printf("[%d] ", i + 1);
        afficherTrajet(&tab[i]);
    }
}


// fin de la question 1


int trouverTrajet(int numBus, const Trajet tab[], int n) {
    int i = 0;
    while (i < n) {
        if (tab[i].numBus == numBus) {
            return i;  // trouvé
        }
        i++;
    }
    return -1;  // pas trouvé
}


void afficherTrajetDepuisNumBus(int numBus, const Trajet tab[], int n) {
    int index = trouverTrajet(numBus, tab, n);

    if (index == -1) {
        printf("Aucun trajet avec le bus %d.\n", numBus);
    } else {
        afficherTrajet(&tab[index]);
    }
}


// question 3

int comparerChaines(const char *a, const char *b) {
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] < b[i])
            return -1;
        if (a[i] > b[i])
            return 1;
        i++;
    }

    // Si une chaîne est plus courte
    if (a[i] == '\0' && b[i] != '\0')
        return -1;
    if (a[i] != '\0' && b[i] == '\0')
        return 1;

    return 0;
}


int comparerDates(const Trajet *t1, const Trajet *t2) {
    if ((*t1).annee < (*t2).annee)
        return -1;
    if ((*t1).annee > (*t2).annee)
        return 1;

    if ((*t1).mois < (*t2).mois)
        return -1;
    if ((*t1).mois > (*t2).mois)
        return 1;

    if ((*t1).jour < (*t2).jour)
        return -1;
    if ((*t1).jour > (*t2).jour)
        return 1;

    return 0;
}

int comparerTrajets(const Trajet *t1, const Trajet *t2) {
    int cmp = comparerChaines((*t1).villeDepart, (*t2).villeDepart);

    if (cmp != 0) return cmp;

    return comparerDates(t1, t2);
}

void trierTrajets(Trajet tab[], int n) {
    int i = 0;

    while (i < n - 1) {
        int min = i;
        int j = i + 1;

        while (j < n) {
            if (comparerTrajets(&tab[j], &tab[min]) < 0) {
                min = j;
            }
            j++;
        }

        if (min != i) {
            Trajet temp = tab[i];
            tab[i] = tab[min];
            tab[min] = temp;
        }

        i++;
    }
}

void afficherTrajetsTries(Trajet tab[], int n) {
    trierTrajets(tab, n);

    printf("VOILA TRAJETS TRIES PAR DATE\n");

    int i = 0;
    while (i < n) {
        afficherTrajet(&tab[i]);
        i++;
    }
}

// question 4

int convertirEntier(const char *s) {
    int n = 0;
    int i = 0;

    while (s[i] >= '0' && s[i] <= '9') {
        n = n * 10 + (s[i] - '0');
        i++;
    }

    return n;
}

float convertirFloat(const char *s) {
    int i = 0;
    float resultat = 0.0;
    float puissance = 0.1;
    int partieDec = 0;

    // partie entière
    while (s[i] >= '0' && s[i] <= '9') {
        resultat = resultat * 10 + (s[i] - '0');
        i++;
    }

    // partie décimale
    if (s[i] == '.') {
        i++;
        while (s[i] >= '0' && s[i] <= '9') {
            resultat = resultat + (s[i] - '0') * puissance;
            puissance *= 0.1;
            i++;
        }
    }

    return resultat;
}

void extraireChamp(const char *ligne, char *champ, int *pos) {
    int i = 0;

    while (ligne[*pos] != ',' && ligne[*pos] != '\0' && ligne[*pos] != '\n') {
        champ[i] = ligne[*pos];
        i++;
        (*pos)++;
    }

    champ[i] = '\0';

    if (ligne[*pos] == ',')
        (*pos)++;
}

int lirePassagers(const char *nomFichier, Passager tab[], int max) {
    FILE *f = fopen(nomFichier, "r");
    if (f == NULL) {
        printf("Erreur d'ouverture du fichier.\n");
        return 0;
    }

    char ligne[200];
    int n = 0;

    while (fgets(ligne, 200, f) != NULL && n < max) {

        int pos = 0;
        char champ[50];

        // ID
        extraireChamp(ligne, champ, &pos);
        tab[n].id = convertirEntier(champ);

        // NOM
        extraireChamp(ligne, tab[n].nom, &pos);

        // PRIX
        extraireChamp(ligne, champ, &pos);
        tab[n].prix = convertirFloat(champ);

        n++;
    }

    fclose(f);
    return n;
}

void afficherListePassagers(Passager tab[], int n) {
    int i = 0;
    while (i < n) {
        printf("ID: %d | Nom: %s | Prix: %.2f\n", tab[i].id, tab[i].nom, tab[i].prix);

        i++;
    }
}


