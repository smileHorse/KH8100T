#ifndef RDBTABLEMODEL_H
#define RDBTABLEMODEL_H

#include <QtWidgets/QtWidgets>

/************************************************************************/
/* 实时库表名模型                                                       */
/************************************************************************/
class RdbTableNameModel : public QAbstractListModel
{
	Q_OBJECT

public:
	RdbTableNameModel(QObject* parent = 0);

	int rowCount(const QModelIndex &parent /* = QModelIndex() */) const;

	QVariant data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const;
};

/************************************************************************/
/* 实时库表字段模型                                                     */
/************************************************************************/

#endif