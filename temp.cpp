#include "temp.h"

using namespace std;
using namespace libpawdfox;

PawdFox::PawdFox() {
}

int PawdFox::OpenIni(const string &path) {
    firefox_profile p;
    p.path = "/path1/";
    p.name = "Path1";
    p.password = "";
    profiles.push_back(p);
    firefox_profile p2;
    p2.path = "/test2/";
    p2.name = "Path2";
    p2.password = "";
    profiles.push_back(p2);
    firefox_profile p3;
    p3.path = "/test3/";
    p3.name = "Path3";
    p3.password = "";
    profiles.push_back(p3);
    return LIBPAWD_STATUS_SUCCESS;
}

int PawdFox::ReadLogins(const firefox_profile profile) {
    if (profile.name == "Path2" && profile.password != "senha") {
        return LIBPAWD_STATUS_WRONG_PASSWORD;
    }
    firefox_credential credential;
    credential.hostname = "http://domain1.com/";
    credential.encrypted_username = "myUserName";
    credential.username = "myUsername";
    credential.encrypted_password = "encPassword";
    credentials.push_back(credential);
    if (profile.path == "test3")
    {
        firefox_credential credential2;
        credential2.hostname = "http://domain2.com/";
        credential2.encrypted_username = "Name";
        credential2.username = "Name";
        credential2.encrypted_password = "Password";
        credentials.push_back(credential2);
    }
    return LIBPAWD_STATUS_SUCCESS;
}

int PawdFox::CloseProfile() {
    return 0;
}

vector<firefox_credential> PawdFox::Filter(const string query) {
    vector<firefox_credential> ret;
    for (std::vector<firefox_credential>::const_iterator iterator = credentials.begin(),
                end = credentials.end(); iterator != end; ++iterator) {
        if (iterator->hostname.find(query) != string::npos ||
                iterator->username.find(query) != string::npos) {
            ret.push_back(*iterator);
        }
    }
    return ret;
}

string PawdFox::GetPassword(const firefox_credential credential) {
    return "senha";
}

string PawdFox::GetPassword(const string encrypted_password) {
    return "senha";
}
