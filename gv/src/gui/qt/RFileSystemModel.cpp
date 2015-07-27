#include "RFileSystemModel.h"

#include <QDebug>

RFileSystemModel::RFileSystemModel(QObject* parent) :
	QFileSystemModel(parent) {
}

void RFileSystemModel::fetchMore(const QModelIndex & parent) {
	QFileSystemModel::fetchMore(parent);
}

bool RFileSystemModel::canFetchMore(const QModelIndex & parent) const {
	return QFileSystemModel::canFetchMore(parent);
}

QModelIndex RFileSystemModel::setRootPath(const QString & newPath) {
	return QFileSystemModel::setRootPath(newPath);
}

QModelIndex RFileSystemModel::index(const QString& path, int column) const {
	return QFileSystemModel::index(path, column);
}

QModelIndex RFileSystemModel::index(int row, int column,
		const QModelIndex & parent) const {
	return QFileSystemModel::index(row, column, parent);
}

QString RFileSystemModel::filePath(const QModelIndex& index) const {
	return QFileSystemModel::filePath(index);
}

void RFileSystemModel::setFilter(QDir::Filters filters) {
	QFileSystemModel::setFilter(filters);
}

bool RFileSystemModel::isDir(const QModelIndex& index) const {
	return QFileSystemModel::isDir(index);
}

bool RFileSystemModel::setHeaderData(int section, Qt::Orientation orientation,
		const QString& value, int role) {
	return setHeaderData(section, orientation, QVariant(value), role);
}

bool RFileSystemModel::setHeaderData(int section, Qt::Orientation orientation,
		const QVariant& value, int role) {
	return QFileSystemModel::setHeaderData(section, orientation, value, role);
}

int RFileSystemModel::rowCount(const QModelIndex & parent) const {
	return QFileSystemModel::rowCount(parent);
}

void RFileSystemModel::setIconProvider(QFileIconProvider* provider) {
	QFileSystemModel::setIconProvider(provider);
}

void RFileSystemModel::setNameFilter(const QString& filter) {
	setNameFilters(QStringList(filter));
}

void RFileSystemModel::setNameFilters(const QStringList& filters) {
	QFileSystemModel::setNameFilters(filters);
}

void RFileSystemModel::setNameFilterDisables(bool enable) {
	QFileSystemModel::setNameFilterDisables(enable);
}

bool RFileSystemModel::setItemData(const QModelIndex& index, const QMap<int,
		QVariant>& roles) {
	return QFileSystemModel::setItemData(index, roles);
}

bool RFileSystemModel::setItemData(const QModelIndex& index,
		Qt::ItemDataRole role, const QString& value) {
	qDebug() << "RFileSystemModel::setItemData: ";
	QMap<int, QVariant> map;
	map.insert(role, value);
	qDebug() << "RFileSystemModel::setItemData: map:" << map;
	return QFileSystemModel::setItemData(index, map);
}

bool RFileSystemModel::setItemData(const QModelIndex& index,
		Qt::ItemDataRole role, const QColor& value) {
	QMap<int, QVariant> map;
	map.insert(role, value);
	return QFileSystemModel::setItemData(index, map);
}

QMap<int, QVariant> RFileSystemModel::itemData(const QModelIndex & index) const {
	return QFileSystemModel::itemData(index);
}
