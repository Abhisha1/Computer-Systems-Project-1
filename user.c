#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "user.h"


User* new_user(char* id){
    User *user = malloc(sizeof(User*));
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
    users->users = malloc(sizeof(User*)*3);
    assert(users->users);
    return users;
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