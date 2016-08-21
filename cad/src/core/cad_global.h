/*****************************************************************
 * $Id: cad_global.h 2248 2016-08-21 09:13:00Z rutger $
 * Created: 2016-08-21 17:53:01 - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna CAD project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include <QtCore/QtGlobal>

#if defined(CAD_DLL)
#    define CAD_EXPORT Q_DECL_EXPORT
#else
#    define CAD_EXPORT Q_DECL_IMPORT
#endif
