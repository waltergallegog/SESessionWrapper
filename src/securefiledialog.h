#ifndef SECUREFILEDIALOG_H
#define SECUREFILEDIALOG_H

#include <QDialog>
#include <QListView>
#include <QLabel>
#include <QDir>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemModel>
#include <QFileDialog>

#include "SEfile.h"
#include "se3/L1.h"

#define MAX_SHOWN_FILE 20

class SecureFileDialog: public QDialog
{
    Q_OBJECT

private:

    QDir oldPath, currentPath;
    int newFile;
    QString chosenFile;
    QStringList *listFiles;

    QLabel* labelCurrentPath;
    QLabel* labelFileView;
    QLabel* labelFileView2;

    QGridLayout* formGridLayout;

    QLineEdit* choosePath;
    QFileDialog* browsePath;
    QDialogButtonBox* openBrowse;
    QListView* chooseFile;
    QLineEdit* chooseNewFile;


    /*!
    * The standard dialog button box.
    */
    QDialogButtonBox* buttons;

    /*!
    * A method to set up all dialog components and
    * initialize them.
    */
    void setUpGUI();

public:
    explicit SecureFileDialog(QWidget *parent = 0, int newFile = 1);
    ~SecureFileDialog();
    QString getChosenFile();

signals:

//    void acceptLogin( QString& password );
//    void abortSignal();

private slots:

    void slotAbortFile();
    void slotAcceptFile();
    void refreshFileView();
    void updateNewFile();
};

#endif // SECUREFILEDIALOG_H
