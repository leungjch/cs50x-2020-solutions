#include <cs50.h>
#include <stdio.h>
#include <string.h>
// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);
void transfer_votes();

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // Search for the name and increase the vote count
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }

    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // loop through preferences and sum votes from the candidates
    for (int voter = 0; voter < voter_count; voter++)
    {
        for (int pref = 0; pref < candidate_count; pref++)
        {
            if (!candidates[preferences[voter][pref]].eliminated)
            {
                candidates[preferences[voter][pref]].votes += 1;
                // printf("candidate %s has %i votes\n", candidates[candidate].name, candidates[candidate].votes);
                break;
            }
        }

    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // printf("name: %s, votes: %i\n", candidates[i].name,candidates[i].votes);
        // printf("voterpct: %f\n", ((double)candidates[i].votes / (double)voter_count));
        // printf("%d\n", ( ((double) candidates[i].votes / (double) voter_count) > 0.5));
        if (((double) candidates[i].votes / (double) voter_count) > 0.5)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;

}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int min = candidates[0].votes;
    for (int candidate = 1; candidate < candidate_count; candidate++)
    {
        if (candidates[candidate].votes < min && !candidates[candidate].eliminated )
        {
            min = candidates[candidate].votes;
        }
    }
    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    for (int candidate = 0; candidate < candidate_count; candidate++)
    {
        if (candidates[candidate].votes != min && !candidates[candidate].eliminated)
        {
            return false;
        }
    }
    return true;
}

// Eliminate the candidate (or candidiates) in last place
void eliminate(int min)
{
    for (int candidate = 0; candidate < candidate_count; candidate++)
    {
        if (candidates[candidate].votes == min)
        {
            candidates[candidate].eliminated = true;
            // printf("%s is eliminated", candidates[candidate].name);
            // transfer_votes();
        }
    }

    return;
}

// update preferences after a candidate is eliminated
// void transfer_votes()
// {
//     for (int vot = 0; vot < voter_count; vot++)
//     {
//         // if top choice is eliminated, shift them to the bottom
//         if (candidates[preferences[vot][0]].eliminated)
//         {
//             int cand_elim = preferences[vot][0];
//             for (int prefs = 0; prefs < candidate_count-1 || candidates[preferences[vot][prefs+1]].eliminated; prefs++)
//             {
//                 for (int c = 0; c < candidate_count; c++)
//                 {
//                     printf("%i", preferences[vot][c]);
//                 }
//                 printf("\n");
//                 int temp = preferences[vot][prefs+1];
//                 preferences[vot][prefs+1] = cand_elim;
//                 preferences[vot][prefs] = temp;

//             }

//         }
//     }
// }
