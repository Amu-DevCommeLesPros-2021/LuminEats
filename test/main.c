#include "test_harness/test_harness.h"

#include "db/db.h"
#include "vector/vector.h"

#include <stdio.h>

// Valeurs pour le harnais de test spécifiques à ce programme.
int const tests_total = 60;
int const test_column_width = 60;

int main()
{
    // Tests de lecture et d'écriture de la table 'restaurants'.
    {
        FILE *test_db_restaurants = fopen("build/test-db/restaurants.csv", "r");
        vector restaurants = lecture_table_restaurants(test_db_restaurants);
        fclose(test_db_restaurants);

        TEST(size(restaurants) == 3);

        restaurant *r = (restaurant*)value(begin(&restaurants));
        TEST(r->index == 1);
        TEST(strcmp(r->nom, "Chez Michel") == 0);
        TEST(strcmp(r->code_postal, "13001") == 0);
        TEST(strcmp(r->telephone, "04 13 13 13 13") == 0);
        TEST(strcmp(r->type, "Provencal") == 0);
        TEST(r->items[0] == 1);
        TEST(r->items[1] == 4);
        TEST(r->items[2] == 5);
        TEST(r->items[3] == 0);
        TEST(r->items[TAILLE_ITEMS - 1] == 0);
        TEST(r->solde == 50);

        r = (restaurant*)value(at(&restaurants, 2));
        TEST(r->index == 3);
        TEST(strcmp(r->nom, "Joe's International House of Pancakes") == 0);
        TEST(strcmp(r->code_postal, "13010") == 0);
        TEST(strcmp(r->telephone, "04 22 33 44 55") == 0);
        TEST(strcmp(r->type, "Americain") == 0);
        TEST(r->items[0] == 6);
        TEST(r->items[1] == 7);
        TEST(r->items[2] == 0);
        TEST(r->items[TAILLE_ITEMS - 1] == 0);
        TEST(r->solde == 44);

        FILE *test_db_restaurants_copie = fopen("build/test-db/restaurants-copie.csv", "w");
        ecriture_table_restaurants(test_db_restaurants_copie, &restaurants);
        fclose(test_db_restaurants_copie);
        TEST_FILE("build/test-db/restaurants.csv", "build/test-db/restaurants-copie.csv");
    }

    // Tests de lecture et d'écriture de la table 'items'.
    {
        FILE *test_db_items = fopen("build/test-db/items.csv", "r");
        vector items = lecture_table_items(test_db_items);
        fclose(test_db_items);

        TEST(size(items) == 7);

        item *i = (item*)value(begin(&items));
        TEST(i->index == 1);
        TEST(strcmp(i->nom, "bouillabaise") == 0);
        TEST(strcmp(i->ingredients[0], "poissons de roche") == 0);
        TEST(strcmp(i->ingredients[1], "pommes de terre") == 0);
        TEST(strcmp(i->ingredients[2], "") == 0);
        TEST(i->prix == 25);

        i = (item*)value(at(&items, 6));
        TEST(i->index == 7);
        TEST(strcmp(i->nom, "petit-dej du champion") == 0);
        TEST(strcmp(i->ingredients[0], "oeufs") == 0);
        TEST(strcmp(i->ingredients[1], "toast") == 0);
        TEST(strcmp(i->ingredients[2], "bacon") == 0);
        TEST(strcmp(i->ingredients[3], "pomme de terre") == 0);
        TEST(strcmp(i->ingredients[4], "") == 0);
        TEST(i->prix == 12);

        FILE *test_db_items_copie = fopen("build/test-db/items-copie.csv", "w");
        ecriture_table_items(test_db_items_copie, &items);
        fclose(test_db_items_copie);
        TEST_FILE("build/test-db/items.csv", "build/test-db/items-copie.csv");
    }

    // Tests de lecture et d'écriture de la table 'livreurs'.
    {
        FILE *test_db_livreurs = fopen("build/test-db/livreurs.csv", "r");
        vector livreurs = lecture_table_livreurs(test_db_livreurs);
        fclose(test_db_livreurs);

        TEST(size(livreurs) == 3);

        livreur *l = (livreur*)value(begin(&livreurs));
        TEST(l->index == 1);
        TEST(strcmp(l->nom, "Francois Pignon") == 0);
        TEST(strcmp(l->telephone, "06 00 00 00 00") == 0);
        TEST(strcmp(l->deplacements[0], "13001") == 0);
        TEST(strcmp(l->deplacements[1], "13002") == 0);
        TEST(strcmp(l->deplacements[2], "13003") == 0);
        TEST(strcmp(l->deplacements[3], "") == 0);
        TEST(l->restaurant == 1);
        TEST(l->solde == 20);

        l = (livreur*)value(at(&livreurs, 2));
        TEST(l->index == 3);
        TEST(strcmp(l->nom, "Mickey Mouse") == 0);
        TEST(strcmp(l->telephone, "06 11 22 33 44") == 0);
        TEST(strcmp(l->deplacements[0], "13008") == 0);
        TEST(strcmp(l->deplacements[1], "13009") == 0);
        TEST(strcmp(l->deplacements[2], "13010") == 0);
        TEST(strcmp(l->deplacements[3], "13011") == 0);
        TEST(strcmp(l->deplacements[4], "") == 0);
        TEST(l->restaurant == 0);
        TEST(l->solde == 0);

        FILE *test_db_livreurs_copie = fopen("build/test-db/livreurs-copie.csv", "w");
        ecriture_table_livreurs(test_db_livreurs_copie, &livreurs);
        fclose(test_db_livreurs_copie);
        TEST_FILE("build/test-db/livreurs.csv", "build/test-db/livreurs-copie.csv");
    }

    return 0;
}