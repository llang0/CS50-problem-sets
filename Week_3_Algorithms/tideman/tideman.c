#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

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
void lock_pairs(void);
void print_winner(void);
bool dfs(int loser, int winner, bool v[]);

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
    // look for candidate called name
    for (int i = 0; i < candidate_count; i++)
    {
        // if candidate is found, update ranks and return true
        if (strcmp(candidates[i], name) == 0)
        {
            // ranks[n] is the voters preference
            ranks[rank] = i;
            return true;
        }
    }
    // if no candidate is found, return false
    return false;
}

// Update preferences 2D array given one voter's ranks
void record_preferences(int ranks[])
{
    // cycle through ranks, length of ranks is candidate_count
    // we can stop at n-1, because candidate ranks[n] is preferred over no one
    for (int i = 0; i < candidate_count - 1; i++)
    {
        // cycle through ranks[i+1] - ranks[n]
        // candidate i is preferred over every candidate in ranks[i+1] - ranks[n]
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Add each pair of candidates to pairs array, as long as one candidate is
    // preferred over the other

    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // for each pair of candidates, compare their votes
            // in preferenes array to determine the winner
            int i_votes = preferences[i][j];
            int j_votes = preferences[j][i];

            if (i_votes > j_votes)
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (i_votes < j_votes)
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // bubble sort
    // Repeat n-1 times
    for (int i = 0; i < pair_count - 1; i++)
    {
        // for j from 0 to n-2
        int swaps = 0;
        for (int j = 0; j < pair_count - 2; j++)
        {
            // find the strength of pairs j -> a and j+1 -> b
            int a = preferences[pairs[j].winner][pairs[j].loser];
            int b = preferences[pairs[j + 1].winner][pairs[j + 1].loser];
            // if a and b out of order
            if (b > a)
            {
                // swap them
                pair temp = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = temp;
                swaps++;
            }
        }
        if (swaps == 0)
        {
            return;
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Add a pair to locked pairs
    // Remove pair if there is a cycle 

    // for pair in pairs
    for (int i = 0; i < pair_count; i++)
    {
        pair next = pairs[i];
        // Add pair
        locked[next.winner][next.loser] = true;

        // check for cycles
        bool visited[candidate_count];
        for (int h = 0; h < candidate_count; h++)
        {
            visited[h] = false;
        }
        // if theres a cycle
        bool cycle = dfs(next.loser, next.winner, visited);
        if (cycle)
        {
            // remove pair
            locked[next.winner][next.loser] = false;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // for every candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // temporarily mark candidate winner
        bool winner = true;

        // look for any edge that points to that candidate

        // iterate over every opponent (j) and
        // look for an edge from opponent(j) to candidate(i)
        for (int j = 0; j < candidate_count; j++)
        {
            
            bool visited[candidate_count];
            for (int h = 0; h < candidate_count; h++)
            {
                visited[h] = false;
            }
            // if dfs detects an edge pointing to candidate i
            if (dfs(j, i, visited) && i != j)
            {
                // mark not winner
                winner = false;
            }
        }
        // if, after running dfs for every other candidate...
        // candidate is still a winner
        if (winner)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }

    return;
}

bool dfs(int start, int end, bool v[])
{
    // helper function to check if an edge goes from start to finish

    // check for cycles by looking for an edge going from the loser of one pair
    // to the winner of the same pair

    // check for the winner by looking for edges
    // that go from any candidate to the potential winner

    v[start] = true;

    if (start == end)
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[start][i] && !v[i])
        {
            bool cycle = dfs(i, end, v);
            if (cycle)
            {
                return true;
            }
        }
    }
    return false;
}
