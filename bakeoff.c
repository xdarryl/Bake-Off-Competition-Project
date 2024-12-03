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

void gather_pantry_ingredients(int baker_id, PantryIngredient *needed_ingredients, int num_ingredients)
{
    sem_wait(&pantry); // Lock access to the pantry
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
    sem_post(&pantry); // Release access to the pantry
}

void gather_fridge_ingredients(int baker_id, FridgeIngredient *needed_ingredients, int num_ingredients)
{
    sem_wait(&refrigerator); // Lock access to the refrigerator
    printf("Baker %d is in the refrigerator gathering ingredients.\n", baker_id);

    for (int i = 0; i < num_ingredients; i++)
    {
        printf("Baker %d is taking %s from the refrigerator.\n", baker_id,
               (needed_ingredients[i] == Eggs) ? "Eggs" : (needed_ingredients[i] == Milk) ? "Milk"
                                                                                          : "Butter");
    }

    sem_post(&refrigerator); // Release access to the refrigerator
}

// Baker Struct to hold some information about our bakers.
typedef struct 
{
    int id; //Id for the baker, used mostly for print statements.
    bool ramsied; // default at first. 
} Baker;

// This is where our bakers will operate.
void *kitchen(void *args)
{
    Baker *baker = (Baker *) args;
    printf("Baker %d is ready! Welcome to the competition.\n", baker->id);

}

int main(int argc, char *argv[])
{

    int total_bakers; // 

    printf("Welcome to the 2024 CS452 Bake Off!\n");
    printf("Enter the number of bakers for competition: \n");

    scanf("%d", &total_bakers);

    Baker bakers[total_bakers];

    pthread_t bakerThreads[total_bakers];

    for (int i = 0; i < total_bakers; i++)
    {
        bakers[i].id = i + 1;
        bakers[i].ramsied = false;
        pthread_create(&bakerThreads[i], NULL, kitchen, (void *)&bakers[i]); // Baker Thread, NULL, function to work on, bakerID
    }

    // Initialize semaphores
    sem_init(&mixer, 0, MIXERS);
    sem_init(&pantry, 0, PANTRIES);
    sem_init(&refrigerator, 0, REFRIGERATORS);
    sem_init(&bowl, 0, BOWLS);
    sem_init(&spoon, 0, SPOONS);
    sem_init(&oven, 0, OVEN);

    // Initialize ingredients
    for (int i = 0; i < total_bakers; ++i) 
    {
        pthread_join(bakerThreads[i], NULL);
    }

    printf("All bakers have completed their tasks!\n");

    return 0;
}
