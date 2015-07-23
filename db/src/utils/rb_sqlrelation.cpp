/*****************************************************************
 * $Id: rb_sqlrelation.cpp 1680 2012-06-23 15:08:20Z rutger $
 * Created: Jan 25, 2012 9:43:49 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_sqlrelation.h"


/**
 * Constructor
 * change tableName to lower case
 */
RB_SqlRelation::RB_SqlRelation(const QString& tableName,
                               const QString& indexColumn,
                               const QString& displayColumn)
                : QSqlRelation(tableName.toLower(),
                               indexColumn,
                               displayColumn) {

    // nothing
}
