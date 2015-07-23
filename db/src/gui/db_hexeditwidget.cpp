#include "db_hexeditwidget.h"

#include "db_actionfactory.h"
#include "db_actionfilesaveas.h"
#include "db_dialogfactory.h"
#include "rb_mdiwindow.h"
#include "ui_db_hexeditwidget.h"

DB_HexEditWidget::DB_HexEditWidget(QWidget* parent) :
        RB_Widget(parent), ui(new Ui::DB_HexEditWidget) {

    ui->setupUi(this);
}

DB_HexEditWidget::~DB_HexEditWidget() {
    delete ui;
}

/**
 * Get name for MDI window title
 * @return file name or type of window (in case of unique window)
 */
RB_String DB_HexEditWidget::getName() const {
    if (mFullFileName.isEmpty()) {
        return getId();
    }
    return QFileInfo(mFullFileName).fileName();
}

/**
 * @returns QHexedit
 */
QWidget* DB_HexEditWidget::getMainWidget() {
    return NULL;
}

void DB_HexEditWidget::init() {
    DB_ACTIONFACTORY->enableEditMenu(getWidgetType());
    DB_ACTIONFACTORY->enableFormatMenu(getWidgetType());
    readSettings();
}

bool DB_HexEditWidget::fileSave(bool /*withSelect*/) {
    if (isNewWidget()) {
        return fileSaveAs();
    }
    return saveFile(mFullFileName);
}

bool DB_HexEditWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String DB_HexEditWidget::getHelpSubject() const {
    return objectName();
}

/**
 * Load file
 * @param fn filename
 * @return true at success
 */
bool DB_HexEditWidget::loadFile(const QString& fn) {
    // Check if file can be read
    QFile file(fn);

    if (!ui->hexEdit->setData(file)) {
        DB_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot read file %1:\n%2.").arg(fn).arg(file.errorString()));
        return false;
    }

    setCurrentFileName(fn);
    emit modificationChanged(false);
    return true;
}

/**
 * Save file
 * @param fn filename
 * @return true if success
 */
bool DB_HexEditWidget::saveFile(const RB_String &fn) {
    QString tmpFileName = fn + ".~tmp";

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QFile file(tmpFileName);
    bool ok = ui->hexEdit->write(file);

    if (QFile::exists(fn)) {
        ok = QFile::remove(fn);
    }

    if (ok) {
        file.setFileName(tmpFileName);
        ok = file.copy(fn);

        if (ok) {
            ok = QFile::remove(tmpFileName);
        }
    }
    QApplication::restoreOverrideCursor();

    if (!ok) {
        DB_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        DB_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
        return false;
    }

    setCurrentFileName(fn);
    setIsNewWidget(false);
    emit modificationChanged(false);
    DB_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
    return true;
}
