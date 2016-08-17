#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_shop_clicked();

    void on_attackIncrease_clicked();

    void on_defenseIncrease_clicked();

    void on_healthIncrease_clicked();

    void on_restore_clicked();

    void on_forest_clicked();

    void updateStats();

    void endTurn();

    void off();

    void on();

    void on_save_clicked();

    void on_load_clicked();

    void createMonster();

    void on_dragon_clicked();

    void gameOver();

    void monsterKilled();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
