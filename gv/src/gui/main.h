/*****************************************************************
 * $Id: main.h 692 2008-04-25 16:31:15Z rutger $
 * Created: Mar 19, 2008 9:24:57 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef MAIN_H
#define MAIN_H

// #include <QStringList>

#define RB_APPDIR "gv"
#define RB_APPKEY "/GV"

#ifdef RB_DEMO
# define RB_APPNAME "GV Demo"
#else
# define RB_APPNAME "GV - Graphics View for P&ID"
#endif

#define RB_COMPANYDOMAIN "red-bag.com"
#define RB_COMPANYKEY "Red-Bag"
#define RB_VERSION "0.8.4.24" // 2007-02-19

#ifndef RB_DEBUGGING
# define RB_DEBUGGING false
#endif


// QStringList handleArgs(int argc, char** argv);


#endif /*MAIN_H*/
