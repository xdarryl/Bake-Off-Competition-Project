/*
*   CIS 452 Bake-Off
*   Names: Darryl G.
           Mateo V.
*   12/2/2024
*/

// Includes
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

// Globel variable to keep track of all bakers who complete all their recipes.
int total_completed = 0;

// Structs to store ingredients
// This enum has all our ingredients.
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
// This enum has all our pantry ingredients,
// Use for the pantry function and to seperate fridge
// ingredients.
typedef enum
{
    Flour,
    Sugar,
    Yeast,
    Baking_soda,
    Salt,
    Cinnamon
} PantryIngredient;
// This enum has all our fridge ingredients,
// Use for the fridge function and to seperate pantry
// ingredients.
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
    // Bool for us to pick a Baker to be ramsied aka restarted!
    // Gordon Ramsey: "What are you?!?"
    // The Lady: "An idot sandwich... :("
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

// Helper function to hold and release pantry resources.
// This simulates a baker going to the pantry taking the lock
// Holding it and then giving it back.
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

// Helper function to hold and release fridge resources.
// This simulates a baker going to the fridge taking the lock
// Holding it and then giving it back.
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

// This function handles all our ingredient enums and compares them
// to strings. This is useful for our switch statement(s).
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
    // This pointers to our baker information which allows us to use that id throughout.
    Baker *baker = (Baker *)args;

    // Gordon Ramsay Controller, we will limit him to only one RAMSEID!
    int hasBeenRamsied = false;

    // Print that the baker has been created and is ready to start.
    printf("Baker %d is ready! Welcome to the competition.\n", baker->id);

    // Initialize ingredients
    Recipe recipeList[] =
        {
            {"Cookies", {"Flour", "Sugar", "Milk", "Butter"}, 4, false},
            {"Pancakes", {"Flour", "Sugar", "Baking_soda", "Salt", "Eggs", "Milk", "Butter"}, 7, false},
            {"Homemade pizza dough", {"Yeast", "Sugar", "Salt"}, 3, false},
            {"Soft Pretzels", {"Flour", "Sugar", "Salt", "Yeast", "Baking_soda", "Eggs"}, 6, false},
            {"Cinnamon Rolls", {"Flour", "Sugar", "Salt", "Butter", "Eggs", "Cinnamon"}, 6, false}};

    // printf("Size of list %ld\n", sizeof(recipeList) / sizeof(recipeList[0])); //Debugging & Size Validation

    for (int i = 0; i < (sizeof(recipeList) / sizeof(recipeList[0])); i++)
    {
        // Allows to know what recipe were working on.
        Recipe curentRecipe = recipeList[i];

        // Print to the console that a baker has started x recipe.
        printf("Baker %d is starting on recipe: %s\n", baker->id, recipeList[i].name);

        for (int j = 0; j < recipeList[i].ingredient_count; j++)
        {
            // Ramsied logic. If the baker's ramsied is true and has not been ramsied yet
            // Then we need to restart them to the beginning.
            if (baker->ramsied && !hasBeenRamsied)
            {
                printf("Gordon Ramsay: WHAT ARE YOU!?\n");
                printf("Baker %d: An idiot sandwich :(\n", baker->id);
                printf("Gordon Ramsay: AN IDIOT SANDWICH IS RIGHT\n");
                printf("Baker %d: An idiot sandwich, Chef Ramsay :(\n", baker->id);
                i = -1;
                hasBeenRamsied = true;
                break;
            }

            // Print message for what hte baker is doing. 
            printf("Baker %d needs %s\n", baker->id, recipeList[i].ingredients[j]);
            
            // We use this compare the ingredient in the recipe which is a string
            // and return back a enum that is that ingredient to use in our switch statment.
            AllIngredients ingredient_enum = get_ingredient_enum(recipeList[i].ingredients[j]);

            // Switch statement to find what ingreident the baker needs,
            // once found, the correct function is called and
            // this simulates them accessing either the pantry or fridge.
            // Once in either, they take the lock and then give it back moments after.
            switch (ingredient_enum)
            {
            case FLOUR:
                printf("Baker %d is walking to the Flour\n", baker->id);
                access_pantry(baker->id, Flour);
                break;

            case SUGAR:
                printf("Baker %d is walking to the Sugar\n", baker->id);
                access_pantry(baker->id, Sugar);
                break;

            case YEAST:
                printf("Baker %d is walking to the Yeast\n", baker->id);
                access_pantry(baker->id, Yeast);
                break;

            case BAKING_SODA:
                printf("Baker %d is walking to the Baking Soda\n", baker->id);
                access_pantry(baker->id, Baking_soda);
                break;

            case SALT:
                printf("Baker %d is walking to the Salt\n", baker->id);
                access_pantry(baker->id, Salt);
                break;

            case CINNAMON:
                printf("Baker %d is walking to the Cinnamon\n", baker->id);
                access_pantry(baker->id, Cinnamon);
                break;

            case EGGS:
                printf("Baker %d is walking to the Eggs\n", baker->id);
                access_fridge(baker->id, Eggs);
                break;

            case MILK:
                printf("Baker %d is walking to the Milk\n", baker->id);
                access_fridge(baker->id, Milk);
                break;

            case BUTTER:
                printf("Baker %d is walking to the Butter\n", baker->id);
                access_fridge(baker->id, Butter);
                break;

            default:
                printf("Unknown ingredient\n");
                break;
            }
        }
        // If ramsied, there's only one time when i = -1, so we can use this to skip this outer loop.
        // Probably a better way to do this, but I found this works.
        if (i == -1) 
        {
            continue; // Skip the rest of the outer loop
        }
        // Baker has all ingredients, now they need tools, and oven time.
        printf("Baker %d has got all the ingredients for %s\n", baker->id, recipeList[i].name);

        // Baker now needs all the tools, Take access, give back access and print what the baker is doing.
        // Baker taking bowl.
        printf("Baker %d is waiting for a bowl.\n", baker->id);
        sem_wait(&bowl);
        printf("Baker %d has a bowl.\n", baker->id);

        // Baker taking spoon.
        printf("Baker %d is waiting for a spoon.\n", baker->id);
        sem_wait(&spoon);
        printf("Baker %d has a spoon.\n", baker->id);

        // Baker taking mixer.
        printf("Baker %d is waiting for a mixer.\n", baker->id);
        sem_wait(&mixer);
        printf("Baker %d has a mixer.\n", baker->id);

        // Baker needs tools, end.

        // Give back access to all the tools.
        printf("Baker %d has all the tools & is putting them back.\n", baker->id);
        sem_post(&bowl);
        sem_post(&spoon);
        sem_post(&mixer);

        // Baker Using Oven, Take access, give back access and print what the baker is doing.
        printf("Baker %d is waiting to use the oven. \n", baker->id);
        sem_wait(&oven);
        printf("Baker %d is using the oven. \n", baker->id);

        printf("Baker %d is waiting to use the oven. \n", baker->id);
        sem_post(&oven);
        printf("Baker %d is done with the oven. \n", baker->id);
        // Baker using Oven, end.

        // Baker completing that recipe.
        printf("Baker %d has completed %s\n", baker->id, recipeList[i].name);

    }

    // This baker has completed all the recipes.
    total_completed += 1;

    // A fun little switch statment to see what baker finished 1st, 2nd, and 3rd.
    switch (total_completed)
    {
    case 1:
        printf("Congratulations, Baker %d! You finished in 1st place!\n", baker->id);
        break;
    case 2:
        printf("Well done, Baker %d! You finished in 2nd place!\n", baker->id);
        break;
    case 3:
        printf("Great effort, Baker %d! You finished in 3rd place!\n", baker->id);
        break;
    default:
        printf("Thank you for participating, Baker %d! Better luck next in next year's bake off!\n", baker->id);
        break;
    }

    // printf("Baker %d has completed the competition & came in %d (st,nd,rd) place!\n", baker->id, total_completed); // Old way, Debugging and logic Vaildation.

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

    // Print messages to user and to start the competition.
    printf("Welcome to the 2024 CS452 Bake Off!\n");
    printf("Enter the number of bakers for competition: \n");

    // Get the user input for the number of bakers.
    scanf("%d", &total_bakers);

    // Used to finish the program and clean up after resources are no longer needed.
    // i.e the competition is over.
    int total_to_complete = total_bakers;

    // Initialize Structs for Bakers information.
    Baker bakers[total_bakers];

    // Initialize Threads for the Bakers.
    pthread_t bakerThreads[total_bakers];

    // Seed the random number generator with the current time
    srand(time(NULL));

    // Our min and max for our random number.
    int min = 0;
    int max = total_bakers;

    // Generate a random number between 0 and RAND_MAX
    int random_num = min + rand() % (max - min + 1);

    // Create and Initialize Bakers Threads and combining them.
    for (int i = 0; i < total_bakers; i++)
    {
        // Give our baker their id based on the order they are created.
        // Ex. First baker is i=0 which is is our first baker and we add 1 to give it the correct id number.
        bakers[i].id = i + 1;
        // We can pick one baker to be ramsied later on, using the random_num set up init above.
        // This only chooses one baker to be ramsied.
        if (i == random_num)
        {
            // The baker won't be ramsied right away but will be later on.
            bakers[i].ramsied = true;
            printf("Baker %d will be ramsied later on...\n", bakers[i].id);
        }
        else
        {
            // Every other baker will not be ramsied.
            bakers[i].ramsied = false;
        }
        // Create our bakers using all the information we created.
        //              Baker Thread,   NULL,  function to work on, bakerID
        pthread_create(&bakerThreads[i], NULL, kitchen, (void *)&bakers[i]);

        // EXPERIMENT: If we uncommenet this, then each baker will wait for each other to complete.
        // This isn't really want we want as we want to simulate a competition of resources.
        // pthread_join(bakerThreads[i], NULL);
    }

    while (total_completed < total_to_complete)
        ;
    // printf("total_completed: %d, total_to_complete: %d\n", total_completed, total_to_complete); Debugging and Value Validation

    printf("All bakers have completed their tasks!\n");

    // Clean up!!
    sem_destroy(&mixer);
    sem_destroy(&pantry);
    sem_destroy(&refrigerator);
    sem_destroy(&bowl);
    sem_destroy(&spoon);
    sem_destroy(&oven);

    return 0;
}
