#include "planttaxonomymodel.h"

struct TaxoItem {
    TaxoItem(QVariant data, QVariant id, int row, TaxoItem *parent)
        :data(data), id(id), row(row), parent(parent)
    {
        parent->children.push_back(this);
    }

    TaxoItem(QVariant data)
        :data(data), row(0), parent(0)
    {
    }

    ~TaxoItem()
    {
        qDeleteAll(children);
    }

    QVariant data;
    QVariant id;
    int row;
    TaxoItem* parent;
    QList<TaxoItem*> children;
};

PlantTaxonomyModel::PlantTaxonomyModel(QObject *parent, QSqlDatabase db) :
    QAbstractItemModel(parent),
    m_db(db)
{
    m_root = new TaxoItem(tr("Caption"));
}

PlantTaxonomyModel::~PlantTaxonomyModel()
{
    delete m_root;
}

QVariant PlantTaxonomyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        TaxoItem *item = static_cast<TaxoItem*>(index.internalPointer());
        return item->data;
    }
    else if (role == IdRole) {
        TaxoItem *item = static_cast<TaxoItem*>(index.internalPointer());
        return item->id;
    }

    return QVariant();
}

QVariant PlantTaxonomyModel::headerData(int, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_root->data;
    return QVariant();
}

QModelIndex PlantTaxonomyModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TaxoItem* parentItem;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<TaxoItem*>( parent.internalPointer() );

    TaxoItem *child = parentItem->children[row];

    return createIndex(row, column, child);
}

QModelIndex PlantTaxonomyModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TaxoItem *item = static_cast<TaxoItem*> (index.internalPointer());
    TaxoItem *parentItem = item->parent;

    if (parentItem == m_root)
        return QModelIndex();

    return createIndex(parentItem->row, 0, parentItem);
}

int PlantTaxonomyModel::rowCount(const QModelIndex &parent) const
{
    TaxoItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<TaxoItem*>(parent.internalPointer());

    return parentItem->children.size();
}

int PlantTaxonomyModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QSqlError PlantTaxonomyModel::lastError() const
{
    return m_lastError;
}

bool PlantTaxonomyModel::select()
{
    IdItemMap parentLevel;
    parentLevel[0] = m_root;

    QString divisioQueryStr = "select id, name, 0 from divisio";
    QString classisQueryStr = "select id, name, divisio_id from classis";
    QString familiaQueryStr = "select id, name, classis_id from familia";
    QString genusQueryStr = "select id, name, familia_id from genus";
    QString speciesQueryStr = "select id, "
            "case when (lat_name is not null and rus_name is not null) then "
            "lat_name || ' (' || rus_name || ')' "
            "else coalesce(lat_name, rus_name, other_names) end, "
            "genus_id from species";

    return buildLevel(divisioQueryStr, parentLevel, true)
            && buildLevel(classisQueryStr, parentLevel, true)
            && buildLevel(familiaQueryStr, parentLevel, true)
            && buildLevel(genusQueryStr, parentLevel, true)
            && buildLevel(speciesQueryStr, parentLevel, false);
}

bool PlantTaxonomyModel::buildLevel(const QString &query, PlantTaxonomyModel::IdItemMap &parentLevel, bool buildNextParents)
{
    QSqlQuery q;

    if (!q.exec(query)) {
        m_lastError = q.lastError();
        return false;
    }

    if (!buildLevelHelper(q, parentLevel, buildNextParents))
        return false;

    m_lastError = QSqlError();
    return true;
}

bool PlantTaxonomyModel::buildLevelHelper(QSqlQuery &q, IdItemMap &parents, bool buildNextParents)
{
    IdItemMap nextParents;

    int row=0;
    while(q.next()) {
        QVariant id = q.value(0);
        QVariant data = q.value(1);
        int parentId = q.value(2).toInt();

        TaxoItem *parent;
        IdItemMap::const_iterator parIt = parents.find(parentId);
        if (parIt == parents.end()) {
            m_lastError = QSqlError(QString(), tr("Taxonomy parent not found"), QSqlError::UnknownError);
            return false;
        }
        parent = parIt.value();

        TaxoItem *item = new TaxoItem(data, id, row, parent);

        if (buildNextParents)
            nextParents[id.toInt()] = item;

        ++row;
    }

    if (buildNextParents)
        nextParents.swap(parents);

    m_lastError = QSqlError();
    return true;
}
