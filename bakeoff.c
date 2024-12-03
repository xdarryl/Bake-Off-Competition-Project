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
#include <time.h>

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
    printf("Baker %d is waiting for the pantry.\n", baker_id);
    // Lock access to the pantry
    sem_wait(&pantry);
    printf("Baker %d is in the pantry gathering ingredients.\n", baker_id);

    // Release access to the pantry
    printf("Baker %d is done with the pantry.\n", baker_id);
    sem_post(&pantry);
}

void access_fridge(int baker_id, FridgeIngredient needed_ingredient)
{
    printf("Baker %d is waiting for the refrigerator.\n", baker_id);
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
    else if (strcmp(ingredient, "Eggs") == 0)
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
    //while(!competitionStart);

    Baker *baker = (Baker *)args;
    printf("Baker %d is ready! Welcome to the competition.\n", baker->id);

    // Initialize ingredients
    Recipe recipeList[] = 
    {
        {"Cookies", {"Flour", "Sugar", "Milk", "Butter"}, 4, false},
        {"Pancakes", {"Flour", "Sugar", "Baking_soda", "Salt", "Eggs", "Milk", "Butter"}, 7, false},
        {"Homemade pizza dough", {"Yeast", "Sugar", "Salt"}, 3, false},
        {"Soft Pretzels", {"Flour", "Sugar", "Salt", "Yeast", "Baking_soda", "Eggs"}, 6, false},
        {"Cinnamon Rolls", {"Flour", "Sugar", "Salt", "Butter", "Eggs", "Cinnamon"}, 6, false}
    };

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
                printf("Baker %d is walking to the Flour\n", baker->id);
                access_pantry(baker->id, Flour);
                break;

            case SUGAR:
                // Handle Sugar
                printf("Baker %d is walking to the Sugar\n", baker->id);
                access_pantry(baker->id, Sugar);
                break;

            case YEAST:
                // Handle Yeast
                printf("Baker %d is walking to the Yeast\n", baker->id);
                access_pantry(baker->id, Yeast);
                break;

            case BAKING_SODA:
                // Handle Baking Soda
                printf("Baker %d is walking to the Baking Soda\n", baker->id);
                access_pantry(baker->id, Baking_soda);
                break;

            case SALT:
                // Handle Salt
                printf("Baker %d is walking to the Salt\n", baker->id);
                access_pantry(baker->id, Salt);
                break;

            case CINNAMON:
                // Handle Cinnamon
                printf("Baker %d is walking to the Cinnamon\n", baker->id);
                access_pantry(baker->id, Cinnamon);
                break;

            case EGGS:
                // Handle Eggs
                printf("Baker %d is walking to the Eggs\n", baker->id);
                access_fridge(baker->id, Eggs);
                break;

            case MILK:
                // Handle Mlik
                printf("Baker %d is walking to the Milk\n", baker->id);
                access_fridge(baker->id, Milk);
                break;

            case BUTTER:
                // Handle Mlik
                printf("Baker %d is walking to the Butter\n", baker->id);
                access_fridge(baker->id, Butter);
                break;

            default:
                // Handle unknown ingredient
                printf("Unknown ingredient\n");
                break;
            }
        }
        printf("Baker %d has got all the ingredients for %s\n", baker->id, recipeList[i].name);

        printf("Baker %d is waiting for a bowl.\n", baker->id);
        sem_wait(&bowl);
        printf("Baker %d has a bowl.\n", baker->id);

        printf("Baker %d is waiting for a spoon.\n", baker->id);
        sem_wait(&spoon);
        printf("Baker %d has a spoon.\n", baker->id);

        printf("Baker %d is waiting for a mixer.\n", baker->id);
        sem_wait(&mixer);
        printf("Baker %d has a mixer.\n", baker->id);

        printf("Baker %d has all the tools & is putting them back.\n", baker->id);
        sem_post(&bowl);
        sem_post(&spoon);
        sem_post(&mixer);

        printf("Baker %d is waiting to use the oven. \n", baker->id);
        sem_wait(&oven);
        printf("Baker %d is using the oven. \n", baker->id);

        printf("Baker %d is waiting to use the oven. \n", baker->id);
        sem_post(&oven);
        printf("Baker %d is done with the oven. \n", baker->id);

        printf("Baker %d has completed %s\n", baker->id, recipeList[i].name);
    }

    return NULL;
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

    // Seed the random number generator with the current time
    srand(time(NULL));

    // Our min and max for our random number.
    int min = 0;
    int max = total_bakers;

    // Generate a random number between 0 and RAND_MAX
    int random_num = min + rand() % (max - min + 1);

    // Create and Initialize Bakers Threads.
    for (int i = 0; i < total_bakers; i++)
    {
        bakers[i].id = i + 1;
        if (i == random_num)
        {
            bakers[i].ramsied = true; // The baker won't be ramsied right away but will be later on.
            printf("Baker %d will be ramsied later on...\n", bakers[i].id);
        }
        else
        {
            bakers[i].ramsied = false;
        }

        pthread_create(&bakerThreads[i], NULL, kitchen, (void *)&bakers[i]); // Baker Thread, NULL, function to work on, bakerID
        
        // EXPERIMENT: If we uncommenet this, then each baker will wait for each other to complete.
        // This isn't really want we want as we want to simulate a competition of resources. 
        // pthread_join(bakerThreads[i], NULL);
    }

    while(1);

    printf("All bakers have completed their tasks!\n");

    sem_destroy(&mixer);
    sem_destroy(&pantry);
    sem_destroy(&refrigerator);
    sem_destroy(&bowl);
    sem_destroy(&spoon);
    sem_destroy(&oven);

    return 0;
}
