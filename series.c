//
// Created by:
//  Shay Ben-Ishay 308052463
//  Liav Eliyahu 311502660
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "series.h"
#include "string.h"
#include "user.h"
#include "set.h"

struct Series_t{
    int series_key;
    char* series_name;
    int series_episodesNum;
    Genre series_genre;
    int* series_ages;
    int series_episodeDuration;
};

struct RankSeries_t{
    const char* series_name;
    int rank;
};

//series functions for set
SetElement copySeries(SetElement setElement){
    Series newSeries = seriesCreate(seriesGetName(setElement),seriesGetEpisodesNum(setElement)\
,seriesGetGenre(setElement),seriesGetAges(setElement),seriesGetEpisodesDuration(setElement)\
,seriesGetKey(setElement));
    if(!newSeries) return NULL;
    return newSeries;
}

void freeSeries(SetElement dataElement){
    seriesDestroy(dataElement);
}

int compareSeries(SetElement series1,SetElement series2){
    return (seriesGetKey(series1) - seriesGetKey(series2));
}

int compareSeriesByGenre(ListElement series1,ListElement series2){
    Series seriesOne = (Series)series1;
    Series seriesTwo = (Series)series2;
    char seriesOneGenre[20], seriesTwoGenre[20];
    if(seriesOne->series_genre == COMEDY) strcpy(seriesOneGenre,"COMEDY");
    if(seriesOne->series_genre == SCIENCE_FICTION) strcpy(seriesOneGenre,"SCIENCE_FICTION");
    if(seriesOne->series_genre == DRAMA) strcpy(seriesOneGenre,"DRAMA");
    if(seriesOne->series_genre == CRIME) strcpy(seriesOneGenre,"CRIME");
    if(seriesOne->series_genre == MYSTERY) strcpy(seriesOneGenre,"MYSTERY");
    if(seriesOne->series_genre == DOCUMENTARY) strcpy(seriesOneGenre,"DOCUMENTARY");
    if(seriesOne->series_genre == ROMANCE) strcpy(seriesOneGenre,"ROMANCE");
    if(seriesOne->series_genre == HORROR) strcpy(seriesOneGenre,"HORROR");
    if(seriesTwo->series_genre == COMEDY) strcpy(seriesTwoGenre,"COMEDY");
    if(seriesTwo->series_genre == SCIENCE_FICTION) strcpy(seriesTwoGenre,"SCIENCE_FICTION");
    if(seriesTwo->series_genre == DRAMA) strcpy(seriesTwoGenre,"DRAMA");
    if(seriesTwo->series_genre == CRIME) strcpy(seriesTwoGenre,"CRIME");
    if(seriesTwo->series_genre == MYSTERY) strcpy(seriesTwoGenre,"MYSTERY");
    if(seriesTwo->series_genre == DOCUMENTARY) strcpy(seriesTwoGenre,"DOCUMENTARY");
    if(seriesTwo->series_genre == ROMANCE) strcpy(seriesTwoGenre,"ROMANCE");
    if(seriesTwo->series_genre == HORROR) strcpy(seriesTwoGenre,"HORROR");
    int diff = strcmp(seriesOneGenre,seriesTwoGenre);
    //free
    return diff;
}

int compareSeriesByName(ListElement series1,ListElement series2){
    Series seriesOne = (Series)series1;
    Series seriesTwo = (Series)series2;
    return strcmp(seriesOne->series_name,seriesTwo->series_name);
}

void seriesPrint(Series series) {
    if (!series) return;
    printf("Series name is:%s, key is:%d,episodesNum is:%d,series_episodeDuration is:%d\n", \
    series->series_name, series->series_key, series->series_episodesNum, series->series_episodeDuration);
    if (!series->series_ages) return;
    printf("Ages range are in between:%d and %d\n",series->series_ages[0],series->series_ages[1]);
}

int compareSeriesForReport(ListElement s1,ListElement s2){
    int genre = compareSeriesByGenre(s1, s2);
    if(genre != 0) {
        return genre;
    } else{
        return compareSeriesByName(s1, s2);
    }
}

Series seriesCreate(const char* name,int episodesNum,Genre genre,int* ages,int episodeDuration, int seriesKey){
    if (!name || episodeDuration<=0) return NULL;
    Series series = malloc(sizeof(*series));
    if(!series) return NULL;
    series->series_ages = malloc(2*sizeof(*series->series_ages));
    if(!series->series_ages) {
        free(series);
        return NULL;
    }
    int name_len = strlen(name);
    series->series_name=malloc(name_len* sizeof(char)+1);
    if (!series->series_name) {
        free(series->series_ages);
        free(series);
        return NULL;
    }
    strcpy(series->series_name,name);
    series->series_episodesNum = episodesNum;
    series->series_genre = genre;
    if (!ages) {
        series->series_ages[0] = MTM_MIN_AGE;
        series->series_ages[1] = MTM_MAX_AGE;
    } else {
        series->series_ages[0] = ages[0] < MTM_MIN_AGE ? MTM_MIN_AGE : ages[0];
        series->series_ages[1] = ages[1] > MTM_MAX_AGE ? MTM_MAX_AGE : ages[1];
    }
    series->series_episodeDuration = episodeDuration;
    series->series_key=seriesKey;
    return series;
}

void seriesDestroy(Series series){
    if (!series) return;
    free(series->series_name);
    free(series->series_ages);
    free(series);
}
const char *seriesGetName(Series series){
    if (!series)  return NULL;
    return series->series_name;
}

int seriesGetEpisodesNum(Series series){
    if (!series)  return -1;
    return series->series_episodesNum;
}

Genre seriesGetGenre(Series series){
    if (!series)  return (Genre)NULL;
    return series->series_genre;
}

int *seriesGetAges(Series series){
    if (!series)  return NULL;
    return series->series_ages;
}

int seriesGetMinAge(Series series){
    if(!series) return -1;
    return series->series_ages[0];
}
int seriesGetMaxAge(Series series){
    if(!series) return -1;
    return series->series_ages[1];
}
int seriesGetEpisodesDuration(Series series){
    if (!series)  return -1;
    return series->series_episodeDuration;

}

int seriesGetKey(Series series){
    if (!series)  return -1;
    return series->series_key;
}

const char* seriesGenreToString(Genre genre){
    if(genre == COMEDY) return "COMEDY";
    if(genre == SCIENCE_FICTION) return "SCIENCE_FICTION";
    if(genre == DRAMA) return "DRAMA";
    if(genre == CRIME) return "CRIME";
    if(genre == MYSTERY) return "MYSTERY";
    if(genre == DOCUMENTARY) return "DOCUMENTARY";
    if(genre == ROMANCE) return "ROMANCE";
    if(genre == HORROR) return "HORROR";
    return "NULL";
}

//RankSeries Implementation
RankSeries rankSeriesCreate(const char* series_name,int rank){
    if(!series_name || rank<
                               0) return NULL;
    RankSeries rankSeries = malloc(sizeof(*rankSeries));
    if (!rankSeries)    return NULL;
    int length = strlen(series_name);
    char* series_name_copy = malloc(sizeof(char)*length+1);
    if(!series_name_copy) {
        free (rankSeries);
        return NULL;
    }
    rankSeries->series_name = strcpy(series_name_copy,series_name);
    rankSeries->rank = rank;
    return rankSeries;
}

const char* rankSeriesGetName(RankSeries rankSeries){
    if(!rankSeries) return NULL;
    return rankSeries->series_name;
}
int rankSeriesGetRank(RankSeries rankSeries){
    if(!rankSeries) return -1;
    return rankSeries->rank;
}

ListElement copyRankSeries(ListElement listElement){
    RankSeries newRankSeries = rankSeriesCreate(rankSeriesGetName(listElement),rankSeriesGetRank(listElement));
    if(!newRankSeries) return NULL;
    return newRankSeries;
}

void freeRankSeries(ListElement listElement){
     rankSeriesDestroy((RankSeries)listElement);
}
void rankSeriesDestroy(RankSeries rankSeries){
    if(!rankSeries) return;
    free((char*)rankSeries->series_name);
    free(rankSeries);
}
int compareRankAndName(ListElement rankSeries1,ListElement rankSeries2) {
    if (compareRank(rankSeries1, rankSeries2) == 0) \
        return strcmp(rankSeriesGetName(rankSeries1), rankSeriesGetName(rankSeries2));
    return compareRank(rankSeries1, rankSeries2);
}

int compareRank(ListElement rankSeries1,ListElement rankSeries2){
    RankSeries rankSeriesOne = (RankSeries)rankSeries1;
    RankSeries rankSeriesTwo = (RankSeries)rankSeries2;
    return ((rankSeriesTwo->rank) - (rankSeriesOne->rank));
}

List listSeriesToString(List list_series){
    List newList = listCreate(copyString,freeString);
    LIST_FOREACH(Series,series,list_series){
        int name_length = strlen(seriesGetName(series));
        char* name = malloc(sizeof(*name)*name_length+1);
        if(!name) {
            listDestroy(newList);
            return NULL;
        }
        strcpy(name,seriesGetName(series));
        listInsertFirst(newList,name);
        free(name);
    }
    return newList;
}