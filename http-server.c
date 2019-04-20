/***
 * Written originally by COMP30023 for lab 6 but modified by Abhisha Nirmalathas 913405 for Project 1
 * Builds a http server, using a persistent TCP connection to create an image tagging game.
 * 
 * */

#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <signal.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/select.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "response-formatter.h"
#include "http-parser.h"
#include "http-response.h"
#include "hashtable.h"
#include "user.h"


// constants
static char const * const HTTP_200_FORMAT = "HTTP/1.1 200 OK\r\n\
Content-Type: text/html\r\n\
Content-Length: %ld\r\n\r\n";
static char const * const HTTP_400 = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
static int const HTTP_400_LENGTH = 47;
static char const * const HTTP_404 = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
static int const HTTP_404_LENGTH = 45;

int keep_alive = 1;


bool player_session(char* buff, int sockfd, char* file_name, char* response){
    /**
     * Sends response which creates a cookie for user
     * */
    struct stat st;
    stat(file_name, &st);
    int n = sprintf(buff, response, st.st_size);
    // send the header first
    if (write(sockfd, buff, n) < 0)
    {
        perror("write");
        return false;
    }
    // send the file
    int filefd = open(file_name, O_RDONLY);
    do
    {
        n = sendfile(sockfd, filefd, NULL, 2048);
    }
    while (n > 0);
    if (n < 0)
    {
        perror("sendfile");
        close(filefd);
        return false;
    }
    close(filefd);
    return true;
}
bool text_render_response(char *buff, int sockfd, char* file_name, char* text){
    /**
     * Sends a response which adds text to the html webpage
     * */
    char *user_name = calloc(200,sizeof(char));
    assert(user_name);
    strcpy(user_name, text);
    struct stat st;
    stat(file_name, &st);
    // increase file size to accommodate the username
    long size = st.st_size + strlen(user_name);
    int n = sprintf(buff, HTTP_200_FORMAT, size);
    if (write(sockfd, buff, n) < 0)
    {
        perror("write");
        return false;
    }
    // read the content of the HTML file
    int filefd = open(file_name, O_RDONLY);
    n = read(filefd, buff, size);
    // adds text to the response html
    render_text(buff, user_name);
    clean_trailing_buffer(buff);
    if (n < 0)
    {
        perror("read");
        close(filefd);
        return false;
    }
    close(filefd);
    free(user_name);
    if (write(sockfd, buff, size) < 0)
    {
        perror("write");
        return false;
    }
	return true;
}

bool response(char *buff, int sockfd, char* file_name){
    /**
     * Sends a response with the given webpage
     * */
    struct stat st;
    stat(file_name, &st);
    int n = sprintf(buff, HTTP_200_FORMAT, st.st_size);
    // send the header first
    if (write(sockfd, buff, n) < 0)
    {
        perror("write");
        return false;
    }
    // read the content of the HTML file
    int filefd = open(file_name, O_RDONLY);
    n = read(filefd, buff, st.st_size);
    clean_trailing_buffer(buff);
    if (n < 0)
    {
        perror("read");
        close(filefd);
        return false;
    }
    close(filefd);
    if (write(sockfd, buff, st.st_size) < 0)
    {
        perror("write");
        return false;
    }
	return true;
}

bool game_change(char* buff, int sockfd, char* file_name, int round){
    /**
     * Sends a response with the given webpage, ensuring that the right game image is rendered
     * depending on users current round
     * */
    struct stat st;
    stat(file_name, &st);
    int n = sprintf(buff, HTTP_200_FORMAT, st.st_size);
    // send the header first
    if (write(sockfd, buff, n) < 0)
    {
        perror("write");
        return false;
    }
    // read the content of the HTML file
    int filefd = open(file_name, O_RDONLY);
    n = read(filefd, buff, st.st_size);
    // changes the image of response html
    change_game_image(buff, round);
    clean_trailing_buffer(buff);
    if (n < 0)
    {
        perror("read");
        close(filefd);
        return false;
    }
    close(filefd);
    if (write(sockfd, buff, st.st_size) < 0)
    {
        perror("write");
        return false;
    }
	return true;
}
bool text_render_game_play(char *buff, int sockfd, char* file_name, char* text, int round){
    /**
     * Sends a response with the given webpage, ensuring that the right game image is rendered
     * depending on users current round, whilst rendering some text on game page
     * */
    char *user_name = calloc(200,sizeof(char));
    assert(user_name);
    strcpy(user_name, text);
    struct stat st;
    stat(file_name, &st);
    // increase file size to accommodate the username
    long size = st.st_size + strlen(user_name);
    int n = sprintf(buff, HTTP_200_FORMAT, size);
    if (write(sockfd, buff, n) < 0)
    {
        perror("write");
        return false;
    }
    // read the content of the HTML file
    int filefd = open(file_name, O_RDONLY);
    n = read(filefd, buff, size);
    // change image to match current round
    change_game_image(buff, round);
    // add the text to response html
    render_text(buff, user_name);
    clean_trailing_buffer(buff);
    if (n < 0)
    {
        perror("read");
        close(filefd);
        return false;
    }
    close(filefd);
    // move the trailing part backward
    free(user_name);
    if (write(sockfd, buff, size) < 0)
    {
        perror("write");
        return false;
    }
	return true;
}

static bool handle_http_request(int sockfd, User_list *users){
    /**
     * Parses the http request and sends the approapriate response
     * */
    char buff[2049];
    memset(buff, '\0', 2049);
    int n = read(sockfd, buff, 2049);
    if (n <= 0)
    {
        if (n < 0)
            perror("read");
        else
            // printf("socket %d close the connection\n", sockfd);
        return false;
    }

    // terminate the string
    buff[n] = 0;

    char * curr = buff;
    // parse the method
    Request* req = parse_request(curr);
    User* user = get_current_user(users, sockfd);
    // printf("REQUEST BODY IS \n\n%s\n", req->url);

    // user should quit game
    if(strncmp(req->body, "quit=Quit", 9)  == 0){
        printf("run\n\n\n\n\\n");
        change_player_status(sockfd,users, QUIT);
        response(buff,sockfd, "7_gameover.html");
        // free_request(req);
        // return false;
    }

    // user starting game 
    else if (strncmp(req->url, "/?start=Start", 24)  == 0){
        
        // user on first turn
        if (req->method == GET){
            change_player_status(sockfd, users, READY);
            int round = change_player_round(sockfd, users);
            game_change(buff,sockfd, "3_first_turn.html", round);
        }
        if (req->method == POST){
            if(strncmp(req->body, "keyword=", 8)  == 0){

                // checks if other player has quit
                if(should_player_quit(users)){
                    printf("is quit");
                    change_player_status(sockfd,users, QUIT);
                    response(buff,sockfd, "7_gameover.html");
                    // free_request(req);
                    // keep_alive = 0;
                    // return false;
                }

                // checks if other player has not completed previous round
                else if (different_round_discard(sockfd,users)){
                    game_change(buff,sockfd, "5_discarded.html", user->round);
                }

                // checks if game won
                else if(player_won(users)){
                    response(buff,sockfd, "6_endgame.html");
                    change_player_status(sockfd, users, WAIT);
                }

                //checks if the other player is not ready and discards keyword
                else if(!players_ready(users) && user!= NULL){
                    game_change(buff,sockfd, "5_discarded.html", user->round);
                    //request(buff,sockfd, "5_discarded.html");
                }
                else{
                    char* keyword = add_keyword(sockfd, users, req->body);

                    // game ends as submitted keyword has been submitted by other player
                    if(has_match_ended(users, keyword, sockfd)){
                        response(buff,sockfd, "6_endgame.html");
                        change_all_status(users, COMPLETE);
                    }
                    else{
                        // keyword hasn't been submitted yet so is accepted and rendered on screen
                        if(user!= NULL){
                            char* keywords = return_all_keywords(user);
                            text_render_game_play(buff,sockfd, "4_accepted.html", keywords, user->round);
                            free(keywords);
                        }
                    }
                }
            }
        }
    }

    // game start screen
    else if (*req->url == '/' && (strlen(req->url) == 1)){

        // user's name stored and rendered on screen
        if (req->method == POST)
        {
           char *name = strchr(req->body, '=')+1;
            // printf("**%s**\n", name);
            if (name != NULL){
                 for (int i=0; i < users->n_users; i++){
                     if(users->users[i]->id == sockfd){
                         users->users[i]->name = name;
                     }
                 }
                // get_request(buff,sockfd, "2_start.html");
               text_render_response(buff,sockfd, "2_start.html", req->body+5);
            }
        }

        // user is created
        else if (req->method == GET)
        {
            // printf("matches a / url but url isize is %zu\n\n", strlen(req->url));
            // Response* resp = initialise_session(req);
            // char* resp_string = parse_response(resp);
            // printf("COOKIE CREATING RESP %s\n", resp_string);
            // player_session(buff, sockfd, "1_welcome.html", resp_string);
            // User* new_player = new_user(sockfd);
            // add_user(new_player, users);
            // char* cookie = hash_table_get(resp->header, "Set-cookie: ")+13;
            // cookie = strtok(cookie, ";");
            // printf("the cookie token is %s*****\n", cookie);
            // free(resp_string);
            // free_response(resp); 

            // ensures user is not already playing/in system
            for(int i =0; i< users->n_users; i++){
                if(sockfd == users->users[i]->id){
                    return false;
                }
            }
            User* new_player = new_user(sockfd);
            add_user(new_player, users);
            response(buff,sockfd, "1_welcome.html");
        }
        else
            // unrecognised methods
            fprintf(stderr, "no other methods supported");   
    } 
    // send 404	
    else if (write(sockfd, HTTP_404, HTTP_404_LENGTH) < 0)
    {
        free_request(req);
        perror("write");
        return false;
    }
    printf("the numer of users is %d\n", users->n_users);
    for(int i=0; i < users->n_users; i++){
        printf("USER ID %d", users->users[i]->id);
        if(users->users[i]->status == READY){
            printf("is ready\n");
        }
        if(users->users[i]->status == WAIT){
            printf("is wait\n");
        }
        if(users->users[i]->status == QUIT){
            printf("is quit\n");
        }
        if(users->users[i]->status == COMPLETE){
            printf("is complete\n");
        }
        if(users->users[i]->status == RESTART){
            printf("is restart\n");
        }
    }
	free_request(req);
    return true;
}

static void exit_handler(int sig){
    /**
     * Closes TCP Connection
    */
    keep_alive = 0;
    printf("keep alive changed");
}



int main(int argc, char * argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "usage: %s ip port\n", argv[0]);
        return 0;
    }

    // create TCP socket which only accept IPv4
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // reuse the socket if possible
    int const reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // create and initialise address we will listen on
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // if ip parameter is not specified
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    // bind address to socket
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // listen on the socket
    listen(sockfd, 5);

    // initialise an active file descriptors set
    fd_set masterfds;
    FD_ZERO(&masterfds);
    FD_SET(sockfd, &masterfds);
    // record the maximum socket number
    int maxfd = sockfd;

    // game players
    User_list* users = initialise_player_list();
    signal(SIGINT, exit_handler);
    while (keep_alive)
    {
        // monitor file descriptors
        fd_set readfds = masterfds;
        if (select(FD_SETSIZE, &readfds, NULL, NULL, NULL) < 0)
        {
            perror("select");
            exit(EXIT_FAILURE);
        }
        // loop all possible descriptor
        for (int i = 0; i <= maxfd; ++i){
            // determine if the current file descriptor is active
            if (FD_ISSET(i, &readfds))
            {
                // create new socket if there is new incoming connection request
                if (i == sockfd)
                {
                    struct sockaddr_in cliaddr;
                    socklen_t clilen = sizeof(cliaddr);
                    int newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
                    if (newsockfd < 0)
                        perror("accept");
                    else
                    {
                        // add the socket to the set
                        FD_SET(newsockfd, &masterfds);
                        // update the maximum tracker
                        if (newsockfd > maxfd)
                            maxfd = newsockfd;
                        
                        // print out the IP and the socket number
                        char ip[INET_ADDRSTRLEN];
                        printf(
                            "new connection from %s on socket %d\n",
                            // convert to human readable string
                            inet_ntop(cliaddr.sin_family, &cliaddr.sin_addr, ip, INET_ADDRSTRLEN),
                            newsockfd
                        );
                    }
                }
                // a request is sent from the client
                else if (!handle_http_request(i,users))
                {
                    close(i);
                    FD_CLR(i, &masterfds);
                }
            }
        }
    }
    printf("break loop");
    free_users(users);
    return 0;
}
