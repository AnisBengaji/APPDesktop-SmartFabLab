#include "formation.h"
#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include<QDate>
#include<QDebug>
#include<QSqlError>
#include <QPieSeries>
#include <QChart>
#include <QChartView>
#include <QGraphicsScene>
#include <QPainter>
#include <QPixmap>
#include <QBrush>




formation::formation(int id_formation,QDate d, QDate f, int dr, int p, QString dm,int n)
{
    this->id_formation = id_formation;
    this->date_debut = d;
    this->date_fin = f;
    this->duree = dr;
    this->prix = p;
    this->domaine = dm;
    this->nb_particip = n;
}


bool formation::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO formation (id_formation, date_debut, date_fin, duree, prix, domaine, nb_particip) "
                  "VALUES (:id, :date_debut, :date_fin, :duree, :prix, :domaine, :nb_particip)");
    query.bindValue(":id", id_formation);
    query.bindValue(":date_debut", date_debut);
    query.bindValue(":date_fin", date_fin);
    query.bindValue(":duree", duree);
    query.bindValue(":prix", prix);
    query.bindValue(":domaine", domaine);
    query.bindValue(":nb_particip", nb_particip);

    if (query.exec()) {
        return true; // Insertion successful
    } else {
        qDebug() << "Error adding formation:" << query.lastError().text();
        return false; // Insertion failed
    }
}
QSqlQueryModel * formation::afficher()
{
    QSqlQueryModel *model=new QSqlQueryModel();
    model->setQuery("SELECT * FROM FORMATION");
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("id_formation"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("date_debut"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("date_fin"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("Duree"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("prix"));
    model->setHeaderData(5,Qt::Horizontal,QObject::tr("domaine"));
    model->setHeaderData(6,Qt::Horizontal,QObject::tr("nb_particip"));
    return model;
}
bool formation::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM formation WHERE id_formation = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        return true; // Deletion successful
    } else {
        qDebug() << "Error deleting formation:" << query.lastError().text();
        return false; // Deletion failed
    }
}

bool formation::modifier(QDate new_date_debut, QDate new_date_fin, int new_duree, int new_prix, QString new_domaine, int new_nb_particip) {
    QSqlQuery query;
    query.prepare("UPDATE formation SET date_debut = :date_debut, date_fin = :date_fin, "
                  "duree = :duree, prix = :prix, domaine = :domaine, nb_particip = :nb_particip "
                  "WHERE id_formation = :id");
    query.bindValue(":id", id_formation);
    query.bindValue(":date_debut", new_date_debut);
    query.bindValue(":date_fin", new_date_fin);
    query.bindValue(":duree", new_duree);
    query.bindValue(":prix", new_prix);
    query.bindValue(":domaine", new_domaine);
    query.bindValue(":nb_particip", new_nb_particip);

    if (query.exec()) {
        // Update object's data if the query was successful
        date_debut = new_date_debut;
        date_fin = new_date_fin;
        duree = new_duree;
        prix = new_prix;
        domaine = new_domaine;
        nb_particip = new_nb_particip;

        return true; // Update successful
    } else {
        qDebug() << "Error updating formation:" << query.lastError().text();
        return false; // Update failed
    }
}
bool formation::idExists(int id_formation) const
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM formation WHERE id_formation = :id");
    query.bindValue(":id", id_formation);
    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        return count > 0;
    }
    return false;
}


void formation::searchById(const QString &id, QTableView *tableView)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM formation WHERE id_formation = :id");
    query.bindValue(":id", id.toInt());
    if(query.exec())
    {
        model->setQuery(query);
        tableView->setModel(model);
    }
    else
    {
        qDebug() << "Search failed:" << query.lastError().text();
        delete model;
    }
}

QString formation::getListeFormationsAsHtml() {
    QSqlQueryModel* model = afficher();
    QString html;
    QTextStream stream(&html);
    stream << "<html><body><table border='1'>\n";
    stream << "<tr>";
    for (int col = 0; col < model->columnCount(); ++col) {
        stream << "<th>" << model->headerData(col, Qt::Horizontal).toString() << "</th>";
    }
    stream << "</tr>\n";
    for (int row = 0; row < model->rowCount(); ++row) {
        stream << "<tr>";
        for (int col = 0; col < model->columnCount(); ++col) {
            stream << "<td>" << model->data(model->index(row, col)).toString() << "</td>";
        }
        stream << "</tr>\n";
    }

    stream << "</table></body></html>";
    delete model;
    return html;
}

QSqlQuery formation::getinfocalender(QDate date)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM FORMATION WHERE date_fin >= :today and date_debut <= :today");
    query.bindValue(":today",date);
    query.exec();
    return query;



}










