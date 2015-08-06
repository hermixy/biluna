#ifndef DB_PLUGINTESTERDIALOG_H
#define DB_PLUGINTESTERDIALOG_H

#include <QtWidgets>
#include <stdlib.h>
#include "rb_dialog.h"

#include "ui_db_plugintesterdialog.h"

class DB_EXPORT DB_PluginTesterDialog : public RB_Dialog, private Ui::DB_PluginTesterDialog {

    Q_OBJECT

public:
    DB_PluginTesterDialog(QWidget* parent = 0);
    void init() { }

protected:
    void findFiles(const QString &directory);

private slots:
    void on_refresh_clicked();
    void on_clearCache_clicked();
    void on_saveButton_clicked();

private:
    QStringList mSearchPaths;
    QStandardItemModel mModel;
    QSettings mSettings;
};

#endif // DB_PLUGINTESTERDIALOG_H
