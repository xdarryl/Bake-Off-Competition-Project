/*
*   CIS 452 Bake-Off
*   Names: Darryl G.
           Mateo V.
*   12/2/2024
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

// Resources in our shared kitchen
#define MIXERS 2
#define PANTRIES 1
#define REFRIGERATORS 2
#define BOWLS 3
#define SPOONS 5
#define OVEN 1

// Max number of ingredients
#define MAX_INGREDIENTS 10

// Resource semaphores
sem_t mixer, pantry, refrigerator, bowl, spoon, oven;

// Structs to store ingredients
typedef enum
{
    FLOUR,
    SUGAR,
    YEAST,
    BAKING_SODA,
    SALT,
    CINNAMON,
    EGGS,
    MILK,
    BUTTER,
    Unknown
} AllIngredients;

typedef enum
{
    Flour,
    Sugar,
    Yeast,
    Baking_soda,
    Salt,
    Cinnamon
} PantryIngredient;

typedef enum
{
    Eggs,
    Milk,
    Butter
} FridgeIngredient;

// Baker Struct to hold some information about our bakers.
typedef struct
{
    // Id for the baker, used mostly for print statements.
    int id;
    // default at first.
    bool ramsied;
} Baker;

// Recipe Struct to hold information about a recipe.
typedef struct
{
    // Name of recipe
    const char *name;
    // An array of ingredients
    const char *ingredients[MAX_INGREDIENTS];
    // Number of ingredients in the recipe, doesn't need to be the max can be less.
    int ingredient_count;
    // Once the baker is done, change the bool.
    bool isCompleted;
} Recipe;

void access_pantry(int baker_id, PantryIngredient needed_ingredient)
{
    printf("Baker %d is waitning for the refrigerator.\n", baker_id);
    // Lock access to the pantry
    sem_wait(&pantry);
    printf("Baker %d is in the pantry gathering ingredients.\n", baker_id);

    // Release access to the pantry
    printf("Baker %d is done with the refrigerator.\n", baker_id);
    sem_post(&pantry);
}

void access_fridge(int baker_id, FridgeIngredient needed_ingredient)
{
    printf("Baker %d is waitning for the refrigerator.\n", baker_id);
    // Lock access to the refrigerator
    sem_wait(&refrigerator);

    printf("Baker %d is in the refrigerator gathering ingredients.\n", baker_id);

    // Release access to the refrigerator
    printf("Baker %d is done with the refrigerator.\n", baker_id);
    sem_post(&refrigerator);
}

AllIngredients get_ingredient_enum(const char *ingredient)
{
    if (strcmp(ingredient, "Flour") == 0)
    {
        return FLOUR;
    }
    else if (strcmp(ingredient, "Sugar") == 0)
    {
        return SUGAR;
    }
    else if (strcmp(ingredient, "Yeast") == 0)
    {
        return YEAST;
    }
    else if (strcmp(ingredient, "Baking_soda") == 0)
    {
        return BAKING_SODA;
    }
    else if (strcmp(ingredient, "Salt") == 0)
    {
        return SALT;
    }
    else if (strcmp(ingredient, "Cinnamon") == 0)
    {
        return CINNAMON;
    }
    else if (strcmp(ingredient, "Egg") == 0)
    {
        return EGGS;
    }
    else if (strcmp(ingredient, "Milk") == 0)
    {
        return MILK;
    }
    else if (strcmp(ingredient, "Butter") == 0)
    {
        return BUTTER;
    }
    else
    {
        return Unknown;
    }
}

// This is where our bakers will operate.
void *kitchen(void *args)
{
    Baker *baker = (Baker *)args;
    printf("Baker %d is ready! Welcome to the competition.\n", baker->id);

    // Initialize ingredients
    Recipe recipeList[] = {
        {"Cookies", {"Flour", "Sugar", "Milk", "Butter"}, 4, false},
        {"Pancakes", {"Flour", "Sugar", "Baking soda", "Salt", "Egg", "Milk", "Butter"}, 7, false},
        {"Homemade pizza dough", {"Yeast", "Sugar", "Salt"}, 3, false},
        {"Soft Pretzels", {"Flour", "Sugar", "Salt", "Yeast", "Baking Soda", "Egg"}, 6, false},
        {"Cinnamon Rolls", {"Flour", "Sugar", "Salt", "Butter", "Eggs", "Cinnamon"}, 6, false}};

    // printf("Size of list %ld\n", sizeof(recipeList) / sizeof(recipeList[0])); //Debugging & Size Validation

    for (int i = 0; i < (sizeof(recipeList) / sizeof(recipeList[0])); i++)
    {
        Recipe curentRecipe = recipeList[i];

        printf("Baker %d is starting on recipe: %s\n", baker->id, recipeList[i].name);
        for (int j = 0; j < recipeList[i].ingredient_count; j++)
        {
            printf("Baker %d needs %s\n", baker->id, recipeList[i].ingredients[j]);

            AllIngredients ingredient_enum = get_ingredient_enum(recipeList[i].ingredients[j]);

            switch (ingredient_enum)
            {
            case FLOUR:
                // Handle Flour
                printf("Walking to the Flour\n");
                access_pantry(baker->id, Flour);
                break;

            case SUGAR:
                // Handle Sugar
                printf("Waling to the Sugar\n");
                access_pantry(baker->id, Sugar);
                break;

            case YEAST:
                // Handle Yeast
                printf("Walking to the Yeast\n");
                access_pantry(baker->id, Yeast);
                break;

            case BAKING_SODA:
                // Handle Baking Soda
                printf("walking to the Baking Soda\n");
                access_pantry(baker->id, Baking_soda);
                break;

            case SALT:
                // Handle Salt
                printf("Walking to the Salt\n");
                access_pantry(baker->id, Salt);
                break;

            case CINNAMON:
                // Handle Cinnamon
                printf("Walking to the Cinnamon\n");
                access_pantry(baker->id, Cinnamon);
                break;

            case EGGS:
                // Handle Eggs
                printf("Walking to the Eggs\n");
                access_fridge(baker->id, Eggs);
                break;

            case MILK:
                // Handle Mlik
                printf("Walking to the Milk\n");
                access_fridge(baker->id, Milk);
                break;

            case BUTTER:
                // Handle Mlik
                printf("Walking to the Butter\n");
                access_fridge(baker->id, Butter);
                break;

            default:
                // Handle unknown ingredient
                printf("Unknown ingredient\n");
                break;
            }
        }
        printf("Baker %d has completed %s\n", baker->id, recipeList[i].name);
    }
}

int main(int argc, char *argv[])
{
    // Initialize semaphores
    sem_init(&mixer, 0, MIXERS);
    sem_init(&pantry, 0, PANTRIES);
    sem_init(&refrigerator, 0, REFRIGERATORS);
    sem_init(&bowl, 0, BOWLS);
    sem_init(&spoon, 0, SPOONS);
    sem_init(&oven, 0, OVEN);

    // Variable for total bakers.
    int total_bakers;

    printf("Welcome to the 2024 CS452 Bake Off!\n");
    printf("Enter the number of bakers for competition: \n");

    // Get the user input for the number of bakers.
    scanf("%d", &total_bakers);

    Baker bakers[total_bakers];

    pthread_t bakerThreads[total_bakers];

    // Create and Initialize Bakers Threads.
    for (int i = 0; i < total_bakers; i++)
    {
        bakers[i].id = i + 1;
        bakers[i].ramsied = false;
        pthread_create(&bakerThreads[i], NULL, kitchen, (void *)&bakers[i]); // Baker Thread, NULL, function to work on, bakerID
    }

    // Join the Baker Threads.
    for (int i = 0; i < total_bakers; ++i)
    {
        pthread_join(bakerThreads[i], NULL);
    }

    printf("All bakers have completed their tasks!\n");

    return 0;
}
