/*****************************************************************
 * $Id: db_global.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: 2015-06-08 17:53:01 - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include <QtCore/QtGlobal>

#if defined(DB_DLL)
#    define DB_EXPORT Q_DECL_EXPORT
#else
#    define DB_EXPORT Q_DECL_IMPORT
#endif
