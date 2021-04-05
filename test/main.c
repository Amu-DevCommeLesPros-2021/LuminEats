#include "test_harness/test_harness.h"

#include "db/db.h"
#include "vector/vector.h"

#include <stdio.h>

// Valeurs pour le harnais de test spécifiques à ce programme.
int const tests_total = 23;
int const test_column_width = 60;

int main()
{
    // Tests de lecture et d'écriture de la table 'restaurants'.
    {
        FILE *test_db_restaurants = fopen("build/test-db/restaurants.csv", "r");
        vector restaurants = lecture_table_restaurant(test_db_restaurants);
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
        ecriture_table_restaurant(test_db_restaurants_copie, &restaurants);
        fclose(test_db_restaurants_copie);
        TEST_FILE("build/test-db/restaurants.csv", "build/test-db/restaurants-copie.csv");
    }



    return 0;
}