/**
 * Written by Abhisha Nirmalathas 913405 for COMP30023 Project 1
 * This program handles all clients/game players.
 * */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "user.h"

// implementation specific constants for users
#define INITIAL_KEYWORDS 5
#define INITAL_N_USERS 5
#define INITIAL_KEYWORD_LENGTH 30

User* new_user(int id){
    /**
     * Creates a new user/client
     * */
    User *user = calloc(1, sizeof *user);
    assert(user);
    user->id = id;
    user->n_capacity = INITIAL_KEYWORDS;
    user->n_keywords = 0;

    // user is set to WAIT by default
    user->status = WAIT;
    user->keywords = calloc(user->n_capacity,sizeof(char*));

    // user is on round 0 initially
    user->round = 0;
    assert(user->keywords);
    return user;
}

User_list* initialise_player_list(){
    /**
     * Initialises a list of clients/users
     * */
    User_list *user_list = calloc(1, sizeof(User_list));
    assert(user_list);
    user_list->n_users = 0;
    user_list->users = calloc(INITAL_N_USERS, sizeof(User*));
    assert(user_list->users);
    return user_list;
}

void resize_keywords(User* user, char* keyword){
    /**
     * Adds keyword to the user
     * */

    // adds more space if max keywords of user reached
    if(user->n_capacity == user->n_keywords){
        user->n_capacity *= 2;
        user->keywords = realloc(user->keywords,sizeof(char*) * (user->n_capacity));
    }
    // adds keyword to user
    user->keywords[user->n_keywords] = keyword;
    user->n_keywords++;
    for(int j=0; j<user->n_keywords; j++){
        printf("%d: %s\n",j, user->keywords[j]);
    }
}


char* add_keyword(int id, User_list* user_list, char* query){
    /**
     * Parses submitted keyword and adds to the user
     * */
    const char s[2] = "&";
    char * curr = query;
    char * token = calloc(INITIAL_KEYWORD_LENGTH, sizeof(char));
    token[0] = '\0';
    assert(token);

    // Skips keyword=
    curr += 8;

    // Parses keyword
    strcat(token, strtok(curr, s));
    for(int i=0; i < user_list->n_users; i++){
        if(user_list->users[i]->id == id){
            // adds keyword to user
            resize_keywords(user_list->users[i], token);
        }
    }
    return token;
}

void add_user(User* user, User_list* user_list){
    /**
     * Adds user to list of users
     * */
    // Adds user and updates number of users
    user_list->users[user_list->n_users] = user;
    user_list->n_users++;
}

void free_users(User_list* user_list){
    /**
     * Deallocate memory
     * */
    for (int i=0; i <user_list->n_users; i++){
        // for(int j=0; j < users->users[i]->n_keywords; j++){
        //     free(&users->users[i]->keywords[j]);
        // }
        free(user_list->users[i]->keywords);
        free(user_list->users[i]);
    }
    free(user_list->users);
    free(user_list);
}

bool should_player_quit(User_list* user_list){
    /**
     * Checks if any player has quit, and tells player to quit if so 
     * */
    for(int i=0; i < user_list->n_users; i++){
        if (user_list->users[i]->status == QUIT){
            return true;
        }
    }
    return false;
}
bool players_ready(User_list* user_list){
    /**
     * checks if all players are ready
     * */
    for(int i=0; i < user_list->n_users; i++){
        if (user_list->users[i]->status != READY){
            return false;
        }
    }
    return true;
}
void change_player_status(int user_id, User_list* user_list, STATUS status){
    /**
     * Changes A player's status
     * */
    for(int i=0; i < user_list->n_users; i++){
        if (user_list->users[i]->id == user_id){
            user_list->users[i]->status = status;
        }
    }
}

void change_all_status(User_list* user_list, STATUS status){
    /**
     * Changes all player's status
     * */
    for(int i=0; i < user_list->n_users; i++){
        user_list->users[i]->status = status;
    }
}

int change_player_round(int user_id, User_list* user_list){
    /**
     * Changes players round based on current round
     * */
    int round = 1;
    for(int i=0; i < user_list->n_users; i++){
        if (user_list->users[i]->id == user_id){
            if (user_list->users[i]->round < 4){
                user_list->users[i]->round++;
            }
            // resets round to 1
            else{
                user_list->users[i]->round = 1;
            }
            round = user_list->users[i]->round;
        }
    }return round;
}
bool different_round_discard(int user_id,User_list* user_list){
     /**
     * Checks if current player is on a different game round and
     * is trying to submit a keyword whilst previous player still
     * in previous round
     * */
    User *user = get_current_user(user_list, user_id);
    if (user != NULL && user->status ==READY){
        for(int i=1; i< user_list->n_users; i++){
            if(user_list->users[i]->round != user_list->users[i-1]->round){
                return true;
            }
        }
    }
    return false;
}

bool keyword_match(User* user, char* keyword){
    /**
     * Checks if keyword has already been submitted
     * */
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

void reset_players(User_list *user_list){
    /**
     * Resets players keywords 
     * */
    for(int i=0; i< user_list->n_users;i++){
        user_list->users[i]->n_keywords = 0;
    }
}

bool has_match_ended(User_list* user_list, char* keyword, int id){
    /**
     * Checks if the round is completed
     * */
    for(int i=0; i< user_list->n_users;i++){
        // Checks that players are READY to play
        if(user_list->users[i]->status == READY && id != user_list->users[i]->id){
            // checks if the keyword has been submitted
            if(keyword_match(user_list->users[i], keyword)){
                // game has been won and players reset
                reset_players(user_list);
                return true;
            }
        }
    }
    return false;
}

bool player_won(User_list *user_list){
    /**
     * Checks if the game has been completed
     * */
    for(int i=0; i < user_list->n_users; i++){
        if (user_list->users[i]->status == COMPLETE){
            return true;
        }
    }
    return false;
}

User* get_current_user(User_list* user_list, int id){
    /**
     * Gets the user with matching id in list of users/players
     * */
    User *user = NULL;
    for(int i=0; i < user_list->n_users; i++){
        if (user_list->users[i]->id == id){
            user = user_list->users[i];
        }
    }
    return user;
}

char* return_all_keywords(User* user){
    //RETURNS ADDRESS OF LOCAL VARIABLE
    /**
     * Returns all the keywords for a particular user
     * */
    printf("%d\n\n\n", user->n_keywords);
    
    int length = (user->n_keywords)*(INITIAL_KEYWORD_LENGTH+1);
    char *keywords = calloc(length, user->n_keywords);
    assert(keywords);

    // adds each keywords
    for(int i=0; i< user->n_keywords; i++){
        strcat(keywords, user->keywords[i]);
        strcat(keywords,",");
    }
    // terminating byte
    keywords[strlen(keywords)-1] = '\0';
    printf("**** %s \n   ", keywords);
    return keywords;
}