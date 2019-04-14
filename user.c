#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "user.h"


User* new_user(int id){
    User *user = malloc(sizeof *user);
    assert(user);
    user->id = id;
    user->n_capacity = 5;
    user->n_keywords = 0;
    user->status = WAIT;
    user->keywords = malloc(sizeof(char*)*user->n_capacity);
    assert(user->keywords);
    return user;
}

User_list* initialise_player_list(){
    User_list *users = malloc(sizeof(User_list));
    assert(users);
    users->n_users = 0;
    users->users = malloc(sizeof(User*)*5);
    assert(users->users);
    return users;
}

void resize_keywords(User* user, char* keyword){
    if(user->n_capacity == user->n_keywords){
        user->n_capacity *= 2;
        user->keywords = realloc(user->keywords,sizeof(char*) * (user->n_capacity));
    }
    user->keywords[user->n_keywords] = keyword;
    user->n_keywords++;
}


void add_keyword(int id, User_list* users, char* query){
    const char s[2] = "&";
    char * curr = query;
    char * token;
    curr += 8;
    token = strtok(curr, s);
    printf("the keywod is %s\n", token);
    for(int i=0; i < users->n_users; i++){
        if(users->users[i]->id == id){
            resize_keywords(users->users[i], token);
        }
    }
}

void add_user(User* user, User_list* users){
    users->users[users->n_users] = user;
    users->n_users++;
}

void free_users(User_list* users){
    for (int i=0; i <users->n_users; i++){
        free(users->users[i]->keywords);
        free(users->users[i]);
    }
    free(users->users);
    free(users);
}

bool is_players_ready(User_list* users){
    for(int i=0; i < users->n_users; i++){
        if (users->users[i]->status != READY){
            return false;
        }
    }
    return true;
}

bool should_player_quit(User_list* users){
    for(int i=0; i < users->n_users; i++){
        if (users->users[i]->status == QUIT){
            return true;
        }
    }
    return false;
}
bool players_ready(User_list* users){
    for(int i=0; i < users->n_users; i++){
        if (users->users[i]->status != READY){
            return false;
        }
    }
    return true;
}
void change_player_status(int user_id, User_list* users, STATUS status){
    for(int i=0; i < users->n_users; i++){
        if (users->users[i]->id == user_id){
            users->users[i]->status = status;
        }
    }
}