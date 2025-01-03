/* filepath: /home/appuser/web-app/include/authentication/auth_token_validator.h */
#ifndef AUTH_TOKEN_VALIDATOR_H
#define AUTH_TOKEN_VALIDATOR_H

#include "../standards.h"

/* Function prototypes */
int validate_token(const char *token);
int check_token_expiry(const char *token);
int revoke_token(const char *token);

#endif /* AUTH_TOKEN_VALIDATOR_H */
