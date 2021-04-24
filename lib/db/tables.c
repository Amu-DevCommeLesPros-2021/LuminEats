#include "db/types.h"

#include "vector/vector.h"

#include <stdio.h>
#include <string.h>

vector table_restaurants = {0}, table_items = {0}, table_livreurs = {0}, table_clients = {0};

vector lecture_table_restaurants(
    FILE* fichier)
{
    vector restaurants = make_vector(sizeof(restaurant), 0);

    rewind(fichier);

    char *buffer = NULL;
    size_t buffer_size;
    getline(&buffer, &buffer_size, fichier); // Skip the header line.
    while(getline(&buffer, &buffer_size, fichier) != -1)
    {
        restaurant r = {0};
        sscanf(buffer, "%zu,%[^,],%[^,],%[^,],%[^,],%[^,],%zu\n", &r.index, r.nom, r.code_postal, r.telephone, r.type, r.menu_s, &r.solde);
        
        // Parse string of semicolon separated items.
        char m[TAILLE_MENU * 3];
        strcpy(m, r.menu_s);
        memset(r.menu, 0, sizeof(r.menu));
        int i = 0;
        for(char *item = strtok(m, ";"); item; item = strtok(NULL, ";"))
        {
            r.menu[i++] = atoi(item);
        }

        push_back(&restaurants, &r);
    }
    free(buffer);

    return restaurants;
}

void ecriture_table_restaurants(
    FILE* fichier,
    vector const* db)
{
    fprintf(fichier, "id,nom,code postal,telephone,type,menu,solde\n"); // Header line.

    for(iterator i = begin(db); compare(i, end(db)) != 0; increment(&i, 1))
    {
        restaurant *r = (restaurant*)value(i);

        fprintf(fichier, "%zu,%s,%s,%s,%s,", r->index, r->nom, r->code_postal, r->telephone, r->type);

        for(int i = 0; i != TAILLE_MENU && r->menu[i] != 0; ++i)
        {
            fprintf(fichier, "%zu", r->menu[i]);
            if(i != TAILLE_MENU && r->menu[i + 1] != 0)
            {
                fprintf(fichier, ";");
            }
        }

        fprintf(fichier, ",%zu\n", r->solde);
    }
}

vector lecture_table_items(
    FILE* fichier)
{
    vector items = make_vector(sizeof(item), 0);

    rewind(fichier);

    char *buffer = NULL;
    size_t buffer_size;
    getline(&buffer, &buffer_size, fichier); // Skip the header line.
    while(getline(&buffer, &buffer_size, fichier) != -1)
    {
        item i = {0};
        sscanf(buffer, "%zu,%[^,],%[^,],%zu\n", &i.index, i.nom, i.ingredients_s, &i.prix);
        
        // Parse string of semicolon separated ingredients.
        char g[TAILLE_INGREDIENTS * TAILLE_CHAMP_INGREDIENT];
        strcpy(g, i.ingredients_s);
        memset(i.ingredients, 0, TAILLE_INGREDIENTS * TAILLE_CHAMP_INGREDIENT);
        int j = 0;
        for(char *ingredient = strtok(g, ";"); ingredient; ingredient = strtok(NULL, ";"))
        {
            strcpy(i.ingredients[j++], ingredient);
        }

        push_back(&items, &i);
    }
    free(buffer);

    return items;
}

void ecriture_table_items(
    FILE* fichier,
    vector const* db)
{
    fprintf(fichier, "id,nom,ingredients,prix\n"); // Header line.

    for(iterator j = begin(db); compare(j, end(db)) != 0; increment(&j, 1))
    {
        item *i = (item*)value(j);

        fprintf(fichier, "%zu,%s,", i->index, i->nom);

        for(int k = 0; k != TAILLE_INGREDIENTS && i->ingredients[k][0] != '\0'; ++k)
        {
            fprintf(fichier, "%s", i->ingredients[k]);
            if(k != TAILLE_INGREDIENTS && i->ingredients[k + 1][0] != '\0')
            {
                fprintf(fichier,";");
            }
        }

        fprintf(fichier, ",%zu\n", i->prix);
    }
}

vector lecture_table_livreurs(
    FILE* fichier)
{
    vector livreurs = make_vector(sizeof(livreur), 0);

    rewind(fichier);

    char *buffer = NULL;
    size_t buffer_size;
    getline(&buffer, &buffer_size, fichier); // Skip the header line.
    while(getline(&buffer, &buffer_size, fichier) != -1)
    {
        livreur l = {0};
        sscanf(buffer, "%zu,%[^,],%[^,],%[^,],%zu,%zu\n", &l.index, l.nom, l.telephone, l.deplacements_s, &l.restaurant, &l.solde);
        
        // Parse string of semicolon separated postal codes.
        char d[TAILLE_DEPLACEMENTS * TAILLE_CHAMP_CODEPOSTAL];
        strcpy(d, l.deplacements_s);
        memset(l.deplacements, 0, TAILLE_DEPLACEMENTS * TAILLE_CHAMP_CODEPOSTAL);
        int j = 0;
        for(char *deplacement = strtok(d, ";"); deplacement; deplacement = strtok(NULL, ";"))
        {
            strcpy(l.deplacements[j++], deplacement);
        }

        push_back(&livreurs, &l);
    }
    free(buffer);

    return livreurs;
}

void ecriture_table_livreurs(
    FILE* fichier,
    vector const* db)
{
    fprintf(fichier, "id,nom,telephone,deplacements,restaurant,solde\n"); // Header line.

    for(iterator j = begin(db); compare(j, end(db)) != 0; increment(&j, 1))
    {
        livreur *l = (livreur*)value(j);

        fprintf(fichier, "%zu,%s,%s,", l->index, l->nom, l->telephone);

        for(int k = 0; k != TAILLE_DEPLACEMENTS && l->deplacements[k][0] != '\0'; ++k)
        {
            fprintf(fichier, "%s", l->deplacements[k]);
            if(k != TAILLE_DEPLACEMENTS && l->deplacements[k + 1][0] != '\0')
            {
                fprintf(fichier,";");
            }
        }

        fprintf(fichier, ",%zu,%zu\n", l->restaurant, l->solde);
    }
}

vector lecture_table_clients(
    FILE* fichier)
{
    vector clients = make_vector(sizeof(client), 0);

    rewind(fichier);

    char *buffer = NULL;
    size_t buffer_size;
    getline(&buffer, &buffer_size, fichier); // Skip the header line.
    while(getline(&buffer, &buffer_size, fichier) != -1)
    {
        client c = {0};
        sscanf(buffer, "%zu,%[^,],%[^,],%[^,],%zu\n", &c.index, c.nom, c.code_postal, c.telephone, &c.solde);
        push_back(&clients, &c);
    }
    free(buffer);

    return clients;
}

void ecriture_table_clients(
    FILE* fichier,
    vector const* db)
{
    fprintf(fichier, "id,nom,code postal,telephone,solde\n"); // Header line.

    for(iterator j = begin(db); compare(j, end(db)) != 0; increment(&j, 1))
    {
        client *c = (client*)value(j);

        fprintf(fichier, "%zu,%s,%s,%s,%zu\n", c->index, c->nom, c->code_postal, c->telephone, c->solde);
    }
}
