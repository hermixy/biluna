#include "db_plugintesterdialog.h"

#include <QHeaderView>

// copied from qlibrary.cpp
#ifdef QT_NO_DEBUG
#  define QLIBRARY_AS_DEBUG false
#else
#  define QLIBRARY_AS_DEBUG true
#endif


DB_PluginTesterDialog::DB_PluginTesterDialog(QWidget* parent)
    : RB_Dialog(parent), mSettings(QSettings::UserScope, QLatin1String("Red-Bag")) {

    QApplication::setOverrideCursor(Qt::WaitCursor);

    setupUi(this);
    setWindowTitle(tr("Plugin Testing Tool"));

    QString regkey = QString::fromLatin1("Qt Plugin Cache %1.%2.%3")
             .arg((QT_VERSION & 0xff0000) >> 16)
             .arg((QT_VERSION & 0xff00) >> 8)
             .arg(QLIBRARY_AS_DEBUG ? QLatin1String("debug") : QLatin1String("false"));

    mSettings.beginGroup(regkey);

    QStringList additionalPaths = QApplication::arguments();
    additionalPaths.takeFirst(); // remove executable file
    mSearchPaths = QApplication::libraryPaths();
    mSearchPaths += additionalPaths;

    on_refresh_clicked();

    treeView->setModel(&mModel);
    treeView->expandAll();
    treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QApplication::restoreOverrideCursor();
    readSettings();
}


void DB_PluginTesterDialog::findFiles(const QString &directory) {
    QFont boldFont = QApplication::font();
    boldFont.setBold(true);

    QStringList files = QDir(directory).entryList(QDir::Files);

    foreach(QString file, files) {
        QString filePath = directory + "/" + file;
        if (!QLibrary::isLibrary(filePath))
            continue;
        QList<QStandardItem*> columns;

        QStandardItem *name = new QStandardItem(file);
        name->setToolTip(filePath);
        name->setEditable(false);
        name->setForeground(Qt::darkRed);
        name->setFont(boldFont);

        QStandardItem *isLibrary = new QStandardItem;
        isLibrary->setEditable(false);
        columns << name << isLibrary;

        QLibrary library(filePath);
        bool libraryTest = false;
#if !defined(QT_NO_EXCEPTIONS)
        try
#endif
        {
            libraryTest = library.load();
#if !defined(QT_NO_EXCEPTIONS)
        } catch(...) {
            isLibrary->setText("Exception caught when loading library");
            isLibrary->setForeground(Qt::red);
            isLibrary->setFont(boldFont);
            mModel.appendRow(columns);
            continue;
#endif
        }
        isLibrary->setCheckState(libraryTest ? Qt::Checked : Qt::Unchecked);
#if !defined(QT_NO_EXCEPTIONS)
        try
#endif
        {
            library.unload();
#if !defined(QT_NO_EXCEPTIONS)
        } catch(...) {
            isLibrary->setText("Exception caught when unloading library");
            isLibrary->setForeground(Qt::red);
            isLibrary->setFont(boldFont);
            mModel.appendRow(columns);
            continue;
#endif
        }
        if (!libraryTest) {
            QString errorString = library.errorString();
            errorString.remove(filePath);
            isLibrary->setText(errorString);
            isLibrary->setWhatsThis("Check the dependencies of " + file
                                    + " and make sure that they are present in "
                                    + QApplication::applicationDirPath());
            mModel.appendRow(columns);
            continue;
        }
        isLibrary->setText("Success.");

        QStandardItem *isPlugin = new QStandardItem;
        isPlugin->setEditable(false);
        columns << isPlugin;
        QStringList cachedValue = mSettings.value(filePath).toStringList();
        QStandardItem *cachedData = new QStandardItem;
        if (cachedValue.isEmpty()) {
            cachedData->setText(tr("No cached data for this plugin"));
        } else {
            cachedData->setText(cachedValue.join("|"));
            cachedData->setWhatsThis("If data different from received build key, clear the cache.");
        }
        columns << cachedData;

        QPluginLoader plugin(filePath);
        bool pluginTest = false;
#if !defined(QT_NO_EXCEPTIONS)
        try
#endif
        {
            pluginTest = plugin.load();
#if !defined(QT_NO_EXCEPTIONS)
        } catch(...) {
            isPlugin->setText("Exception caught when loading plugin");
            isPlugin->setForeground(Qt::red);
            isPlugin->setFont(boldFont);
            mModel.appendRow(columns);
            continue;
#endif
        }
        isPlugin->setCheckState(pluginTest ? Qt::Checked : Qt::Unchecked);

        if (!pluginTest) {
            QString errorString = plugin.errorString();
            errorString.remove(" '" + filePath + "'");
            isPlugin->setText(errorString);
            isPlugin->setWhatsThis("Re-compile " + file + " against a compatible Qt library.");
            mModel.appendRow(columns);
            continue;
        }

        QObject *instance = 0;
#if !defined(QT_NO_EXCEPTIONS)
        try
#endif
        {
            instance = plugin.instance();
#if !defined(QT_NO_EXCEPTIONS)
        } catch(...) {
            isPlugin->setText("Exception caught when instantiating plugin object");
            isPlugin->setForeground(Qt::red);
            isPlugin->setFont(boldFont);
            mModel.appendRow(columns);
            continue;
#endif
        }
        if (!instance) {
            isPlugin->setText("Plugin disfunctional - no plugin object created");
            isPlugin->setForeground(Qt::red);
            isPlugin->setFont(boldFont);
            mModel.appendRow(columns);
            continue;
        }
        isPlugin->setToolTip(instance->metaObject()->className());

#if !defined(QT_NO_EXCEPTIONS)
        try
#endif
        {
            delete instance;
#if !defined(QT_NO_EXCEPTIONS)
        } catch(...) {
            isPlugin->setText("Exception caught when deleting plugin object");
            isPlugin->setForeground(Qt::red);
            isPlugin->setFont(boldFont);
            mModel.appendRow(columns);
            continue;
#endif
        }
#if !defined(QT_NO_EXCEPTIONS)
        try
#endif
        {
            plugin.unload();
#if !defined(QT_NO_EXCEPTIONS)
        } catch(...) {
            isPlugin->setText("Exception caught when unloading plugin");
            isPlugin->setForeground(Qt::red);
            isPlugin->setFont(boldFont);
            mModel.appendRow(columns);
            continue;
#endif
        }

        // success
        isPlugin->setText("Success.");
        name->setForeground(Qt::darkGreen);
        name->setFont(boldFont);
        mModel.appendRow(columns);
    }
    QStringList subdirs = QDir(directory).entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(QString subdir, subdirs) {
        subdir = directory + "/" + subdir;
        findFiles(subdir);
    }
}


void DB_PluginTesterDialog::on_refresh_clicked() {
    textEdit->clear();
    mModel.clear();
    mModel.setHorizontalHeaderLabels(QStringList() <<"Filename" << "QLibrary" << "QPluginLoader" << "Cache status");

    foreach(QString searchPath, mSearchPaths) {
        textEdit->append(searchPath);

        QStringList subdirs = QDir(searchPath).entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        foreach(QString subdir, subdirs) {
            subdir = searchPath + "/" + subdir;
            findFiles(subdir);
        }
    }
}

void DB_PluginTesterDialog::on_clearCache_clicked() {
    mSettings.remove(QLatin1String(""));

    on_refresh_clicked();
}

void DB_PluginTesterDialog::on_saveButton_clicked()     {
    QString fileName = QString::fromLatin1("plugintester_%1.log").arg(QLatin1String(QT_VERSION_STR));

    QFileDialog fileDialog;
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setNameFilters(QStringList() << tr("Log files (*.log)") << tr("Any file (*.*)"));
    fileDialog.setConfirmOverwrite(true);
    fileDialog.selectFile(fileName);

    if (!fileDialog.exec())
        return;

    QStringList files = fileDialog.selectedFiles();
    if (files.isEmpty())
        return;
    fileName = files.at(0);

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("Write Error"),
                             tr("Saving the log to file '%1' failed")
                             .arg(fileName));
        return;
    }

    QTextStream out(&file);
    for (int column = 0; column < mModel.columnCount(); ++column) {
        QStandardItem* item = mModel.horizontalHeaderItem(column);
        if (item)
            out << item->text() << "\t\t\t";
        else
            out << "item->text() ERROR" << "\t\t\t";
    }
    out << endl;
    for (int row = 0; row < mModel.rowCount(); ++row) {
        for (int column = 0; column < mModel.columnCount(); ++column) {
            QStandardItem* item = mModel.item(row, column);
            if (item)
                out << item->text() << "\t\t\t";
            else
                out << "item->text() ERROR" << "\t\t\t";
        }
        out << endl;
    }
}


