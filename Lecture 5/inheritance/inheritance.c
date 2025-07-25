// Simulate genetic inheritance of blood type
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Each person has two parents and two alleles
typedef struct person
{
    struct person *parents[2];
    char alleles[2];
} person;

// ---------------------------------
// My code starts here
typedef unsigned int u_int;
// My code ends here
// ---------------------------------

const int GENERATIONS = 3;
const int INDENT_LENGTH = 4;

person *create_family(int generations);
void print_family(person *p, int generation);
void free_family(person *p);
// ---------------------------------
// My code starts here
void AssignRandomAllelesBasedOnParents(person *pointerToPerson);
// My code ends here
// ---------------------------------
char random_allele();

int main(void)
{
    // Seed random number generator
    srand(time(0));

    // Create a new family with three generations
    person *p = create_family(GENERATIONS);

    // Print family tree of blood types
    print_family(p, 0);

    // Free memory
    free_family(p);
}

// Create a new individual with `generations`
person* create_family(int generations)
{
    // ---------------------------------
    // My code starts here
    person *child = malloc(sizeof(person));

    if (child == NULL)
    {
        return NULL;
    }
    // My code ends here
    // ---------------------------------

    // If there are still generations left to create
    if (generations > 1)
    {
        // Create two new parents for current person by recursively calling create_family
        person *parent0 = create_family(generations - 1);
        person *parent1 = create_family(generations - 1);

        // ---------------------------------
        // My code starts here
        child->parents[0] = parent0;
        child->parents[1] = parent1;

        AssignRandomAllelesBasedOnParents(child);
        // My code ends here
        // ---------------------------------
    }

    // If there are no generations left to create
    else
    {
        // ---------------------------------
        // My code starts here
        child->parents[0] = NULL;
        child->parents[1] = NULL;

        child->alleles[0] = random_allele();
        child->alleles[1] = random_allele();
        // My code ends here
        // ---------------------------------
    }

    // ---------------------------------
    // My code starts here
    return child;
    // My code ends here
    // ---------------------------------
}

// Free `p` and all ancestors of `p`.
void free_family(person *p)
{
    // ---------------------------------
    // My code starts here
    if (p == NULL)
    {
        return;
    }


    if (p->parents[0]->parents[0] == NULL && p->parents[1]->parents[1] == NULL)
    {
        free(p->parents[0]);
        p->parents[0] = NULL;
        free(p->parents[1]);
        p->parents[1] = NULL;
    }
    else
    {
        free_family(p->parents[0]);
        free_family(p->parents[1]);
    }

    free(p);
    p = NULL;
    // My code ends here
    // ---------------------------------
}

// Print each family member and their alleles.
void print_family(person *p, int generation)
{
    // Handle base case
    if (p == NULL)
    {
        return;
    }

    // Print indentation
    for (int i = 0; i < generation * INDENT_LENGTH; i++)
    {
        printf(" ");
    }

    // Print person
    if (generation == 0)
    {
        printf("Child (Generation %i): blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }
    else if (generation == 1)
    {
        printf("Parent (Generation %i): blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }
    else
    {
        for (int i = 0; i < generation - 2; i++)
        {
            printf("Great-");
        }
        printf("Grandparent (Generation %i): blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }

    // Print parents of current generation
    print_family(p->parents[0], generation + 1);
    print_family(p->parents[1], generation + 1);
}

// ---------------------------------
// My code starts here
void AssignRandomAllelesBasedOnParents(person *pointerToPerson)
{
    u_int firstParentAllele, secondParentAllele;
    u_int randomNumber = rand() % 2;

    if (randomNumber == 0)
    {
        firstParentAllele = 0;
        secondParentAllele = 1;
    }
    else
    {
        firstParentAllele = 1;
        secondParentAllele = 0;
    }

    pointerToPerson->alleles[0] = pointerToPerson->parents[0]->alleles[firstParentAllele];
    pointerToPerson->alleles[1] = pointerToPerson->parents[1]->alleles[secondParentAllele];
}
// My code ends here
// ---------------------------------

// Randomly chooses a blood type allele.
char random_allele()
{
    int r = rand() % 3;
    if (r == 0)
    {
        return 'A';
    }
    else if (r == 1)
    {
        return 'B';
    }
    else
    {
        return 'O';
    }
}
