#include "db.h"

#include "vector/vector_api.h"
#include "vector/vector_types.h"

#include <limits.h>
#include <stdio.h>
#include <string.h>

vector lecture_table_restaurants(
    FILE* fichier)
{
    vector restaurants = make_vector(sizeof(restaurant), 0, 2.0);

    rewind(fichier);

    char *buffer = NULL;
    size_t buffer_size;
    getline(&buffer, &buffer_size, fichier); // Skip the header line.
    while(getline(&buffer, &buffer_size, fichier) != -1)
    {
        restaurant r;
        sscanf(buffer, "%zu,%[^,],%[^,],%[^,],%[^,],%[^,],%zu\n", &r.index, r.nom, r.code_postal, r.telephone, r.type, r.items_s, &r.solde);
        
        // Parse string of semicolon separated items.
        memset(r.items, 0, sizeof(r.items));
        int i = 0;
        for(char *item = strtok(r.items_s, ";"); item; item = strtok(NULL, ";"))
        {
            r.items[i++] = atoi(item);
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

        for(int i = 0; i != TAILLE_ITEMS && r->items[i] != 0; ++i)
        {
            fprintf(fichier, "%zu", r->items[i]);
            if(i != TAILLE_ITEMS && r->items[i + 1] != 0)
            {
                fprintf(fichier,";");
            }
        }

        fprintf(fichier, ",%zu\n", r->solde);
    }
}

vector lecture_table_items(
    FILE* fichier)
{
    vector items = make_vector(sizeof(item), 0, 2.0);

    rewind(fichier);

    char *buffer = NULL;
    size_t buffer_size;
    getline(&buffer, &buffer_size, fichier); // Skip the header line.
    while(getline(&buffer, &buffer_size, fichier) != -1)
    {
        item i;
        sscanf(buffer, "%zu,%[^,],%[^,],%zu\n", &i.index, i.nom, i.ingredients_s, &i.prix);
        
        // Parse string of semicolon separated ingredients.
        memset(i.ingredients, 0, TAILLE_INGREDIENTS * TAILLE_CHAMP_INGREDIENT);
        int j = 0;
        for(char *ingredient = strtok(i.ingredients_s, ";"); ingredient; ingredient = strtok(NULL, ";"))
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
    vector livreurs = make_vector(sizeof(livreur), 0, 2.0);

    rewind(fichier);

    char *buffer = NULL;
    size_t buffer_size;
    getline(&buffer, &buffer_size, fichier); // Skip the header line.
    while(getline(&buffer, &buffer_size, fichier) != -1)
    {
        livreur l;
        sscanf(buffer, "%zu,%[^,],%[^,],%[^,],%zu,%zu\n", &l.index, l.nom, l.telephone, l.deplacements_s, &l.restaurant, &l.solde);
        
        // Parse string of semicolon separated postal codes.
        memset(l.deplacements, 0, TAILLE_DEPLACEMENTS * TAILLE_CHAMP_CODEPOSTAL);
        int j = 0;
        for(char *deplacement = strtok(l.deplacements_s, ";"); deplacement; deplacement = strtok(NULL, ";"))
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
    vector clients = make_vector(sizeof(client), 0, 2.0);

    rewind(fichier);

    char *buffer = NULL;
    size_t buffer_size;
    getline(&buffer, &buffer_size, fichier); // Skip the header line.
    while(getline(&buffer, &buffer_size, fichier) != -1)
    {
        client c;
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

void lecture_db(
    char const* dossier, vector* restaurants, vector* items, vector* livreurs, vector* clients)
{
    char chemin[PATH_MAX];
    
    // Read and copy restaurants.
    sprintf(chemin, "%s/%s", dossier, "restaurants.csv");
    FILE *test_db_restaurants = fopen(chemin, "r");
    vector r = lecture_table_restaurants(test_db_restaurants);
    fclose(test_db_restaurants);

    assign(restaurants, begin(&r), end(&r));
    destroy(&r);

    // Read and copy items.
    sprintf(chemin, "%s/%s", dossier, "items.csv");
    FILE *test_db_items = fopen(chemin, "r");
    vector i = lecture_table_items(test_db_items);
    fclose(test_db_items);

    assign(items, begin(&i), end(&i));
    destroy(&i);

    // Read and copy deliverers.
    sprintf(chemin, "%s/%s", dossier, "livreurs.csv");
    FILE *test_db_livreurs = fopen(chemin, "r");
    vector l = lecture_table_livreurs(test_db_livreurs);
    fclose(test_db_livreurs);

    assign(livreurs, begin(&l), end(&l));
    destroy(&l);

    // Read and copy customers.
    sprintf(chemin, "%s/%s", dossier, "clients.csv");
    FILE *test_db_clients = fopen(chemin, "r");
    vector c = lecture_table_clients(test_db_clients);
    fclose(test_db_clients);

    assign(clients, begin(&c), end(&c));
    destroy(&c);
}

void ecriture_db(
    char const* dossier, vector* restaurants, vector* items, vector* livreurs, vector* clients)
{
    char chemin[PATH_MAX];
    
    sprintf(chemin, "%s/%s", dossier, "restaurants.csv");
    FILE *test_db_restaurants = fopen(chemin, "w");
    ecriture_table_restaurants(test_db_restaurants, restaurants);
    fclose(test_db_restaurants);

    sprintf(chemin, "%s/%s", dossier, "items.csv");
    FILE *test_db_items = fopen(chemin, "w");
    ecriture_table_items(test_db_items, items);
    fclose(test_db_items);

    sprintf(chemin, "%s/%s", dossier, "livreurs.csv");
    FILE *test_db_livreurs = fopen(chemin, "w");
    ecriture_table_livreurs(test_db_livreurs, livreurs);
    fclose(test_db_livreurs);

    sprintf(chemin, "%s/%s", dossier, "clients.csv");
    FILE *test_db_clients = fopen(chemin, "w");
    ecriture_table_clients(test_db_clients, clients);
    fclose(test_db_clients);
}