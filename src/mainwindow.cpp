#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "planttaxonomymodel.h"

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include <QFileDialog>
#include<QSqlError>
#include <algorithm>
#include <memory>
#include<addition.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    setupModel();
    fillTableView();


    PlantTaxonomyModel *plantModel = new PlantTaxonomyModel(this);
    plantModel->select();
    ui->treeView->setModel(plantModel);
    ui->treeView->header()->hide();


    //    QSqlQuery query;
    //       // qDebug()<<query.exec("SELECT * FROM main");
    //        query.exec("DROP TABLE main;");
    //        query.exec("CREATE TABLE main(department_name char(255),class char(255),number_box int,family char(255),genus char(255),latin_name char(255),rus_name char(255),sinonim_vida char(255),date_collect int,regoin char(255),district char(255),position char(255),mestoobitania char(255),kollektor char(255),who_opredelil char(255),note char(255),data_voda_v_katalog int)");


    //    QFileDialog dg;
    //    dg.setFileMode(QFileDialog::ExistingFile);

    //    QStringList Qs;
    //    if(dg.exec() != QDialog::Accepted) {
    //        return;
    //    }
    //    Qs  = dg.selectedFiles();
    //    QString myString = Qs.at(0);

    //    QFile f(myString);

    //    if (f.open(QIODevice::ReadOnly | QIODevice::Text))

    //    {
    //        QTextStream ts(&f);
    //        ts.setCodec(QTextCodec::codecForName("ANSI"));
    //      //  int i=0;
    //        while(!ts.atEnd())

    //        {
    //            QString line = ts.readLine();
    //            QStringList words = line.split(";");
    //           // qDebug()<<words;

    //            const int maxColCount = 17;

    //       //     QString z = "INSERT INTO main(department_name,class,number_box,family,genus,latin_name,rus_name,sinonim_vida,date_collect,regoin,district,position,mestoobitania,kollektor,who_opredelil,note,data_voda_v_katalog)"
    //                        "VALUES('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11', '%12', '%13', '%14', '%15', '%16', '%17');";

    //      //      QString x = z;
    //      //      for(int i=0; i<words.size() && i<maxColCount; ++i)
    //      //          x = x.arg(words.at(i));

    //            //int individual_species_id;

    //          QString divisio_1=words.at(0);
    //             int class_div_id=divisio(divisio_1)-1;

    //          QString classis_1=words.at(1);
    //              int familia_class_id=classis(classis_1,class_div_id)-1;

    //          QString familia_1=words.at(3);
    //             int genus_familia_id=familia(familia_1,familia_class_id)-1;

    //          QString genus_1=words.at(4);
    //             int species_genus_id=genus(genus_1,genus_familia_id)-1;


    //             species(words.at(5),words.at(6),words.at(7),species_genus_id);
    //             individual(words.at(8),words.at(9),words.at(10),words.at(11),words.at(12),words.at(13),words.at(14),words.at(15),words.at(16),words.at(2),species_genus_id);

    //         //   qDebug()<<words.at(16);
    //        // qDebug()<<i;i++;
    //    //    qDebug()<<query.exec(x);
    //       // qDebug()<<query.lastError().text();
    //        }

    //    }

    //    f.close();
    //    fillTableView();

}
void MainWindow::setupModel()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("herbarium.sqlite3");
    if (!db.open()) {
        qDebug()<<db.lastError().text();
    }
    else{
        qDebug()<<"ok!";
    }
}
void MainWindow::fillTableView()
{
    QSqlQuery queryMain;
    // ui->tableView->clearSpans();
    m_csvModel = new QSqlTableModel(this);
    //queryMain.exec("CREATE VIEW tableview as SELECT * FROM divisio,classis,familia,genus,species,individual "
    //               "WHERE divisio.id=classis.divisio_id AND classis.id=familia.classis_id AND familia.id=genus.familia_id AND genus.id=species.genus_id AND species.id=individual.species_id");
    m_csvModel->setTable("tableview");
    m_csvModel->select();
    ui->tableView->setModel(m_csvModel);
    qDebug()<<queryMain.lastError();
}
MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::divisio(QString divisio_str)
{
    QSqlQuery divisio_query;


    if (!divisio_str.isEmpty())
    {

        QString divInsert="INSERT INTO divisio(id,name) VALUES('%1','%2')";
        QString divQuery= divInsert.arg(div_id)
                .arg(divisio_str);
        div_id++;
        divisio_query.exec(divQuery);

    }
    return div_id;
}

int MainWindow::classis(QString classis_str, int classis_div_id)
{
    if (!classis_str.isEmpty())
    {
        QSqlQuery classis_query;
        QString classisInsert="INSERT INTO classis(id,name,divisio_id) VALUES('%1','%2','%3')";
        QString classisInsertQuery= classisInsert.arg(class_id)
                .arg(classis_str)
                .arg(classis_div_id);
        class_id++;


        classis_query.exec(classisInsertQuery);

    }
    return class_id;
}

int MainWindow::familia(QString familia_str, int familia_classis_id)
{
    if (!familia_str.isEmpty())
    {
        QSqlQuery familia_query;
        QString familiaInsert="INSERT INTO familia(id,name,classis_id) VALUES('%1','%2','%3')";
        QString familiaInsertQuery= familiaInsert.arg(fam_id)
                .arg(familia_str)
                .arg(familia_classis_id);
        fam_id++;

        familia_query.exec(familiaInsertQuery);

    }
    return fam_id;
}

int MainWindow::genus(QString genus_str, int genus_familia_id)
{
    if (!genus_str.isEmpty())
    {
        QSqlQuery genus_query;
        QString genusInsert="INSERT INTO genus(id,name,familia_id) VALUES('%1','%2','%3')";
        QString genusInsertQuery= genusInsert.arg(gen_id)
                .arg(genus_str)
                .arg(genus_familia_id);
        gen_id++;
        genus_query.exec(genusInsertQuery);
    }
    return gen_id;
}

int MainWindow::species(QString lat_name, QString rus_name, QString other_names, int species_genus_id)
{
    if(!lat_name.isEmpty())
    {

        QSqlQuery species_query;
        QString speciesInsert="INSERT INTO species(id,genus_id,lat_name,rus_name,other_names) VALUES ('%1','%2','%3','%4','%5')";
        QString speciesInsertQuery= speciesInsert.arg(spec_id)
                .arg(species_genus_id)
                .arg(lat_name)
                .arg(rus_name)
                .arg(other_names);

        spec_id++ ;

        species_query.exec(speciesInsertQuery);


    }
    return spec_id;
}

int MainWindow::individual(QString collection_date,QString region,QString district,QString location, QString habitat, QString collectors, QString who_identified,QString note,QString catalogiong_date, QString box_no,int species_genus_id)
{
    if (!collection_date.isEmpty() || !region.isEmpty() || !district.isEmpty() || !location.isEmpty() || !habitat.isEmpty() || !collectors.isEmpty() || who_identified.isEmpty())
    {
        QSqlQuery individual_query;
        QString individualInsert="INSERT INTO individual(id,species_id,collection_date,region,district,location,habitat,collectors,who_identified,note,catalogiong_date,box_no) VALUES ('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12')";
        QString individualInsertQuery= individualInsert.arg(indi_id)
                .arg(species_genus_id)
                .arg(collection_date)
                .arg(region)
                .arg(district)
                .arg(location)
                .arg(habitat)
                .arg(collectors)
                .arg(who_identified)
                .arg(note)
                .arg(catalogiong_date)
                .arg(box_no);

        indi_id++;
        individual_query.exec(individualInsertQuery);
        individual_query;
    }
    return indi_id;
}

void MainWindow::on_pushButton_clicked()
{
    addition f1;
    f1.exec();


}

