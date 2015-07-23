#ifndef DB_HEXEDITWIDGET_H
#define DB_HEXEDITWIDGET_H

#include "qhexedit.h"
#include "rb_string.h"
#include "rb_widget.h"


namespace Ui {
class DB_HexEditWidget;
}

class DB_EXPORT DB_HexEditWidget : public RB_Widget {

    Q_OBJECT

public:
    explicit DB_HexEditWidget(QWidget* parent = 0);
    virtual ~DB_HexEditWidget();

    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }
    virtual QWidget* getMainWidget();

    virtual void init();

    virtual bool fileSave(bool withSelect);
    virtual bool fileSaveAs();

    virtual RB_String getHelpSubject() const;

    virtual bool loadFile(const RB_String& fn);
    virtual bool saveFile(const RB_String& fn);

private:
    Ui::DB_HexEditWidget* ui;
};

#endif // DB_HEXEDITWIDGET_H
