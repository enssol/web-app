#include "../../include/authentication/user_authenticator.h"
#include "../../include/constant.h"
#include "../../include/log_handler.h"
#include "../../include/standards.h"

int
check_auth(const char *username, const char *password)
{
    FILE *fp;
    char line[MAX_BUFFER_SIZE];
    char file_user[256];
    char file_pass[256];
    char *token;

    if (!username || !password) {
        return 0;
    }

    fp = fopen(AUTH_FILE, "r");
    if (!fp) {
        return 0;
    }

    while (fgets(line, sizeof(line), fp)) {
        /* Skip comments and empty lines */
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }

        /* Parse line - username is first field, password is second */
        token = strtok(line, ":");
        if (!token) {
            continue;
        }
        strncpy(file_user, token, sizeof(file_user) - 1);
        file_user[sizeof(file_user) - 1] = '\0';

        token = strtok(NULL, ":");
        if (!token) {
            continue;
        }
        strncpy(file_pass, token, sizeof(file_pass) - 1);
        file_pass[sizeof(file_pass) - 1] = '\0';

        /* Compare credentials */
        if (strcmp(username, file_user) == 0 &&
            strcmp(password, file_pass) == 0) {
            log_audit(username, ACTION_LOGIN);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}
