#include <stdbool.h>
typedef enum
{
    READY,
    WAIT
} STATUS;


typedef struct User {
    char* name;
    char** keywords;
    int n_keywords;
    STATUS status;
    int n_capacity;
}User;

typedef struct User_list{
    int n_users;
    User **users;
}User_list;

User* new_user();

User_list* initialise_player_list();

void add_user(User* user, User_list* users);

void free_users(User_list* users);

bool is_players_ready(User_list* users);