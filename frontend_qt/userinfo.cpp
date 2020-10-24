#include "userinfo.h"

userinfo::userinfo()
{
    id = -1;
    email = "";
    ctime = "";
    role = "";
    password = "";

    token = "";
}

bool userinfo::get_user_info(communhttp* requester){
    return true;
}

bool userinfo::register_user_info(communhttp *requester){
    return true;
}

bool userinfo::get_user_token(communhttp *requester){
    return true;
}

bool userinfo::delete_user_token(communhttp *requester){
    return true;
}

bool userinfo::head_token(communhttp *requester){
    return true;
}
