#include "dialognewtable.h"
#include "ui_dialognewtable.h"
#include <QtWidgets/QMessageBox>
#include <QtDebug>

#include "mysqlit.h"
#include "mainwindow.h"

DialogNewTable::DialogNewTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewTable)
{
    ui->setupUi(this);

    QStringList headerList;

    headerList << "商品编号" << "名称" << "规格" << "单位" << "数量" << "单价" << "备注";

    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->horizontalHeader()->setMinimumHeight(30);

    ui->tableWidget->setHorizontalHeaderLabels(headerList);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{"
                                                   "background-color: #b4b8bb;"
                                                   "font: 12pt}");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);

    ui->dateEdit->setCalendarPopup(true);
//    ui->dateEdit->setFocusPolicy(Qt::NoFocus);
    ui->dateEdit->setDate(QDateTime::currentDateTime().date());
    ui->dateEdit->setDisplayFormat("yyyy-MM-dd");
}

DialogNewTable::~DialogNewTable()
{
    delete ui;
}

void DialogNewTable::on_buttonAdd_clicked()
{
    qDebug() << "1111111";
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
}

void DialogNewTable::on_buttonDel_clicked()
{
    QMap<int, int> list;
    QList<QTableWidgetSelectionRange > rangList = ui->tableWidget->selectedRanges();

    for (int i = 0; i < rangList.size(); i++) {
        int topRow = rangList[i].topRow();
        int bottomRow = rangList[i].bottomRow();
        for (int j = topRow; j <= bottomRow; j++) {
            list.insert(j, j);
        }
    }

    for (auto item = --list.end(); item != --list.begin(); item--) {
        qDebug() << "delete line = " << item.value();
        ui->tableWidget->removeRow(item.value());
    }
}

void DialogNewTable::closeEvent(QCloseEvent *e)
{
    mysqlit::stru_table table;
    QMessageBox box(QMessageBox::Warning, "提示"," 是否保存？");
    box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    box.setButtonText(QMessageBox::Ok, QString("保 存"));
    box.setButtonText(QMessageBox::Cancel, QString("不保存"));
    int button = box.exec();
    if (button == QMessageBox::Ok) {
        table.name = ui->lineEditName->text();
        table.trancetion_time = ui->dateEdit->date().toString("yyyy-MM-dd");
        table.time_group = ui->dateEdit->date().toString(("yyyy-MM"));
        table.create_time = QDateTime::currentDateTime().toTime_t();   //获取当前时间
        table.type = ui->comboBox->currentIndex();
        MainWindow *mainWin = (MainWindow*) parentWidget();
        mainWin->mySqlitDB.sqlitSetTableInfo(table);

        qDebug() << "time " << table.create_time<< "type" << table.type;

        int id;
        mainWin->mySqlitDB.sqlitGetNewestTableID(id);
        qDebug() << "id = " << id;

        float sum  = 0;
        mysqlit::stru_table_data data;
        for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
            data.table_id = QString("%1").arg(id);
            data.time_group = table.time_group;

            if (ui->tableWidget->item(i, 0) != 0) {
                data.name_number =  ui->tableWidget->item(i, 0)->text();
            }

            if (ui->tableWidget->item(i, 1) != 0) {
                data.name =  ui->tableWidget->item(i, 1)->text();
            }

            if (ui->tableWidget->item(i, 2) != 0) {
                data.specification =  ui->tableWidget->item(i, 2)->text();
            }

            if (ui->tableWidget->item(i, 3) != 0) {
                data.unit =  ui->tableWidget->item(i, 3)->text();
            }

            if (ui->tableWidget->item(i, 4) != 0) {
                data.number =  ui->tableWidget->item(i, 4)->text();
            }

            if (ui->tableWidget->item(i, 5) != 0) {
                data.price =  ui->tableWidget->item(i, 5)->text();
            }

            if (ui->tableWidget->item(i, 6) != 0) {
                data.comment =  ui->tableWidget->item(i, 6)->text();
            }

            sum = data.number.toFloat() * data.price.toFloat();
            data.price_sum = QString("%1").arg(sum);

            mainWin->mySqlitDB.sqlitInsertData(data);
            emit ExitWin();

        }
    } else {

    }

}

