//mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <fstream>
#include <regex>
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QMainWindow>
#include <QComboBox>
#include <QRadioButton>
#include <QCommandLinkButton>
#include <QDialog>
#include <QMessageBox>
#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QString>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QRegularExpression>
#include <QStatusBar>
#include <QProgressDialog>
#include <QTreeWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QStringListModel>
#include <QDesktopServices>
#include <QDebug>
#include <QListWidget>
#include <sstream>
#include <sys/stat.h>

#include<io.h>


#include <iostream>
using namespace std;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow() {
    }
public:
    map<string, string> dict;

    QLineEdit* Path;
    QPushButton* selectPath;
    QLabel* text1;
    QLabel* text2;
    QLabel* text4;
    QComboBox* Format;
    QRadioButton* forge;
    QRadioButton* recover;
    QPushButton* execute;
    QPushButton* fixFormat;

    QGridLayout* layout;
    QMainWindow* mainwindow;
    QWidget* centerContent;
    QMenuBar* menubar;
    QMenu* more;
    QAction* aboutInfo;
    QAction* aboutQT;
    QLabel* pathtext = new QLabel;
    QLabel* Tip;
    
    QTreeWidget* formatList;
    QTreeWidgetItem* m_myItem;
    int m_myCol;
    QLineEdit* fName;
    QLineEdit* fData;
    QDialog* add;
public:
    string getFilePath();
    void getFormatData(map<string, string>& dict);
public slots:
    void showAboutWindow();
    void selectPathWindow();
    void executeProcess();
    int  isFilePathFormatOK();
    void showFixFormatWindow();
    void showAboutQtWindow();
    void showOpenUrl();
    void showBiliBiliDetails();
    void changeStatusText();
    void save();

    void treeWidgetOpenEditor(QTreeWidgetItem* item, int col);
    void treeWidgetCloseEditor();
    void addItemAction();
    //void addToDict(QTreeWidgetItem*);
    void addItemActionConfirm();
    void delItem();
    void updateCombobox();

    void setFormatDisabled();
    void setFormatEnabled();

    static bool isFileExists(string& name);
};

#endif // MAINWINDOW_H
