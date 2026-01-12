#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int lireEntier(const char *s, int *i) {
    int val = 0;
    while (s[*i] >= '0' && s[*i] <= '9') {
        val = val * 10 + (s[*i] - '0');
        (*i)++;
    }
    return val;
}

float lireReel(const char *s, int *i) {
    float val = 0.0f;
    while (s[*i] >= '0' && s[*i] <= '9') {
        val = val * 10.0f + (s[*i] - '0');
        (*i)++;
    }
    if (s[*i] == '.') {
        (*i)++;
        float factor = 0.1f;
        while (s[*i] >= '0' && s[*i] <= '9') {
            val += (s[*i] - '0') * factor;
            factor *= 0.1f;
            (*i)++;
        }
    }
    return val;
}


void lireChamp(const char *s, int *i, char *dest) {
    int k = 0;
    while (s[*i] != ',' && s[*i] != '\0') {
        if (k < 49) { 
            dest[k++] = s[*i];
        }
        (*i)++;
    }
    dest[k] = '\0';
    if (s[*i] == ',') (*i)++; 
}


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
    if (t1->annee < t2->annee)
        return -1;
    if (t1->annee > t2->annee)
        return 1;

    if (t1->mois < t2->mois)
        return -1;
    if (t1->mois > t2->mois)
        return 1;

    if (t1->jour < t2->jour)
        return -1;
    if (t1->jour > t2->jour)
        return 1;

    return 0;
}

int comparePassagersPrixDesc(const void *a, const void *b) {
    const Passager *p1 = (const Passager *)a;
    const Passager *p2 = (const Passager *)b;
    if (p1->prix < p2->prix) return 1;
    if (p1->prix > p2->prix) return -1;
    return 0;
}

long unifieDateHeure(const Trajet *t) {
    long minutes = t->annee * 525600 + t->mois * 43200 + t->jour * 1440 
                   + (t->heureDepart / 100) * 60 + (t->heureDepart % 100);
    return minutes;
}
