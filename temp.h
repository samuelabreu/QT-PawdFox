#ifndef TEMP_H
#define TEMP_H

#include <vector>
#include <string>

#define LIBPAWD_STATUS_SUCCESS 0
#define LIBPAWD_STATUS_UNKNOWN -1
#define LIBPAWD_STATUS_FILE_DOESNT_EXIST -2
#define LIBPAWD_STATUS_DECRYPT_FAILED -3
#define LIBPAWD_STATUS_WRONG_PASSWORD -4
#define LIBPAWD_STATUS_FAIL_SHUTDOWN -5

namespace libpawdfox {
    struct firefox_profile {
        std::string name;
        std::string path;
        std::string password;
    };

    struct firefox_credential {
        std::string encrypted_username;
        std::string encrypted_password;
        std::string username;
        std::string hostname;
        std::string username_field;
        std::string password_field;
        int enc_type;
        int64_t time_created;
        int64_t time_last_used;
        int64_t time_password_changed;
        int times_used;
    };

    const std::string emptyString = std::string();

    class PawdFox {

    public:
        PawdFox();
        std::vector<firefox_profile> profiles;
        std::vector<firefox_credential> credentials;
        int OpenIni(const std::string &path = emptyString);
        int ReadLogins(const firefox_profile profile);
        int CloseProfile();
        std::vector<firefox_credential> Filter(const std::string query);
        std::string GetPassword(const firefox_credential cretential);
        std::string GetPassword(const std::string encrypted_password);
    };
}

#endif // TEMP_H
