#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QFileDialog>
#include <fstream>
#include <QUrl>
#include <QTextEdit>
#include <map>
#include <vector>
#include <algorithm>
#include <string>

struct structVectorDictionary{
    std::string symbol;
    int number;

    bool operator<(const structVectorDictionary& a) const{
        return a.number < number;
    }
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool compareDictionary(structVectorDictionary i1, structVectorDictionary i2);

private slots:
    void on_pushButtonDecrypt_clicked();

    void on_pushButtonEncrypt_clicked();

    void on_pushButtonOpenFile_clicked();

    void on_pushButtonSaveFile_clicked();

    void on_textEditSource_textChanged();

private:
    Ui::MainWindow *ui;
    QString fileName;

    std::ifstream sourceFile;
    std::ofstream editedFile;

    std::string sourceText;
    std::string editedText;

    std::map<int, std::string> keyDictionary;
    std::map<std::string, int> reversedKeyDictionary;
};
#endif // MAINWINDOW_H
