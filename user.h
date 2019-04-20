/**
 * Written by Abhisha Nirmalathas 913405 for COMP30023 Project 1
 * This program handles all clients/game players.
 * */
#include <stdbool.h>

// the different status' a user can have
typedef enum
{
    READY,
    WAIT,
    QUIT,
    COMPLETE,
    RESTART
} STATUS;


typedef struct User {
    char* name;
    int id;
    char** keywords;
    int n_keywords;
    STATUS status;
    int n_capacity;
    int round;
}User;

typedef struct User_list{
    int n_users;
    User **users;
}User_list;

// Creates a new user/client
User* new_user(int id);

// Initialises a list of clients/users
User_list* initialise_player_list();

// Adds keyword to the user
void resize_keywords(User* user, char* keyword);

// Parses submitted keyword and adds to the user
char* add_keyword(int id, User_list* users, char* query);

// Adds user and updates number of users
void add_user(User* user, User_list* users);

// Deallocate memory
void free_users(User_list* users);

// Checks if any player has quit, and tells player to quit if so 
bool should_player_quit(User_list* users);

// Checks if all players are ready
bool players_ready(User_list* users);

// Changes A players status
void change_player_status(int user_id, User_list* users, STATUS status);

// Changes ALL players status
void change_all_status(User_list* users, STATUS status);

// Changes players round based on current round
int change_player_round(int user_id, User_list* users);

// Checks if current player is trying to submit keyword on a different round
bool different_round_discard(int user_id, User_list* users);

// Checks if keyword has been submitted
bool keyword_match(User* user, char* keyword);

// Resets players keywords 
void reset_players(User_list *users);

// Checks if the round is completed
bool has_match_ended(User_list* users, char* keyword, int id);

// Checks if the game has been completed
bool player_won(User_list *users);

// Gets the user with matching id in list of users/players
User* get_current_user(User_list* users, int id);

// Returns all the keywords for a particular user
char* return_all_keywords(User* user);