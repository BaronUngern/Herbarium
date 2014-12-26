#ifndef PLANTTAXONOMYMODEL_H
#define PLANTTAXONOMYMODEL_H

#include <QAbstractItemModel>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QHash>

struct TaxoItem;

class PlantTaxonomyModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum TaxonomyRole {IdRole = 0x0200};

    explicit PlantTaxonomyModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    ~PlantTaxonomyModel();

    QVariant data(const QModelIndex &index, int role) const;    
    QVariant headerData(int, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QSqlError lastError() const;

public slots:
    bool select();

private:
    typedef QHash<int, TaxoItem*> IdItemMap;

    bool buildLevel(const QString &query, IdItemMap &parentLevel, bool buildNextParents);
    bool buildLevelHelper(QSqlQuery &q, IdItemMap& parents, bool buildNextParents);

    QSqlDatabase m_db;
    QSqlError m_lastError;
    TaxoItem* m_root;
};

#endif // PLANTTAXONOMYMODEL_H
