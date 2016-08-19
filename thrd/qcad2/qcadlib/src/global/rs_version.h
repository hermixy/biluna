/****************************************************************************
** $Id: rs.h 4006 2006-12-27 19:02:50Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef RS_VERSION_H
#define RS_VERSION_H

#define RS_VERSION "2.2.2.0"

#ifndef QC_APPNAME
#  ifdef RS_DEMO
#    ifdef RS_CAM
#      define QC_APPNAME "CAM Expert Demo"
#    else
#      define QC_APPNAME "Cad Demo"
#    endif
#  else
#    ifdef RS_CAM
#      define QC_APPNAME "CAM Expert"
#    else
#      define QC_APPNAME "Cad"
#    endif
#  endif
#endif

#ifndef QC_COMPANYKEY
// # define QC_COMPANYKEY "RibbonSoft"
# define QC_COMPANYKEY "Biluna"
#endif

#ifdef RS_DEMO
# ifndef QC_APPKEY
#  define QC_APPKEY "/CadDemo"
# endif
# ifndef QC_APPDIR
#  define QC_APPDIR "caddemo"
# endif
#else
# ifndef QC_APPKEY
#  define QC_APPKEY "/Cad"
# endif
# ifndef QC_APPDIR
#  define QC_APPDIR "cad"
# endif
#endif

#ifndef QC_COMPANYDOMAIN
# define QC_COMPANYDOMAIN "biluna.com"
#endif

#endif
