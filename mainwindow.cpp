#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonOpenFile_clicked(){
    fileName = QFileDialog::getOpenFileName(this, "Select a .txt file to open", QDir::homePath(), tr("Text files (*.txt)"));
    sourceFile.open(fileName.toStdString());

    std::string tempLine;

    sourceText.clear();

    while(getline(sourceFile, tempLine)){
       sourceText.append(tempLine + "\n");
    }

    ui->textEditSource->setText(QString::fromStdString(sourceText));

    sourceFile.close();
}

void MainWindow::on_pushButtonEncrypt_clicked(){
    std::vector<structVectorDictionary> tempDict;

    std::transform(sourceText.begin(), sourceText.end(), sourceText.begin(), ::tolower);
    sourceText.erase(remove_if(sourceText.begin(), sourceText.end(), isspace), sourceText.end());


    for(int i = 0; i < sourceText.length(); i++){
        std::string symbol = sourceText.substr(i, 1);
        bool inVector = false;
        for(int j = 0; j < tempDict.size(); j++){
            if(tempDict[j].symbol == symbol){
                tempDict[j].number++;
                inVector = true;
                break;
            }
        }
        if(!inVector){
            structVectorDictionary temp;
            temp.number = 1;
            temp.symbol = symbol;
            tempDict.push_back(temp);
        }
    }
    std::sort(tempDict.begin(), tempDict.end());

    reversedKeyDictionary.clear();
    keyDictionary.clear();
    for (int i = 0; i < tempDict.size(); i++) {
        keyDictionary.insert(std::pair<int, std::string>(i, tempDict[i].symbol));
        reversedKeyDictionary.insert(std::pair<std::string, int>(tempDict[i].symbol, i));
    }

    std::string dictionaryInEditedText;
    for (std::map<int, std::string>::iterator iter=keyDictionary.begin(); iter!=keyDictionary.end(); ++iter){
        dictionaryInEditedText += std::to_string(iter->first) + "/"+ iter->second + "\n";
    }

    std::string encryptedText;
    for(int i = 0; i < sourceText.length(); i++){
        std::string symbol = sourceText.substr(i, 1);
        encryptedText +=(std::to_string(reversedKeyDictionary.find(symbol)->second)+",");
    }

    editedText = "DICTIONARY\n" + dictionaryInEditedText + "\nENCRYPTED_TEXT\n" + encryptedText;
    ui->textEditEdited->setText(QString::fromStdString(editedText));
}

void MainWindow::on_pushButtonDecrypt_clicked(){
    std::transform(sourceText.begin(), sourceText.end(), sourceText.begin(), ::tolower);
    sourceText.erase(remove_if(sourceText.begin(), sourceText.end(), isspace), sourceText.end());

    if(sourceText.substr(0, 10) != "dictionary"){
        editedText = "ERROR";
        ui->textEditEdited->setText(QString::fromStdString(editedText));
        return;
    }
    reversedKeyDictionary.clear();
    keyDictionary.clear();

    editedText = sourceText;
    editedText.erase(0, 10); //erase word "dictionary"

    while(editedText.substr(0, 14) != "encrypted_text"){
        keyDictionary.insert(std::pair<int, std::string>(std::stoi(editedText.substr(0, editedText.find("/"))),
                                                         editedText.substr(editedText.find("/") + 1, 1)
                                                         ));
        editedText.erase(0, editedText.find("/")+2);
    }

    std::string tempString;

    editedText.erase(0, 14); // erase word "ecrypted_text"

    while(true) {
        tempString += keyDictionary.find(std::stoi(editedText.substr(0, editedText.find(","))))->second;
        editedText.erase(0, editedText.find(",") + 1);
        if(editedText.find(",") == -1){
            break;
        }
    }

    editedText = tempString;
    ui->textEditEdited->setText(QString::fromStdString(editedText));
}

void MainWindow::on_pushButtonSaveFile_clicked(){
    fileName = QFileDialog::getSaveFileName(this, "Select a .txt file to write in", QDir::homePath(), tr("Text files (*.txt)"));
    editedFile.open(fileName.toStdString());

    editedFile << editedText;

    editedFile.close();
}

void MainWindow::on_textEditSource_textChanged(){
    sourceText = ui->textEditSource->toPlainText().toStdString();
}

