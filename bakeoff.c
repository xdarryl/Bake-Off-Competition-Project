#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

// Resource semaphores
sem_t mixer, pantry, refrigerator, bowl, spoon, oven;

// Structs to store ingredients
typedef enum {
	Flour,
	Sugar,
	Yeast,
	Baking_soda,
	Salt,
	Cinnamon
} PantryIngredient;

typedef enum {
	Eggs,
	Milk,
	Butter
} FridgeIngredient;

void gather_pantry_ingredients(int baker_id, PantryIngredient *needed_ingredients, int num_ingredients) {
    sem_wait(&pantry); // Lock access to the pantry
    printf("Baker %d is in the pantry gathering ingredients.\n", baker_id);

    for (int i = 0; i < num_ingredients; i++) {
        printf("Baker %d is taking %s from the pantry.\n", baker_id, 
               (needed_ingredients[i] == Flour) ? "Flour" :
               (needed_ingredients[i] == Sugar) ? "Sugar" :
               (needed_ingredients[i] == Yeast) ? "Yeast" :
               (needed_ingredients[i] == Baking_soda) ? "Baking Soda" :
               (needed_ingredients[i] == Salt) ? "Salt" :
               "Cinnamon");
    }
    sem_post(&pantry); // Release access to the pantry
}

void gather_fridge_ingredients(int baker_id, FridgeIngredient *needed_ingredients, int num_ingredients) {
    sem_wait(&refrigerator); // Lock access to the refrigerator
    printf("Baker %d is in the refrigerator gathering ingredients.\n", baker_id);

    for (int i = 0; i < num_ingredients; i++) {
        printf("Baker %d is taking %s from the refrigerator.\n", baker_id, 
               (needed_ingredients[i] == Eggs) ? "Eggs" :
               (needed_ingredients[i] == Milk) ? "Milk" :
               "Butter");
    }

    sem_post(&refrigerator); // Release access to the refrigerator
}

void *runner(void *param);

int main(int argc, char* argv[])
{
	
	int total_bakers;
	
	printf("Welcome to the 2024 CS452 Bake Off!\n");
	printf("Enter the number of bakers for competition: \n");
	
	scanf("%d", &total_bakers);	
		
	// Initialize semaphores
    sem_init(&mixer, 0, 2);
    sem_init(&pantry, 0, 1);
    sem_init(&refrigerator, 0, 2);
    sem_init(&bowl, 0, 3);
    sem_init(&spoon, 0, 5);
    sem_init(&oven, 0, 1);

	// Initialize ingredients



	return 0;

}
