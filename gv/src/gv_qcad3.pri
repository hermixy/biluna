# ----------------------------------------------------------
# gv.pri, 2008-03-06
# - forms, headers and sources for graphics view
# ----------------------------------------------------------

FORMS += \
#    ../../ldt/src/gui/ldt_linemainwidget.ui \
    ../../gv/src/gui/gv_drawinglistwidget.ui \
    ../../gv/src/gui/gv_drawingwidget.ui \
    ../../gv/src/gui/gv_graphicsviewqtwidget.ui \
    ../../gv/src/gui/gv_graphicsviewqvwidget.ui \
    ../../gv/src/gui/gv_legendwidget.ui \
    ../../gv/src/gui/gv_lineattributesdialog.ui \
    ../../gv/src/gui/gv_onetimedialog.ui \
    ../../gv/src/gui/gv_pipelinedialog.ui \
    ../../gv/src/gui/gv_previewwidget.ui \
    ../../gv/src/gui/gv_propertieswidget.ui

HEADERS += \
#    ../../gl/src/model/rb_component.h \
#    ../../gl/src/model/RMath.h \
#    ../../gl/src/model/rb_matrix.h \
#    ../../gl/src/model/rb_node.h \
#    ../../gl/src/model/rb_port.h \
#    ../../gl/src/model/RVector.h \
#    ../../ldt/src/model/ldt_line.h \
#    ../../ldt/src/model/ldt_project.h \
#    ../../ldt/src/gui/ldt_linemainwidget.h \
    ../../gv/src/actions/gv_actionbase.h \
    ../../gv/src/actions/gv_actioncomponent.h \
    ../../gv/src/actions/gv_actiondeleteitem.h \
    ../../gv/src/actions/gv_actiondrawinglist.h \
    ../../gv/src/actions/gv_actiondrawline.h \
    ../../gv/src/actions/gv_actiondrawsymbol.h \
    ../../gv/src/actions/gv_actiondrawtext.h \
    ../../gv/src/actions/gv_actioneditcopy.h \
    ../../gv/src/actions/gv_actioneditcopyautoref.h \
    ../../gv/src/actions/gv_actioneditcut.h \
    ../../gv/src/actions/gv_actioneditcutautoref.h \
    ../../gv/src/actions/gv_actioneditpaste.h \
    ../../gv/src/actions/gv_actioneditredo.h \
    ../../gv/src/actions/gv_actioneditundo.h \
    ../../gv/src/actions/gv_actionfactory.h \
    ../../gv/src/actions/gv_actionfileclose.h \
    ../../gv/src/actions/gv_actionfilenew.h \
    ../../gv/src/actions/gv_actionfileopen.h \
    ../../gv/src/actions/gv_actionfileprint.h \
    ../../gv/src/actions/gv_actionfilequit.h \
    ../../gv/src/actions/gv_actionfilesave.h \
    ../../gv/src/actions/gv_actionfilesaveas.h \
    ../../gv/src/actions/gv_actionformatitemcolor.h \
    ../../gv/src/actions/gv_actionformatlineattributes.h \
    ../../gv/src/actions/gv_actionhelpabout.h \
    ../../gv/src/actions/gv_actionhelpmanual.h \
    ../../gv/src/actions/gv_actionhelpwebsite.h \
    ../../gv/src/actions/gv_actionmodifymergeline.h \
    ../../gv/src/actions/gv_actionmodifymove.h \
    ../../gv/src/actions/gv_actionmodifysplitline.h \
#    ../../gv/src/actions/gv_actionselectbase.h \
#    ../../gv/src/actions/gv_actionselectsingle.h \
    ../../gv/src/actions/gv_actionsetsnapmode.h \
    ../../gv/src/actions/gv_actionsetsnapmodeauto.h \
    ../../gv/src/actions/gv_actionsetsnapmodefree.h \
    ../../gv/src/actions/gv_actionsetsnapmodegrid.h \
    ../../gv/src/actions/gv_actionsetsnaprestriction.h \
    ../../gv/src/actions/gv_actionsetsnaprestrictionnothing.h \
    ../../gv/src/actions/gv_actionsetsnaprestrictionorthogonal.h \
    ../../gv/src/actions/gv_actionsystemdatabaseconnect.h \
    ../../gv/src/actions/gv_actionsystemselectproject.h \
    ../../gv/src/actions/gv_actiontestnew.h \
    ../../gv/src/actions/gv_actiontestnewqt.h \
    ../../gv/src/actions/gv_actionviewdraft.h \
    ../../gv/src/actions/gv_actionviewgrid.h \
    ../../gv/src/actions/gv_actionviewstatusbar.h \
    ../../gv/src/actions/gv_actionzoomauto.h \
    ../../gv/src/actions/gv_actionzoomin.h \
    ../../gv/src/actions/gv_actionzoomout.h \
    ../../gv/src/actions/gv_actionzoompan.h \
    ../../gv/src/actions/gv_actionzoomprevious.h \
    ../../gv/src/actions/gv_actionzoomredraw.h \
    ../../gv/src/actions/gv_actionzoomselection.h \
    ../../gv/src/actions/gv_actionzoomwindow.h \
    ../../gv/src/actions/gv_snapper.h \
    ../../gv/src/filters/gv_filterxml.h \
    ../../gv/src/gui/gv_commandevent.h \
    ../../gv/src/gui/gv_coordinateevent.h \
    ../../gv/src/gui/gv_dialogfactory.h \
    ../../gv/src/gui/gv_drawinglistwidget.h \
    ../../gv/src/gui/gv_drawingwidget.h \
    ../../gv/src/gui/gv_eventhandler.h \
    ../../gv/src/gui/gv_eventhandlerqt.h \
    ../../gv/src/gui/gv_eventhandlerqv.h \
    ../../gv/src/gui/gv_graphicsscene.h \
    ../../gv/src/gui/gv_graphicsview.h \
    ../../gv/src/gui/gv_graphicsviewqtwidget.h \
    ../../gv/src/gui/gv_graphicsviewqvwidget.h \
    ../../gv/src/gui/gv_gvlegend.h \
#    ../../gv/src/gui/gv_gvmdi_old.h \
    ../../gv/src/gui/gv_grid.h \
    ../../gv/src/gui/gv_legendwidget.h \
    ../../gv/src/gui/gv_lineattributesdialog.h \
#    ../../gv/src/gui/gv_mdiwindow_old.h \
    ../../gv/src/gui/gv_onetimedialog.h \
    ../../gv/src/gui/gv_orthogrid.h \
#    ../../gv/src/gui/gv_pipelinedialog.h \
    ../../gv/src/gui/gv_previewwidget.h \
    ../../gv/src/gui/gv_projectdialog.h \
    ../../gv/src/gui/gv_propertieswidget.h \
#    ../../gv/src/mathb/rb_boundary.h \
#    ../../gv/src/mathb/rb_math.h \
#    ../../gv/src/mathb/rb_matrix.h \
#    ../../gv/src/mathb/rb_node.h \
#    ../../gv/src/mathb/rb_port.h \
#    ../../gv/src/mathb/rb_vector.h \
    ../../gv/src/model/gv.h \
    ../../gv/src/model/gv_component.h \
    ../../gv/src/model/gv_drawing.h \
#    ../../gv/src/model/gv_equipment.h \
    ../../gv/src/model/gv_line.h \
    ../../gv/src/model/gv_modelfactory.h \
    ../../gv/src/model/gv_node.h \
    ../../gv/src/model/gv_objectfactory.h \
#    ../../gv/src/model/gv_pipeline.h \
    ../../gv/src/model/gv_port.h \
    ../../gv/src/model/gv_project.h \
    ../../gv/src/model/gv_resizehandle.h \
    ../../gv/src/model/gv_rotationlever.h \
    ../../gv/src/model/gv_symbol.h \
    ../../gv/src/model/gv_text.h \
    ../../gv/src/model/rb_component.h \
    ../../gv/src/model/rb_node.h \
    ../../gv/src/model/rb_port.h \
    ../../gv/src/operations/gv_dwgscenesync.h
#    ../../gv/src/snap/gv_snapauto.h \
#    ../../gv/src/snap/gv_snapcenter.h \
#    ../../gv/src/snap/gv_snapdistance.h \
#    ../../gv/src/snap/gv_snapend.h \
#    ../../gv/src/snap/gv_snapentitybase.h \
#    ../../gv/src/snap/gv_snapfree.h \
#    ../../gv/src/snap/gv_snapgrid.h \
#    ../../gv/src/snap/gv_snapintersection.h \
#    ../../gv/src/snap/gv_snapmiddle.h \
#    ../../gv/src/snap/gv_snaponentity.h \
#    ../../gv/src/snap/gv_snapreference.h

SOURCES += \
#    ../../gl/src/model/rb_component.cpp \
#    ../../gl/src/model/rb_math.cpp \
#    ../../gl/src/model/rb_matrix.cpp \
#    ../../gl/src/model/rb_node.cpp \
#    ../../gl/src/model/rb_port.cpp \
#    ../../gl/src/model/rb_vector.cpp \
#    ../../ldt/src/model/ldt_line.cpp \
#    ../../ldt/src/model/ldt_project.cpp \
#    ../../ldt/src/gui/ldt_linemainwidget.cpp \
    ../../gv/src/actions/gv_actionbase.cpp \
    ../../gv/src/actions/gv_actioncomponent.cpp \
    ../../gv/src/actions/gv_actiondeleteitem.cpp \
    ../../gv/src/actions/gv_actiondrawinglist.cpp \
    ../../gv/src/actions/gv_actiondrawline.cpp \
    ../../gv/src/actions/gv_actiondrawsymbol.cpp \
    ../../gv/src/actions/gv_actiondrawtext.cpp \
    ../../gv/src/actions/gv_actioneditcopy.cpp \
    ../../gv/src/actions/gv_actioneditcopyautoref.cpp \
    ../../gv/src/actions/gv_actioneditcut.cpp \
    ../../gv/src/actions/gv_actioneditcutautoref.cpp \
    ../../gv/src/actions/gv_actioneditredo.cpp \
    ../../gv/src/actions/gv_actioneditundo.cpp \
    ../../gv/src/actions/gv_actioneditpaste.cpp \
    ../../gv/src/actions/gv_actionfactory.cpp \
    ../../gv/src/actions/gv_actionfileclose.cpp \
    ../../gv/src/actions/gv_actionfilenew.cpp \
    ../../gv/src/actions/gv_actionfileopen.cpp \
    ../../gv/src/actions/gv_actionfileprint.cpp \
    ../../gv/src/actions/gv_actionfilequit.cpp \
    ../../gv/src/actions/gv_actionfilesave.cpp \
    ../../gv/src/actions/gv_actionfilesaveas.cpp \
    ../../gv/src/actions/gv_actionformatitemcolor.cpp \
    ../../gv/src/actions/gv_actionformatlineattributes.cpp \
    ../../gv/src/actions/gv_actionhelpabout.cpp \
    ../../gv/src/actions/gv_actionhelpmanual.cpp \
    ../../gv/src/actions/gv_actionhelpwebsite.cpp \
    ../../gv/src/actions/gv_actionmodifymergeline.cpp \
    ../../gv/src/actions/gv_actionmodifymove.cpp \
    ../../gv/src/actions/gv_actionmodifysplitline.cpp \
 #   ../../gv/src/actions/gv_actionselectbase.cpp \
 #   ../../gv/src/actions/gv_actionselectsingle.cpp \
    ../../gv/src/actions/gv_actionsetsnapmode.cpp \
    ../../gv/src/actions/gv_actionsetsnapmodeauto.cpp \
    ../../gv/src/actions/gv_actionsetsnapmodefree.cpp \
    ../../gv/src/actions/gv_actionsetsnapmodegrid.cpp \
    ../../gv/src/actions/gv_actionsetsnaprestriction.cpp \
    ../../gv/src/actions/gv_actionsetsnaprestrictionnothing.cpp \
    ../../gv/src/actions/gv_actionsetsnaprestrictionorthogonal.cpp \
    ../../gv/src/actions/gv_actionsystemdatabaseconnect.cpp \
    ../../gv/src/actions/gv_actionsystemselectproject.cpp \
    ../../gv/src/actions/gv_actiontestnew.cpp \
    ../../gv/src/actions/gv_actiontestnewqt.cpp \
    ../../gv/src/actions/gv_actionviewdraft.cpp \
    ../../gv/src/actions/gv_actionviewgrid.cpp \
    ../../gv/src/actions/gv_actionviewstatusbar.cpp \
    ../../gv/src/actions/gv_actionzoomauto.cpp \
    ../../gv/src/actions/gv_actionzoomin.cpp \
    ../../gv/src/actions/gv_actionzoomout.cpp \
    ../../gv/src/actions/gv_actionzoompan.cpp \
    ../../gv/src/actions/gv_actionzoomprevious.cpp \
    ../../gv/src/actions/gv_actionzoomredraw.cpp \
    ../../gv/src/actions/gv_actionzoomselection.cpp \
    ../../gv/src/actions/gv_actionzoomwindow.cpp \
    ../../gv/src/actions/gv_snapper.cpp \
    ../../gv/src/filters/gv_filterxml.cpp \
    ../../gv/src/gui/gv_dialogfactory.cpp \
    ../../gv/src/gui/gv_drawinglistwidget.cpp \
    ../../gv/src/gui/gv_drawingwidget.cpp \
    ../../gv/src/gui/gv_eventhandler.cpp \
    ../../gv/src/gui/gv_eventhandlerqt.cpp \
    ../../gv/src/gui/gv_eventhandlerqv.cpp \
    ../../gv/src/gui/gv_graphicsscene.cpp \
    ../../gv/src/gui/gv_graphicsview.cpp \
    ../../gv/src/gui/gv_graphicsviewqtwidget.cpp \
    ../../gv/src/gui/gv_graphicsviewqvwidget.cpp \
    ../../gv/src/gui/gv_gvlegend.cpp \
#    ../../gv/src/gui/gv_gvmdi_old.cpp \
    ../../gv/src/gui/gv_grid.cpp \
    ../../gv/src/gui/gv_legendwidget.cpp \
    ../../gv/src/gui/gv_lineattributesdialog.cpp \
#    ../../gv/src/gui/gv_mdiwindow_old.cpp \
    ../../gv/src/gui/gv_onetimedialog.cpp \
    ../../gv/src/gui/gv_orthogrid.cpp \
#    ../../gv/src/gui/gv_pipelinedialog.cpp \
    ../../gv/src/gui/gv_previewwidget.cpp \
    ../../gv/src/gui/gv_propertieswidget.cpp \
#    ../../gv/src/mathb/rb_boundary.cpp \
#    ../../gv/src/mathb/rb_math.cpp \
#    ../../gv/src/mathb/rb_matrix.cpp \
#    ../../gv/src/mathb/rb_node.cpp \
#    ../../gv/src/mathb/rb_port.cpp \
#    ../../gv/src/mathb/rb_vector.cpp \
    ../../gv/src/model/gv_component.cpp \
    ../../gv/src/model/gv_drawing.cpp \
#    ../../gv/src/model/gv_equipment.cpp \
    ../../gv/src/model/gv_line.cpp \
    ../../gv/src/model/gv_modelfactory.cpp \
    ../../gv/src/model/gv_node.cpp \
    ../../gv/src/model/gv_objectfactory.cpp \
#    ../../gv/src/model/gv_pipeline.cpp \
    ../../gv/src/model/gv_port.cpp \
    ../../gv/src/model/gv_project.cpp \
    ../../gv/src/model/gv_resizehandle.cpp \
    ../../gv/src/model/gv_rotationlever.cpp \
    ../../gv/src/model/gv_symbol.cpp \
    ../../gv/src/model/gv_text.cpp \
    ../../gv/src/model/rb_component.cpp \
    ../../gv/src/model/rb_node.cpp \
    ../../gv/src/model/rb_port.cpp \
    ../../gv/src/operations/gv_dwgscenesync.cpp
#    ../../gv/src/snap/gv_snapauto.cpp \
#    ../../gv/src/snap/gv_snapcenter.cpp \
#    ../../gv/src/snap/gv_snapdistance.cpp \
#    ../../gv/src/snap/gv_snapend.cpp \
#    ../../gv/src/snap/gv_snapentitybase.cpp \
#    ../../gv/src/snap/gv_snapfree.cpp \
#    ../../gv/src/snap/gv_snapgrid.cpp \
#    ../../gv/src/snap/gv_snapintersection.cpp \
#    ../../gv/src/snap/gv_snapmiddle.cpp \
#    ../../gv/src/snap/gv_snaponentity.cpp \
#    ../../gv/src/snap/gv_snapreference.cpp

RESOURCES += \
    ../../gv/src/gui/gv.qrc \
    ../../db/src/gui/db.qrc
