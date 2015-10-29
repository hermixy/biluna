#-------------------------------------------------
# $Id: srm.pri 1848 2012-12-23 14:40:30Z rutger $
# Project created 2013-02-21T14:00:00
# - for building supplier relation management plugin SRM
# - ../src path is required to use .pri file also from ../test
# ----------------------------------------------------------

HEADERS += \
    ../../acc/src/model/acc_contact.h \
    ../../acc/src/model/acc_project.h \
    ../../acc/src/model/acc_purchdata.h \
    ../../acc/src/model/acc_supplier.h \
    ../src/actions/srm_actionactivity.h \
    ../src/actions/srm_actionactivityreport.h \
    ../src/actions/srm_actioncampaigntarget.h \
    ../src/actions/srm_actionclose.h \
    ../src/actions/srm_actioncontact.h \
    ../src/actions/srm_actioncontactbysuppreport.h \
    ../src/actions/srm_actioncontactreport.h \
    ../src/actions/srm_actiondocument.h \
    ../src/actions/srm_actionfactory.h \
    ../src/actions/srm_actiongroupcontact.h \
    ../src/actions/srm_actionleadsourcetype.h \
    ../src/actions/srm_actionplugin.h \
    ../src/actions/srm_actionprojectedit.h \
#    ../src/actions/srm_actionselectcontact.h \
#    ../src/actions/srm_actionselectsupplier.h \
    ../src/actions/srm_actionselectproject.h \
    ../src/actions/srm_actionsupplier.h \
    ../src/actions/srm_actionsupplierreport.h \
    ../src/actions/srm_actionsysseqno.h \
    ../src/actions/srm_actiontemplate.h \
    ../src/gui/srm_activityreportwidget.h \
    ../src/gui/srm_activitywidget.h \
    ../src/gui/srm_campaigntargetwidget.h \
    ../src/gui/srm_contactbysuppreportwidget.h \
    ../src/gui/srm_contactreportwidget.h \
    ../src/gui/srm_contactwidget.h \
    ../src/gui/srm_dialogfactory.h \
    ../src/gui/srm_documentwidget.h \
    ../src/gui/srm_leadsourcetypedialog.h \
    ../src/gui/srm_groupcontactwidget.h \
    ../src/gui/srm_projectdialog.h \
    ../src/gui/srm_projecteditdialog.h \
    ../src/gui/srm_reportwidget.h \
    ../src/gui/srm_selectcontactdialog.h \
    ../src/gui/srm_selectsupplierdialog.h \
    ../src/gui/srm_selectgroupdialog.h \
    ../src/gui/srm_selectprojectdialog.h \
    ../src/gui/srm_selecttemplatedialog.h \
    ../src/gui/srm_supplierreportwidget.h \
    ../src/gui/srm_supplierwidget.h \
    ../src/gui/srm_sysseqnodialog.h \
    ../src/gui/srm_templatewidget.h \
    ../src/model/srm_activity.h \
    ../src/model/srm_campaign.h \
    ../src/model/srm_campaigntarget.h \
    ../src/model/srm_campaigntype.h \
    ../src/model/srm_contactdetail.h \
    ../src/model/srm_document.h \
    ../src/model/srm_group.h \
    ../src/model/srm_groupcontact.h \
    ../src/model/srm_leadsourcetype.h \
    ../src/model/srm_modelfactory.h \
    ../src/model/srm_objectfactory.h \
    ../src/model/srm_supplierdetail.h \
    ../src/model/srm_sysseqno.h \
    ../src/model/srm_syssetting.h \
    ../src/model/srm_template.h \
    ../src/operations/srm_printtext.h \
    ../src/utils/srm_sqlcommonfunctions.h

SOURCES += \
    ../../acc/src/model/acc_contact.cpp \
    ../../acc/src/model/acc_project.cpp \
    ../../acc/src/model/acc_purchdata.cpp \
    ../../acc/src/model/acc_supplier.cpp \
    ../src/actions/srm_actionactivity.cpp \
    ../src/actions/srm_actionactivityreport.cpp \
    ../src/actions/srm_actioncampaigntarget.cpp \
    ../src/actions/srm_actionclose.cpp \
    ../src/actions/srm_actioncontact.cpp \
    ../src/actions/srm_actioncontactbysuppreport.cpp \
    ../src/actions/srm_actioncontactreport.cpp \
    ../src/actions/srm_actiondocument.cpp \
    ../src/actions/srm_actionfactory.cpp \
    ../src/actions/srm_actiongroupcontact.cpp \
    ../src/actions/srm_actionleadsourcetype.cpp \
    ../src/actions/srm_actionplugin.cpp \
    ../src/actions/srm_actionprojectedit.cpp \
#    ../src/actions/srm_actionselectcontact.cpp \
#    ../src/actions/srm_actionselectsupplier.cpp \
    ../src/actions/srm_actionselectproject.cpp \
    ../src/actions/srm_actionsupplier.cpp \
    ../src/actions/srm_actionsupplierreport.cpp \
    ../src/actions/srm_actionsysseqno.cpp \
    ../src/actions/srm_actiontemplate.cpp \
    ../src/gui/srm_activityreportwidget.cpp \
    ../src/gui/srm_activitywidget.cpp \
    ../src/gui/srm_campaigntargetwidget.cpp \
    ../src/gui/srm_contactbysuppreportwidget.cpp \
    ../src/gui/srm_contactreportwidget.cpp \
    ../src/gui/srm_contactwidget.cpp \
    ../src/gui/srm_dialogfactory.cpp \
    ../src/gui/srm_documentwidget.cpp \
    ../src/gui/srm_groupcontactwidget.cpp \
    ../src/gui/srm_projecteditdialog.cpp \
    ../src/gui/srm_reportwidget.cpp \
    ../src/gui/srm_supplierreportwidget.cpp \
    ../src/gui/srm_supplierwidget.cpp \
    ../src/gui/srm_templatewidget.cpp \
    ../src/model/srm_activity.cpp \
    ../src/model/srm_campaign.cpp \
    ../src/model/srm_campaigntarget.cpp \
    ../src/model/srm_campaigntype.cpp \
    ../src/model/srm_contactdetail.cpp \
    ../src/model/srm_document.cpp \
    ../src/model/srm_group.cpp \
    ../src/model/srm_groupcontact.cpp \
    ../src/model/srm_leadsourcetype.cpp \
    ../src/model/srm_modelfactory.cpp \
    ../src/model/srm_objectfactory.cpp \
    ../src/model/srm_supplierdetail.cpp \
    ../src/model/srm_sysseqno.cpp \
    ../src/model/srm_syssetting.cpp \
    ../src/model/srm_template.cpp \
    ../src/operations/srm_printtext.cpp \
    ../src/utils/srm_sqlcommonfunctions.cpp


FORMS += \
    ../src/gui/srm_activityreportwidget.ui \
    ../src/gui/srm_activitywidget.ui \
    ../src/gui/srm_campaigntargetwidget.ui \
    ../src/gui/srm_contactbysuppreportwidget.ui \
    ../src/gui/srm_contactreportwidget.ui \
    ../src/gui/srm_contactwidget.ui \
    ../src/gui/srm_documentwidget.ui \
    ../src/gui/srm_groupcontactwidget.ui \
    ../src/gui/srm_supplierreportwidget.ui \
    ../src/gui/srm_supplierwidget.ui \
    ../src/gui/srm_templatewidget.ui

RESOURCES += \
#    ../../acc/src/gui/acc.qrc \
    ../../db/src/gui/db.qrc

OTHER_FILES += \
    ../src/actions/srm_actionplugin.json \
    ../src/gui/srm_activityreportwidget.dox \
    ../src/gui/srm_activitywidget.dox \
    ../src/gui/srm_campaigntargetwidget.dox \
    ../src/gui/srm_contactbysuppreportwidget.dox \
    ../src/gui/srm_contactreportwidget.dox \
    ../src/gui/srm_contactwidget.dox \
    ../src/gui/srm_documentwidget.dox \
    ../src/gui/srm_groupcontactwidget.dox \
    ../src/gui/srm_index.dox \
    ../src/gui/srm_leadsourcetypedialog.dox \
    ../src/gui/srm_projectdialog.dox \
    ../src/gui/srm_projecteditdialog.dox \
    ../src/gui/srm_supplierreportwidget.dox \
    ../src/gui/srm_supplierwidget.dox \
    ../src/gui/srm_sysseqnodialog.dox \
    ../src/gui/srm_templatewidget.dox
