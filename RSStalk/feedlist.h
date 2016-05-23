﻿#ifndef FEEDLIST_H
#define FEEDLIST_H

#include <QList>
#include "feed.h"

class FeedList
{
public:
    FeedList();

    QList<Feed *> getFeedList();

    void addNewFeed(Feed);
    void deleteFeed(int);
    void renameFeed(int, QString);

    bool getFavoriteMark();
    void setFavoriteMark(bool favorite);

private:
    QString listName;
    QList<Feed *> feedList;
    bool favoriteMark;
};

#endif // FEEDLIST_H
