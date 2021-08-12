//
// Created by:
//  Shay Ben-Ishay 308052463
//  Liav Eliyahu 311502660
//
#ifndef PROJECT_USER_H
#define PROJECT_USER_H

#include <stdio.h>
#include "series.h"
#include "set.h"

typedef struct User_t *User;

typedef enum UserResult_t{
    USER_SUCCESS,
    USER_OUT_OF_MEMORY,
    USER_NULL_ARGUMENT,
    USER_DOES_NOT_EXIST,
} UserResult;

User userCreate(const char* name,int age, int userKey);
void userDestroy(User user);
const char *userGetName(User user);
int userGetAge(User user);
int userGetKey(User user);
Set userGetFriends(User user);
Set userGetFavorites(User user);
UserResult userAddSeries(User user,Series series);
UserResult userRemoveSeries(User user,Series series);
UserResult userAddFriend(User user1,User user2);
UserResult userRemoveFriend(User username1,User username2);
void userPrint(User user);
SetElement copyUser(SetElement setElement);
void freeString(ListElement string);
ListElement copyString(ListElement string);
void freeUser(SetElement setElement);
int compareString(ListElement string1,ListElement string2);
int compareUser(SetElement user1,SetElement user2);
int compareUserByName(ListElement name1,ListElement name2);
//int userGetAverageFavorites(User user);
void userSetFriends(User user,Set set);
void userSetFavorites(User user,Set set);
List listUserToString(List list_user);


#endif //PROJECT_USER_H
