#ifndef RFILESYSTEMMODEL_H_
#define RFILESYSTEMMODEL_H_

#include <QFileSystemModel>

#define QMODELINDEX QModelIndex()

/**
 * \scriptable
 */
class RFileSystemModel: public QFileSystemModel {

Q_OBJECT

public:
	RFileSystemModel(QObject* parent = 0);

	QModelIndex setRootPath(const QString& newPath);
	QModelIndex index(const QString& path, int column = 0) const;
	QModelIndex index(int row, int column, const QModelIndex & parent =
			QMODELINDEX) const;
	QString filePath(const QModelIndex& index) const;
	void setFilter(QDir::Filters filters);
	bool isDir(const QModelIndex& index) const;
	bool setHeaderData(int section, Qt::Orientation orientation,
			const QString& value, int role = Qt::EditRole);
	bool setHeaderData(int section, Qt::Orientation orientation,
			const QVariant& value, int role = Qt::EditRole);
	int rowCount(const QModelIndex & parent = QMODELINDEX) const;
	void setIconProvider(QFileIconProvider* provider);
	void setNameFilter(const QString& filter);
	void setNameFilters(const QStringList& filters);
	void setNameFilterDisables(bool enable);
	bool setItemData(const QModelIndex& index,
			const QMap<int, QVariant>& roles);
	bool setItemData(const QModelIndex& index, Qt::ItemDataRole role,
			const QString& value);
	bool setItemData(const QModelIndex& index, Qt::ItemDataRole role,
			const QColor& value);
	QMap<int, QVariant> itemData(const QModelIndex & index) const;
	bool canFetchMore(const QModelIndex & parent) const;
	void fetchMore(const QModelIndex & parent);
};

Q_DECLARE_METATYPE(RFileSystemModel*)

#endif /* RFILESYSTEMMODEL_H_ */
