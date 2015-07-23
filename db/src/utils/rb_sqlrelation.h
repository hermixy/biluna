/*****************************************************************
 * $Id: rb_sqlrelation.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 25, 2012 9:43:49 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_SQLRELATION_H
#define RB_SQLRELATION_H

#include <QSqlRelation>
#include "db_global.h"


/**
 * Wrapper class of RB_SqlRelation,
 * only changes the tableName to lower case
 */
class DB_EXPORT RB_SqlRelation : public QSqlRelation {

public:
    RB_SqlRelation(const QString& tableName,
                   const QString& indexColumn,
                   const QString& displayColumn);
    
};

#endif
