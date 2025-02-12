#include "rendezvous.h"
#include "ui_rendezvous.h"
#include"formation.h"
#include "email.h"
#include<QMessageBox>
#include<QSqlQueryModel>
#include <QSqlQueryModel>
#include <QTableWidgetItem>
#include<QtDebug>
#include<QInputDialog>
#include<QSqlError>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QPieSeries>
#include <QChart>
#include <QChartView>
#include <QGraphicsScene>
#include <QPainter>
#include <QPixmap>
#include <QBrush>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QVBoxLayout>
#include <QLabel>
#include <QPdfWriter>
#include <QTextDocumentWriter>
#include <QTextDocument>
#include <QtPrintSupport/QPrinter>
#include <QtGui/QTextDocument>
#include <QFileDialog>
#include "assistance.h"
#include "arduino.h"


rendezVous::rendezVous(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::rendezVous)
{
    ui->setupUi(this);
    fr = new formation();
    ui->tableView->setModel(fr->afficher());
    ui->stackedWidget->setCurrentIndex(0);
    int ret=A.connect_arduino(); // lancer la connexion à arduino
    switch(ret){
    case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
        break;
    case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
       break;
    case(-1):qDebug() << "arduino is not available";
    }
     QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label())); // permet de lancer
     //le slot update_label suite à la reception du signal readyRead (reception des données).
}

rendezVous::~rendezVous()
{
    delete ui;
}

void rendezVous::on_addBtn_clicked()
{
    try {
        QString id_formation_str = ui->id_formation->text();
        if (id_formation_str.isEmpty()) {
            QMessageBox::critical(nullptr, QObject::tr("Error"),
                                  QObject::tr("ID Formation cannot be empty."), QMessageBox::Cancel);
            return;
        }
        int id_formation = id_formation_str.toInt();

        QString domaine = ui->domaine->text();
        QRegExp regex("^[A-Za-z]+$");
        if (!regex.exactMatch(domaine)) {
            QMessageBox::critical(nullptr, QObject::tr("Error"),
                                  QObject::tr("Domaine invalide."), QMessageBox::Cancel);
            return;
        }

        QDate date_debut = ui->date_debut->date();
        QDate date_fin = ui->date_fin->date();
        int duree = ui->duree->text().toInt();
        int prix = ui->prix->text().toInt();
        int nb_particip = ui->nb_particip->text().toInt();

        formation fr(id_formation, date_debut, date_fin, duree, prix, domaine, nb_particip);

        if (fr.idExists(id_formation)){
            QMessageBox::critical(nullptr, QObject::tr("Error"),
                                  QObject::tr("IDENTIFIANT EXISTE DEJA!!"), QMessageBox::Cancel);
            return;
        }

        // condition date
        if (date_debut > date_fin) {
            QMessageBox::critical(nullptr, QObject::tr("Error"),
                                  QObject::tr("date fin doit etre superieure a la date debut!."), QMessageBox::Cancel);
            return;
        }

        // condition durée prix nb_particip
        if (duree <= 0 || prix <= 0 || nb_particip <= 0) {
            QMessageBox::critical(nullptr, QObject::tr("Error"),
                                  QObject::tr("Durée, prix, et le nombre de participant doit etre des entiers!!."), QMessageBox::Cancel);
            return;
        }




        bool test = fr.ajouter();
        if (test) {
            ui->tableView->setModel(fr.afficher());
            QMessageBox::information(nullptr, QObject::tr("OK"),
                                     QObject::tr("Ajout effectué\n"
                                                 "Click Cancel to exit."), QMessageBox::Cancel);
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                                  QObject::tr("Ajout non effectué.\n"
                                              "Click cancel."), QMessageBox::Cancel);
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception caught:" << e.what();
    } catch (...) {
        qDebug() << "Unknown exception caught";
    }
}

void rendezVous::on_pushButton_8_clicked()
{
    bool ok;
    int id_formation = QInputDialog::getInt(this, tr("ajouter l'identifiant fr"),
                                            tr("ID:"), 0, 0, 1000000, 1, &ok);
    if (!ok) {
        qDebug() << "quit";
        return;
    }

    try {
        // Check if id_formation exists
        formation fr;
        if (!fr.idExists(id_formation)) {
            QMessageBox::critical(nullptr, QObject::tr("Error"),
                                  QObject::tr("ID Formation n'existe pas."), QMessageBox::Cancel);
            return;
        }

        QDate date_debut = ui->date_debut->date();
        QDate date_fin = ui->date_fin->date();
        int duree = ui->duree->text().toInt();
        int prix = ui->prix->text().toInt();
        QString domaine = ui->domaine->text();
        int nb_particip = ui->nb_particip->text().toInt();

        fr.setid_formation(id_formation);
        fr.setdate_debut(date_debut);
        fr.setdate_fin(date_fin);
        fr.setduree(duree);
        fr.setprix(prix);
        fr.setdomaine(domaine);
        fr.setnb_particip(nb_particip);

        bool test = fr.supprimer(id_formation);
        if (test) {
            ui->tableView->setModel(fr.afficher());
            QMessageBox::information(nullptr, QObject::tr("OK"),
                                     QObject::tr("Suppression effectuée\n"
                                                 "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                  QObject::tr("Suppression non effectuée.\n"
                                              "Cliquez sur Annuler."), QMessageBox::Cancel);
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception caught:" << e.what();
    } catch (...) {
        qDebug() << "Unknown exception caught";
    }
}


void rendezVous::on_pushButton_10_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);


}

void rendezVous::on_pushButton_9_clicked()
{
    try {

        bool ok;
        QString id_formation_str = QInputDialog::getText(this, tr("Identifiant ?"),
                                                          tr("ID Formation:"), QLineEdit::Normal, "", &ok);
        if (!ok) {
            qDebug() << "User canceled input";
            return;
        }

        int id_formation = id_formation_str.toInt();

        int start_day = QInputDialog::getInt(this, tr("saisir le jour"),
                                              tr("Day:"), QDate::currentDate().day(), 1, 31, 1, &ok);
        if (!ok) {
            qDebug() << "quit";
            return;
        }

        int start_month = QInputDialog::getInt(this, tr("saisir mois"),
                                                tr("Month:"), QDate::currentDate().month(), 1, 12, 1, &ok);
        if (!ok) {
            qDebug() << "quit";
            return;
        }

        int start_year = QInputDialog::getInt(this, tr("saisir l'anneé"),
                                               tr("Year:"), QDate::currentDate().year(), 1900, 2100, 1, &ok);
        if (!ok) {
            qDebug() << "quit";
            return;
        }

        QDate new_date_debut(start_year, start_month, start_day);


        int end_day = QInputDialog::getInt(this, tr("Enter New End Date"),
                                              tr("Day:"), QDate::currentDate().day(), 1, 31, 1, &ok);
        if (!ok) {
            qDebug() << "quit";
            return;
        }

        int end_month = QInputDialog::getInt(this, tr("Enter New End Date"),
                                                tr("Month:"), QDate::currentDate().month(), 1, 12, 1, &ok);
        if (!ok) {
            qDebug() << "quit";
            return;
        }

        int end_year = QInputDialog::getInt(this, tr("Enter New End Date"),
                                               tr("Year:"), QDate::currentDate().year(), 1900, 2100, 1, &ok);
        if (!ok) {
            qDebug() << "quit";
            return;
        }

        QDate new_date_fin(end_year, end_month, end_day);


        int new_duree = QInputDialog::getInt(this, tr("nouveau durée"),
                                              tr("Duration (days):"), 1, 1, 365, 1, &ok);
        if (!ok) {
            qDebug() << "User canceled input";
            return;
        }


        int new_prix = QInputDialog::getInt(this, tr("nouveau prix"),
                                             tr("Price:"), 0, 0, 1000000, 1, &ok);
        if (!ok) {
            qDebug() << "User canceled input";
            return;
        }


        QString new_domaine = QInputDialog::getText(this, tr("nouveau domaine"),
                                                    tr("Domain:"), QLineEdit::Normal, "", &ok);
        if (!ok) {
            qDebug() << "User canceled input";
            return;
        }


        int new_nb_particip = QInputDialog::getInt(this, tr("nouveau nombre de participant"),
                                                    tr("Number of Participants:"), 1, 1, 1000, 1, &ok);
        if (!ok) {
            qDebug() << "User canceled input";
            return;
        }

        formation fr(id_formation, new_date_debut, new_date_fin, new_duree, new_prix, new_domaine, new_nb_particip);

        bool test = fr.modifier(new_date_debut, new_date_fin, new_duree, new_prix, new_domaine, new_nb_particip);
        if (test) {
            ui->tableView->setModel(fr.afficher());
            QMessageBox::information(nullptr, QObject::tr("OK"),
                                     QObject::tr("Modification effectuée\n"
                                                 "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                  QObject::tr("Modification non effectuée.\n"
                                              "Cliquez sur Annuler."), QMessageBox::Cancel);
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception caught:" << e.what();
    } catch (...) {
        qDebug() << "Unknown exception caught";
    }
}

void rendezVous::on_pushButton_6_clicked()
{
    //liste des informat
    fr = new formation();
    ui->tableView->setModel(fr->afficher());

}



void rendezVous::on_pushButton_clicked()//export
{

}

void rendezVous::on_pushButton_5_clicked()//recherche
{
    QString id = ui->searchEdit->text();
    formation form;
    form.searchById(id, ui->tableView);
}

//trie
void rendezVous::on_commandLinkButton_2_clicked()
{

    QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(ui->tableView->model());
    if (!model) {
        qDebug() << "Error: Unable to cast model to QSqlQueryModel.";
        return;
    }


    QString query = model->query().executedQuery();
    int orderByIndex = query.indexOf("ORDER BY");
    if (orderByIndex != -1) {
        query = query.left(orderByIndex);
    }
    query += " ORDER BY nb_particip ASC";


    model->setQuery(query);
}

//stat
void rendezVous::on_pushButton_16_clicked()
{

    int totalParticipants = calculateTotalParticipants();


    QWidget *statisticsPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    QLabel *titleLabel = new QLabel("Statistics");
    QLabel *participantsLabel = new QLabel(QString("Total Participants: %1").arg(totalParticipants));

    layout->addWidget(titleLabel);
    layout->addWidget(participantsLabel);
    statisticsPage->setLayout(layout);


    ui->stackedWidget->addWidget(statisticsPage);


    ui->stackedWidget->setCurrentWidget(statisticsPage);
}


int rendezVous::calculateTotalParticipants()
{
    int totalParticipants = 0;


    QSqlQuery query;
    if (query.exec("SELECT SUM(nb_particip) FROM formation")) {
        if (query.next()) {
            totalParticipants = query.value(0).toInt();
        }
    } else {
        qDebug() << "Error calculating total participants:" << query.lastError().text();
    }

    return totalParticipants;
}



void rendezVous::on_exportbutton_clicked() {
    formation formationInstance;
    QString html = formationInstance.getListeFormationsAsHtml();
    qDebug() << "HTML Content:" << html;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save PDF"), "", tr("PDF Files (*.pdf)"));
    if (fileName.isEmpty()) {
        qDebug() << "PDF generation canceled.";
        return;
    }
    qDebug() << "Attempting to write PDF to:" << fileName;

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPagedPaintDevice::A4);
    QPainter painter(&pdfWriter);
    QTextDocument doc;
    doc.setHtml(html);
    doc.drawContents(&painter);

    if (!painter.end()) {
        qDebug() << "Error occurred while generating PDF!";
    } else {
        qDebug() << "PDF successfully generated!";
    }
}

void rendezVous::on_pushButton_12_clicked()//gestion client butotn
{

 //code

      }




void rendezVous::on_calendarWidget_activated(const QDate &date)
{

    QSqlQuery query = fr->getinfocalender(date);
    QString htmlOutput;

    if (!query.next()) {
        htmlOutput = "<h3 style='color:red;'>Pas de Formation Pour ce Jour !</h3>";
    } else {
        htmlOutput = "<h2 style='color:green;'>Formation En Cours :</h2><ul>";
        do {
            htmlOutput += "<li><b>Formation Id:</b> " + query.value(0).toString() + "</li>";
            htmlOutput += "<li><b>Date Debut:</b> " + query.value(1).toString() + "</li>";
            htmlOutput += "<li><b>Date Fin:</b> " + query.value(2).toString() + "</li>";
            htmlOutput += "<li><b>Duree:</b> " + query.value(3).toString() + " heure</li>";
            htmlOutput += "<li><b>Prix:</b> " + query.value(4).toString() + "$</li>";
            htmlOutput += "<li><b>Domaine:</b> " + query.value(5).toString() + "</li>";
            htmlOutput += "<li><b>Nombre de Participants:</b> " + query.value(6).toString() + " Per</li>";
            htmlOutput += "<li>******************************************************************************************** </li>";
        } while (query.next());
        htmlOutput += "</ul>";
    }
    ui->infocalender->setHtml(htmlOutput);



}


void rendezVous::on_listeA_clicked()
{
    //liste des informat
assistance As;
    ui->viewA->setModel(As.afficher());
}

void rendezVous::on_deleteA_clicked()
{

    bool ok;
    int id_formation = QInputDialog::getInt(this, tr("ajouter l'identifiant fr"),
                                            tr("ID:"), 0, 0, 1000000, 1, &ok);
    if (!ok) {
        qDebug() << "quit";
        return;
    }

    try {
        // Check if id_formation exists
        assistance As;
        if (!As.idExists(id_formation)) {
            QMessageBox::critical(nullptr, QObject::tr("Error"),
                                  QObject::tr("ID Formation n'existe pas."), QMessageBox::Cancel);
            return;
        }


        bool test = As.supprimer(id_formation);
        if (test) {
            ui->viewA->setModel(As.afficher());
            QMessageBox::information(nullptr, QObject::tr("OK"),
                                     QObject::tr("Suppression effectuée\n"
                                                 "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                  QObject::tr("Suppression non effectuée.\n"
                                              "Cliquez sur Annuler."), QMessageBox::Cancel);
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception caught:" << e.what();
    } catch (...) {
        qDebug() << "Unknown exception caught";
    }

}

void rendezVous::on_updateA_clicked()
{
    try {

        bool ok;
        QString id_formation_str = QInputDialog::getText(this, tr("Identifiant ?"),
                                                          tr("ID Formation:"), QLineEdit::Normal, "", &ok);
        assistance As;

        int id_formation = id_formation_str.toInt();
        if (!ok) {
            qDebug() << "User canceled input";
            return;
        }

        QString idf_str = QInputDialog::getText(this, tr(" nouveau Id formation ?"),
                                                          tr(" new id_formation:"), QLineEdit::Normal, "", &ok);
        int idf = idf_str.toInt();
        if (!ok) {
            qDebug() << "User canceled input";
            return;
        }

        int idc = QInputDialog::getInt(this, tr("id_client"),
                                             tr("nouveau client:"), 0, 0, 1000000, 1, &ok);

        if ( !(As.idExists(id_formation) ) &&!(As.idExists(idc)) && !(As.idExists(idf)))
            ok=0;
        if (!ok) {
            QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                  QObject::tr(" contraint Modification non effectuée.\n"
                                              "Cliquez sur Annuler."), QMessageBox::Cancel);
            return;
        }
         assistance Asi(idf,idc);
         qDebug()<<"/<<"<<idf<<"/<<"<<idc<<"/"<<id_formation;
        bool test = Asi.modifier(id_formation);
        if (test) {
            ui->viewA->setModel(Asi.afficher());

            Email em;
            int resultat=em.sendEmail(Asi.recupereadr(idc),"mise a jour de la formation","bonjour ");
            qDebug()<<Asi.recupereadr(idc);
            if(resultat == 0)
                {
                    QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("Email sent succefully\n"
                            "Click Cancel to exit."), QMessageBox::Cancel);
                }
                else
                {
                    QMessageBox::warning(nullptr, QObject::tr("Erreur"), QObject::tr("Message non envoyé"), QMessageBox::Cancel);
                    return;
                }
            QMessageBox::information(nullptr, QObject::tr("OK"),
                                     QObject::tr("Modification effectuée\n"
                                                 "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                  QObject::tr("Modification non effectuée.\n"
                                              "Cliquez sur Annuler."), QMessageBox::Cancel);
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception caught:" << e.what();
    } catch (...) {
        qDebug() << "Unknown exception caught";
    }

}

void rendezVous::on_ajoutA_clicked()
{
    try {

        bool ok;
        QString id_formation_str = QInputDialog::getText(this, tr("Identifiant ?"),
                                                          tr("ID Formation:"), QLineEdit::Normal, "", &ok);
        assistance As;
        int id_formation = id_formation_str.toInt();
        if (!ok) {
            qDebug() << "User canceled input";
            return;
        }

        int idc = QInputDialog::getInt(this, tr("id_client"),

                                      tr("id_client:"), 0, 0, 1000000, 1, &ok);
        if ( !(As.idExists(id_formation)) &&!(As.idExists(idc)))
            ok=0;
        if (!ok) {
            QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                  QObject::tr("ajout non effectuée.\n"
                                              "Cliquez sur Annuler."), QMessageBox::Cancel);
            return;
        }
         assistance Asi(id_formation,idc);
        bool test = Asi.ajouter();
        if (test) {
            ui->viewA->setModel(Asi.afficher());
            Email em;
            int resultat=em.sendEmail(Asi.recupereadr(idc),"validation de la formation","bonjouuur ");
            qDebug()<<Asi.recupereadr(idc);
            if(resultat == 0)
                {
                    QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("Email sent succefully\n"
                            "Click Cancel to exit."), QMessageBox::Cancel);
                }
                else
                {
                    QMessageBox::warning(nullptr, QObject::tr("Erreur"), QObject::tr("Message non envoyé"), QMessageBox::Cancel);
                    return;
                }
            QMessageBox::information(nullptr, QObject::tr("OK"),
                                     QObject::tr("Modification effectuée\n"
                                                 "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                                  QObject::tr("Modification non effectuée.\n"
                                              "Cliquez sur Annuler."), QMessageBox::Cancel);
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception caught:" << e.what();
    } catch (...) {
        qDebug() << "Unknown exception caught";
    }

}

void rendezVous::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void rendezVous::on_pushButton_17_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void rendezVous::on_pushButton_18_clicked()
{
     ui->stackedWidget->setCurrentIndex(0);
}

void rendezVous::on_arduino_clicked() {

    QByteArray data = A.read_from_arduino();
    qDebug() << QString::fromUtf8(data);
    int id = data.toInt();
    bool idExists = checkIDInDatabase(id);
    if (idExists) {
        qDebug() << "ID exists in the database.";
        A.openDoor();

    } else {
        qDebug() << "ID does not exist in the database.";

    }
}

bool rendezVous::checkIDInDatabase(int id) {
    // Implement the logic to check if the ID exists in the 'formation' table
    // For example, you might use QSqlQuery to execute a SELECT query

    QSqlQuery query;
    query.prepare("SELECT * FROM formation WHERE id_formation = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        // ID exists in the database
        return true;
    } else {
        // ID does not exist in the database
        return false;
    }
}
