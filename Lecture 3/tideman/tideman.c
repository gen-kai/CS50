#include <cs50.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX] = { {0} };

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

bool isCyclic = false;

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
bool isCurrentPreferenceStronger(int currentIndex, int maxStrengthIndex);
void lock_pairs(void);
void print_winner(void);
bool DetectCycle(int i);
bool ModifiedDepthFirstSearch(int currentVertex, bool* visitedVerites, bool* finishedVertices,
    int edgeCount);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;

            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
            preferences[ranks[i]][ranks[j]] += 1;
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] != 0 && preferences[i][j] > preferences[j][i])
            {
                pair_count += 1;

                pairs[pair_count - 1].winner = i;
                pairs[pair_count - 1].loser = j;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count - 1; i++)
        // pair_count - 1 because we are comparing element[i] with element[j] and j is i + 1
    {
        int maxStrengthIndex = i;

        for (int j = i + 1; j < pair_count; j++)
            // i + 1 because we don't need to compare i with i
        {
            if (isCurrentPreferenceStronger(j, maxStrengthIndex) ||
                pairs[j].winner < pairs[i].winner)
                // sort by preference strength descending order and index number in ascending order
            {
                maxStrengthIndex = j;
            }
        }

        pair currentPair = pairs[i];
        pairs[i].winner = pairs[maxStrengthIndex].winner;
        pairs[i].loser = pairs[maxStrengthIndex].loser;

        pairs[maxStrengthIndex].winner = currentPair.winner;
        pairs[maxStrengthIndex].loser = currentPair.loser;
    }

    return;
}

bool isCurrentPreferenceStronger(int currentIndex, int maxStrengthIndex)
{
    int currentWinner = pairs[currentIndex].winner;
    int currentLoser = pairs[currentIndex].loser;
    int currentPreference = preferences[currentWinner][currentLoser];

    int maxStrengthWinner = pairs[maxStrengthIndex].winner;
    int maxStrengthLoser = pairs[maxStrengthIndex].loser;
    int maxStrengthPreference = preferences[maxStrengthWinner][maxStrengthLoser];

    return currentPreference > maxStrengthPreference;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;

        if (i > 1)
            // i = 0 can't cause cyclic graph cause the graph will only contain 1 edge
            // i = 1 can't cause cyclic graph cause pairs[] array doesn't contain equal preference pairs
        {
            if (DetectCycle(i))
            {
                locked[pairs[i].winner][pairs[i].loser] = false;
                isCyclic = false;
            }
            // if we created a cyclic graph with the most recent edge addition - remove that edge
            // and reset isCyclic var
        }
    }

    return;
}

bool DetectCycle(int pairCount)
{
    bool visitedVerites[MAX] = { false };
    bool finishedVertices[MAX] = { false };

    int currentVertex = pairs[pairCount].loser;

    return ModifiedDepthFirstSearch(currentVertex, &visitedVerites[0], &finishedVertices[0],
        pairCount);
}

bool ModifiedDepthFirstSearch(int currentVertex, bool* visitedVerites, bool* finishedVertices,
    int edgeCount)
{
    // function based on Depth-First Search algo that only check out-neighbors of the current vertex
    // it sets isCyclic to true if by checking out-neighbours it ends up checking the vertex that
    // was already checked we don't need the full Depth-First Search algo implementation because
    // we're creating the graph from 0 edge by edge
    if (finishedVertices[currentVertex])
    {
        return false;
    }

    if (visitedVerites[currentVertex])
    {
        return true;
    }

    visitedVerites[currentVertex] = true;

    for (int i = 0; i < edgeCount; i++)
    {
        if (pairs[i].winner == currentVertex)
        {
            ModifiedDepthFirstSearch(pairs[i].loser, visitedVerites, finishedVertices, edgeCount);
        }
    }

    finishedVertices[currentVertex] = true;

    return false;
}

// Print the winner of the election
void print_winner(void)
{
    bool electionWinners[MAX] = { false };

    for (int i = 0; i < pair_count; i++)
    {
        bool isWinner = true;

        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][pairs[i].winner])
            {
                isWinner = false;
            }
        }

        if (isWinner)
        {
            electionWinners[pairs[i].winner] = true;
        }
    }
    // check the resulting graph and populate the array with the vertices that don't have
    // in-neighbors

    for (int i = 0; i < candidate_count; i++)
    {
        if (electionWinners[i])
        {
            printf("%s\n", candidates[i]);
        }
    }
}
