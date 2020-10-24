#ifndef USERINFO_H
#define USERINFO_H

#include <QString>
#include "communhttp.h"

class userinfo
{
private:
    // TODO: decide content of a user
    int id;
    QString email;
    QString ctime;
    QString role;
    QString password;

    QString token;

public:
    // constructor
    userinfo();

    // GET: get user info with a token
    // return true if success
    bool get_user_info(communhttp* requester);

    // POST: register as a new user
    // return true if success
    bool register_user_info(communhttp* requester);

    // POST: get a new user token
    // return true if success
    bool get_user_token(communhttp* requester);

    // DELETE: revoke a user token
    // return true if success
    bool delete_user_token(communhttp* requester);

    // HEAD: validate user token
    // return true if success
    bool head_token(communhttp* requester);
};

#endif // USERINFO_H
