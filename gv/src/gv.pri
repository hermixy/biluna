# ----------------------------------------------------------
# gv.pri, 2008-03-06
# - forms, headers and sources for graphics view
# ----------------------------------------------------------

FORMS += \
#    ../../ldt/src/gui/ldt_linemainwidget.ui \

HEADERS += \
    ../../gv/src/model/gv.h \
    ../../gv/src/entity/fparser.h \
    ../../gv/src/entity/gv_arc.h \
    ../../gv/src/entity/gv_atomicentity.h \
    ../../gv/src/entity/gv_block.h \
    ../../gv/src/entity/gv_circle.h \
    ../../gv/src/entity/gv_constructionline.h \
    ../../gv/src/entity/gv_dimaligned.h \
    ../../gv/src/entity/gv_dimangular.h \
    ../../gv/src/entity/gv_dimdiametric.h \
    ../../gv/src/entity/gv_dimension.h \
    ../../gv/src/entity/gv_dimlinear.h \
    ../../gv/src/entity/gv_dimordinate.h \
    ../../gv/src/entity/gv_dimradial.h \
    ../../gv/src/entity/gv_document.h \
    ../../gv/src/entity/gv_ellipse.h \
    ../../gv/src/entity/gv_entity.h \
    ../../gv/src/entity/gv_entitycontainer.h \
    ../../gv/src/entity/gv_fontchar.h \
    ../../gv/src/entity/gv_graphic.h \
    ../../gv/src/entity/gv_hatch.h \
    ../../gv/src/entity/gv_image.h \
    ../../gv/src/entity/gv_insert.h \
    ../../gv/src/entity/gv_leader.h \
    ../../gv/src/entity/gv_line.h \
    ../../gv/src/entity/gv_pattern.h \
    ../../gv/src/entity/gv_point.h \
    ../../gv/src/entity/gv_polyline.h \
    ../../gv/src/entity/gv_solid.h \
    ../../gv/src/entity/gv_spline.h \
    ../../gv/src/entity/gv_text.h \
    ../../gv/src/gui/gv_graphicview.h \
    ../../gv/src/gui/gv_grid.h \
    ../../gv/src/gui/gv_painter.h \
    ../../gv/src/tools/gv_application.h \
    ../../gv/src/tools/gv_blocklist.h \
    ../../gv/src/tools/gv_blocklistlistener.h \
    ../../gv/src/tools/gv_clipboard.h \
    ../../gv/src/tools/gv_color.h \
    ../../gv/src/tools/gv_datetime.h \
    ../../gv/src/tools/gv_dir.h \
    ../../gv/src/tools/gv_extptrlist.h \
    ../../gv/src/tools/gv_file.h \
    ../../gv/src/tools/gv_fileinfo.h \
    ../../gv/src/tools/gv_flags.h \
    ../../gv/src/tools/gv_font.h \
    ../../gv/src/tools/gv_fontlist.h \
    ../../gv/src/tools/gv_hash.h \
    ../../gv/src/tools/gv_img.h \
    ../../gv/src/tools/gv_infoarea.h \
    ../../gv/src/tools/gv_information.h \
    ../../gv/src/tools/gv_layer.h \
    ../../gv/src/tools/gv_layerlist.h \
    ../../gv/src/tools/gv_linetypepattern.h \
    ../../gv/src/tools/gv_math.h \
    ../../gv/src/tools/gv_matrix.h \
    ../../gv/src/tools/gv_patternlist.h \
    ../../gv/src/tools/gv_pen.h \
    ../../gv/src/tools/gv_pixmap.h \
    ../../gv/src/tools/gv_pointarray.h \
    ../../gv/src/tools/gv_propertyowner.h \
    ../../gv/src/tools/gv_propertyownercontainerinterface.h \
    ../../gv/src/tools/gv_ptrlist.h \
    ../../gv/src/tools/gv_ptrqueue.h \
    ../../gv/src/tools/gv_settings.h \
    ../../gv/src/tools/gv_system.h \
    ../../gv/src/tools/gv_textstream.h \
    ../../gv/src/tools/gv_translator.h \
    ../../gv/src/tools/gv_undo.h \
    ../../gv/src/tools/gv_undoable.h \
    ../../gv/src/tools/gv_undocycle.h \
    ../../gv/src/tools/gv_units.h \
    ../../gv/src/tools/gv_utility.h \
    ../../gv/src/tools/gv_valuelist.h \
    ../../gv/src/tools/gv_valuevector.h \
    ../../gv/src/tools/gv_variable.h \
    ../../gv/src/tools/gv_variabledict.h \
    ../../gv/src/tools/gv_vector.h

SOURCES += \
    ../../gv/src/entity/fparser.cpp \
    ../../gv/src/entity/gv_arc.cpp \
    ../../gv/src/entity/gv_block.cpp \
    ../../gv/src/entity/gv_circle.cpp \
    ../../gv/src/entity/gv_constructionline.cpp \
    ../../gv/src/entity/gv_dimaligned.cpp \
    ../../gv/src/entity/gv_dimangular.cpp \
    ../../gv/src/entity/gv_dimdiametric.cpp \
    ../../gv/src/entity/gv_dimension.cpp \
    ../../gv/src/entity/gv_dimlinear.cpp \
    ../../gv/src/entity/gv_dimordinate.cpp \
    ../../gv/src/entity/gv_dimradial.cpp \
    ../../gv/src/entity/gv_document.cpp \
    ../../gv/src/entity/gv_ellipse.cpp \
    ../../gv/src/entity/gv_entity.cpp \
    ../../gv/src/entity/gv_entitycontainer.cpp \
    ../../gv/src/entity/gv_graphic.cpp \
    ../../gv/src/entity/gv_hatch.cpp \
    ../../gv/src/entity/gv_image.cpp \
    ../../gv/src/entity/gv_insert.cpp \
    ../../gv/src/entity/gv_leader.cpp \
    ../../gv/src/entity/gv_line.cpp \
    ../../gv/src/entity/gv_pattern.cpp \
    ../../gv/src/entity/gv_point.cpp \
    ../../gv/src/entity/gv_polyline.cpp \
    ../../gv/src/entity/gv_solid.cpp \
    ../../gv/src/entity/gv_spline.cpp \
    ../../gv/src/entity/gv_text.cpp \
    ../../gv/src/gui/gv_graphicview.cpp \
    ../../gv/src/gui/gv_grid.cpp \
    ../../gv/src/gui/gv_painter.cpp \
    ../../gv/src/tools/gv_blocklist.cpp \
    ../../gv/src/tools/gv_clipboard.cpp \
    ../../gv/src/tools/gv_font.cpp \
    ../../gv/src/tools/gv_fontlist.cpp \
    ../../gv/src/tools/gv_infoarea.cpp \
    ../../gv/src/tools/gv_information.cpp \
    ../../gv/src/tools/gv_layer.cpp \
    ../../gv/src/tools/gv_layerlist.cpp \
    ../../gv/src/tools/gv_linetypepattern.cpp \
    ../../gv/src/tools/gv_math.cpp \
    ../../gv/src/tools/gv_matrix.cpp \
    ../../gv/src/tools/gv_patternlist.cpp \
    ../../gv/src/tools/gv_propertyowner.cpp \
    ../../gv/src/tools/gv_settings.cpp \
    ../../gv/src/tools/gv_system.cpp \
    ../../gv/src/tools/gv_undo.cpp \
    ../../gv/src/tools/gv_undoable.cpp \
    ../../gv/src/tools/gv_units.cpp \
    ../../gv/src/tools/gv_utility.cpp \
    ../../gv/src/tools/gv_variabledict.cpp \
    ../../gv/src/tools/gv_vector.cpp

RESOURCES += \
#    ../../gv/src/gui/gv.qrc \
    ../../db/src/gui/db.qrc
