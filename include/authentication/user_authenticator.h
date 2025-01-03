/* filepath: /home/appuser/web-app/include/authentication/user_authenticator.h */
#ifndef USER_AUTHENTICATOR_H
#define USER_AUTHENTICATOR_H

#include "../standards.h"

/* Function prototypes */
int check_auth(const char *username, const char *password);
int authenticate_user(const char *username, const char *password);
int verify_credentials(const char *username);
int create_user_session(const char *username);

#endif /* USER_AUTHENTICATOR_H */
