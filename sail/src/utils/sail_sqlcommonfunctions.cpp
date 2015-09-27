/*****************************************************************
 * $Id: sail_sqlcommonfunctions.cpp 1746 2012-08-23 21:01:10Z rutger $
 * Created: Aug 15, 2012 4:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Sailing (SAIL) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_sqlcommonfunctions.h"

#include "sail_dialogfactory.h"
#include "sail_modelfactory.h"
#include "sail_objectfactory.h"
#include "rb_settings.h"

/**
 * Constructor
 */
SAIL_SqlCommonFunctions::SAIL_SqlCommonFunctions() {

}

/**
 * Destructor
 */
SAIL_SqlCommonFunctions::~SAIL_SqlCommonFunctions() {

}

/**
 * Simple query execution SELECT 'field' FROM 'table' WHERE id='id'
 * @return sql result value
 */
RB_Variant SAIL_SqlCommonFunctions::selectFromWhereId(const RB_String& field,
                                                     const RB_String& table,
                                                     const RB_String& id) {
    if (!SAIL_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "SAIL_SqlCommonFunctions::selectFromWhereId()"
                        " database not open ERROR");
        return -1;
    }

    RB_String qStr = "";
    qStr = "SELECT " + field + " FROM " + table.toLower() + " WHERE id='" + id + "';";
    QSqlQuery query(SAIL_MODELFACTORY->getDatabase());

    if (query.exec(qStr)) {
        if (!query.first()) return RB_Variant();
        return query.value(0);
    } else {
        RB_DEBUG->error("SAIL_SqlCommonFunctionsFunction::selectFromWhereId() "
                        + query.lastError().text() + " ERROR");
    }

    return RB_Variant();
}

/**
 * Simple query execution UPDATE 'table' SET 'field'='value' WHERE id='id'
 * @returns true on success
 */
bool SAIL_SqlCommonFunctions::update(const RB_String& table,
                                    const RB_String& field,
                                    const RB_String& value,
                                    const RB_String& id) {
    if (!SAIL_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "SAIL_SqlCommonFunctions::updateStringWhereId()"
                        " database not open ERROR");
        return false;
    }

    RB_String qStr = "";
    qStr = "UPDATE " + table.toLower() + " SET " + field + "='" + value + "' WHERE id='" + id + "';";
    QSqlQuery query(SAIL_MODELFACTORY->getDatabase());

    bool success = query.exec(qStr);
    if (!success) {
        RB_DEBUG->error("SAIL_SqlCommonFunctionsFunction::updateStringWhereId() "
                        + query.lastError().text() + " ERROR");
    }

    return success;
}

/**
 * Simple query execution SELECT 'field' FROM 'table' WHERE 'whereStr'
 * @param field field name
 * @param table table name
 * @param whereStr where string
 * @return variant result value
 */
RB_Variant SAIL_SqlCommonFunctions::selectFromWhere(const RB_String& field,
                                                   const RB_String& table,
                                                   const RB_String& whereStr) {
    if (!SAIL_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "SAIL_SqlCommonFunctions::selectFromWhereId()"
                        " database not open ERROR");
        return -1;
    }

    RB_String qStr = "";
    qStr = "SELECT " + field + " FROM " + table.toLower() + " WHERE " + whereStr + ";";
    QSqlQuery query(SAIL_MODELFACTORY->getDatabase());

    if (query.exec(qStr)) {
        if (!query.first()) return RB_Variant();
        return query.value(0);
    } else {
        RB_DEBUG->error("SAIL_SqlCommonFunctionsFunction::selectFromWhereId() "
                        + query.lastError().text() + " ERROR");
    }

    return RB_Variant();
}

/**
 * Simple query execution SELECT * FROM 'table' WHERE 'whereStr'
 * @param query query that will contain the result record set
 * @param field field name
 * @param table table name
 * @param whereStr where string
 */
void SAIL_SqlCommonFunctions::selectAllFromWhere(QSqlQuery& query,
                                                   const RB_String& table,
                                                   const RB_String& whereStr) {
    RB_String qStr = "";
    qStr = "SELECT * FROM " + table.toLower() + " WHERE " + whereStr + ";";

    if (!query.exec(qStr)) {
        RB_DEBUG->error("SAIL_SqlCommonFunctionsFunction::getSelectAllFromWhere() "
                        + query.lastError().text() + " ERROR");
    }
}

void SAIL_SqlCommonFunctions::selectRouteDistanceData(QSqlQuery& query,
                                                      const RB_StringList& idList) {
    RB_String qStr = "";
    qStr = "SELECT sail_routecoordinate.seqno, sail_coordinate.mname, "
                "sail_coordinate.latitude, sail_coordinate.longitude "
            "FROM sail_routecoordinate "
                "INNER JOIN sail_coordinate ON sail_coordinate.id=";
    substrIdxId("sail_routecoordinate.coordinate_idx", qStr);
    if (idList.count() == 1) {
        qStr += " WHERE sail_routecoordinate.parent='" + idList.at(0)
                + "' ORDER BY sail_routecoordinate.seqno ASC;";
    } else {
        qStr += " WHERE ";
        int rowCount = idList.count();

        for (int i = 0; i < rowCount; ++i) {
            if (i != 0) {
                qStr += "' OR ";
            }
            qStr += " sail_routecoordinate.id='" + idList.at(i);
        }
        qStr += "' ORDER BY sail_routecoordinate.seqno ASC;";
    }

    if (!query.exec(qStr)) {
        RB_DEBUG->error("SAIL_SqlCommonFunctions::selectRouteDistanceData() "
                        + query.lastError().text() + " ERROR");
        SAIL_DIALOGFACTORY->commandMessage("SAIL_SqlCommonFunctions::selectRouteDistanceData() "
                                          + query.lastError().text() + "\n");
    }
}

/**
 * Select data for track distance report, query members are:
 * 0 - mname
 * 1 - from_name
 * 2 - to_name
 * 3 - weightfactor
 * 4 - from_lon
 * 5 - from_lat
 * 6 - to_lon
 * 7 - to_lat
 * 8 - comment
 * @param query query object to be set with data
 */
void SAIL_SqlCommonFunctions::selectTrackDistanceData(QSqlQuery& query) {

    /* TEST:
    SELECT sail_track.mname, SUBSTR(sail_track.from_idx, 39) as from_name,
        SUBSTR(sail_track.to_idx, 39) as to_name, sail_track.weightfactor,
        c1.latitude as from_lat, c1.longitude as from_lon,
        c2.latitude as to_lat, c2.longitude as to_lon,
        sail_track.comment
    FROM sail_track
    INNER JOIN sail_coordinate c1 ON c1.id=SUBSTR(sail_track.from_idx, 1, 38)
    INNER JOIN sail_coordinate c2 ON c2.id=SUBSTR(sail_track.to_idx, 1, 38)
    WHERE sail_track.parent='{c1e18831-d988-4096-9a8d-43578496a0c9}';
    */

    RB_String rootId = SAIL_MODELFACTORY->getRootId();
    RB_String qStr = "";
    qStr = "SELECT sail_track.mname, ";
    substrIdxContent("sail_track.from_idx", qStr);
    qStr +=     " as from_name, ";
    substrIdxContent("sail_track.to_idx", qStr);
    qStr +=     " as to_name, sail_track.weightfactor, "
                "c1.latitude as from_lat, c1.longitude as from_lon, "
                "c2.latitude as to_lat, c2.longitude as to_lon, "
                "sail_track.comment "
            "FROM sail_track "
                "INNER JOIN sail_coordinate c1 ON c1.id=";
    substrIdxId("sail_track.from_idx", qStr);
    qStr +=     " INNER JOIN sail_coordinate c2 ON c2.id=";
    substrIdxId("sail_track.to_idx", qStr);
    qStr += " WHERE sail_track.parent='" + rootId + "';";

    if (!query.exec(qStr)) {
        RB_DEBUG->error("SAIL_SqlCommonFunctions::selectTrackDistanceData() "
                        + query.lastError().text() + " ERROR");
        SAIL_DIALOGFACTORY->commandMessage("SAIL_SqlCommonFunctions::selectTrackDistanceData() "
                                          + query.lastError().text() + "\n");
    }
}

/**
 * Select data for track distance report, query members are:
 * 0 - sail_routecoordinate.seqno
 * 1 - sail_routecoordinate.coordinate_idx
 * 2 - sail_routecoordinate.comment
 * 3 - sail_coordinate.latitude
 * 4 - sail_coordinate.longitude
 * @param query query object to be set with data
 * @param parentId parent ID
 */
void SAIL_SqlCommonFunctions::selectRouteCoordinates(QSqlQuery& query,
                                                     RB_String& parentId) {
    /*
    SELECT sail_routecoordinate.seqno, sail_routecoordinate.coordinate_idx, sail_routecoordinate.comment,
    sail_coordinate.latitude, sail_coordinate.longitude
    FROM sail_routecoordinate
    INNER JOIN sail_coordinate ON sail_coordinate.id=SUBSTR(sail_routecoordinate.coordinate_idx, 1, 38)
    WHERE sail_routecoordinate.parent='{3bee1110-614e-48e2-9c6e-9eb91840d524}'
    ORDER BY sail_routecoordinate.seqno ASC;
    */

    RB_String qStr = "";
    qStr = "SELECT sail_routecoordinate.seqno, sail_routecoordinate.coordinate_idx, "
                "sail_routecoordinate.comment, sail_coordinate.latitude, sail_coordinate.longitude "
            "FROM sail_routecoordinate "
            "INNER JOIN sail_coordinate ON sail_coordinate.id=";
    substrIdxId("sail_routecoordinate.coordinate_idx", qStr);
    qStr += " WHERE sail_routecoordinate.parent='" + parentId + "'";
    qStr += " ORDER BY sail_routecoordinate.seqno ASC;";

    if (!query.exec(qStr)) {
        RB_DEBUG->error("SAIL_SqlCommonFunctions::selectRouteCoordinates() "
                        + query.lastError().text() + " ERROR");
        SAIL_DIALOGFACTORY->commandMessage("SAIL_SqlCommonFunctions::selectTrackDistanceData() "
                                          + query.lastError().text() + "\n");
    }
}
