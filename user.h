#include <stdbool.h>
typedef enum
{
    READY,
    WAIT,
    QUIT
} STATUS;


typedef struct User {
    char* name;
    int id;
    char** keywords;
    int n_keywords;
    STATUS status;
    int n_capacity;
}User;

typedef struct User_list{
    int n_users;
    User **users;
}User_list;

User* new_user(int id);

User_list* initialise_player_list();

void resize_keywords(User* user, char* keyword);

void add_keyword(int id, User_list* users, char* query);

void add_user(User* user, User_list* users);

void free_users(User_list* users);

bool is_players_ready(User_list* users);

bool should_player_quit(User_list* users);

bool players_ready(User_list* users);

void change_player_status(int user_id, User_list* users, STATUS status);