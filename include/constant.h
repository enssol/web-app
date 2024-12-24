#ifndef CONSTANTS_H
#define CONSTANTS_H
/* web_server.h */
#define MAX_BUFFER_SIZE 4096
#define AUTH_FILE "./etc/auth.passwd"
#define DEFAULT_PORT 8080
#define WWW_ROOT "./www"
#define ACTION_LOGIN "User login"
#define ACTION_LOGOUT "User logout"
#define ACTION_VIEW_PROFILE "Viewed profile"
#define ACTION_UPDATE_PROFILE "Updated profile"
#define ACTION_VIEW_PROJECT "Viewed project"
#define ACTION_MANAGE_USERS "Managed users"
#define UNUSED(x) ((void)(x))

int check_auth(const char *username, const char *password);
int setup_server(int port);
int handle_client(int client_socket, const char *www_root);
int handle_users_request(int client_socket);
int handle_update_user(int client_socket, const char *username, const char *fullname, const char *email, const char *project);
int log_audit(const char *username, const char *action);
void parse_query_string(const char *query, char *username, char *password);

#endif
