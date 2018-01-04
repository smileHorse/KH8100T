
#include "rdbTableDefine.h"
#include "rdbTableModel.h"

/************************************************************************/
/* 实时库表名模型                                                       */
/************************************************************************/
RdbTableNameModel::RdbTableNameModel( QObject* parent /*= 0*/ )
	: QAbstractListModel(parent)
{

}

int RdbTableNameModel::rowCount( const QModelIndex &parent /* = QModelIndex() */ ) const
{
	return RdbTableFactory::getTableCount();
}

QVariant RdbTableNameModel::data( const QModelIndex &index, int role /* = Qt::DisplayRole */ ) const
{
	if (role == Qt::DisplayRole)
	{
		return RdbTableFactory::getTableName(index.row());
	}

	return QVariant();
}