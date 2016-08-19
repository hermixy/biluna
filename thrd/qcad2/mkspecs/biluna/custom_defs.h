#define QC_SUBVERSION "" /* subversion (remove this for final releases) */

#ifndef QC_SUBVERSION
# define QC_SUBVERSION
#endif

#ifdef RS_DEMO
# define QC_APPNAME "QCad 2008 Demo" QC_SUBVERSION
# define QC_CUSTOM_SPLASH ":/biluna/images/qcad_biluna_2012_splash_demo.png"
//# define QC_SPLASH_SMALL ":/qcad/images/qcad_oopl_2008_splash_demo_small.png"
# define QC_SPLASH_SMALL ":/qcad/images/qcad_oopl_2008_splash_demo.png"
# define QC_APPKEY "/BilunaDemo"
# define QC_APPDIR "qcaddemo"
#else
# define QC_APPNAME "Biluna 2008" QC_SUBVERSION
# define QC_CUSTOM_SPLASH ":/qcad/images/qcad_oopl_2008_splash.png"
//# define QC_SPLASH_SMALL ":/qcad/images/qcad_oopl_2008_splash_small.png"
# define QC_SPLASH_SMALL ":/qcad/images/qcad_oopl_2008_splash.png"
# define QC_APPKEY "/Biluna"
# define QC_APPDIR "qcad"
#endif

#define QC_COMPANYKEY "/OpenOfficePL"

#define QC_DELAYED_SPLASH_SCREEN 1

#define QC_SPLASH_TXTCOL Qt::white


#define QC_PREDEFINED_LOCALE "pl" /* locale is predefined: no choice at 1st run, etc. */

#define QC_PREDEFINED_UNIT "Millimeter"

#define QC_ABOUT_HEADER "<img src=\"" QC_SPLASH_SMALL "\"><br>"

#define QC_ABOUT_ADD_COMPANY "<img src=\":/qcad/images/openoffice_logo.png\"><br>(c) 2004-2008 by OpenOffice Polska<br><a href=\"http://www.openoffice.com.pl\">http://www.openoffice.com.pl</a>"
#define QC_ABOUT_NO_ICON
#define QC_ABOUT_NO_QT_VER

#define QC_CUSTOM_VENDOR "OpenOffice Polska,\ntel. (0 22) 736 76 60\ninfolinia 0 801 557 097\nfax. (0 22) 736 76 77\ne-mail: info@openoffice.com.pl"

#define QC_APP_ICON "qcad_oopl.png"
#define QC_APP_ICON16 "qcad_oopl16.png"
