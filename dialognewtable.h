#ifndef DIALOGNEWTABLE_H
#define DIALOGNEWTABLE_H

#include <QDialog>

namespace Ui {
class DialogNewTable;
}

class DialogNewTable : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewTable(QWidget *parent = nullptr);
    ~DialogNewTable();

private:
    Ui::DialogNewTable *ui;

public slots:
    void on_buttonAdd_clicked();
    void on_buttonDel_clicked();
    void closeEvent(QCloseEvent *e);

signals:
    void ExitWin();
};

#endif // DIALOGNEWTABLE_H
