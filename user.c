#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "user.h"

#define INITIAL_KEYWORDS 5
#define INITAL_N_USERS 5
#define INITIAL_KEYWORD_LENGTH 30

User* new_user(int id){
    User *user = calloc(1, sizeof *user);
    assert(user);
    user->id = id;
    user->n_capacity = INITIAL_KEYWORDS;
    user->n_keywords = 0;
    user->status = WAIT;
    user->keywords = calloc(user->n_capacity,sizeof(char*));
    user->round = 0;
    assert(user->keywords);
    return user;
}

User_list* initialise_player_list(){
    User_list *users = calloc(1, sizeof(User_list));
    assert(users);
    users->n_users = 0;
    users->users = calloc(INITAL_N_USERS, sizeof(User*));
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
    for(int j=0; j<user->n_keywords; j++){
        printf("%d: %s\n",j, user->keywords[j]);
    }
}


char* add_keyword(int id, User_list* users, char* query){
    const char s[2] = "&";
    char * curr = query;
    char * token = calloc(INITIAL_KEYWORD_LENGTH, sizeof(char));
    token[0] = '\0';
    assert(token);
    curr += 8;
    strcat(token, strtok(curr, s));
    printf("the keywod is %s\n", token);
    for(int i=0; i < users->n_users; i++){
        if(users->users[i]->id == id){
            resize_keywords(users->users[i], token);
        }
    }
    return token;
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

void change_player_round(int user_id, User_list* users){
    for(int i=0; i < users->n_users; i++){
        if (users->users[i]->id == user_id){
            if (users->users[i]->round < 4){
                users->users[i]->round++;
            }
            else{
                users->users[i]->round = 1;
            }
        }
    }
}


bool keyword_match(User* user, char* keyword){
    printf("keyword is %s\n\n", keyword);
    for(int i=0; i <user->n_keywords;i++){
        printf("words being matched is with %s and %s\n", keyword, user->keywords[i]);
        if(strcmp(keyword, user->keywords[i]) == 0){
            printf("match is with %s and %s\n", keyword, user->keywords[i]);
            return true;
        }
    }
    return false;
}

void reset_players(User_list *users){
    for(int i=0; i< users->n_users;i++){
        users->users[i]->n_keywords = 0;
    }
}

bool has_match_ended(User_list* users, char* keyword, int id){
    for(int i=0; i< users->n_users;i++){
        if(users->users[i]->status == READY && id != users->users[i]->id){
            if(keyword_match(users->users[i], keyword)){
                reset_players(users);
                return true;
            }
        }
    }
    return false;
}

bool player_won(User_list *users){
    for(int i=0; i < users->n_users; i++){
        if (users->users[i]->status == COMPLETE){
            return true;
        }
    }
    return false;
}

User* get_current_user(User_list* users, char* keyword, int id){
    User *user = NULL;
    for(int i=0; i < users->n_users; i++){
        if (users->users[i]->id == id){
            user = users->users[i];
        }
    }
    return user;
}

char* return_all_keywords(User* user){
    //RETURNS ADDRESS OF LOCAL VARIABLE
    char *keywords = calloc(user->n_keywords*(INITIAL_KEYWORD_LENGTH+1), user->n_keywords);
    assert(keywords);
    for(int i=0; i< user->n_keywords; i++){
        strcat(keywords, user->keywords[i]);
        strcat(keywords,",");
    }
    keywords[strlen(keywords)-1] = '\0';
    printf("**** %s \n   ", keywords);
    return keywords;
}