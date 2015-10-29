#-------------------------------------------------
# $Id: crm.pri 2185 2014-10-09 15:09:35Z rutger $
# Project created 2012-02-11T14:00:00
# - for building customer relation management plugin CRM
# - ../src path is required to use .pri file also from ../test
# ----------------------------------------------------------

HEADERS += \
    ../../acc/src/model/acc_contact.h \
    ../../acc/src/model/acc_customer.h \
    ../../acc/src/model/acc_project.h \
    ../../acc/src/model/acc_salesorder.h \
    ../src/actions/crm_actionactivity.h \
    ../src/actions/crm_actionactivityreport.h \
    ../src/actions/crm_actioncampaigntarget.h \
    ../src/actions/crm_actionclose.h \
    ../src/actions/crm_actioncontact.h \
    ../src/actions/crm_actioncontactbycustreport.h \
    ../src/actions/crm_actioncontactreport.h \
    ../src/actions/crm_actioncustomer.h \
    ../src/actions/crm_actioncustomerreport.h \
    ../src/actions/crm_actiondocument.h \
    ../src/actions/crm_actionfactory.h \
    ../src/actions/crm_actiongroupcontact.h \
    ../src/actions/crm_actionleadsourcetype.h \
    ../src/actions/crm_actionplugin.h \
    ../src/actions/crm_actionprojectedit.h \
#    ../src/actions/crm_actionselectcontact.h \
#    ../src/actions/crm_actionselectcustomer.h \
    ../src/actions/crm_actionselectproject.h \
    ../src/actions/crm_actionsysseqno.h \
    ../src/actions/crm_actiontemplate.h \
    ../src/gui/crm_activityreportwidget.h \
    ../src/gui/crm_activitywidget.h \
    ../src/gui/crm_campaigntargetwidget.h \
    ../src/gui/crm_contactbycustreportwidget.h \
    ../src/gui/crm_contactreportwidget.h \
    ../src/gui/crm_contactwidget.h \
    ../src/gui/crm_customerreportwidget.h \
    ../src/gui/crm_customerwidget.h \
    ../src/gui/crm_dialogfactory.h \
    ../src/gui/crm_documentwidget.h \
    ../src/gui/crm_leadsourcetypedialog.h \
    ../src/gui/crm_groupcontactwidget.h \
    ../src/gui/crm_projectdialog.h \
    ../src/gui/crm_projecteditdialog.h \
    ../src/gui/crm_reportwidget.h \
    ../src/gui/crm_selectcontactdialog.h \
    ../src/gui/crm_selectcustomerdialog.h \
    ../src/gui/crm_selectgroupdialog.h \
    ../src/gui/crm_selectprojectdialog.h \
    ../src/gui/crm_selecttemplatedialog.h \
    ../src/gui/crm_sysseqnodialog.h \
    ../src/gui/crm_templatewidget.h \
    ../src/model/crm_activity.h \
    ../src/model/crm_campaign.h \
    ../src/model/crm_campaigntarget.h \
    ../src/model/crm_campaigntype.h \
    ../src/model/crm_contactdetail.h \
    ../src/model/crm_customerdetail.h \
    ../src/model/crm_document.h \
    ../src/model/crm_group.h \
    ../src/model/crm_groupcontact.h \
    ../src/model/crm_leadsourcetype.h \
    ../src/model/crm_modelfactory.h \
    ../src/model/crm_objectfactory.h \
    ../src/model/crm_sysseqno.h \
    ../src/model/crm_syssetting.h \
    ../src/model/crm_template.h \
    ../src/utils/crm_sqlcommonfunctions.h

SOURCES += \
    ../../acc/src/model/acc_contact.cpp \
    ../../acc/src/model/acc_customer.cpp \
    ../../acc/src/model/acc_project.cpp \
    ../../acc/src/model/acc_salesorder.cpp \
    ../src/actions/crm_actionactivity.cpp \
    ../src/actions/crm_actionactivityreport.cpp \
    ../src/actions/crm_actioncampaigntarget.cpp \
    ../src/actions/crm_actionclose.cpp \
    ../src/actions/crm_actioncontact.cpp \
    ../src/actions/crm_actioncontactbycustreport.cpp \
    ../src/actions/crm_actioncontactreport.cpp \
    ../src/actions/crm_actioncustomer.cpp \
    ../src/actions/crm_actioncustomerreport.cpp \
    ../src/actions/crm_actiondocument.cpp \
    ../src/actions/crm_actionfactory.cpp \
    ../src/actions/crm_actiongroupcontact.cpp \
    ../src/actions/crm_actionleadsourcetype.cpp \
    ../src/actions/crm_actionplugin.cpp \
    ../src/actions/crm_actionprojectedit.cpp \
#    ../src/actions/crm_actionselectcontact.cpp \
#    ../src/actions/crm_actionselectcustomer.cpp \
    ../src/actions/crm_actionselectproject.cpp \
    ../src/actions/crm_actionsysseqno.cpp \
    ../src/actions/crm_actiontemplate.cpp \
    ../src/gui/crm_activityreportwidget.cpp \
    ../src/gui/crm_activitywidget.cpp \
    ../src/gui/crm_campaigntargetwidget.cpp \
    ../src/gui/crm_contactbycustreportwidget.cpp \
    ../src/gui/crm_contactreportwidget.cpp \
    ../src/gui/crm_contactwidget.cpp \
    ../src/gui/crm_customerreportwidget.cpp \
    ../src/gui/crm_customerwidget.cpp \
    ../src/gui/crm_dialogfactory.cpp \
    ../src/gui/crm_documentwidget.cpp \
    ../src/gui/crm_groupcontactwidget.cpp \
    ../src/gui/crm_projecteditdialog.cpp \
    ../src/gui/crm_reportwidget.cpp \
    ../src/gui/crm_templatewidget.cpp \
    ../src/model/crm_activity.cpp \
    ../src/model/crm_campaign.cpp \
    ../src/model/crm_campaigntarget.cpp \
    ../src/model/crm_campaigntype.cpp \
    ../src/model/crm_contactdetail.cpp \
    ../src/model/crm_customerdetail.cpp \
    ../src/model/crm_document.cpp \
    ../src/model/crm_group.cpp \
    ../src/model/crm_groupcontact.cpp \
    ../src/model/crm_leadsourcetype.cpp \
    ../src/model/crm_modelfactory.cpp \
    ../src/model/crm_objectfactory.cpp \
    ../src/model/crm_sysseqno.cpp \
    ../src/model/crm_syssetting.cpp \
    ../src/model/crm_template.cpp \
    ../src/utils/crm_sqlcommonfunctions.cpp


FORMS += \
    ../src/gui/crm_activityreportwidget.ui \
    ../src/gui/crm_activitywidget.ui \
    ../src/gui/crm_campaigntargetwidget.ui \
    ../src/gui/crm_contactbycustreportwidget.ui \
    ../src/gui/crm_contactreportwidget.ui \
    ../src/gui/crm_contactwidget.ui \
    ../src/gui/crm_customerreportwidget.ui \
    ../src/gui/crm_customerwidget.ui \
    ../src/gui/crm_documentwidget.ui \
    ../src/gui/crm_groupcontactwidget.ui \
    ../src/gui/crm_templatewidget.ui

RESOURCES += \
#    ../../acc/src/gui/acc.qrc \
    ../../db/src/gui/db.qrc

OTHER_FILES += \
    ../src/actions/crm_actionplugin.json \
    ../src/gui/crm_activityreportwidget.dox \
    ../src/gui/crm_activitywidget.dox \
    ../src/gui/crm_campaigntargetwidget.dox \
    ../src/gui/crm_contactbycustreportwidget.dox \
    ../src/gui/crm_contactreportwidget.dox \
    ../src/gui/crm_contactwidget.dox \
    ../src/gui/crm_customerreportwidget.dox \
    ../src/gui/crm_customerwidget.dox \
    ../src/gui/crm_documentwidget.dox \
    ../src/gui/crm_groupcontactwidget.dox \
    ../src/gui/crm_index.dox \
    ../src/gui/crm_leadsourcetypedialog.dox \
    ../src/gui/crm_projectdialog.dox \
    ../src/gui/crm_projecteditdialog.dox \
    ../src/gui/crm_sysseqnodialog.dox \
    ../src/gui/crm_templatewidget.dox
