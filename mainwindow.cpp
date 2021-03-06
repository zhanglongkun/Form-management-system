/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#if QT_CONFIG(printpreviewdialog)
#include <QPrintPreviewDialog>
#endif
#endif
#endif

#include "mainwindow.h"
#include "mysqlit.h"


MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
{
//    mySqlitDB.testInit();

    setupUi(this);

    sampleSizes << 32 << 24 << 16 << 14 << 12 << 8 << 4 << 2 << 1;
    markedCount = 0;
    tableInit();

//    setupFontTreeAll();
    setupFontTreeRecently();

    connect(quitAction, &QAction::triggered,
            qApp, &QApplication::quit);
    connect(fontTree, &QTreeWidget::currentItemChanged,
            this, &MainWindow::showFont);
    connect(fontTree, &QTreeWidget::itemChanged,
            this, &MainWindow::updateStyles);

    connect(tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)),
            tableWidget, SLOT(resizeRowsToContents()));


    fontTree->topLevelItem(0)->setSelected(true);
    showFont(fontTree->topLevelItem(0));
}

void MainWindow::setupFontTreeAll()
{
    checkBoxAll->setChecked(true);
    checkBoxRecently->setChecked(false);

    fontTree->clear();

    QStringList timeGroup;
    mySqlitDB.sqlitGetTimeGroup(timeGroup);

    if (timeGroup.isEmpty()) {
        QTreeWidgetItem *familyItem = new QTreeWidgetItem(fontTree);
        familyItem->setText(0, "无数据");
        familyItem->setFlags(familyItem->flags() | Qt::ItemIsAutoTristate);
    } else {
        for (const QString &family : timeGroup) {
            QTreeWidgetItem *familyItem = new QTreeWidgetItem(fontTree);
            familyItem->setText(0, family);
          //  familyItem->setCheckState(0, Qt::Unchecked);
            familyItem->setFlags(familyItem->flags() | Qt::ItemIsAutoTristate);

            QVector<mysqlit::stru_table> tableList;
            mySqlitDB.sqlitGetTableForMonth(family, tableList);
            for (const mysqlit::stru_table &table : tableList) {
                QTreeWidgetItem *styleItem = new QTreeWidgetItem(familyItem);
                styleItem->setText(0, table.name);
                styleItem->setText(1, table.id);
                styleItem->setText(2, table.trancetion_time);
                if (0 == table.type) {
                    styleItem->setText(3, "其他");
                } else if ( 1 == table.type) {
                    styleItem->setText(3, "出货");
                } else {
                    styleItem->setText(3, "进货");
                }
               // styleItem->setCheckState(0, Qt::Unchecked);
            }
        }
    }
    qDebug()<< "count " << timeGroup.count();
}

void MainWindow::setupFontTreeRecently()
{
    checkBoxAll->setChecked(false);
    checkBoxRecently->setChecked(true);

    qDebug() << __LINE__;
    fontTree->clear();

    QTreeWidgetItem *familyItem = new QTreeWidgetItem(fontTree);
    familyItem->setText(0, "最近");
  //  familyItem->setCheckState(0, Qt::Unchecked);
    familyItem->setFlags(familyItem->flags() | Qt::ItemIsAutoTristate);

    QVector<mysqlit::stru_table> tableList;
    mySqlitDB.sqlitGetTableRecently(tableList);
    if (tableList.isEmpty()) {
        QTreeWidgetItem *styleItem = new QTreeWidgetItem(familyItem);
        styleItem->setText(0, "无数据");
    } else {
        for (const mysqlit::stru_table &table : tableList) {
            QTreeWidgetItem *styleItem = new QTreeWidgetItem(familyItem);
            styleItem->setText(0, table.name);
            styleItem->setText(1, table.id);
            styleItem->setText(2, table.trancetion_time);
            if (0 == table.type) {
                styleItem->setText(3, "其他");
            } else if ( 1 == table.type) {
                styleItem->setText(3, "出货");
            } else {
                styleItem->setText(3, "进货");
            }
           // styleItem->setCheckState(0, Qt::Unchecked);
        }
    }
}

void MainWindow::on_clearAction_triggered()
{
    const QList<QTreeWidgetItem *> items = fontTree->selectedItems();
    for (QTreeWidgetItem *item : items)
        item->setSelected(false);
    fontTree->currentItem()->setSelected(true);
}

void MainWindow::on_markAction_triggered()
{
    markUnmarkFonts(Qt::Checked);
}

void MainWindow::on_unmarkAction_triggered()
{
    markUnmarkFonts(Qt::Unchecked);
}

void MainWindow::markUnmarkFonts(Qt::CheckState state)
{
    const QList<QTreeWidgetItem *> items = fontTree->selectedItems();
    for (QTreeWidgetItem *item : items) {
        if (item->checkState(0) != state)
            item->setCheckState(0, state);
    }
}

void MainWindow::showFont(QTreeWidgetItem *item)
{
    if (!item)
        return;

    QString family;
    QString style;
    int weight;
    bool italic;

    if (item->parent()) {
        family = item->parent()->text(0);
        style = item->text(0);
        weight = item->data(0, Qt::UserRole).toInt();
        italic = item->data(0, Qt::UserRole + 1).toBool();

        showTableData(item, 0);
    } else {
//        showTableData(item, 1);
    }
}

void MainWindow::updateStyles(QTreeWidgetItem *item, int column)
{
    if (!item || column != 0)
        return;

    Qt::CheckState state = item->checkState(0);
    QTreeWidgetItem *parent = item->parent();

    if (parent) {
        // Only count style items.
        if (state == Qt::Checked)
            ++markedCount;
        else
            --markedCount;
    }

    printAction->setEnabled(markedCount > 0);
    printPreviewAction->setEnabled(markedCount > 0);
}

QMap<QString, StyleItems> MainWindow::currentPageMap()
{
    QMap<QString, StyleItems> pageMap;

    for (int row = 0; row < fontTree->topLevelItemCount(); ++row) {
        QTreeWidgetItem *familyItem = fontTree->topLevelItem(row);
        QString family;

        if (familyItem->checkState(0) == Qt::Checked) {
            family = familyItem->text(0);
            pageMap[family] = StyleItems();
        }

        for (int childRow = 0; childRow < familyItem->childCount(); ++childRow) {
            QTreeWidgetItem *styleItem = familyItem->child(childRow);
            if (styleItem->checkState(0) == Qt::Checked)
                pageMap[family].append(styleItem);
        }
    }

    return pageMap;
}

void MainWindow::on_printAction_triggered()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    pageMap = currentPageMap();

    if (pageMap.count() == 0)
        return;

    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() != QDialog::Accepted)
        return;

    int from = printer.fromPage();
    int to = printer.toPage();
    if (from <= 0 && to <= 0)
        printer.setFromTo(1, pageMap.keys().count());

    printDocument(&printer);
#endif
}

void MainWindow::printDocument(QPrinter *printer)
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    printer->setFromTo(1, pageMap.count());

    QProgressDialog progress(tr("Preparing font samples..."), tr("&Cancel"),
                             0, pageMap.count(), this);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setWindowTitle(tr("Font Sampler"));
    progress.setMinimum(printer->fromPage() - 1);
    progress.setMaximum(printer->toPage());

    QPainter painter;
    painter.begin(printer);
    bool firstPage = true;

    for (int page = printer->fromPage(); page <= printer->toPage(); ++page) {

        if (!firstPage)
            printer->newPage();

        qApp->processEvents();
        if (progress.wasCanceled())
            break;

        printPage(page - 1, &painter, printer);
        progress.setValue(page);
        firstPage = false;
    }

    painter.end();
#endif
}

void MainWindow::on_printPreviewAction_triggered()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printpreviewdialog)
    pageMap = currentPageMap();

    if (pageMap.count() == 0)
        return;

    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, &QPrintPreviewDialog::paintRequested,
            this, &MainWindow::printDocument);
    preview.exec();
#endif
}

void MainWindow::printPage(int index, QPainter *painter, QPrinter *printer)
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    const QString family = (pageMap.begin() + index).key();
    const StyleItems items = pageMap.value(family);

    // Find the dimensions of the text on each page.
    qreal width = 0.0;
    qreal height = 0.0;
    for (const QTreeWidgetItem *item : items) {
        QString style = item->text(0);
        int weight = item->data(0, Qt::UserRole).toInt();
        bool italic = item->data(0, Qt::UserRole + 1).toBool();

        // Calculate the maximum width and total height of the text.
        for (int size : qAsConst(sampleSizes)) {
            QFont font(family, size, weight, italic);
            font.setStyleName(style);
            font = QFont(font, painter->device());
            QFontMetricsF fontMetrics(font);
            QRectF rect = fontMetrics.boundingRect(
            QString("%1 %2").arg(family).arg(style));
            width = qMax(rect.width(), width);
            height += rect.height();
        }
    }

    qreal xScale = printer->pageRect().width() / width;
    qreal yScale = printer->pageRect().height() / height;
    qreal scale = qMin(xScale, yScale);

    qreal remainingHeight = printer->pageRect().height()/scale - height;
    qreal spaceHeight = (remainingHeight / 4.0) / (items.count() + 1);
    qreal interLineHeight = (remainingHeight / 4.0) / (sampleSizes.count() * items.count());

    painter->save();
    painter->translate(printer->pageRect().width() / 2.0, printer->pageRect().height() / 2.0);
    painter->scale(scale, scale);
    painter->setBrush(QBrush(Qt::black));

    qreal x = -width / 2.0;
    qreal y = -height / 2.0 - remainingHeight / 4.0 + spaceHeight;

    for (const QTreeWidgetItem *item : items) {
        QString style = item->text(0);
        int weight = item->data(0, Qt::UserRole).toInt();
        bool italic = item->data(0, Qt::UserRole + 1).toBool();

        // Draw each line of text.
        for (int size : qAsConst(sampleSizes)) {
            QFont font(family, size, weight, italic);
            font.setStyleName(style);
            font = QFont(font, painter->device());
            QFontMetricsF fontMetrics(font);
            QRectF rect = fontMetrics.boundingRect(QString("%1 %2").arg(
                          font.family()).arg(style));
            y += rect.height();
            painter->setFont(font);
            painter->drawText(QPointF(x, y), QString("%1 %2").arg(family).arg(style));
            y += interLineHeight;
        }
        y += spaceHeight;
    }

    painter->restore();
#endif
}

void MainWindow::tableInit()
{
    fontTree->setColumnCount(4);
    fontTree->setHeaderLabels({ tr("组"), tr("id"), tr("日期"), tr("type") });
    fontTree->hideColumn(1);

    QStringList headerList;

    headerList << "id" << "table_id"<< "商品编号" << "名称" << "规格" << "单位" << "数量" << "单价" << "金额" << "备注";

    tableWidget->setColumnCount(10);
    tableWidget->setRowCount(0);
//    tableWidget->hideColumn(0);
//    tableWidget->hideColumn(1);

    tableWidget->setHorizontalHeaderLabels(headerList);
    tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{"
                                                   "background-color: #b4b8bb;"
                                                   "font: 12pt}");

    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setHighlightSections(false);
    tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableWidget->resizeColumnsToContents();
    tableWidget->resizeRowsToContents();
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setAlternatingRowColors(true);
    tableWidget->verticalHeader()->setDefaultSectionSize(30);
    tableWidget->horizontalHeader()->setMinimumHeight(40);
    tableWidget->verticalHeader()->setVisible(true);
    tableWidget->horizontalHeader()->setVisible(true);

    dateEdit->setCalendarPopup(true);
    dateEdit->setFocusPolicy(Qt::NoFocus);

    lineEditSum->setEnabled(false);
    QPalette palette;
    palette.setColor(QPalette::Text,Qt::red);
    lineEditSum->setPalette(palette);

    checkBoxRecently->setChecked(true);
    checkBoxAll->setChecked(false);
}

//递归删除节点
void MainWindow::removeItem(QTreeWidgetItem *item)
{
    int count = item->childCount();
    if (count == 0) {
        //没有子节点，直接删除
        delete item;
        return;
    }

    for (int i = 0; i < count; i++) {
        QTreeWidgetItem *childItem = item->child(0);//删除子节点
        removeItem(childItem);
    }
    delete item;//最后将自己删除

}

//删除选中的节点及子节点
void  MainWindow::removeSelectedItems(QTreeWidget *treeWidget)
{
    QList<QTreeWidgetItem*> items = treeWidget->selectedItems();

    for (int i = 0; i < items.size(); ++i) {
        removeItem(items[i]);
    }
}

void MainWindow::removeAllItems(QTreeWidget *treeWidget)
{
    treeWidget->clear();
}

void MainWindow::showTableData(QTreeWidgetItem *item, int type)
{
    QVector<mysqlit::stru_table_data> list;

    if (1 == type) {
        mySqlitDB.sqlitGetTableData(item->text(0), list);
        lineEditName->setText(item->text(0));
        comboBox->setCurrentIndex(3);
        dateEdit->setDisplayFormat("yyyy-MM");
        dateEdit->setDate(QDate::fromString(item->text(0),"yyyy-MM"));
    } else {
        mySqlitDB.sqlitGetTableData(item->text(1).toInt(), list);
        lineEditName->setText(item->text(0));
        dateEdit->setDisplayFormat("yyyy-MM-dd");
        dateEdit->setDate(QDate::fromString(item->text(2),"yyyy-MM-dd"));
        if (1 == item->text(3).toInt()) {
            comboBox->setCurrentIndex(1);
        } else if (2 == item->text(3).toInt()) {
            comboBox->setCurrentIndex(2);
        } else if (2 == item->text(3).toInt()) {
            comboBox->setCurrentIndex(3);
        } else {
            comboBox->setCurrentIndex(0);
        }
    }

    for(int i = tableWidget->rowCount() - 1; i >= 0; i--) {
        tableWidget->removeRow(i);
    }

    int i = 0;
    float sum = 0;
    for (const mysqlit::stru_table_data &data : list) {
        tableWidget->insertRow(tableWidget->rowCount());
        QTableWidgetItem *item0 = new QTableWidgetItem(data.id);
        tableWidget->setItem(i, 0, item0);

        QTableWidgetItem *item1 = new QTableWidgetItem(data.table_id);
        tableWidget->setItem(i, 1, item1);

        QTableWidgetItem *item2 = new QTableWidgetItem(data.name_number);
        tableWidget->setItem(i, 2, item2);

        QTableWidgetItem *item3 = new QTableWidgetItem(data.name);
        tableWidget->setItem(i, 3, item3);

        QTableWidgetItem *item4 = new QTableWidgetItem(data.specification);
        tableWidget->setItem(i, 4, item4);

        QTableWidgetItem *item5 = new QTableWidgetItem(data.unit);
        tableWidget->setItem(i, 5, item5);

        QTableWidgetItem *item6 = new QTableWidgetItem(data.number);
        tableWidget->setItem(i, 6, item6);

        QTableWidgetItem *item7 = new QTableWidgetItem(data.price);
        tableWidget->setItem(i, 7, item7);

        QTableWidgetItem *item8 = new QTableWidgetItem(data.price_sum);
        tableWidget->setItem(i, 8, item8);
        sum += data.price_sum.toFloat();

        QTableWidgetItem *item9 = new QTableWidgetItem(data.comment);
        tableWidget->setItem(i, 9, item9);

        i++;
    }

    lineEditSum->setText(QString("%1").arg(sum));
}

void MainWindow::showTableList(QString group)
{
    QVector<mysqlit::stru_table> tableList;

    mySqlitDB.sqlitGetTableListForGroup(group, tableList);
}

void MainWindow::showCurrentTable()
{
    QTreeWidgetItem *item = fontTree->currentItem();
    showTableData(item, 0);
}

void MainWindow::on_buttonAdd_clicked()
{
    tableWidget->insertRow(tableWidget->rowCount());
}

void MainWindow::on_checkBoxAll_clicked()
{
    if (checkBoxAll->isChecked()) {
        checkBoxRecently->setChecked(false);
        setupFontTreeAll();
    } else {
        checkBoxRecently->setChecked(true);
        setupFontTreeRecently();
    }
}

void MainWindow::on_checkBoxRecently_clicked()
{
    if (checkBoxRecently->isChecked()) {
        checkBoxAll->setChecked(false);
        setupFontTreeRecently();
    } else {
        checkBoxAll->setChecked(true);
        setupFontTreeAll();
    }
}

void MainWindow::on_buttonDel_clicked()
{
    QMap<int, int> list;
    QList<QTableWidgetSelectionRange > rangList = tableWidget->selectedRanges();

    for (int i = 0; i < rangList.size(); i++) {
        int topRow = rangList[i].topRow();
        int bottomRow = rangList[i].bottomRow();
        for (int j = topRow; j <= bottomRow; j++) {
            list.insert(j, j);
        }
    }
    
    float sum = lineEditSum->text().toFloat();

    for (auto item = --list.end(); item != --list.begin(); item--) {
        QString id = tableWidget->item(item.value(), 0)->text();
        mySqlitDB.sqlitDeleteData(id);
        sum -= tableWidget->item(item.value(), 8)->text().toFloat();
        lineEditSum->setText(QString("%1").arg(sum));
        qDebug() << "delete line = " << item.value();
        tableWidget->removeRow(item.value());
    }

    
#if 0
    /*获取选中的列表里的所有条目*/
    QList<QTableWidgetItem*> list = tableWidget->selectedItems();
    
    if (list.count() <= 0) {
        QMessageBox::warning(this, tr("提示"),
        tr("左击选择行，按住ctrl左击选择多行"),
        QMessageBox::Ok);
        return;
    }

    /*从列表中依次移除条目*/
    for(int i = 0; i < list.count(); i++)
    {
        /*获取条目的行号*/
        int row=tableWidget->row(list.at(i));
        qDebug()<<"即将卸载的行号:"<<row;
        delete list.at(i); //彻底删除条目

        //因为上面的循环是以条目数量为准，所以卸载行号只需要卸载一行即可
        if(row != -1) {
            tableWidget->removeRow(row);
        }
    }
#endif


#if 0
    int rowIndex = tableWidget->currentRow();
     if (rowIndex != -1) {
         tableWidget->removeRow(rowIndex);
     }
#endif
//    QTableWidgetItem *curItem = tableWidget->currentItem();
//    if(curItem != NULL) {
//         //当前条目非空
//        if(curItem->isSelected()) {
//            //当前条目被选中，那么确定是删除该行
//            tableWidget->removeRow(curItem->row());
//        }
//    }
}

void MainWindow::on_buttonSave_clicked()
{
    float sum = 0;
    mysqlit::stru_table_data data;

    for (int i = 0; i < tableWidget->rowCount(); i++) {
        mysqlit::stru_table_data data;

         data.table_id = fontTree->currentItem()->text(1);
        if (tableWidget->item(i, 0) != 0) {
            data.id = tableWidget->item(i, 0)->text();
        }

        if (tableWidget->item(i, 1) != 0) {
//            data.table_id = tableWidget->item(i, 1)->text();
        }

        if (tableWidget->item(i, 2) != 0) {
            data.name_number =  tableWidget->item(i, 2)->text();
        }

        if (tableWidget->item(i, 3) != 0) {
            data.name =  tableWidget->item(i, 3)->text();
        }

        if (tableWidget->item(i, 4) != 0) {
            data.specification =  tableWidget->item(i, 4)->text();
        }

        if (tableWidget->item(i, 5) != 0) {
            data.unit =  tableWidget->item(i, 5)->text();
        }

        if (tableWidget->item(i, 6) != 0) {
            data.number =  tableWidget->item(i, 6)->text();
        }

        if (tableWidget->item(i, 7) != 0) {
            data.price =  tableWidget->item(i, 7)->text();
        }

        if (tableWidget->item(i, 9) != 0) {
            data.comment =  tableWidget->item(i, 9)->text();
        }

        sum = data.number.toFloat() * data.price.toFloat();
        data.price_sum = QString("%1").arg(sum);

        data.time_group = dateEdit->date().toString(("yyyy-MM"));


        if (tableWidget->item(i, 0) != 0) {
            mySqlitDB.sqlitUpdateData(data);
        } else {
            mySqlitDB.sqlitInsertData(data);
        }
    }

    showCurrentTable();
}

void MainWindow::on_actionNewTable_triggered()
{
    newWindow = new DialogNewTable();
    newWindow->show();

    connect(newWindow, SIGNAL(ExitWin()), this, SLOT(setupFontTreeRecently()));
}
