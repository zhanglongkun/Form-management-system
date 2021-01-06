#include "dialognewtable.h"
#include "ui_dialognewtable.h"
#include <QtWidgets/QMessageBox>
#include <QtDebug>

DialogNewTable::DialogNewTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewTable)
{
    ui->setupUi(this);

    QStringList headerList;

    headerList << "商品编号" << "名称" << "规格" << "单位" << "数量" << "单价" << "备注";

    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setRowCount(0);

    ui->tableWidget->setHorizontalHeaderLabels(headerList);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{"
                                                   "background-color: #b4b8bb;"
                                                   "font: 12pt}");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->dateEdit->setCalendarPopup(true);
//    ui->dateEdit->setFocusPolicy(Qt::NoFocus);
    ui->dateEdit->setDate(QDateTime::currentDateTime().date());
    ui->dateEdit->setDisplayFormat("yyyy-MM-dd");

    QPushButton *pBtn = new QPushButton ();
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setCellWidget(0, 0, pBtn);
    ui->tableWidget->setSpan(0, 0, 3, 1);




}

DialogNewTable::~DialogNewTable()
{
    delete ui;
}

void DialogNewTable::on_ok_clicked()
{
    qDebug() << "1111111";
}

void DialogNewTable::on_cancel_clicked()
{

}

void DialogNewTable::closeEvent(QCloseEvent *e)
{
    QMessageBox box(QMessageBox::Warning, "提示"," 是否保存？");
    box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    box.setButtonText(QMessageBox::Ok, QString("保 存"));
    box.setButtonText(QMessageBox::Cancel, QString("不保存"));
    int button = box.exec();
    if (button == QMessageBox::Ok) {

    } else {

    }
}

