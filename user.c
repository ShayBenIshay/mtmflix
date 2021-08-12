//
// Created by:
//  Shay Ben-Ishay 308052463
//  Liav Eliyahu 311502660
//
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "user.h"
#include "set.h"
#include "string.h"

struct User_t{
    int user_key;
    const char* user_name;
    int user_age;
    Set user_friends;
    Set user_favorites;
};

//user functions for set
SetElement copyUser(SetElement setElement){
    //copy basic fields to new user
    //User newUser = userCreate(userGetName(setElement),userGetAge(setElement),userGetKey(setElement));
    //if(!newUser) return NULL;
    //copy rest of fields
    User user = (User)setElement;
    Set setFriends = setCopy(user->user_friends);
    if(!setFriends) return NULL;
    Set setFavorites = setCopy(user->user_favorites);
    if (!setFavorites){
        setDestroy(setFriends);
        return NULL;
    }
    User newUser = userCreate(userGetName(user),userGetAge(user),userGetKey(user));
    if(!newUser){
        setDestroy(setFavorites);
        setDestroy(setFriends);
        return NULL;
    }
    userSetFriends(newUser,setFriends);
    userSetFavorites(newUser,setFavorites);
    return newUser;
}

void freeUser(SetElement setElement){
    userDestroy(setElement);
}

int compareUser(SetElement user1,SetElement user2){
    return (userGetKey(user1) - userGetKey(user2));
}
void freeString(ListElement string){
    free((char*)string);
}
ListElement copyString(ListElement string){
    int length = strlen(string);
    char* newString = malloc(sizeof(*newString)*length+1);
    return strcpy(newString,(const char*)string);
}

int compareString(ListElement string1,ListElement string2){
    return strcmp((const char*)string1,(const char*)string2);
}

void userPrint(User user) {
    if (!user) return;
    printf("User name is:%s, key is:%d,age is:%d\n",user->user_name,user->user_key,user->user_age);
    printf("User Friends are:");
    printf("\nUser Favorites are:\n");
    if(!user->user_friends) printf("user favorites is NULL\n");
    SET_FOREACH(SetElement ,userIterator,user->user_friends){
        printf(" %s",userGetName(userIterator));
    }
    if(!user->user_favorites) printf("user favorites is NULL\n");
    int i=1;
    printf("friends set size is %d\n",setGetSize(user->user_favorites));
    SET_FOREACH(SetElement,seriesIterator,user->user_favorites){
        printf("iteration %d series set\n",i++);
        seriesPrint(seriesIterator);
    }
}

User userCreate(const char* name,int age, int userKey){
    if (!name) return NULL;
    User user = malloc(sizeof(*user));
    if(!user) return NULL;
    int name_len = strlen(name);
    user->user_name=malloc(name_len* sizeof(char)+1);
    if (!user->user_name) {
        free(user);
        return NULL;
    }
    strcpy((char*)user->user_name,(char*)name);
    assert(user->user_name);
    user->user_age = age;
    //create sets
    Set setFriends = setCreate(copyUser,freeUser,compareUser);
    if (!setFriends)   {
        free((char*)user->user_name);
        free(user);
        return NULL;
    }
    user->user_friends = setFriends;
    Set setFavorites = setCreate(copySeries,freeSeries,compareSeries);
    if (!setFavorites)   {
        setDestroy(user->user_friends);
        free((char*)user->user_name);
        free(user);
        return NULL;
    }
    user->user_favorites = setFavorites;
    user->user_key=userKey;
    return user;
}

void userDestroy(User user){
    if (!user) return;
    free((char*)user->user_name);
    setDestroy(user->user_friends);
    setDestroy(user->user_favorites);
    free(user);
}

const char *userGetName(User user){
    if (!user)  return NULL;
    return user->user_name;
}

int userGetAge(User user){
    if(!user) return -1;
    return user->user_age;
}

int userGetKey(User user){
    if (!user) return -1;
    return user->user_key;
}

Set userGetFriends(User user){
    if(!user) return NULL;
    return user->user_friends;
}
Set userGetFavorites(User user){
    if(!user) return NULL;
    return user->user_favorites;
}
UserResult userAddSeries(User user,Series series){
    if(!user || !series) return USER_NULL_ARGUMENT;
    SetResult setResult = setAdd(user->user_favorites,series);
    if(setResult == SET_NULL_ARGUMENT) return USER_NULL_ARGUMENT;
    if(setResult == SET_ITEM_ALREADY_EXISTS) return USER_SUCCESS;
    if(setResult == SET_OUT_OF_MEMORY) return USER_OUT_OF_MEMORY;
    return USER_SUCCESS;
}

UserResult userAddFriend(User user,User userFriend){
    if(!user || !userFriend) return USER_NULL_ARGUMENT;
    SetResult setResult = setAdd(user->user_friends,userFriend);
    if(setResult == SET_NULL_ARGUMENT) return USER_NULL_ARGUMENT;
    if(setResult == SET_ITEM_ALREADY_EXISTS) return USER_SUCCESS;
    if(setResult == SET_OUT_OF_MEMORY) return USER_OUT_OF_MEMORY;
    return USER_SUCCESS;
}

UserResult userRemoveFriend(User username1,User username2){
    if(!username1 || !username2) return USER_NULL_ARGUMENT;
    SetResult setResult = setRemove(username1->user_friends,username2);
    if(setResult == SET_NULL_ARGUMENT) return USER_NULL_ARGUMENT;
    if(setResult == SET_ITEM_DOES_NOT_EXIST) return USER_DOES_NOT_EXIST;
    return USER_SUCCESS;
}

UserResult userRemoveSeries(User user,Series series){
    if(!user || !series) return USER_NULL_ARGUMENT;
    SetResult setResult = setRemove(user->user_favorites,series);
    if(setResult == SET_NULL_ARGUMENT) return USER_NULL_ARGUMENT;
    if(setResult == SET_ITEM_DOES_NOT_EXIST) return USER_DOES_NOT_EXIST;
    return USER_SUCCESS;
}

int compareUserByName(ListElement user1,ListElement user2){
    User useOne = (User)user1;
    User userTwo = (User)user2;
    return strcmp(useOne->user_name,userTwo->user_name);
}



void userSetFriends(User user,Set set){
    if(!user || !set) return;
    setDestroy(user->user_friends);
    user->user_friends = set;
}
void userSetFavorites(User user,Set set){
    if(!user || !set) return;
    setDestroy(user->user_favorites);
    user->user_favorites= set;
}

List listUserToString(List list_user){
    List newList = listCreate(copyString,freeString);
    LIST_FOREACH(User,user,list_user){
        int name_length = strlen(userGetName(user));
        char* name = malloc(sizeof(*name)*name_length+1);
        if(!name) {
            listDestroy(newList);
            return NULL;
        }
        strcpy(name,userGetName(user));
        listInsertFirst(newList,name);
        free(name);
    }
    return newList;
}

