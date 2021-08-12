//
// Created by:
//  Shay Ben-Ishay 308052463
//  Liav Eliyahu 311502660
//
#ifndef PROJECT_SERIES_H
#define PROJECT_SERIES_H

#include <stdio.h>
#include "mtm_ex3.h"
#include "set.h"

typedef struct Series_t *Series;
typedef struct RankSeries_t *RankSeries;

typedef enum SeriesResult_t{
    SERIES_SUCCESS,
    SERIES_OUT_OF_MEMORY,
    SERIES_NULL_ARGUMENT,
} SeriesResult;

Series seriesCreate(const char* name,int episodesNum,Genre genre,int* ages,int episodeDuration,int seriesKey);
void seriesDestroy(Series series);
const char *seriesGetName(Series series);
int seriesGetEpisodesNum(Series series);
Genre seriesGetGenre(Series series);
int *seriesGetAges(Series series);
int seriesGetMinAge(Series series);
int seriesGetMaxAge(Series series);
int seriesGetEpisodesDuration(Series series);
int seriesGetKey(Series series);
void seriesPrint(Series series);
SetElement copySeries(SetElement setElement);
void freeSeries(SetElement dataElement);
int compareSeries(SetElement series1,SetElement series2);
int compareSeriesByName(ListElement series1,ListElement series2);
int compareSeriesByGenre(ListElement seriesOne,ListElement seriesTwo);
const char* seriesGenreToString(Genre genre);
List listSeriesToString(List list_series);


//RankSeries declare
RankSeries rankSeriesCreate(const char* series_name,int rank);
void rankSeriesDestroy(RankSeries rankSeries);
const char* rankSeriesGetName(RankSeries rankSeries);
int rankSeriesGetRank(RankSeries rankSeries);
ListElement copyRankSeries(ListElement listElement);
void freeRankSeries(ListElement listElement);
int compareRankAndName(ListElement rankSeries1,ListElement rankSeries2);
int compareRank(ListElement rankSeries1,ListElement rankSeries2);
int compareSeriesForReport(ListElement s1,ListElement s2);
#endif //PROJECT_SERIES_H
