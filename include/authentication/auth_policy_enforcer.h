/* filepath: /home/appuser/web-app/include/authentication/auth_policy_enforcer.h */
#ifndef AUTH_POLICY_ENFORCER_H
#define AUTH_POLICY_ENFORCER_H

#include "../standards.h"

/* Function prototypes */
int check_user_permission(const char *username, const char *resource);
int enforce_access_policy(const char *username, const char *action);
int validate_user_role(const char *username, const char *required_role);

#endif /* AUTH_POLICY_ENFORCER_H */
