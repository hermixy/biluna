#-------------------------------------------------
# $Id: acc.pri 2214 2015-01-28 17:07:32Z rutger $
# Project created by QtCreator 2009-12-18T18:11:30
# - for building accounting application ACC
# - ../src path is required to use .pri file also from ../test
# ----------------------------------------------------------

FORMS += \
    ../src/gui/acc_accountcontroldialog.ui \
    ../src/gui/acc_accountgroupdialog.ui \
    ../src/gui/acc_allocnreportwidget.ui \
    ../src/gui/acc_assetmasterwidget.ui \
    ../src/gui/acc_assetvaluationwidget.ui \
    ../src/gui/acc_assetwidget.ui \
    ../src/gui/acc_bankimportcheckdialog.ui \
    ../src/gui/acc_bankmappingdialog.ui \
    ../src/gui/acc_bankpaymentreportwidget.ui \
    ../src/gui/acc_chartmasterdialog.ui \
    ../src/gui/acc_cogsglpostingdialog.ui \
    ../src/gui/acc_contacteditwidget.ui \
    ../src/gui/acc_costcenterbalancereportwidget.ui \
    ../src/gui/acc_costcenterbudgetdialog.ui \
    ../src/gui/acc_costcenterreportwidget.ui \
    ../src/gui/acc_costgroupdialog.ui \
    ../src/gui/acc_custbranchwidget.ui \
    ../src/gui/acc_custcostpartwidget.ui \
    ../src/gui/acc_custmasterwidget.ui \
    ../src/gui/acc_doctransreportwidget.ui \
    ../src/gui/acc_factorcompanydialog.ui \
    ../src/gui/acc_gltransactionwidget.ui \
    ../src/gui/acc_gltransreportwidget.ui \
    ../src/gui/acc_invoiceaccruedreportwidget.ui \
    ../src/gui/acc_invoiceagedreportwidget.ui \
    ../src/gui/acc_locationdialog.ui \
    ../src/gui/acc_navigationwidget.ui \
    ../src/gui/acc_orderpreviewdialog.ui \
    ../src/gui/acc_orderreportwidget.ui \
    ../src/gui/acc_pricewidget.ui \
    ../src/gui/acc_projecteditdialog.ui \
    ../src/gui/acc_purchaseglpostingdialog.ui \
    ../src/gui/acc_purchaseorderwidget.ui \
#    ../src/gui/acc_reportpreviewdialog.ui \
#    ../src/gui/acc_reportpreviewtedialog.ui \
    ../src/gui/acc_salesglpostingdialog.ui \
    ../src/gui/acc_salesorderwidget.ui \
    ../src/gui/acc_salestaxpostingdialog.ui \
    ../src/gui/acc_salestaxreportwidget.ui \
    ../src/gui/acc_selectcustbranchdialog.ui \
    ../src/gui/acc_selectstockitemdialog.ui \
    ../src/gui/acc_selectyeardialog.ui \
    ../src/gui/acc_settingreportwidget.ui \
    ../src/gui/acc_stockcategorywidget.ui \
    ../src/gui/acc_stockitemwidget.ui \
    ../src/gui/acc_stockmasterwidget.ui \
    ../src/gui/acc_supplcostpartwidget.ui \
    ../src/gui/acc_supplierdetailwidget.ui \
    ../src/gui/acc_suppliermasterwidget.ui \
    ../src/gui/acc_taxauthratedialog.ui \
    ../src/gui/acc_taxconfirmationdialog.ui \
    ../src/gui/acc_taxgroupauthdialog.ui \
    ../src/gui/acc_trialbalancewidget.ui

HEADERS += \
    ../src/actions/acc_actionaccountcontrol.h \
    ../src/actions/acc_actionaccountgroup.h \
    ../src/actions/acc_actionallocnreport.h \
    ../src/actions/acc_actionarea.h \
    ../src/actions/acc_actionasset.h \
    ../src/actions/acc_actionassettype.h \
    ../src/actions/acc_actionassetvaluation.h \
    ../src/actions/acc_actionbankcsvmapping.h \
    ../src/actions/acc_actionbankimport.h \
    ../src/actions/acc_actionbankpaymentreport.h \
    ../src/actions/acc_actionchartmaster.h \
    ../src/actions/acc_actionclose.h \
    ../src/actions/acc_actioncogsglposting.h \
    ../src/actions/acc_actioncontactcustomeredit.h \
    ../src/actions/acc_actioncontactsupplieredit.h \
    ../src/actions/acc_actioncostcenterbalance.h \
    ../src/actions/acc_actioncostcenterbudget.h \
    ../src/actions/acc_actioncostcentersum.h \
    ../src/actions/acc_actioncostgroup.h \
    ../src/actions/acc_actioncurrency.h \
    ../src/actions/acc_actioncustbranch.h \
    ../src/actions/acc_actioncustcostpart.h \
    ../src/actions/acc_actiondebtortype.h \
    ../src/actions/acc_actiondoctransreport.h \
    ../src/actions/acc_actionfactorcompany.h \
    ../src/actions/acc_actionfactory.h \
    ../src/actions/acc_actiongltransaction.h \
    ../src/actions/acc_actiongltransreport.h \
    ../src/actions/acc_actionholdreason.h \
    ../src/actions/acc_actioninvoiceaccruedreport.h \
    ../src/actions/acc_actionlocation.h \
    ../src/actions/acc_actionmaintexportsettings.h \
    ../src/actions/acc_actionmaintimportsettings.h \
    ../src/actions/acc_actionpaymentterm.h \
    ../src/actions/acc_actionplugin.h \
    ../src/actions/acc_actionpoinvoiceagedreport.h \
    ../src/actions/acc_actionprice.h \
    ../src/actions/acc_actionprojectedit.h \
    ../src/actions/acc_actionpurchaseglposting.h \
    ../src/actions/acc_actionpurchaseorderreport.h \
    ../src/actions/acc_actionpurchaseorder.h \
    ../src/actions/acc_actionrecreatebalance.h \
    ../src/actions/acc_actionsalesglposting.h \
    ../src/actions/acc_actionsalesman.h \
    ../src/actions/acc_actionsalesorderreport.h \
    ../src/actions/acc_actionsalesorder.h \
    ../src/actions/acc_actionsalestaxposting.h \
    ../src/actions/acc_actionsalestaxreport.h \
    ../src/actions/acc_actionsalestype.h \
    ../src/actions/acc_actionsettingsdefault.h \
    ../src/actions/acc_actionsettingsreport.h \
    ../src/actions/acc_actionselectproject.h \
    ../src/actions/acc_actionshipper.h \
    ../src/actions/acc_actionsoinvoiceagedreport.h \
    ../src/actions/acc_actionstockitem.h \
    ../src/actions/acc_actionstockcategory.h \
    ../src/actions/acc_actionsupplcostpart.h \
    ../src/actions/acc_actionsupplier.h \
    ../src/actions/acc_actionsystype.h \
    ../src/actions/acc_actiontaxauthrate.h \
    ../src/actions/acc_actiontaxcategory.h \
    ../src/actions/acc_actiontaxgroupauth.h \
    ../src/actions/acc_actiontaxprovince.h \
    ../src/actions/acc_actiontrialbalance.h \
    ../src/actions/acc_actionunitofmeasure.h \
    ../src/filters/acc_bankparser.h \
    ../src/filters/acc_csvbankparser.h \
    ../src/filters/acc_mt940abnparser.h \
    ../src/filters/acc_mt940bankparser.h \
    ../src/filters/acc_mt940ingparser.h \
#    ../src/filters/acc_mt940parser.h \
    ../src/filters/acc_mt940raboparser.h \
    ../src/gui/acc_accountcontroldialog.h \
    ../src/gui/acc_accountgroupdialog.h \
    ../src/gui/acc_allocnreportwidget.h \
    ../src/gui/acc_areadialog.h \
    ../src/gui/acc_assetmasterwidget.h \
    ../src/gui/acc_assettypedialog.h \
    ../src/gui/acc_assetvaluationwidget.h \
    ../src/gui/acc_assetwidget.h \
    ../src/gui/acc_bankimportcheckdialog.h \
    ../src/gui/acc_bankmappingdialog.h \
    ../src/gui/acc_bankpaymentreportwidget.h \
    ../src/gui/acc_chartmasterdialog.h \
    ../src/gui/acc_cogsglpostingdialog.h \
    ../src/gui/acc_contacteditwidget.h \
    ../src/gui/acc_costcenterbalancereportwidget.h \
    ../src/gui/acc_costcenterbudgetdialog.h \
    ../src/gui/acc_costcenterreportwidget.h \
    ../src/gui/acc_costgroupdialog.h \
    ../src/gui/acc_currencydialog.h \
    ../src/gui/acc_custbranchwidget.h \
    ../src/gui/acc_custcostpartwidget.h \
    ../src/gui/acc_custmasterwidget.h \
    ../src/gui/acc_debtortypedialog.h \
    ../src/gui/acc_dialogfactory.h \
    ../src/gui/acc_doctransreportwidget.h \
    ../src/gui/acc_factorcompanydialog.h \
#    ../src/gui/acc_finamtdelegate.h \
    ../src/gui/acc_gltransactionwidget.h \
    ../src/gui/acc_gltransreportwidget.h \
    ../src/gui/acc_holdreasondialog.h \
    ../src/gui/acc_invoiceaccruedreportwidget.h \
    ../src/gui/acc_invoiceagedreportwidget.h \
    ../src/gui/acc_locationdialog.h \
    ../src/gui/acc_navigationwidget.h \
    ../src/gui/acc_orderpreviewdialog.h \
    ../src/gui/acc_orderreportwidget.h \
    ../src/gui/acc_paymenttermdialog.h \
    ../src/gui/acc_pricewidget.h \
    ../src/gui/acc_projectdialog.h \
    ../src/gui/acc_projecteditdialog.h \
    ../src/gui/acc_purchaseglpostingdialog.h \
    ../src/gui/acc_purchaseorderwidget.h \
#    ../src/gui/acc_reportpreviewdialog.h \
#    ../src/gui/acc_reportpreviewtedialog.h \
    ../src/gui/acc_salesglpostingdialog.h \
    ../src/gui/acc_salesmandialog.h \
    ../src/gui/acc_salesorderwidget.h \
    ../src/gui/acc_salestaxpostingdialog.h \
    ../src/gui/acc_salestaxreportwidget.h \
    ../src/gui/acc_salestypedialog.h \
    ../src/gui/acc_selectallocndialog.h \
    ../src/gui/acc_selectchartmasterdialog.h \
    ../src/gui/acc_selectcontactdialog.h \
    ../src/gui/acc_selectcostcenterdialog.h \
    ../src/gui/acc_selectcostgroupdialog.h \
    ../src/gui/acc_selectcustbranchdialog.h \
    ../src/gui/acc_selectstockcategorydialog.h \
    ../src/gui/acc_selectstockitemdialog.h \
    ../src/gui/acc_selectsupplierdialog.h \
    ../src/gui/acc_selecttaxprovincedialog.h \
    ../src/gui/acc_selectyeardialog.h \
    ../src/gui/acc_settingreportwidget.h \
    ../src/gui/acc_shipperdialog.h \
    ../src/gui/acc_stockcategorywidget.h \
    ../src/gui/acc_stockitemwidget.h \
    ../src/gui/acc_stockmasterwidget.h \
    ../src/gui/acc_supplcostpartwidget.h \
    ../src/gui/acc_supplierdetailwidget.h \
    ../src/gui/acc_suppliermasterwidget.h \
    ../src/gui/acc_systypedialog.h \
    ../src/gui/acc_taxauthratedialog.h \
    ../src/gui/acc_taxcategorydialog.h \
    ../src/gui/acc_taxconfirmationdialog.h \
    ../src/gui/acc_taxgroupauthdialog.h \
    ../src/gui/acc_taxprovincedialog.h \
    ../src/gui/acc_trialbalancewidget.h \
    ../src/gui/acc_unitofmeasuredialog.h \
    ../src/model/acc.h \
    ../src/model/acc_accountcontrol.h \
    ../src/model/acc_accountgroup.h \
    ../src/model/acc_area.h \
    ../src/model/acc_asset.h \
    ../src/model/acc_assetattribute.h \
    ../src/model/acc_assetcustattr.h \
    ../src/model/acc_assetcustinter.h \
    ../src/model/acc_assetsupplattr.h \
    ../src/model/acc_assetsupplinter.h \
    ../src/model/acc_assettype.h \
    ../src/model/acc_bankimport.h \
    ../src/model/acc_bankmapping.h \
    ../src/model/acc_banktrans.h \
    ../src/model/acc_chartmaster.h \
    ../src/model/acc_cogsglposting.h \
    ../src/model/acc_contact.h \
    ../src/model/acc_costbudget.h \
    ../src/model/acc_costcenter.h \
    ../src/model/acc_costgroup.h \
    ../src/model/acc_costsum.h \
    ../src/model/acc_currency.h \
    ../src/model/acc_creditortrans.h \
    ../src/model/acc_custcostpart.h \
    ../src/model/acc_customerattr.h \
    ../src/model/acc_customernote.h \
    ../src/model/acc_customer.h \
    ../src/model/acc_debtortrans.h \
    ../src/model/acc_debtortype.h \
    ../src/model/acc_factorcompany.h \
    ../src/model/acc_glsum.h \
    ../src/model/acc_gltrans.h \
    ../src/model/acc_holdreason.h \
    ../src/model/acc_location.h \
    ../src/model/acc_memotrans.h \
    ../src/model/acc_modelfactory.h \
    ../src/model/acc_objectfactory.h \
    ../src/model/acc_paymentterm.h \
    ../src/model/acc_price.h \
    ../src/model/acc_project.h \
    ../src/model/acc_purchaseglposting.h \
    ../src/model/acc_purchaseorder.h \
    ../src/model/acc_purchaseorderdetail.h \
    ../src/model/acc_purchdata.h \
    ../src/model/acc_salesglposting.h \
    ../src/model/acc_salesman.h \
    ../src/model/acc_salesorder.h \
    ../src/model/acc_salesorderdetail.h \
    ../src/model/acc_salestaxposting.h \
    ../src/model/acc_salestype.h \
    ../src/model/acc_shipper.h \
    ../src/model/acc_stockcategory.h \
    ../src/model/acc_stockmaster.h \
    ../src/model/acc_stockmove.h \
#    ../src/model/acc_stockmovetaxes.h \
    ../src/model/acc_supplcostpart.h \
    ../src/model/acc_supplierattr.h \
    ../src/model/acc_suppliernote.h \
    ../src/model/acc_supplier.h \
    ../src/model/acc_sysacctcontrol.h \
#    ../src/model/acc_syssetting.h \
    ../src/model/acc_systype.h \
    ../src/model/acc_taxauthority.h \
    ../src/model/acc_taxauthrate.h \
    ../src/model/acc_taxcategory.h \
    ../src/model/acc_taxgroup.h \
    ../src/model/acc_taxgroupauth.h \
    ../src/model/acc_taxprovince.h \
    ../src/model/acc_transallocn.h \
    ../src/model/acc_transdoc.h \
    ../src/model/acc_unitofmeasure.h \
    ../src/model/acc_workorder.h \
    ../src/model/acc_workorderdetail.h \
    ../src/operations/acc_banktransdoc.h \
    ../src/operations/acc_calcsalestax.h \
    ../src/operations/acc_createalloclist.h \
    ../src/operations/acc_createbankpaymentlist.h \
    ../src/operations/acc_createcostcenter3report.h \
    ../src/operations/acc_createcostcenterbalancereport.h \
    ../src/operations/acc_createcostcenterreport.h \
    ../src/operations/acc_createdoctrans.h \
    ../src/operations/acc_creategltransactions.h \
    ../src/operations/acc_creategltrialbalance.h \
    ../src/operations/acc_createinvoiceaged.h \
    ../src/operations/acc_createorderreport.h \
    ../src/operations/acc_handleallocns.h \
#    ../src/operations/acc_handleallocns_old.h \
    ../src/operations/acc_invoiceaccrued.h \
    ../src/operations/acc_operation.h \
    ../src/operations/acc_parseimportbank.h \
    ../src/operations/acc_postgltransaction.h \
    ../src/operations/acc_potransdoc.h \
#    ../src/operations/acc_processinvoice.h \
    ../src/operations/acc_processtransdoc.h \
    ../src/operations/acc_purchaseorderhtml.h \
    ../src/operations/acc_salesinvoicehtml.h \
    ../src/operations/acc_salesquotationhtml.h \
    ../src/operations/acc_sotransdoc.h \
    ../src/operations/acc_valueasset.h \
    ../src/utils/acc_qachartmaster.h \
    ../src/utils/acc_qasysacctcontrol.h \
    ../src/utils/acc_sqlcommonfunctions.h \
    ../src/utils/acc_utilityfactory.h \
    ../src/visitors/acc_dbvisitor.h \
    ../src/visitors/acc_settingidvisitor.h \
    ../src/visitors/acc_syssetxmlreader.h \
    ../src/visitors/acc_syssetxmlwriter.h \
    ../src/visitors/acc_valueassethelper.h

SOURCES += \
    ../src/actions/acc_actionaccountcontrol.cpp \
    ../src/actions/acc_actionaccountgroup.cpp \
    ../src/actions/acc_actionallocnreport.cpp \
    ../src/actions/acc_actionarea.cpp \
    ../src/actions/acc_actionasset.cpp \
    ../src/actions/acc_actionassettype.cpp \
    ../src/actions/acc_actionassetvaluation.cpp \
    ../src/actions/acc_actionbankcsvmapping.cpp \
    ../src/actions/acc_actionbankimport.cpp \
    ../src/actions/acc_actionbankpaymentreport.cpp \
    ../src/actions/acc_actionchartmaster.cpp \
    ../src/actions/acc_actionclose.cpp \
    ../src/actions/acc_actioncogsglposting.cpp \
    ../src/actions/acc_actioncontactcustomeredit.cpp \
    ../src/actions/acc_actioncontactsupplieredit.cpp \
    ../src/actions/acc_actioncostcenterbalance.cpp \
    ../src/actions/acc_actioncostcenterbudget.cpp \
    ../src/actions/acc_actioncostcentersum.cpp \
    ../src/actions/acc_actioncostgroup.cpp \
    ../src/actions/acc_actioncurrency.cpp \
    ../src/actions/acc_actioncustbranch.cpp \
    ../src/actions/acc_actioncustcostpart.cpp \
    ../src/actions/acc_actiondebtortype.cpp \
    ../src/actions/acc_actiondoctransreport.cpp \
    ../src/actions/acc_actionfactorcompany.cpp \
    ../src/actions/acc_actionfactory.cpp \
    ../src/actions/acc_actiongltransaction.cpp \
    ../src/actions/acc_actiongltransreport.cpp \
    ../src/actions/acc_actionholdreason.cpp \
    ../src/actions/acc_actioninvoiceaccruedreport.cpp \
    ../src/actions/acc_actionlocation.cpp \
    ../src/actions/acc_actionmaintimportsettings.cpp \
    ../src/actions/acc_actionmaintexportsettings.cpp \
    ../src/actions/acc_actionpaymentterm.cpp \
    ../src/actions/acc_actionplugin.cpp \
    ../src/actions/acc_actionpoinvoiceagedreport.cpp \
    ../src/actions/acc_actionprice.cpp \
    ../src/actions/acc_actionprojectedit.cpp \
    ../src/actions/acc_actionpurchaseglposting.cpp \
    ../src/actions/acc_actionpurchaseorderreport.cpp \
    ../src/actions/acc_actionpurchaseorder.cpp \
    ../src/actions/acc_actionrecreatebalance.cpp \
    ../src/actions/acc_actionsalesglposting.cpp \
    ../src/actions/acc_actionsalesman.cpp \
    ../src/actions/acc_actionsalesorderreport.cpp \
    ../src/actions/acc_actionsalesorder.cpp \
    ../src/actions/acc_actionsalestaxposting.cpp \
    ../src/actions/acc_actionsalestaxreport.cpp \
    ../src/actions/acc_actionsalestype.cpp \
    ../src/actions/acc_actionselectproject.cpp \
    ../src/actions/acc_actionsettingsdefault.cpp \
    ../src/actions/acc_actionsettingsreport.cpp \
    ../src/actions/acc_actionshipper.cpp \
    ../src/actions/acc_actionsoinvoiceagedreport.cpp \
    ../src/actions/acc_actionstockitem.cpp \
    ../src/actions/acc_actionstockcategory.cpp \
    ../src/actions/acc_actionsupplcostpart.cpp \
    ../src/actions/acc_actionsupplier.cpp \
    ../src/actions/acc_actionsystype.cpp \
    ../src/actions/acc_actiontaxauthrate.cpp \
    ../src/actions/acc_actiontaxcategory.cpp \
    ../src/actions/acc_actiontaxgroupauth.cpp \
    ../src/actions/acc_actiontaxprovince.cpp \
    ../src/actions/acc_actiontrialbalance.cpp \
    ../src/actions/acc_actionunitofmeasure.cpp \
    ../src/filters/acc_bankparser.cpp \
    ../src/filters/acc_csvbankparser.cpp \
    ../src/filters/acc_mt940abnparser.cpp \
    ../src/filters/acc_mt940bankparser.cpp \
    ../src/filters/acc_mt940ingparser.cpp \
#    ../src/filters/acc_mt940parser.cpp \
    ../src/filters/acc_mt940raboparser.cpp \
    ../src/gui/acc_accountcontroldialog.cpp \
    ../src/gui/acc_accountgroupdialog.cpp \
    ../src/gui/acc_allocnreportwidget.cpp \
    ../src/gui/acc_assetmasterwidget.cpp \
    ../src/gui/acc_assetvaluationwidget.cpp \
    ../src/gui/acc_assetwidget.cpp \
    ../src/gui/acc_bankimportcheckdialog.cpp \
    ../src/gui/acc_bankmappingdialog.cpp \
    ../src/gui/acc_bankpaymentreportwidget.cpp \
    ../src/gui/acc_chartmasterdialog.cpp \
    ../src/gui/acc_cogsglpostingdialog.cpp \
    ../src/gui/acc_contacteditwidget.cpp \
    ../src/gui/acc_costcenterbalancereportwidget.cpp \
    ../src/gui/acc_costcenterbudgetdialog.cpp \
    ../src/gui/acc_costcenterreportwidget.cpp \
    ../src/gui/acc_costgroupdialog.cpp \
    ../src/gui/acc_custbranchwidget.cpp \
    ../src/gui/acc_custcostpartwidget.cpp \
    ../src/gui/acc_custmasterwidget.cpp \
    ../src/gui/acc_dialogfactory.cpp \
    ../src/gui/acc_doctransreportwidget.cpp \
    ../src/gui/acc_factorcompanydialog.cpp \
#    ../src/gui/acc_finamtdelegate.cpp \ TODO remove
    ../src/gui/acc_gltransactionwidget.cpp \
    ../src/gui/acc_gltransreportwidget.cpp \
    ../src/gui/acc_invoiceaccruedreportwidget.cpp \
    ../src/gui/acc_invoiceagedreportwidget.cpp \
    ../src/gui/acc_locationdialog.cpp \
    ../src/gui/acc_navigationwidget.cpp \
    ../src/gui/acc_orderpreviewdialog.cpp \
    ../src/gui/acc_orderreportwidget.cpp \
    ../src/gui/acc_pricewidget.cpp \
    ../src/gui/acc_projecteditdialog.cpp \
    ../src/gui/acc_purchaseglpostingdialog.cpp \
    ../src/gui/acc_purchaseorderwidget.cpp \
#    ../src/gui/acc_reportpreviewdialog.cpp \
#    ../src/gui/acc_reportpreviewtedialog.cpp \
    ../src/gui/acc_salesglpostingdialog.cpp \
    ../src/gui/acc_salesorderwidget.cpp \
    ../src/gui/acc_salestaxpostingdialog.cpp \
    ../src/gui/acc_salestaxreportwidget.cpp \
    ../src/gui/acc_selectallocndialog.cpp \
    ../src/gui/acc_selectcustbranchdialog.cpp \
    ../src/gui/acc_selectstockitemdialog.cpp \
    ../src/gui/acc_selectyeardialog.cpp \
    ../src/gui/acc_settingreportwidget.cpp \
    ../src/gui/acc_stockcategorywidget.cpp \
    ../src/gui/acc_stockitemwidget.cpp \
    ../src/gui/acc_stockmasterwidget.cpp \
    ../src/gui/acc_supplcostpartwidget.cpp \
    ../src/gui/acc_supplierdetailwidget.cpp \
    ../src/gui/acc_suppliermasterwidget.cpp \
    ../src/gui/acc_taxauthratedialog.cpp \
    ../src/gui/acc_taxconfirmationdialog.cpp \
    ../src/gui/acc_taxgroupauthdialog.cpp \
    ../src/gui/acc_trialbalancewidget.cpp \
    ../src/model/acc_accountcontrol.cpp \
    ../src/model/acc_accountgroup.cpp \
    ../src/model/acc_area.cpp \
    ../src/model/acc_asset.cpp \
    ../src/model/acc_assetattribute.cpp \
    ../src/model/acc_assetcustattr.cpp \
    ../src/model/acc_assetcustinter.cpp \
    ../src/model/acc_assetsupplattr.cpp \
    ../src/model/acc_assetsupplinter.cpp \
    ../src/model/acc_assettype.cpp \
    ../src/model/acc_bankimport.cpp \
    ../src/model/acc_bankmapping.cpp \
    ../src/model/acc_banktrans.cpp \
    ../src/model/acc_chartmaster.cpp \
    ../src/model/acc_cogsglposting.cpp \
    ../src/model/acc_contact.cpp \
    ../src/model/acc_costbudget.cpp \
    ../src/model/acc_costcenter.cpp \
    ../src/model/acc_costgroup.cpp \
    ../src/model/acc_costsum.cpp \
    ../src/model/acc_creditortrans.cpp \
    ../src/model/acc_currency.cpp \
    ../src/model/acc_custcostpart.cpp \
    ../src/model/acc_customerattr.cpp \
    ../src/model/acc_customernote.cpp \
    ../src/model/acc_customer.cpp \
    ../src/model/acc_debtortrans.cpp \
    ../src/model/acc_debtortype.cpp \
    ../src/model/acc_factorcompany.cpp \
    ../src/model/acc_glsum.cpp \
    ../src/model/acc_gltrans.cpp \
    ../src/model/acc_holdreason.cpp \
    ../src/model/acc_location.cpp \
    ../src/model/acc_memotrans.cpp \
    ../src/model/acc_modelfactory.cpp \
    ../src/model/acc_objectfactory.cpp \
    ../src/model/acc_paymentterm.cpp \
    ../src/model/acc_price.cpp \
    ../src/model/acc_project.cpp \
    ../src/model/acc_purchaseglposting.cpp \
    ../src/model/acc_purchaseorder.cpp \
    ../src/model/acc_purchaseorderdetail.cpp \
    ../src/model/acc_purchdata.cpp \
    ../src/model/acc_salesglposting.cpp \
    ../src/model/acc_salesman.cpp \
    ../src/model/acc_salesorder.cpp \
    ../src/model/acc_salesorderdetail.cpp \
    ../src/model/acc_salestaxposting.cpp \
    ../src/model/acc_salestype.cpp \
    ../src/model/acc_shipper.cpp \
    ../src/model/acc_stockcategory.cpp \
    ../src/model/acc_stockmaster.cpp \
    ../src/model/acc_stockmove.cpp \
#    ../src/model/acc_stockmovetaxes.cpp \
    ../src/model/acc_supplcostpart.cpp \
    ../src/model/acc_supplierattr.cpp \
    ../src/model/acc_suppliernote.cpp \
    ../src/model/acc_supplier.cpp \
    ../src/model/acc_sysacctcontrol.cpp \
#    ../src/model/acc_syssetting.cpp \
    ../src/model/acc_systype.cpp \
    ../src/model/acc_taxauthority.cpp \
    ../src/model/acc_taxauthrate.cpp \
    ../src/model/acc_taxcategory.cpp \
    ../src/model/acc_taxgroup.cpp \
    ../src/model/acc_taxgroupauth.cpp \
    ../src/model/acc_taxprovince.cpp \
    ../src/model/acc_transallocn.cpp \
    ../src/model/acc_transdoc.cpp \
    ../src/model/acc_unitofmeasure.cpp \
    ../src/model/acc_workorder.cpp \
    ../src/model/acc_workorderdetail.cpp \
    ../src/operations/acc_banktransdoc.cpp \
    ../src/operations/acc_calcsalestax.cpp \
    ../src/operations/acc_createalloclist.cpp \
    ../src/operations/acc_createbankpaymentlist.cpp \
    ../src/operations/acc_createcostcenter3report.cpp \
    ../src/operations/acc_createcostcenterbalancereport.cpp \
    ../src/operations/acc_createcostcenterreport.cpp \
    ../src/operations/acc_createdoctrans.cpp \
    ../src/operations/acc_creategltransactions.cpp \
    ../src/operations/acc_creategltrialbalance.cpp \
    ../src/operations/acc_createinvoiceaged.cpp \
    ../src/operations/acc_createorderreport.cpp \
    ../src/operations/acc_handleallocns.cpp \
#    ../src/operations/acc_handleallocns_old.cpp \
    ../src/operations/acc_invoiceaccrued.cpp \
    ../src/operations/acc_operation.cpp \
    ../src/operations/acc_parseimportbank.cpp \
    ../src/operations/acc_postgltransaction.cpp \
    ../src/operations/acc_potransdoc.cpp \
#    ../src/operations/acc_processinvoice.cpp \
    ../src/operations/acc_processtransdoc.cpp \
    ../src/operations/acc_purchaseorderhtml.cpp \
    ../src/operations/acc_salesinvoicehtml.cpp \
    ../src/operations/acc_salesquotationhtml.cpp \
    ../src/operations/acc_sotransdoc.cpp \
    ../src/operations/acc_valueasset.cpp \
    ../src/utils/acc_qachartmaster.cpp \
    ../src/utils/acc_qasysacctcontrol.cpp \
    ../src/utils/acc_sqlcommonfunctions.cpp \
    ../src/utils/acc_utilityfactory.cpp \
    ../src/visitors/acc_dbvisitor.cpp \
    ../src/visitors/acc_settingidvisitor.cpp \
    ../src/visitors/acc_syssetxmlreader.cpp \
    ../src/visitors/acc_syssetxmlwriter.cpp \
    ../src/visitors/acc_valueassethelper.cpp

RESOURCES += \
    ../../acc/src/gui/acc.qrc \
    ../../db/src/gui/db.qrc
#    ../../gv/src/gui/gv.qrc

OTHER_FILES += \
    ../src/actions/acc_actionplugin.json \
    ../src/gui/acc_accountcontroldialog.dox \
    ../src/gui/acc_accountgroupdialog.dox \
    ../src/gui/acc_allocnreportwidget.dox \
    ../src/gui/acc_areadialog.dox \
    ../src/gui/acc_assettypedialog.dox \
    ../src/gui/acc_assetvaluationwidget.dox \
    ../src/gui/acc_assetwidget.dox \
    ../src/gui/acc_bankimportcheckdialog.dox \
    ../src/gui/acc_bankmappingdialog.dox \
    ../src/gui/acc_bankpaymentreportwidget.dox \
    ../src/gui/acc_chartmasterdialog.dox \
    ../src/gui/acc_cogsglpostingdialog.dox \
    ../src/gui/acc_contacteditwidget.dox \
    ../src/gui/acc_costcenterbalancereportwidget.dox \
    ../src/gui/acc_costcenterbudgetdialog.dox \
    ../src/gui/acc_costcenterreportwidget.dox \
    ../src/gui/acc_costgroupdialog.dox \
    ../src/gui/acc_currencydialog.dox \
    ../src/gui/acc_custcostpartwidget.dox \
    ../src/gui/acc_custbranchwidget.dox \
#    ../src/gui/acc_custmasterwidget.dox \
    ../src/gui/acc_debtortypedialog.dox \
#    ../src/gui/acc_dialogfactory.h \
    ../src/gui/acc_doctransreportwidget.dox \
    ../src/gui/acc_editview.dox \
    ../src/gui/acc_factorcompanydialog.dox \
    ../src/gui/acc_faq.dox \
    ../src/gui/acc_faq_model.dox \
    ../src/gui/acc_faq_sql.dox \
    ../src/gui/acc_general.dox \
    ../src/gui/acc_general_gettingstarted.dox \
    ../src/gui/acc_general_setupfromscratch.dox \
    ../src/gui/acc_gltransactionwidget.dox \
    ../src/gui/acc_gltransactionwidget_adddoc.dox \
    ../src/gui/acc_gltransreportwidget.dox \
    ../src/gui/acc_holdreasondialog.dox \
    ../src/gui/acc_index.dox \
    ../src/gui/acc_invoiceaccruedreportwidget.dox \
    ../src/gui/acc_invoiceagedreportwidget.dox \
    ../src/gui/acc_locationdialog.dox \
    ../src/gui/acc_maintenance.dox \
#    ../src/gui/acc_navigationwidget.dox \
    ../src/gui/acc_orderreportwidget.dox \
    ../src/gui/acc_paymenttermdialog.dox \
    ../src/gui/acc_pricewidget.dox \
    ../src/gui/acc_projectdialog.dox \
    ../src/gui/acc_projecteditdialog.dox \
    ../src/gui/acc_purchaseglpostingdialog.dox \
    ../src/gui/acc_purchaseorderwidget.dox \
    ../src/gui/acc_reporting.dox \
#    ../src/gui/acc_reportpreviewdialog.dox \
#    ../src/gui/acc_reportpreviewtedialog.dox \
    ../src/gui/acc_salesglpostingdialog.dox \
    ../src/gui/acc_salesmandialog.dox \
    ../src/gui/acc_salesorderwidget.dox \
    ../src/gui/acc_salestaxpostingdialog.dox \
    ../src/gui/acc_salestaxreportwidget.dox \
    ../src/gui/acc_salestypedialog.dox \
    ../src/gui/acc_selectallocndialog.dox \
    ../src/gui/acc_selectchartmasterdialog.dox \
    ../src/gui/acc_selectcontactdialog.dox \
    ../src/gui/acc_selectcostcenterdialog.dox \
    ../src/gui/acc_selectcostgroupdialog.dox \
    ../src/gui/acc_selectcustbranchdialog.dox \
#    ../src/gui/acc_selectstockcategorydialog.dox \
#    ../src/gui/acc_selectstockitemdialog.dox \
#    ../src/gui/acc_selectsupplierdialog.dox \
#    ../src/gui/acc_selecttaxprovincedialog.dox \
#    ../src/gui/acc_selectyeardialog.dox \
    ../src/gui/acc_setting.dox \
    ../src/gui/acc_settingreportwidget.dox \
    ../src/gui/acc_shipperdialog.dox \
    ../src/gui/acc_stockcategorywidget.dox \
    ../src/gui/acc_stockitemwidget.dox \
#    ../src/gui/acc_stockmasterwidget.dox \
    ../src/gui/acc_supplcostpartwidget.dox \
    ../src/gui/acc_supplierdetailwidget.dox \
#    ../src/gui/acc_suppliermasterwidget.dox \
    ../src/gui/acc_systypedialog.dox \
    ../src/gui/acc_taxauthratedialog.dox \
    ../src/gui/acc_taxcategorydialog.dox \
    ../src/gui/acc_taxconfirmationdialog.dox \
    ../src/gui/acc_taxgroupauthdialog.dox \
    ../src/gui/acc_taxprovincedialog.dox \
    ../src/gui/acc_trialbalancewidget.dox \
    ../src/gui/acc_unitofmeasuredialog.dox



