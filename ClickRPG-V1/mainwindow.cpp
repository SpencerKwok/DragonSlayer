#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QString>
#include <QFont>
#include <ctime>
#include <fstream>
#include <QTextStream>
#include <cstdlib>
#include <iostream>
#include <QFile>
using namespace std;

class Character{
public:
    Character(string n,int a, int b, int c, int d, int e){
        setGold(a);
        setMaxHealth(b);
        setHealth(c);
        setAttack(d);
        setDefense(e);
        setName(n);
    }
    int getGold(){
        return gold;
    }
    int getMaxHealth(){
        return maxHealth;
    }
    int getHealth(){
        return health;
    }
    int getAttack(){
        return attack;
    }
    int getDefense(){
        return defense;
    }
    string getName(){
        return name;
    }

    void setGold(int x){
        gold = x;
    }
    void setMaxHealth(int x){
        maxHealth = x;
    }
    void setHealth(int x){
        health = x;
    }
    void setAttack(int x){
        attack = x;
    }
    void setDefense(int x){
        defense = x;
    }
    void setName (string x){
        name = x;
    }

private:
    int gold;
    int maxHealth;
    int health;
    int attack;
    int defense;
    string name;
};

class Monsters{
public:
    Monsters(){

    }
    Monsters(int x, int y, QString z, int a, QString p){
        setA(x);
        setH(y);
        setName(z);
        setValue(a);
        setPic(p);
    }
    int getA(){
        return a;
    }
    int getH(){
        return h;
    }
    QString getName(){
        return name;
    }
    int getValue(){
        return value;
    }
    QString getPic(){
        return pic;
    }
    void setA(int x){
        a = x;
    }
    void setH(int x){
        h = x;
    }
    void setName(QString x){
        name = x;
    }
    void setValue(int x){
        value = x;
    }
    void setPic(QString x){
        pic = x;
    }

private:
    int h;
    //health of monster
    int a;
    //attack of monster
    QString name;
    //name of monster
    int value;
    //value of monster
    QString pic;
};

Character *myCharacter = new Character("no name",1000,10,10,1,1);
Monsters *stats = new Monsters(10,8,"olf", 20,"13re");
bool fightingDragon = false;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setStyleSheet("background-color: black;");
    ui->setupUi(this);
    MainWindow::off();
    ui->restore->setVisible(false);
    QFont f( "Arial", 8, QFont::Bold);
    ui->story->setFont(f);
    ui->welcome->setFont(f);
    ui->goldAmount->setFont(f);
    ui->healthAmount->setFont(f);
    ui->attackAmount->setFont(f);
    ui->defenseAmount->setFont(f);
    ui->notifications->setFont(f);
    ui->player->setFont(f);
    ui->welcome->setAlignment(Qt::AlignCenter);
    ui->goldAmount->setAlignment(Qt::AlignCenter);
    ui->healthAmount->setAlignment(Qt::AlignCenter);
    ui->attackAmount->setAlignment(Qt::AlignCenter);
    ui->defenseAmount->setAlignment(Qt::AlignCenter);
    ui->notifications->setAlignment(Qt::AlignCenter);
    ui->player->setAlignment(Qt::AlignCenter);
    ui->player->setText("Enter name\nabove to\nsave/load \nfiles");
    ui->welcome->setText("Welcome to my RPG game!\nAs a lone adventurer, your mission is to gain fame and glory by defeating a powerful dragon.\nHowever, if you were to fight the dragon now, you'd surely fail.\nYou must start off by training and gaining more strength.\nClick the icons in the top left corner to upgrade and train your character.");
    //ofstream myFile("player.txt");
    //myFile << myCharacter->getGold() << " " << myCharacter->getMaxHealth() << " " << myCharacter->getHealth() << " " << myCharacter->getAttack() << " " << myCharacter->getDefense() << endl;
    MainWindow::updateStats();
}
MainWindow::~MainWindow()
{
    delete ui;    
}

void MainWindow::on_shop_clicked()
{
    fightingDragon = false;
    ui->story->setText("Welcome to the shop!\nCost of upgrades is as follows:\nIncrease attack by 1 = 400 gold\nIncrease defense by 1 = 300 gold\nIncrease health by 1 = 300 gold\nRestoring health = 100 gold");
    MainWindow::on();
    QPixmap image = QPixmap (":/pics/shop.png");
    QPixmap newImage = image.scaled(QSize(400,300),  Qt::KeepAspectRatio);
    ui->welcome->setAlignment(Qt::AlignCenter);
    ui->welcome->setPixmap(newImage);
    ui->attackIncrease->setText("Upgrade Attack");
    ui->defenseIncrease->setText("Upgrade Defense");
    ui->healthIncrease->setText("Upgrade Health");
    ui->restore->setVisible(true);
}

void MainWindow::on_attackIncrease_clicked()
{
    if (ui->defenseIncrease->text() == "Upgrade Defense"){
        try{
            if ((myCharacter->getGold()-400)< 0){
                throw 99;
            }
            ui->notifications->clear();
            myCharacter->setGold(myCharacter->getGold() - 400);
            myCharacter->setAttack(myCharacter->getAttack() + 1);
        } catch(...){
            ui->notifications->setText("You don't have\nenough gold!");
        }
    } else if (ui->attackIncrease->text() == "Attack x 2"){
        myCharacter->setAttack(myCharacter->getAttack() * 2);
        MainWindow::endTurn();
        myCharacter->setAttack(myCharacter->getAttack() / 2);
        if (stats->getA() > myCharacter->getDefense()){
            ui->notifications->setText("You dealt " + QString::number(myCharacter->getAttack()) + " damage!\nYou took " + QString::number(stats->getA()-myCharacter->getDefense()) + " damage...");
        } else {
            ui->notifications->setText("You dealt " + QString::number(myCharacter->getAttack()) + " damage!\nYou took no damage!");
        }
        ui->healthAmount->setText(QString::number(myCharacter->getHealth()));
        if (myCharacter->getHealth()<=0){
            MainWindow::gameOver();
        }
        if (stats->getH() <= 0 && !fightingDragon){
            MainWindow::monsterKilled();
        }
        if (stats->getH() <= 0 && fightingDragon){
            ui->notifications->clear();
            ui->story->setText("You beat the game! Congratulations!");
            ui->shop->setVisible(false);
            ui->forest->setVisible(false);
            ui->dragon->setVisible(false);
            MainWindow::off();
        }
    } else {
        MainWindow::endTurn();
        if (stats->getA() > myCharacter->getDefense()){
            ui->notifications->setText("You dealt " + QString::number(myCharacter->getAttack()) + " damage!\nYou took " + QString::number(stats->getA()-myCharacter->getDefense()) + " damage...");
        } else {
            ui->notifications->setText("You dealt " + QString::number(myCharacter->getAttack()) + " damage!\nYou took no damage!");
        }
        ui->healthAmount->setText(QString::number(myCharacter->getHealth()));
        if (myCharacter->getHealth()<=0){
            MainWindow::gameOver();
        }
        if (stats->getH() <= 0 && !fightingDragon){
            MainWindow::monsterKilled();
        }
        if (stats->getH() <= 0 && fightingDragon){
            ui->notifications->clear();
            ui->story->setText("You beat the game! Congratulations!");
            ui->shop->setVisible(false);
            ui->forest->setVisible(false);
            ui->dragon->setVisible(false);
            MainWindow::off();
        }
    }
    MainWindow::updateStats();
}

void MainWindow::on_defenseIncrease_clicked()
{
    if (ui->defenseIncrease->text() == "Upgrade Defense"){
        try{
            if ((myCharacter->getGold()-300)< 0){
                throw 99;
            }
            ui->notifications->clear();
            myCharacter->setGold(myCharacter->getGold()-300);
            myCharacter->setDefense(myCharacter->getDefense()+1);
        } catch(...){
            ui->notifications->setText("You don't have\nenough gold!");
        }
    }
    else {
        ui->attackIncrease->setText("Attack x 2");
        myCharacter->setDefense(myCharacter->getDefense() + 1);
        int saveAttack = myCharacter->getAttack();
        myCharacter->setAttack(0);
        MainWindow::endTurn();
        myCharacter->setDefense(myCharacter->getDefense() - 1);
        myCharacter->setAttack(saveAttack);
        ui->healthAmount->setText(QString::number(myCharacter->getHealth()));
        if (myCharacter->getHealth()<=0){
            MainWindow::gameOver();
        }
    }
    MainWindow::updateStats();
}

void MainWindow::on_healthIncrease_clicked()
{
    if (ui->defenseIncrease->text() == "Upgrade Defense"){
        try{
            if ((myCharacter->getGold()-300)< 0){
                throw 99;
            }
            ui->notifications->clear();
            myCharacter->setGold(myCharacter->getGold()-300);
            myCharacter->setHealth(myCharacter->getHealth() + 1);
            myCharacter->setMaxHealth(myCharacter->getMaxHealth()+1);
            MainWindow::updateStats();
        } catch(...){
            ui->notifications->setText("You don't have\nenough gold!");
        }
    }
    else {
        try{
            if ((myCharacter->getGold()-100)< 0){
                throw 99;
            }
            ui->welcome->clear();
            ui->story->setText("You got away! You lost 100 coins");
            myCharacter->setGold(myCharacter->getGold()-100);
            MainWindow::updateStats();
        } catch(...){
            ui->notifications->setText("You don't have\nenough gold!");
        }
    }
    MainWindow::updateStats();
}

void MainWindow::on_restore_clicked()
{
    try{
        if ((myCharacter->getGold()-100)< 0){
            throw 99;
        }
        ui->notifications->clear();
        myCharacter->setGold(myCharacter->getGold()-100);
        myCharacter->setHealth(myCharacter->getMaxHealth());
    } catch(...){
        ui->notifications->setText("You don't have\nenough gold!");
    }
    MainWindow::updateStats();
}

void MainWindow::on_forest_clicked()
{
    fightingDragon = false;
    if (ui->attackIncrease->text() == "Attack" && stats->getH() > 0){
        try{
            if(myCharacter->getGold()-100 <= 0){
                throw 99;
            }
            myCharacter->setGold(myCharacter->getGold()-100);
            MainWindow::updateStats();
        } catch(...){
            ui->notifications->setText("Not enough gold\nto run away!");
        }
    }
    ui->notifications->clear();
    MainWindow::createMonster();
    if (!ui->defenseIncrease->isVisible()){
        MainWindow::on();
    }
    ui->restore->setVisible(false);
    ui->attackIncrease->setText("Attack");
    ui->defenseIncrease->setText("Defense");
    ui->healthIncrease->setText("Run Away (costs 100 gold)");
    ui->welcome->clear();
    QPixmap image = QPixmap (stats->getPic());
    QPixmap newImage = image.scaled(QSize(400,300),  Qt::KeepAspectRatio);
    ui->welcome->setAlignment(Qt::AlignCenter);
    ui->welcome->setPixmap(newImage);
    ui->story->setText("You encountered a " + stats->getName() + "!\nAttack Power: " + QString::number(stats->getA()) + "\nHealth: " + QString::number(stats->getH()));
}

void MainWindow::updateStats(){
    ui->goldAmount->setText(QString::number(myCharacter->getGold()));
    ui->healthAmount->setText(QString::number(myCharacter->getHealth()));
    ui->attackAmount->setText(QString::number(myCharacter->getAttack()));
    ui->defenseAmount->setText(QString::number(myCharacter->getDefense()));
}

void MainWindow::endTurn(){
    stats->setH(stats->getH()-myCharacter->getAttack());
    if (!(stats->getA() <= myCharacter->getDefense()) && stats->getH() > 0){
         myCharacter->setHealth(myCharacter->getHealth()-stats->getA()+myCharacter->getDefense());
    }
    if (stats->getH() <= 0){
        myCharacter->setGold(myCharacter->getGold()+stats->getValue());
    }
    ui->story->setText(stats->getName() + "\nAttack Power: " + QString::number(stats->getA()) + "\nHealth: " + QString::number(stats->getH()));
    ui->notifications->clear();
}

void MainWindow::off(){
    ui->attackIncrease->setVisible(false);
    ui->defenseIncrease->setVisible(false);
    ui->healthIncrease->setVisible(false);
}

void MainWindow::on(){
    ui->attackIncrease->setVisible(true);
    ui->defenseIncrease->setVisible(true);
    ui->healthIncrease->setVisible(true);
}

void MainWindow::createMonster(){
    srand(time(0));
    ifstream monsterFile("monsterList.txt");
    int id;
    string name;
    int attack;
    int health;
    int value;
    string pic;
    while (monsterFile >> id >> name >> attack >> health >> value >> pic){

    }
    int idMatch = 1+(rand()%id);
    monsterFile.close();
    monsterFile.open("monsterList.txt");
    bool found = false;
    while (monsterFile >> id >> name >> attack >> health >> value >> pic){
        if (id == idMatch){
            found = true;
        }
        if (found) break;
    }
    QString Qname = QString::fromStdString(name);
    QString Qpic = QString::fromStdString(pic);
    monsterFile.close();
    Monsters *m = new Monsters(attack,health,Qname, value, Qpic);
    stats = m;
}

void MainWindow::on_save_clicked()
{
    ui->notifications->clear();
    ifstream savedFile("player.txt");
    QString n = ui->playerName->text();
    bool found = false;
    string findName = "";
    int g;
    int mh;
    int h;
    int a;
    int d;
    int Ng = 0;
    int Nmh = 0;
    int Nh = 0;
    int Na = 0;
    int Nd = 0;
    try{
        while (savedFile >> findName >> g >> mh >> h >> a >> d){
            if (findName == n.toStdString()){
                found = true;
                Ng = g;
                Nmh = mh;
                Nh = h;
                Na = a;
                Nd = d;
            }
        }
        if (!found) {
            throw 99;
        }
        QFile f("player.txt");
        f.open(QIODevice::Text | QIODevice::ReadOnly);
        QString dataText = f.readAll();

        QRegularExpression re(n + QString::fromStdString(" ") + QString::number(Ng) + QString::fromStdString(" ") + QString::number(Nmh) + QString::fromStdString(" ") + QString::number(Nh) + QString::fromStdString(" ") + QString::number(Na) + QString::fromStdString(" ") + QString::number(Nd));
        QString replacementText(n + QString::fromStdString(" ") + QString::number(myCharacter->getGold()) + QString::fromStdString(" ") + QString::number(myCharacter->getMaxHealth()) + QString::fromStdString(" ") + QString::number(myCharacter->getHealth()) + QString::fromStdString(" ") + QString::number(myCharacter->getAttack()) + QString::fromStdString(" ") + QString::number(myCharacter->getDefense()));

        dataText.replace(re, replacementText);
        QFile::remove("player.txt");
        QFile newData("player.txt");
        if(newData.open(QFile::WriteOnly | QFile::Truncate)) {
            QTextStream out(&newData);
            out << dataText;
        }

        newData.rename("player.txt");
        newData.close();
    } catch (...){
        QFile f("player.txt");
        f.open(QIODevice::Text | QIODevice::ReadOnly);
        QString dataText = f.readAll();

        dataText = dataText + QString::fromStdString("\n") + n + QString::fromStdString(" ") + QString::number(myCharacter->getGold()) + QString::fromStdString(" ") + QString::number(myCharacter->getMaxHealth()) + QString::fromStdString(" ") + QString::number(myCharacter->getHealth()) + QString::fromStdString(" ") + QString::number(myCharacter->getAttack()) + QString::fromStdString(" ") + QString::number(myCharacter->getDefense()) + QString::fromStdString(" ");
        QFile::remove("player.txt");

        QFile newData("player.txt");
        if(newData.open(QFile::WriteOnly | QFile::Truncate)) {
            QTextStream out(&newData);
            out << dataText;
        }

        newData.rename("player.txt");
        newData.close();
    }
}

void MainWindow::on_load_clicked()
{
    ifstream savedFile("player.txt");
    QString n = ui->playerName->text();
    bool found = false;
    string findName;
    int g;
    int mh;
    int h;
    int a;
    int d;
    try{
        while (savedFile >> findName >> g >> mh >> h >> a >> d){
            if (findName == n.toStdString()){
                found = true;
                break;
            }
        }
        if (!found) throw 99;
        ui->notifications->clear();
        Character *save = new Character (findName,g,mh,h,a,d);
        myCharacter = save;
        MainWindow::updateStats();
    }
    catch(...){
        ui->notifications->setText("Name not found\n(case sensitive)");
    }
    savedFile.close();
}

void MainWindow::on_dragon_clicked()
{
    fightingDragon = true;
    if (ui->attackIncrease->text() == "Attack" && stats->getH() > 0){
        try{
            if(myCharacter->getGold()-100 < 0){
                throw 99;
            }
            myCharacter->setGold(myCharacter->getGold()-100);
            MainWindow::updateStats();
        } catch(...){
            ui->notifications->setText("Not enough gold\nto run away!");
        }
    }
    ui->notifications->clear();
    stats->setA(20);
    stats->setH(20);
    stats->setName("Dragon");
    stats->setPic(":/pics/dragon.png");
    stats->setValue(0);
    if (!ui->defenseIncrease->isVisible()){
        MainWindow::on();
    }
    ui->restore->setVisible(false);
    ui->attackIncrease->setText("Attack");
    ui->defenseIncrease->setText("Defense");
    ui->healthIncrease->setText("Run Away (costs 100 gold)");
    ui->welcome->clear();
    QPixmap image = QPixmap (stats->getPic());
    QPixmap newImage = image.scaled(QSize(400,300),  Qt::KeepAspectRatio);
    ui->welcome->setAlignment(Qt::AlignCenter);
    ui->welcome->setPixmap(newImage);
    ui->story->setText("Oh no! You encountered the " + stats->getName() + "!\nAttack Power: " + QString::number(stats->getA()) + "\nHealth: " + QString::number(stats->getH()));
}

void MainWindow::gameOver(){
    ui->notifications->setText("You have died.\nGame Over.");
    ui->story->setText("Restart the program to try again...");
    ui->shop->setVisible(false);
    ui->forest->setVisible(false);
    ui->dragon->setVisible(false);
    ui->save->setVisible(false);
    ui->load->setVisible(false);
    MainWindow::off();
}

void MainWindow::monsterKilled(){
    ui->story->setText("You defeated the " + stats->getName() + "!\nYou get " + QString::number(stats->getValue()) + " gold!");
    ui->notifications->clear();
    ui->goldAmount->setText(QString::number(myCharacter->getGold()));
    MainWindow::off();
}
