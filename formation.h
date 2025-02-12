#ifndef FORMATION_H
#define FORMATION_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>
#include<QTableView>


class formation {
    int id_formation;
    QDate date_debut, date_fin;
    int duree, prix;
    QString domaine;
    int nb_particip;

public:
    //constructeurs
    formation() {}
    formation(int id_formation, QDate d, QDate f,int dr , int p, QString dm , int n);

    //getters
    int getid_formation() { return id_formation; }
    QDate getdate_debut() { return date_debut; }
    QDate getdate_fin() { return date_fin; }
    int getduree() { return duree; }
    int getprix() { return prix; }
    QString getdomaine() { return domaine; }
    int getnb_particip() { return nb_particip; }

    //setters:
    void setid_formation(int id_formation) { this->id_formation = id_formation; }
    void setdate_debut(QDate d) { date_debut = d; }
    void setdate_fin(QDate f) { date_fin = f; }
    void setduree(int dr) { duree = dr; }
    void setprix(int p) { prix = p; }
    void setdomaine(QString dm) { domaine = dm; }
    void setnb_particip(int n) { nb_particip = n; }

    //fonctionalité
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool supprimer_tout(int);
    bool modifier(QDate new_date_debut, QDate new_date_fin, int new_duree, int new_prix, QString new_domaine, int new_nb_particip);
    bool idExists(int id_formation) const;
   // export void on_pushButton_clicked();
     void searchById(const QString &id, QTableView *tableView);
     void on_pushButton_clicked();

     QString getListeFormationsAsHtml();
     QSqlQuery getinfocalender(QDate date);






};

#endif // FORMATION_H
