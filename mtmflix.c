//
// Created by:
//  Shay Ben-Ishay 308052463
//  Liav Eliyahu 311502660
//
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "mtmflix.h"
#include "user.h"
#include "series.h"
#include "set.h"
#include "list.h"
#include "map.h"

struct mtmFlix_t {
    Map mapUsers;
    int userNextKey;
    Map mapSeries;
    int seriesNextKey;
};

//STATIC FUNCTIONS FOR MTMFLIX

static List mtmFlixUserSetToList(Set set){
    List list = listCreate(copyUser,freeUser);
    if (!list || !set) return NULL;
    SET_FOREACH(User,user,set){
        //check function return value
        if (listInsertFirst(list,user)!=LIST_SUCCESS){
            listDestroy(list);
            return NULL;
        }
    }
    return list;
}
static List mtmFlixSeriesSetToList(Set set){
    List list = listCreate(copySeries,freeSeries);
    if (!set || !list) return NULL;
    SET_FOREACH(Series,series,set){
        //check function return value
        if (listInsertFirst(list,series)!=LIST_SUCCESS){
            listDestroy(list);
            return NULL;
        }
    }
    return list;
}

static bool mtmFlixUserIsAgeValid(MtmFlix mtmflix,User user,Series series){
    if(!mtmflix || !user || !series) return false;
    int user_age = userGetAge(user);
    return (user_age >= seriesGetMinAge(series)&& seriesGetMaxAge(series));
}

static User mtmFlixKeyGetUser(MtmFlix mtmFlix,MapKeyElement key){
    if(!mtmFlix || !key) return NULL;
    return (User)mapGet(mtmFlix->mapUsers,key);
}

static Series mtmFlixKeyGetSeries(MtmFlix mtmFlix,MapKeyElement key){
    if(!mtmFlix || !key) return NULL;
    return (Series)mapGet(mtmFlix->mapSeries,key);
}
//add static and remove from header
User mtmFlixNameGetUser(MtmFlix mtmFlix,const char *name) {
    if (!mtmFlix || !name) return NULL;
    MAP_FOREACH(MapKeyElement,userKey,mtmFlix->mapUsers){
        User user = mtmFlixKeyGetUser(mtmFlix,userKey);
        if(strcmp(userGetName(user),name)==0) return user;
    }
    return NULL;
}

static Series mtmFlixNameGetSeries(MtmFlix mtmFlix,const char *name) {
    if (!mtmFlix || !name) return NULL;
    MAP_FOREACH(MapKeyElement,seriesKey,mtmFlix->mapSeries){
        Series series = mtmFlixKeyGetSeries(mtmFlix,seriesKey);
        if(strcmp(seriesGetName(series),name)==0) return series;
    }
    return NULL;
}

//key handling functions for map
static MapKeyElement copyInt(MapKeyElement keyElement) {
    int *newInt = malloc(sizeof(int));
    if (!newInt) return NULL;
    *newInt = *(int *) keyElement;
    return newInt;
}

static void freeInt(MapKeyElement e) {
    free(e);
}


static int compareInt(MapKeyElement a, MapKeyElement b) {
    return *(int *) a - *(int *) b;
}

static bool mtmFlixIsNameValid(const char* name){
    if (!name) return false;
    int nameLength = strlen(name);
    if (nameLength<1) return false;
    for (int i=0;i<nameLength;i++){
        if (!((name[i]>='a'&&name[i]<='z')||(name[i]>='A'&&name[i]<='Z')||\
        (name[i]>='0'&&name[i]<='9'))) return false;
    }
    return true;
}

static Series rankSeriesGetSeries(MtmFlix mtmFlix,RankSeries rankSeries){
    if(!rankSeries) return NULL;
    Series series = mtmFlixNameGetSeries(mtmFlix,rankSeriesGetName(rankSeries));
    return series;
}

static void copyMapElementsToList(Map map,List list){
    if(!map || !list) return;
    MAP_FOREACH(MapKeyElement ,keyElement,map){
        listInsertFirst(list,mapGet(map,keyElement));
    }
}

/*
 * functions for handling users serieses and their key's
 */
static List mtmFlixSeriesSort(MtmFlix mtmflix){
    List list = listCreate(copySeries,freeSeries);
    if(!list) return NULL;
    copyMapElementsToList(mtmflix->mapSeries,list);
    listSort(list,compareSeriesForReport);
    return list;
}

static List mtmFlixUserSort(MtmFlix mtmflix){
    List list = listCreate(copyUser,freeUser);
    if(!list) return NULL;
    copyMapElementsToList(mtmflix->mapUsers,list);
    listSort(list,compareUserByName);
    return list;
}

static void mtmFlixRemoveFromAllFavorites(MtmFlix mtmflix,Series series){
    if(!mtmflix || !series) return;
    MAP_FOREACH(MapKeyElement,key,mtmflix->mapUsers){
        User user = mtmFlixKeyGetUser(mtmflix,key);
        userRemoveSeries(user,series);
    }
}
static void mtmFlixRemoveFromAllFriends(MtmFlix mtmflix,User userToRemove) {
    if (!mtmflix || !userToRemove) return;
   // printf("the user to remove\n");
   // userPrint(userToRemove);
  //  printf("STARTING DEBUG SECTION FOR MTMFLIX REMOVE FROM ALL FRIENDS\n");
    MAP_FOREACH(MapKeyElement, key, mtmflix->mapUsers) {
        User user = mtmFlixKeyGetUser(mtmflix, key);
      //  userPrint(user);
        if(user == userToRemove) continue;
        userRemoveFriend(user,userToRemove);
     //   userPrint(user);

    }

}

MtmFlix mtmFlixCreate(){
    MtmFlix mtmFlix = malloc(sizeof(*mtmFlix));
    if (!mtmFlix)   return NULL;
    //insert NULL to fields
    mtmFlix->mapUsers=NULL;
    mtmFlix->mapSeries=NULL;
    mtmFlix->userNextKey=0;
    mtmFlix->seriesNextKey=0;
    mtmFlix->mapUsers = mapCreate(copyUser,copyInt,freeUser, freeInt, compareInt);
    if(!mtmFlix->mapUsers) {
        free(mtmFlix);
        return NULL;
    }
    mtmFlix->mapSeries = mapCreate(copySeries,copyInt,freeSeries, freeInt, compareInt);
    if(!mtmFlix->mapSeries) {
        mapDestroy(mtmFlix->mapUsers);
        free(mtmFlix);
        return NULL;
    }
    return mtmFlix;
}

void mtmFlixDestroy(MtmFlix mtmFlix){
    if (!mtmFlix) return;
    mapDestroy(mtmFlix->mapUsers);
    mapDestroy(mtmFlix->mapSeries);
    free(mtmFlix);
}

MtmFlixResult mtmFlixAddUser(MtmFlix mtmflix, const char* name, int age){
    if(!mtmflix || !name) return MTMFLIX_NULL_ARGUMENT;
    //SHOULD CHECK NAME VALIDATION
    if(mapContains(mtmflix->mapUsers,mtmFlixNameGetUser(mtmflix,name))) return MTMFLIX_USERNAME_ALREADY_USED;
    if(!mtmFlixIsNameValid(name)) return MTMFLIX_ILLEGAL_USERNAME;
    if(age<MTM_MIN_AGE || age>MTM_MAX_AGE) return MTMFLIX_ILLEGAL_AGE;
    User user = userCreate(name,age,mtmflix->userNextKey);
    if (!user) return MTMFLIX_OUT_OF_MEMORY;
    mapPut(mtmflix->mapUsers,&mtmflix->userNextKey,user);
    mtmflix->userNextKey++;     //the next key is ready for the next user
    userDestroy(user);
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixAddSeries(MtmFlix mtmflix, const char* name, int episodesNum, \
 Genre genre, int* ages, int episodesDuration){
    if(!mtmflix || !name) return MTMFLIX_NULL_ARGUMENT;
    if(!mtmFlixIsNameValid(name)) return MTMFLIX_ILLEGAL_SERIES_NAME;
    if(mtmFlixNameGetSeries(mtmflix,name) != NULL) return MTMFLIX_SERIES_ALREADY_EXISTS;
    if (episodesNum<=0) return MTMFLIX_ILLEGAL_EPISODES_NUM;
    if (episodesDuration<=0) return MTMFLIX_ILLEGAL_EPISODES_DURATION;
    Series newSeries = seriesCreate(name,episodesNum,genre,ages,episodesDuration,mtmflix->seriesNextKey);
    if (!newSeries) return MTMFLIX_OUT_OF_MEMORY;
    //THE NEXT LINE IS'NT GIVING THE CURRECT SERIES KEY AS EXPECTED
    // int seriesKey = seriesGetKey(mtmFlixNameGetSeries(mtmflix,name));
    if(mapPut(mtmflix->mapSeries,&mtmflix->seriesNextKey,newSeries)!=MAP_SUCCESS) return MTMFLIX_OUT_OF_MEMORY;
    mtmflix->seriesNextKey++;
    seriesDestroy(newSeries);
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixRemoveSeries(MtmFlix mtmflix, const char* name){
    if (!mtmflix || !name) return MTMFLIX_NULL_ARGUMENT;
    Series series = mtmFlixNameGetSeries(mtmflix,name);
    if(!series) return MTMFLIX_SERIES_DOES_NOT_EXIST;
    int seriesKey = seriesGetKey(series);
    //remove the series from all the users favorites
    mtmFlixRemoveFromAllFavorites(mtmflix,series);
    MapResult mapResult = mapRemove(mtmflix->mapSeries,&seriesKey);
    if(mapResult == MAP_NULL_ARGUMENT) return MTMFLIX_NULL_ARGUMENT;
    if(mapResult == MAP_ITEM_DOES_NOT_EXIST) return MTMFLIX_SERIES_DOES_NOT_EXIST;
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixRemoveUser(MtmFlix mtmflix, const char* username){
    if (!mtmflix || !username) return MTMFLIX_NULL_ARGUMENT;
    User user = mtmFlixNameGetUser(mtmflix,username);
    if(!user) return MTMFLIX_USER_DOES_NOT_EXIST;
    mtmFlixRemoveFromAllFriends(mtmflix,user);
    int userKey = userGetKey(user);
    MapResult mapResult = mapRemove(mtmflix->mapUsers,&userKey);
    if(mapResult == MAP_NULL_ARGUMENT) return MTMFLIX_NULL_ARGUMENT;
    if(mapResult == MAP_ITEM_DOES_NOT_EXIST) return MTMFLIX_USER_DOES_NOT_EXIST;
    //remove user from all users friend
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixSeriesJoin(MtmFlix mtmflix, const char* username, const char* seriesName){
    if(!mtmflix || !username || !seriesName) return MTMFLIX_NULL_ARGUMENT;
    User user = mtmFlixNameGetUser(mtmflix,username);
    if(!user) return MTMFLIX_USER_DOES_NOT_EXIST;
    Series series = mtmFlixNameGetSeries(mtmflix,seriesName);
    if(!series) return MTMFLIX_SERIES_DOES_NOT_EXIST;
    if(!mtmFlixUserIsAgeValid(mtmflix,user,series)) return MTMFLIX_USER_NOT_IN_THE_RIGHT_AGE;
    UserResult userResult = userAddSeries(user,series);
    if(userResult == USER_NULL_ARGUMENT) return MTMFLIX_NULL_ARGUMENT;
    if(userResult == USER_OUT_OF_MEMORY) return MTMFLIX_OUT_OF_MEMORY;
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixAddFriend(MtmFlix mtmflix, const char* username1, const char* username2){
    if(!mtmflix || !username1 || !username2) return MTMFLIX_NULL_ARGUMENT;
    User user1 = mtmFlixNameGetUser(mtmflix,username1);
    User originalUser2 = mtmFlixNameGetUser(mtmflix,username2);
    if(!user1 || !originalUser2) return MTMFLIX_USER_DOES_NOT_EXIST;
    if (compareUser(user1,originalUser2)==0)    return MTMFLIX_SUCCESS;
    UserResult userResult = userAddFriend(user1,originalUser2);
    if(userResult == USER_NULL_ARGUMENT) return MTMFLIX_NULL_ARGUMENT;
    if(userResult == USER_OUT_OF_MEMORY) return MTMFLIX_OUT_OF_MEMORY;
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixRemoveFriend(MtmFlix mtmflix, const char* username1, const char* username2){
    if(!mtmflix || !username1 || !username2) return MTMFLIX_NULL_ARGUMENT;
    User user1 = mtmFlixNameGetUser(mtmflix,username1);
    User user2 = mtmFlixNameGetUser(mtmflix,username2);
    if(!user1 || !user2) return MTMFLIX_USER_DOES_NOT_EXIST;
    UserResult userResult = userRemoveFriend(user1,user2);
    if(userResult == USER_NULL_ARGUMENT) return MTMFLIX_NULL_ARGUMENT;
    if(userResult == USER_DOES_NOT_EXIST) return MTMFLIX_SUCCESS;
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixSeriesLeave(MtmFlix mtmflix, const char* username, const char* seriesName){
    if(!mtmflix || !username || !seriesName) return MTMFLIX_NULL_ARGUMENT;
    User user = mtmFlixNameGetUser(mtmflix,username);
    if(!user) return MTMFLIX_USER_DOES_NOT_EXIST;
    Series series = mtmFlixNameGetSeries(mtmflix,seriesName);
    if(!series) return MTMFLIX_SERIES_DOES_NOT_EXIST;
    UserResult userResult = userRemoveSeries(user,series);
    if(userResult == USER_NULL_ARGUMENT) return MTMFLIX_NULL_ARGUMENT;
    if(userResult == USER_DOES_NOT_EXIST) return MTMFLIX_SUCCESS;
    return MTMFLIX_SUCCESS;
    //        userRemoveSeries(user,series);
    //remove the series from all users favorites
   }

MtmFlixResult mtmFlixReportSeries(MtmFlix mtmflix,int seriesNum, FILE* outputStream){
    if(!mtmflix || !outputStream) return MTMFLIX_NULL_ARGUMENT;
    if(seriesNum<0) return MTMFLIX_ILLEGAL_NUMBER;
    //sorting the series map by genre and each genre alphabetic
    if(mapGetSize(mtmflix->mapSeries)==0) return MTMFLIX_NO_SERIES;
    if(seriesNum == 0) seriesNum = mapGetSize(mtmflix->mapSeries);
    List list = mtmFlixSeriesSort(mtmflix);
    if(!list) return MTMFLIX_OUT_OF_MEMORY;
    Genre genre = seriesGetGenre(listGetFirst(list));
    Genre current_genre;
    int seriesNumCounter=0;
    LIST_FOREACH(Series,series,list) {
        current_genre = seriesGetGenre(series);
        if (genre == current_genre && seriesNumCounter < seriesNum) {
            fprintf(outputStream, "%s", mtmPrintSeries(seriesGetName(series)\
, seriesGenreToString(seriesGetGenre(series))));
            seriesNumCounter++;
            continue;
        }
        if(genre == current_genre) continue;
        else genre = current_genre;
        fprintf(outputStream, "%s", mtmPrintSeries(seriesGetName(series)\
, seriesGenreToString(seriesGetGenre(series))));
        seriesNumCounter=1;
    }
    listDestroy(list);
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixReportUsers(MtmFlix mtmflix, FILE* outputStream){
    if(!mtmflix || !outputStream) return MTMFLIX_NULL_ARGUMENT;
    //sorting the series map by genre and each genre alphabetic
    if(mapGetSize(mtmflix->mapUsers)==0) return MTMFLIX_NO_USERS;
    List list = mtmFlixUserSort(mtmflix);
    //check the list result
    if (!list) return MTMFLIX_OUT_OF_MEMORY;
    LIST_FOREACH(User,user,list) {
        List list_friends = mtmFlixUserSetToList(userGetFriends(user));
        if(!list_friends) {
            listDestroy(list);
            return MTMFLIX_OUT_OF_MEMORY;
        }
        List list_favorites= mtmFlixSeriesSetToList(userGetFavorites(user));
        if(!list_favorites) {
            listDestroy(list_friends);
            listDestroy(list);
            return MTMFLIX_OUT_OF_MEMORY;
        }
     //convert list series and list users to list of strings
        List list_friends_string = listUserToString(list_friends);
        if(!list_friends_string){
            listDestroy(list_friends);
            listDestroy(list_favorites);
            return MTMFLIX_OUT_OF_MEMORY;
        }
        List list_favorites_string = listSeriesToString(list_favorites);
        if(!list_friends_string){
            listDestroy(list_friends);
            listDestroy(list_favorites);
            listDestroy(list_friends_string);
            return MTMFLIX_OUT_OF_MEMORY;
        }
        listSort(list_friends_string,compareString);
        listSort(list_favorites_string,compareString);
       fprintf(outputStream, "%s", mtmPrintUser(userGetName(user)\
, userGetAge(user),list_friends_string,list_favorites_string));
        listDestroy(list_friends);
        listDestroy(list_favorites);
        listDestroy(list_friends_string);
        listDestroy(list_favorites_string);
    }
    listDestroy(list);
    return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixGetRecommendations(MtmFlix mtmflix, const char* username,\
 int count, FILE* outputStream){
    if (!mtmflix || !username || !outputStream) return MTMFLIX_NULL_ARGUMENT;
    User user = mtmFlixNameGetUser(mtmflix,username);
    if(!user) return MTMFLIX_USER_DOES_NOT_EXIST;
    if (count<0) return MTMFLIX_ILLEGAL_NUMBER;
    //check if the set series contain current series
    if(!setGetFirst(userGetFavorites(user)) || !setGetFirst(userGetFriends(user))) return MTMFLIX_SUCCESS;
    List rank_list = listCreate(copyRankSeries,freeRankSeries);
    if(!rank_list) return MTMFLIX_OUT_OF_MEMORY;
    MAP_FOREACH(MapKeyElement,keyElement,mtmflix->mapSeries){
        Series mtmFlix_series = mtmFlixKeyGetSeries(mtmflix,keyElement);
        if(setIsIn(userGetFavorites(user),mtmFlix_series)==true) continue;
        //use set foreach and calculate formula
        if(!mtmFlixUserIsAgeValid(mtmflix,user,mtmFlix_series)) continue;
        int G = 0, F = 0;
        double L=0;
        double CUR = seriesGetEpisodesDuration(mtmFlix_series);
        SET_FOREACH(Series,user_series,userGetFavorites(user)){
            if(compareSeriesByGenre(mtmFlix_series,user_series)==0) G++;
            L+=seriesGetEpisodesDuration(user_series);
        }
        L /= setGetSize(userGetFavorites(user));
        SET_FOREACH(User,user_iterator,userGetFriends(user)){
            int key = userGetKey(user_iterator);
            User user_friends = mapGet(mtmflix->mapUsers,&key);
            if(user==user_friends) continue;
            Set user_friends_favorites = userGetFavorites(user_friends);
            if(!user_friends_favorites) continue;
            if(setIsIn(user_friends_favorites,mtmFlix_series)==true)
                F++;
        }
        //some changed done with absDiff
        double absDiff = CUR-L >= 0 ? CUR-L : L-CUR;
        double rank = (G*F)/(1+absDiff);
        if(rank==0) continue;
        RankSeries rankSeries = rankSeriesCreate(seriesGetName(mtmFlix_series),(int)rank);
        if(!rankSeries) return MTMFLIX_OUT_OF_MEMORY;
        if(listInsertFirst(rank_list,rankSeries)!= LIST_SUCCESS){
            rankSeriesDestroy(rankSeries);
            listDestroy(rank_list);
            return MTMFLIX_OUT_OF_MEMORY;
        }
         rankSeriesDestroy(rankSeries);
    }
    //sort rank_list by rank
    listSort(rank_list,compareRankAndName);
    //print to outputFile by rank(only rank>0)
    int print_iterator =0;
    if (count == 0) count=mapGetSize(mtmflix->mapSeries);
    LIST_FOREACH(RankSeries,rankSeries,rank_list){
        if (print_iterator>=count)  break;
        if(rankSeriesGetRank(rankSeries)<=0) break;
        fprintf(outputStream,"%s",mtmPrintSeries(rankSeriesGetName(rankSeries)\
, seriesGenreToString(seriesGetGenre(rankSeriesGetSeries(mtmflix,rankSeries)))));
    }
    listDestroy(rank_list);
    return MTMFLIX_SUCCESS;
}