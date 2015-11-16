# ----------------------------------------------------------
# $Id: db.pri 2248 2015-06-21 09:13:00Z rutger $
# Created: Nov 22, 2009 11:34:42 AM - rutger
#
# Copyright (C) 2009 Red-Bag. All rights reserved.
# This file is part of the Biluna DB project.
#
# See http://www.red-bag.com for further details.
#
# Comments:
# - form, header and sources listing
# - ../src path is required to use .pri file also from ../test
# ----------------------------------------------------------

FORMS = \
    ../src/gui/db_aboutdialog.ui \
    ../src/gui/db_browserdialog.ui \
    ../src/gui/db_calendarwidget.ui \
    ../src/gui/db_commandwidget.ui \
    ../src/gui/db_connectiondialog.ui \
    ../src/gui/db_dashboardwidget.ui \
#    ../src/gui/db_databasemanagementdialog.ui \
#    ../src/gui/db_databaseselector.ui \
    ../src/gui/db_filterdialog.ui \
    ../src/gui/db_findreplacedialog.ui \
    ../src/gui/db_hexeditwidget.ui \
    ../src/gui/db_htmleditorwidget.ui \
    ../src/gui/db_inserthtmldialog.ui \
    ../src/gui/db_internetbrowserwidget.ui \
    ../src/gui/db_mappingdialog.ui \
    ../src/gui/db_palettecolorpickerwidget.ui \
    ../src/gui/db_permissionperspectiveprojectdialog.ui \
    ../src/gui/db_permissionwidget.ui \
    ../src/gui/db_plaintextwidget.ui \
    ../src/gui/db_plugintesterdialog.ui \
    ../src/gui/db_prepostfixtextdialog.ui \
    ../src/gui/db_screenshotdialog.ui \
    ../src/gui/db_seltreesiblingdialog.ui \
    ../src/gui/db_simplereportwidget.ui \
    ../src/gui/db_systemgroupdialog.ui \
    ../src/gui/db_tabledialog.ui \
    ../src/gui/db_textwidget.ui \
    ../src/gui/db_treedialog.ui \
    ../src/gui/db_xmlviewvalidatorwidget.ui \
    ../src/gui/rb_dialogbase.ui \
    ../src/gui/rb_idxlineedit.ui \
    ../src/gui/rb_projecteditdialog.ui \
    ../src/gui/rb_toolbuttonbar.ui \
    $$PWD/gui/rb_membereditdialog.ui


HEADERS = \
    ../src/actions/db_actionfactory.h \
    ../src/actions/db_actionfilenew.h \
    ../src/actions/db_actionfilenewhtmleditor.h \
    ../src/actions/db_actionfilenewhtmlview.h \
    ../src/actions/db_actionfilenewplaintexteditor.h \
    ../src/actions/db_actionfileopen.h \
    ../src/actions/db_actionfileopenmultiple.h \
    ../src/actions/db_actionfilepdf.h \
    ../src/actions/db_actionfileprint.h \
    ../src/actions/db_actionfileprintpreview.h \
    ../src/actions/db_actionfilequit.h \
    ../src/actions/db_actionfilesaveas.h \
    ../src/actions/db_actionfilesave.h \
    ../src/actions/db_actionformataligncenter.h \
    ../src/actions/db_actionformatalignjustify.h \
    ../src/actions/db_actionformatalignleft.h \
    ../src/actions/db_actionformatalignright.h \
    ../src/actions/db_actionformatbackgroundcolor.h \
    ../src/actions/db_actionformatbold.h \
    ../src/actions/db_actionformatbulletedlist.h \
    ../src/actions/db_actionformatdecreaseindent.h \
    ../src/actions/db_actionformatfontname.h \
    ../src/actions/db_actionformatfontsize.h \
    ../src/actions/db_actionformatincreaseindent.h \
    ../src/actions/db_actionformatitalic.h \
    ../src/actions/db_actionformatnumberedlist.h \
    ../src/actions/db_actionformatremove.h \
    ../src/actions/db_actionformatstrikethrough.h \
    ../src/actions/db_actionformattextcolor.h \
    ../src/actions/db_actionformatunderline.h \
    ../src/actions/db_actioneditcopy.h \
    ../src/actions/db_actioneditcreatelink.h \
    ../src/actions/db_actioneditcut.h \
    ../src/actions/db_actioneditfind.h \
    ../src/actions/db_actioneditfindreplace.h \
    ../src/actions/db_actioneditinserthtml.h \
    ../src/actions/db_actioneditinsertimage.h \
    ../src/actions/db_actioneditpaste.h \
    ../src/actions/db_actioneditredo.h \
    ../src/actions/db_actioneditselectall.h \
    ../src/actions/db_actioneditundo.h \
    ../src/actions/db_actioneditzoomin.h \
    ../src/actions/db_actioneditzoomout.h \
    ../src/actions/db_actionhelpabout.h \
    ../src/actions/db_actionhelpcredits.h \
    ../src/actions/db_actionhelpindex.h \
    ../src/actions/db_actionhelpsubject.h \
    ../src/actions/db_actionstyleaddress.h \
    ../src/actions/db_actionstyleheading1.h \
    ../src/actions/db_actionstyleheading2.h \
    ../src/actions/db_actionstyleheading3.h \
    ../src/actions/db_actionstyleheading4.h \
    ../src/actions/db_actionstyleheading5.h \
    ../src/actions/db_actionstyleheading6.h \
    ../src/actions/db_actionstyleparagraph.h \
    ../src/actions/db_actionstylepreformatted.h \
    ../src/actions/db_actionsystemcalendar.h \
    ../src/actions/db_actionsystemclearsettings.h \
    ../src/actions/db_actionsystemdatabasebrowser.h \
    ../src/actions/db_actionsystemdatabaseconnect.h \
    ../src/actions/db_actionsystemdatabasedisconnect.h \
    ../src/actions/db_actionsystemdatabasemapping.h \
    ../src/actions/db_actionsystemeditproject.h \
    ../src/actions/db_actionsystemgroup.h \
    ../src/actions/db_actionsystempermission.h \
    ../src/actions/db_actionsystemselectproject.h \
    ../src/actions/db_actionsystemstylecolorpicker.h \
    ../src/actions/db_actionsystemstyledark.h \
    ../src/actions/db_actionsystemtoolsdevelop.h \
    ../src/actions/db_actionsystemtoolsexportmodel.h \
    ../src/actions/db_actionsystemtoolsimportmodel.h \
    ../src/actions/db_actionsystemtoolsplugins.h \
    ../src/actions/db_actionsystemtoolsregexp.h \
    ../src/actions/db_actionsystemtoolssignalspy.h \
    ../src/actions/db_actionsystemtoolsviewmodel.h \
    ../src/actions/db_actionsystemversion.h \
    ../src/actions/db_actiontoolscalculator.h \
    ../src/actions/db_actiontoolsscreenshot.h \
    ../src/actions/db_actiontoolssimplechart.h \
    ../src/actions/db_actiontoolssimplereport.h \
    ../src/actions/db_actiontoolstextfilelist.h \
    ../src/actions/db_actiontoolsxmlview.h \
    ../src/actions/db_transferdataaction.h \
    ../src/actions/rb_action.h \
    ../src/actions/rb_actiondebugexportmodel.h \
    ../src/actions/rb_actiondebugplugin.h \
#    ../src/actions/rb_actiondebugsignalspy.h \
    ../src/actions/rb_actionfactory.h \
    ../src/actions/rb_actionplugin.h \
    ../src/actions/rb_guiaction.h \
    ../src/entities/db_activityitem.h \
    ../src/entities/db_calendaritem.h \
    ../src/entities/db_calendarstyle.h \
    ../src/entities/db_clockbar.h \
    ../src/entities/db_daycontentitem.h \
    ../src/entities/db_dayitem.h \
    ../src/entities/db_scrollareaitem.h \
    ../src/entities/db_scrollbar.h \
    ../src/entities/db_weekdayheaderitem.h \
    ../src/entities/db_weekheaderitem.h \
    ../src/entities/db_weekitem.h \
    ../src/filters/db_csvparser.h \
    ../src/gui/db_aboutdialog.h \
    ../src/gui/db_browserdialog.h \
    ../src/gui/db_calculatorwidget.h \
    ../src/gui/db_calendarview.h \
    ../src/gui/db_calendarwidget.h \
    ../src/gui/db_codehighlighter.h \
    ../src/gui/db_commandevent.h \
    ../src/gui/db_commandedit.h \
    ../src/gui/db_commandwidget.h \
    ../src/gui/db_connectiondialog.h \
    ../src/gui/db_connectionwidget.h \
    ../src/gui/db_cpphighlighter.h \
    ../src/gui/db_dashboardwidget.h \
#    ../src/gui/db_databasedescription.h \
#    ../src/gui/db_databasedescription_p.h \
#    ../src/gui/db_databasemanagementdialog.h \
#    ../src/gui/db_databaseselector.h \
    ../src/gui/db_dialogfactory.h \
    ../src/gui/db_filterdialog.h \
    ../src/gui/db_findreplacedialog.h \
    ../src/gui/db_hexeditwidget.h \
    ../src/gui/db_htmleditorwidget.h \
    ../src/gui/db_htmlhighlighter.h \
#    ../src/gui/db_htmlviewwidget.h \
    ../src/gui/db_inserthtmldialog.h \
    ../src/gui/db_internetbrowserfactory.h \
    ../src/gui/db_internetbrowserwidget.h \
    ../src/gui/db_itemdelegate.h \
    ../src/gui/db_keypadfunction.h \
    ../src/gui/db_keypadnumeric.h \
    ../src/gui/db_mappingdelegate.h \
    ../src/gui/db_mappingdialog.h \
    ../src/gui/db_palettecolorpickerwidget.h \
    ../src/gui/db_permissionperspectiveprojectdialog.h \
    ../src/gui/db_permissionwidget.h \
    ../src/gui/db_plaintextwidget.h \
    ../src/gui/db_plugintesterdialog.h \
    ../src/gui/db_prepostfixtextdialog.h \
    ../src/gui/db_projectdialog.h \
    ../src/gui/db_projecteditdialog.h \
    ../src/gui/db_regexpdialog.h \
    ../src/gui/db_screenshotdialog.h \
    ../src/gui/db_selectmappingdialog.h \
    ../src/gui/db_seltreesiblingdialog.h \
    ../src/gui/db_simplereportwidget.h \
    ../src/gui/db_sqlhighlighter.h \
    ../src/gui/db_systemgroupdialog.h \
    ../src/gui/db_tabledialog.h \
    ../src/gui/db_treedialog.h \
    ../src/gui/db_textwidget.h \
    ../src/gui/db_versiondialog.h \
    ../src/gui/db_xmlviewvalidatorwidget.h \
    ../src/gui/rb_cmbdelegate.h \
    ../src/gui/rb_codetextedit.h \
    ../src/gui/rb_datawidgetmapper.h \
    ../src/gui/rb_dialog.h \
    ../src/gui/rb_dialogbase.h \
    ../src/gui/rb_dialogfactory.h \
    ../src/gui/rb_dockwidget.h \
    ../src/gui/rb_flowlayout.h \
    ../src/gui/rb_idxlineedit.h \
    ../src/gui/rb_ledelegate.h \
    ../src/gui/rb_mainwindow.h \
    ../src/gui/rb_mdiwindow.h \
    ../src/gui/rb_memberdelegate.h \
    ../src/gui/rb_plaintextedit.h \
    ../src/gui/rb_plotterwidget.h \
    ../src/gui/rb_projecteditdialog.h \
    ../src/gui/rb_simplereportwidget.h \
    ../src/gui/rb_sqlrelationaldelegate.h \
    ../src/gui/rb_tableview.h \
    ../src/gui/rb_tedelegate.h \
    ../src/gui/rb_toolbar.h \
    ../src/gui/rb_toolbuttonbar.h \
    ../src/gui/rb_textedit.h \
    ../src/gui/rb_textpreview.h \
    ../src/gui/rb_treeview.h \
#    ../src/gui/rb_webfactory.h \
    ../src/gui/rb_webview.h \
#    ../src/gui/rb_webwidget.h \
    ../src/gui/rb_widget.h \
    ../src/model/db_activity.h \
    ../src/model/db_calendar.h \
    ../src/model/db_map.h \
    ../src/model/db_mapping.h \
    ../src/model/db_modelfactory.h \
    ../src/model/db_objectfactory.h \
    ../src/model/db_permissiongroup.h \
    ../src/model/db_permissionperspectiveproject.h \
    ../src/model/db_permissionproject.h \
    ../src/model/db_project.h \
    ../src/model/db_simplereport.h \
    ../src/model/db_systemgroup.h \
    ../src/model/db_systemuser.h \
    ../src/model/db_systemusergroup.h \
    ../src/model/db_systemuserpermission.h \
    ../src/model/db_version.h \
    ../src/model/rb.h \
    ../src/model/rb_flags.h \
    ../src/model/rb_database.h \
    ../src/model/rb_mmabstract.h \
    ../src/model/rb_mmproxy.h \
    ../src/model/rb_mmsource.h \
    ../src/model/rb_modelfactory.h \
    ../src/model/rb_object.h \
    ../src/model/rb_objectatomic.h \
    ../src/model/rb_objectbase.h \
    ../src/model/rb_objectcontainer.h \
    ../src/model/rb_objectfactory.h \
    ../src/model/rb_objectiterator.h \
    ../src/model/rb_objectmember.h \
    ../src/operations/rb_operationhtml.h \
    ../src/utils/db_global.h \
    ../src/utils/db_permissionhandler.h \
    ../src/utils/db_sqlcommonfunctions.h \
    ../src/utils/db_utilityfactory.h \
    ../src/utils/rb_aesencryption.h \
    ../src/utils/rb_debug.h \
    ../src/utils/rb_hash.h \
    ../src/utils/rb_list.h \
    ../src/utils/rb_printer.h \
    ../src/utils/rb_printtextheaderfooter.h \
    ../src/utils/rb_settings.h \
    ../src/utils/rb_signalspydialog.h \
    ../src/utils/rb_sqlcommonfunctions.h \
    ../src/utils/rb_sqlrecord.h \
    ../src/utils/rb_sqlrelation.h \
    ../src/utils/rb_stringlist.h \
    ../src/utils/rb_string.h \
    ../src/utils/rb_system.h \
    ../src/utils/rb_tcpsocket.h \
    ../src/utils/rb_urlbasedrenderer.h \
    ../src/utils/rb_utility.h \
    ../src/utils/rb_utilityfactory.h \
    ../src/utils/rb_uuid.h \
    ../src/utils/rb_variant.h \
    ../src/visitors/db_xmlreader.h \
    ../src/visitors/rb_csvwriter.h \
    ../src/visitors/rb_dbvisitor.h \
    ../src/visitors/rb_flagvisitor.h \
    ../src/visitors/rb_htmlwriter.h \
    ../src/visitors/rb_visitor.h \
    ../src/visitors/rb_xmlreader.h \
    ../src/visitors/rb_xmlwriter.h \
    $$PWD/gui/rb_membereditdialog.h

SOURCES = \
    ../src/actions/db_actionfactory.cpp \
    ../src/actions/db_actionfilenew.cpp \
    ../src/actions/db_actionfilenewhtmleditor.cpp \
    ../src/actions/db_actionfilenewhtmlview.cpp \
    ../src/actions/db_actionfilenewplaintexteditor.cpp \
    ../src/actions/db_actionfileopen.cpp \
    ../src/actions/db_actionfileopenmultiple.cpp \
    ../src/actions/db_actionfilepdf.cpp \
    ../src/actions/db_actionfileprint.cpp \
    ../src/actions/db_actionfileprintpreview.cpp \
    ../src/actions/db_actionfilequit.cpp \
    ../src/actions/db_actionfilesaveas.cpp \
    ../src/actions/db_actionfilesave.cpp \
    ../src/actions/db_actionformataligncenter.cpp \
    ../src/actions/db_actionformatalignjustify.cpp \
    ../src/actions/db_actionformatalignleft.cpp \
    ../src/actions/db_actionformatalignright.cpp \
    ../src/actions/db_actionformatbackgroundcolor.cpp \
    ../src/actions/db_actionformatbold.cpp \
    ../src/actions/db_actionformatbulletedlist.cpp \
    ../src/actions/db_actionformatdecreaseindent.cpp \
    ../src/actions/db_actionformatfontname.cpp \
    ../src/actions/db_actionformatfontsize.cpp \
    ../src/actions/db_actionformatincreaseindent.cpp \
    ../src/actions/db_actionformatitalic.cpp \
    ../src/actions/db_actionformatnumberedlist.cpp \
    ../src/actions/db_actionformatremove.cpp \
    ../src/actions/db_actionformatstrikethrough.cpp \
    ../src/actions/db_actionformattextcolor.cpp \
    ../src/actions/db_actionformatunderline.cpp \
    ../src/actions/db_actioneditcopy.cpp \
    ../src/actions/db_actioneditcreatelink.cpp \
    ../src/actions/db_actioneditcut.cpp \
    ../src/actions/db_actioneditfind.cpp \
    ../src/actions/db_actioneditfindreplace.cpp \
    ../src/actions/db_actioneditinserthtml.cpp \
    ../src/actions/db_actioneditinsertimage.cpp \
    ../src/actions/db_actioneditpaste.cpp \
    ../src/actions/db_actioneditredo.cpp \
    ../src/actions/db_actioneditselectall.cpp \
    ../src/actions/db_actioneditundo.cpp \
    ../src/actions/db_actioneditzoomin.cpp \
    ../src/actions/db_actioneditzoomout.cpp \
    ../src/actions/db_actionhelpabout.cpp \
    ../src/actions/db_actionhelpcredits.cpp \
    ../src/actions/db_actionhelpindex.cpp \
    ../src/actions/db_actionhelpsubject.cpp \
    ../src/actions/db_actionstyleaddress.cpp \
    ../src/actions/db_actionstyleheading1.cpp \
    ../src/actions/db_actionstyleheading2.cpp \
    ../src/actions/db_actionstyleheading3.cpp \
    ../src/actions/db_actionstyleheading4.cpp \
    ../src/actions/db_actionstyleheading5.cpp \
    ../src/actions/db_actionstyleheading6.cpp \
    ../src/actions/db_actionstyleparagraph.cpp \
    ../src/actions/db_actionstylepreformatted.cpp \
    ../src/actions/db_actionsystemcalendar.cpp \
    ../src/actions/db_actionsystemclearsettings.cpp \
    ../src/actions/db_actionsystemdatabasebrowser.cpp \
    ../src/actions/db_actionsystemdatabaseconnect.cpp \
    ../src/actions/db_actionsystemdatabasedisconnect.cpp \
    ../src/actions/db_actionsystemdatabasemapping.cpp \
    ../src/actions/db_actionsystemeditproject.cpp \
    ../src/actions/db_actionsystemgroup.cpp \
    ../src/actions/db_actionsystempermission.cpp \
    ../src/actions/db_actionsystemselectproject.cpp \
    ../src/actions/db_actionsystemstylecolorpicker.cpp \
    ../src/actions/db_actionsystemstyledark.cpp \
    ../src/actions/db_actionsystemtoolsdevelop.cpp \
    ../src/actions/db_actionsystemtoolsexportmodel.cpp \
    ../src/actions/db_actionsystemtoolsimportmodel.cpp \
    ../src/actions/db_actionsystemtoolsplugins.cpp \
    ../src/actions/db_actionsystemtoolsregexp.cpp \
    ../src/actions/db_actionsystemtoolssignalspy.cpp \
    ../src/actions/db_actionsystemtoolsviewmodel.cpp \
    ../src/actions/db_actionsystemversion.cpp \
    ../src/actions/db_actiontoolscalculator.cpp \
    ../src/actions/db_actiontoolsscreenshot.cpp \
    ../src/actions/db_actiontoolssimplechart.cpp \
    ../src/actions/db_actiontoolssimplereport.cpp \
    ../src/actions/db_actiontoolstextfilelist.cpp \
    ../src/actions/db_actiontoolsxmlview.cpp \
    ../src/actions/db_transferdataaction.cpp \
    ../src/actions/rb_action.cpp \
    ../src/actions/rb_actiondebugexportmodel.cpp \
    ../src/actions/rb_actiondebugplugin.cpp \
#    ../src/actions/rb_actiondebugsignalspy.cpp \
    ../src/actions/rb_actionfactory.cpp \
    ../src/actions/rb_guiaction.cpp \
    ../src/entities/db_activityitem.cpp \
    ../src/entities/db_calendaritem.cpp \
    ../src/entities/db_calendarstyle.cpp \
    ../src/entities/db_clockbar.cpp \
    ../src/entities/db_daycontentitem.cpp \
    ../src/entities/db_dayitem.cpp \
    ../src/entities/db_scrollbar.cpp \
    ../src/entities/db_scrollareaitem.cpp \
    ../src/entities/db_weekdayheaderitem.cpp \
    ../src/entities/db_weekitem.cpp \
    ../src/filters/db_csvparser.cpp \
    ../src/gui/db_aboutdialog.cpp \
    ../src/gui/db_browserdialog.cpp \
    ../src/gui/db_calculatorwidget.cpp \
    ../src/gui/db_calendarview.cpp \
    ../src/gui/db_calendarwidget.cpp \
    ../src/gui/db_codehighlighter.cpp \
    ../src/gui/db_commandedit.cpp \
    ../src/gui/db_commandwidget.cpp \
    ../src/gui/db_connectiondialog.cpp \
    ../src/gui/db_connectionwidget.cpp \
    ../src/gui/db_cpphighlighter.cpp \
    ../src/gui/db_dashboardwidget.cpp \
#    ../src/gui/db_databasedescription.cpp \
#    ../src/gui/db_databasedescription_p.cpp \
#    ../src/gui/db_databasemanagementdialog.cpp \
#    ../src/gui/db_databaseselector.cpp \
    ../src/gui/db_dialogfactory.cpp \
    ../src/gui/db_filterdialog.cpp \
    ../src/gui/db_findreplacedialog.cpp \
    ../src/gui/db_hexeditwidget.cpp \
    ../src/gui/db_htmleditorwidget.cpp \
    ../src/gui/db_htmlhighlighter.cpp \
#    ../src/gui/db_htmlviewwidget.cpp \
    ../src/gui/db_inserthtmldialog.cpp \
    ../src/gui/db_internetbrowserfactory.cpp \
    ../src/gui/db_internetbrowserwidget.cpp \
    ../src/gui/db_itemdelegate.cpp \
    ../src/gui/db_keypadfunction.cpp \
    ../src/gui/db_keypadnumeric.cpp \
    ../src/gui/db_mappingdelegate.cpp \
    ../src/gui/db_mappingdialog.cpp \
    ../src/gui/db_palettecolorpickerwidget.cpp \
    ../src/gui/db_permissionperspectiveprojectdialog.cpp \
    ../src/gui/db_permissionwidget.cpp \
    ../src/gui/db_plaintextwidget.cpp \
    ../src/gui/db_plugintesterdialog.cpp \
    ../src/gui/db_prepostfixtextdialog.cpp \
    ../src/gui/db_projecteditdialog.cpp \
    ../src/gui/db_regexpdialog.cpp \
    ../src/gui/db_screenshotdialog.cpp \
    ../src/gui/db_seltreesiblingdialog.cpp \
    ../src/gui/db_simplereportwidget.cpp \
    ../src/gui/db_sqlhighlighter.cpp \
    ../src/gui/db_systemgroupdialog.cpp \
    ../src/gui/db_tabledialog.cpp \
    ../src/gui/db_treedialog.cpp \
    ../src/gui/db_textwidget.cpp \
    ../src/gui/db_xmlviewvalidatorwidget.cpp \
    ../src/gui/rb_cmbdelegate.cpp \
    ../src/gui/rb_codetextedit.cpp \
    ../src/gui/rb_datawidgetmapper.cpp \
    ../src/gui/rb_dialog.cpp \
    ../src/gui/rb_dialogbase.cpp \
    ../src/gui/rb_dialogfactory.cpp \
    ../src/gui/rb_dockwidget.cpp \
    ../src/gui/rb_flowlayout.cpp \
    ../src/gui/rb_idxlineedit.cpp \
    ../src/gui/rb_mainwindow.cpp \
    ../src/gui/rb_mdiwindow.cpp \
    ../src/gui/rb_memberdelegate.cpp \
    ../src/gui/rb_ledelegate.cpp \
    ../src/gui/rb_plaintextedit.cpp \
    ../src/gui/rb_plotterwidget.cpp \
    ../src/gui/rb_projecteditdialog.cpp \
    ../src/gui/rb_simplereportwidget.cpp \
    ../src/gui/rb_sqlrelationaldelegate.cpp \
    ../src/gui/rb_tableview.cpp \
    ../src/gui/rb_tedelegate.cpp \
    ../src/gui/rb_textedit.cpp \
    ../src/gui/rb_textpreview.cpp \
    ../src/gui/rb_toolbar.cpp \
    ../src/gui/rb_toolbuttonbar.cpp \
    ../src/gui/rb_treeview.cpp \
#    ../src/gui/rb_webfactory.cpp \
    ../src/gui/rb_webview.cpp \
#    ../src/gui/rb_webwidget.cpp \
    ../src/gui/rb_widget.cpp \
    ../src/model/db_activity.cpp \
    ../src/model/db_calendar.cpp \
    ../src/model/db_map.cpp \
    ../src/model/db_mapping.cpp \
    ../src/model/db_modelfactory.cpp \
    ../src/model/db_objectfactory.cpp \
    ../src/model/db_permissiongroup.cpp \
    ../src/model/db_permissionperspectiveproject.cpp \
    ../src/model/db_permissionproject.cpp \
    ../src/model/db_project.cpp \
    ../src/model/db_simplereport.cpp \
    ../src/model/db_systemgroup.cpp \
    ../src/model/db_systemuser.cpp \
    ../src/model/db_systemusergroup.cpp \
    ../src/model/db_systemuserpermission.cpp \
    ../src/model/db_version.cpp \
    ../src/model/rb_database.cpp \
    ../src/model/rb_mmproxy.cpp \
    ../src/model/rb_mmsource.cpp \
    ../src/model/rb_modelfactory.cpp \
    ../src/model/rb_object.cpp \
    ../src/model/rb_objectatomic.cpp \
    ../src/model/rb_objectbase.cpp \
    ../src/model/rb_objectcontainer.cpp \
    ../src/model/rb_objectfactory.cpp \
    ../src/model/rb_objectiterator.cpp \
    ../src/model/rb_objectmember.cpp \
    ../src/operations/rb_operationhtml.cpp \
    ../src/utils/db_permissionhandler.cpp \
    ../src/utils/db_sqlcommonfunctions.cpp \
    ../src/utils/db_utilityfactory.cpp \
    ../src/utils/rb_aesencryption.cpp \
    ../src/utils/rb_debug.cpp \
    ../src/utils/rb_printer.cpp \
    ../src/utils/rb_printtextheaderfooter.cpp \
    ../src/utils/rb_settings.cpp \
    ../src/utils/rb_signalspydialog.cpp \
    ../src/utils/rb_sqlcommonfunctions.cpp \
    ../src/utils/rb_sqlrelation.cpp \
    ../src/utils/rb_string.cpp \
    ../src/utils/rb_system.cpp \
    ../src/utils/rb_tcpsocket.cpp \
    ../src/utils/rb_utility.cpp \
    ../src/utils/rb_utilityfactory.cpp \
    ../src/visitors/db_xmlreader.cpp \
    ../src/visitors/rb_csvwriter.cpp \
    ../src/visitors/rb_dbvisitor.cpp \
    ../src/visitors/rb_flagvisitor.cpp \
    ../src/visitors/rb_htmlwriter.cpp \
    ../src/visitors/rb_visitor.cpp \
    ../src/visitors/rb_xmlreader.cpp \
    ../src/visitors/rb_xmlwriter.cpp \
    $$PWD/gui/rb_membereditdialog.cpp


RESOURCES = \
    ../src/gui/db.qrc


OTHER_FILES = \
#    ../src/gui/db_aboutdialog.h \
    ../src/gui/db_browserdialog.dox \
    ../src/gui/db_calendarwidget.dox \
    ../src/gui/db_connectiondialog.dox \
    ../src/gui/db_createsqlitefile.dox \
#    ../src/gui/db_connectionwidget.h \
#    ../src/gui/db_cpphighlighter.h \
#    ../src/gui/db_dashboardwidget.h \
#    ../src/gui/db_dialogfactory.h \
#    ../src/gui/db_filterdialog.h \
#    ../src/gui/db_findreplacedialog.h \
#    ../src/gui/db_hexeditwidget.dox \
    ../src/gui/db_htmleditorwidget.dox \
#    ../src/gui/db_htmlhighlighter.h \
#    ../src/gui/db_htmlviewwidget.dox \
    ../src/gui/db_index.dox \
#    ../src/gui/db_internetbrowserfactory.h \
    ../src/gui/db_internetbrowserwidget.dox \
#    ../src/gui/db_mappingdelegate.h \
    ../src/gui/db_mappingdialog.dox \
    ../src/gui/db_palettecolorpickerwidget.dox \
    ../src/gui/db_permissionprojectdialog.dox \
    ../src/gui/db_permissionwidget.dox \
    ../src/gui/db_plaintextwidget.dox \
#    ../src/gui/db_plugintesterdialog.h \
#    ../src/gui/db_prepostfixtextdialog.h \
    ../src/gui/db_projectdialog.dox \
    ../src/gui/db_projecteditdialog.dox \
#    ../src/gui/db_selectmappingdialog.h \
#    ../src/gui/db_seltreesiblingdialog.h \
    ../src/gui/db_simplereportwidget.dox \
#    ../src/gui/db_sqlhighlighter.h \
    ../src/gui/db_systemdevelopment.dox \
    ../src/gui/db_systemgroup.dox \
#    ../src/gui/db_tabledialog.h \
#    ../src/gui/db_treedialog.h \
    ../src/gui/db_text_specialcharacters.dox \
    ../src/gui/db_text_symbolgreekcharacters.dox \
    ../src/gui/db_textwidget.dox \
    ../src/gui/db_tools.dox \
    ../src/gui/db_tutorialsexamples.dox \
    ../src/gui/db_versiondialog.dox \
#    ../src/gui/rb_cmbdelegate.h \
#    ../src/gui/db_commandevent.h \
#    ../src/gui/db_commandedit.h \
#    ../src/gui/db_commandwidget.h \
#    ../src/gui/rb_datawidgetmapper.h \
#    ../src/gui/rb_dialog.h \
#    ../src/gui/rb_dialogbase.h \
#    ../src/gui/rb_dialogfactory.h \
#    ../src/gui/rb_dockwidget.h \
#    ../src/gui/rb_idxlineedit.h \
#    ../src/gui/rb_ledelegate.h \
#    ../src/gui/rb_mainwindow.h \
#    ../src/gui/rb_mdiwindow.h \
#    ../src/gui/rb_plotterwidget.h \
#    ../src/gui/rb_sqlrelationaldelegate.h \
#    ../src/gui/rb_tableview.h \
#    ../src/gui/rb_tedelegate.h \
#    ../src/gui/rb_textedit.h \
#    ../src/gui/rb_treeview.h \
#    ../src/gui/rb_widget.h \
    ../src/gui/thrd_chartwidget.dox
