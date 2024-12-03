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
typedef struct {
    // Name of recipe
    const char *name;
    // An array of ingredients
    const char *ingredients[MAX_INGREDIENTS];
    // Number of ingredients in the recipe, doesn't need to be the max can be less.
    int ingredient_count;
} Recipe;

void access_pantry(int baker_id, PantryIngredient *needed_ingredients, int num_ingredients)
{
    // Lock access to the pantry
    sem_wait(&pantry); 
    printf("Baker %d is in the pantry gathering ingredients.\n", baker_id);

    for (int i = 0; i < num_ingredients; i++)
    {
        printf("Baker %d is taking %s from the pantry.\n", baker_id,
               (needed_ingredients[i] == Flour) ? "Flour" : (needed_ingredients[i] == Sugar)     ? "Sugar"
                                                          : (needed_ingredients[i] == Yeast)       ? "Yeast"
                                                          : (needed_ingredients[i] == Baking_soda) ? "Baking Soda"
                                                          : (needed_ingredients[i] == Salt)        ? "Salt"
                                                                                                 : "Cinnamon");
    }
    // Release access to the pantry
    sem_post(&pantry); 
}

void gather_fridge(int baker_id, FridgeIngredient *needed_ingredients, int num_ingredients)
{
    // Lock access to the refrigerator
    sem_wait(&refrigerator); 
    printf("Baker %d is in the refrigerator gathering ingredients.\n", baker_id);

    for (int i = 0; i < num_ingredients; i++)
    {
        printf("Baker %d is taking %s from the refrigerator.\n", baker_id,
               (needed_ingredients[i] == Eggs) ? "Eggs" : (needed_ingredients[i] == Milk) ? "Milk"
                                                                                          : "Butter");
    }
    // Release access to the refrigerator
    sem_post(&refrigerator);
}

// This is where our bakers will operate.
void *kitchen(void *args)
{
    Baker *baker = (Baker *) args;
    printf("Baker %d is ready! Welcome to the competition.\n", baker->id);

    // Initialize ingredients
    Recipe recipeList[] =
    {
    {"Cookies", {"Flour", "Sugar", "Milk", "Butter"}, 4},
    {"Pancakes", {"Flour", "Sugar", "Baking soda", "Salt", "Egg", "Milk", "Butter"}, 7},
    {"Homemade pizza dough", {"Yeast", "Sugar", "Salt"}, 3},
    {"Soft Pretzels", {"Flour", "Sugar", "Salt", "Yeast", "Baking Soda", "Egg"}, 6},
    {"Cinnamon Rolls", {"Flour", "Sugar", "Salt", "Butter", "Eggs", "Cinnamon"}, 6}
    };

    //printf("Size of list %ld\n", sizeof(recipeList) / sizeof(recipeList[0])); //Debugging & Size Validation 

    for(int i=0; i < (sizeof(recipeList) / sizeof(recipeList[0])); i++)
    {
        Recipe curentRecipe = recipeList[i];

        printf("Baker %d is starting on recipe: %s\n", baker->id, recipeList[i].name);
        
    }




    

}

int main(int argc, char *argv[])
{
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

    // Initialize semaphores
    sem_init(&mixer, 0, MIXERS);
    sem_init(&pantry, 0, PANTRIES);
    sem_init(&refrigerator, 0, REFRIGERATORS);
    sem_init(&bowl, 0, BOWLS);
    sem_init(&spoon, 0, SPOONS);
    sem_init(&oven, 0, OVEN);

    printf("All bakers have completed their tasks!\n");

    return 0;
}
