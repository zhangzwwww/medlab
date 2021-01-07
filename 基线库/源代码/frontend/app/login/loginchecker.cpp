#include "loginchecker.h"

#include <regex>

LoginChecker::LoginChecker()
{

}

LoginChecker::~LoginChecker() {

}

ERROR_CODE LoginChecker::SignIn(QString id, QString pwd, communhttp *commu, userinfo *user) {
    if (id.isEmpty()) {
        return PARAM_INVALID;
    }
    if (pwd.isEmpty()) {
        return LOGIN_ERROR_PWD;
    }
    // try to log in the user
    if (user->log_user_in(commu, id, pwd)){
        return OK;
    }
    else {
        return LOGIN_ERROR_PWD;
    }
}

ERROR_CODE LoginChecker::SignUp(QString id, QString pwd, communhttp *commu, userinfo *user) {
    if (id.isEmpty()){
        return LOGIN_ID_NOT_FOUND;
    }
    if (pwd.isEmpty()){
        return LOGIN_ERROR_PWD;
    }
    // try to register in the user
    if (user->register_user_info(commu, id, pwd)){
        return OK;
    }
    return PARAM_INVALID;
}

ERROR_CODE LoginChecker::RetrievePwd(QString id) {
    return OK;
}

bool LoginChecker::IsValidID(QString id) {
//    TODO: Check use email as id?
//    std::regex id_regex("^[a-zA-Z0-9_-@.]{1,32}$");
    if (id.length() > 64) {
        return false;
    }
    std::regex id_regex("^[a-zA-Z0-9_-]{1,32}@[a-zA-Z0-9_-]+(\\.[a-zA-Z0-9_-]+)+$");
    return std::regex_match(id.toStdString(), id_regex);
}

bool LoginChecker::IsValidPwd(QString pwd) {
//    std::regex pwd_regex("^.*(?=.{6,})(?=.*\d)(?=.*[A-Z])(?=.*[a-z])(?=.*[!@#$%^&*? ]).*$");
    std::regex pwd_regex("^[a-zA-Z0-9_-!@#$%^&*?~]{6,30}$");
    return std::regex_match(pwd.toStdString(), pwd_regex);

}
