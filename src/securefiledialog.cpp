#include "securefiledialog.h"

SecureFileDialog::SecureFileDialog(QWidget *parent, int newFile) : QDialog(parent)
{
    oldPath = QDir::currentPath();
    currentPath = QDir::currentPath();
    this->newFile=newFile;
    setUpGUI();
    if (!newFile)
        setWindowTitle( tr("Choose Wallet") );
    else
        setWindowTitle( tr("Create New Wallet") );

    setModal( true );
}
SecureFileDialog::~SecureFileDialog()
{
    QDir::setCurrent(oldPath.absolutePath());
}

void SecureFileDialog::setUpGUI(){
    // set up the layout
    //QGridLayout* formGridLayout = new QGridLayout( this );
    formGridLayout = new QGridLayout( this );

    // initialize buttons
    buttons = new QDialogButtonBox( this );
    buttons->addButton( QDialogButtonBox::Ok );
    buttons->addButton( QDialogButtonBox::Cancel );
    buttons->button( QDialogButtonBox::Ok )->setText( tr("Ok") );
    buttons->button( QDialogButtonBox::Cancel )->setText( tr("Abort") );
    buttons->setCenterButtons(true);

    choosePath = new QLineEdit ( this );
    labelCurrentPath = new QLabel ( this );
    labelFileView = new QLabel ( this );
    browsePath = new QFileDialog ( this );
    openBrowse = new QDialogButtonBox ( this );
    openBrowse->addButton(QString("Browse"), QDialogButtonBox::AcceptRole);

    labelCurrentPath->setText( tr("Path:"));
    labelCurrentPath->setBuddy( choosePath );

    browsePath->setFileMode(QFileDialog::DirectoryOnly);
    if( newFile ){
        labelFileView->setText( tr("Wallet name:"));
        chooseNewFile = new QLineEdit ( this );
        labelFileView->setBuddy( chooseNewFile );
        formGridLayout->addWidget( chooseNewFile, 1, 1 );

        chooseFile = new QListView ( this );
        chooseFile->setViewMode(QListView::ListMode);
        chooseFile->setEditTriggers(QListView::NoEditTriggers);
        connect (chooseFile,
                 SIGNAL(clicked(QModelIndex)),
                 this,
                 SLOT(updateNewFile())
                 );
        labelFileView2=new QLabel(this);
        labelFileView2->setText(tr("Wallets:"));
        labelFileView2->setBuddy(chooseFile);
        formGridLayout->addWidget(labelFileView2,2,0);
        formGridLayout->addWidget(chooseFile,2,1);
        formGridLayout->addWidget( buttons, 3, 0, 1, 3);
    }else{
        labelFileView->setText( tr("Wallets:"));
        chooseFile = new QListView ( this );
        chooseFile->setViewMode(QListView::ListMode);
        labelFileView->setBuddy( chooseFile );
        connect( chooseFile,
                 SIGNAL(doubleClicked(QModelIndex)),
                 this,
                 SLOT(slotAcceptFile())
                 );
        formGridLayout->addWidget( chooseFile, 1, 1 );
        formGridLayout->addWidget( buttons, 2, 0, 2, 2 );
    }
    //todo choose which and how many column we want
    refreshFileView();



    // connects slots
    connect( buttons->button( QDialogButtonBox::Cancel ),
             SIGNAL (clicked()),
             this,
             SLOT (slotAbortFile())
             );

    connect( buttons->button( QDialogButtonBox::Ok ),
             SIGNAL (clicked()),
             this,
             SLOT (slotAcceptFile())
             );
    connect( choosePath,
             SIGNAL(editingFinished()),
             this,
             SLOT(refreshFileView())
             );
    connect (openBrowse->buttons().at(0),
             SIGNAL(clicked(bool)),
             browsePath,
             SLOT(exec())
             );
    connect(browsePath,
            SIGNAL(accepted()),
            this,
            SLOT(refreshFileView())
            );

    // place components into the dialog
    formGridLayout->addWidget( labelCurrentPath, 0, 0 );
    formGridLayout->addWidget( choosePath, 0, 1 );
    formGridLayout->addWidget( openBrowse, 0, 2 );
    formGridLayout->addWidget( labelFileView, 1, 0 );


    setLayout( formGridLayout );

}

void SecureFileDialog::slotAcceptFile(){
    if (!newFile ){

        if( chooseFile->currentIndex().row() == -1){
            reject();
            return;
        }
        //        chosenFile = listFiles->at(chooseFile->currentIndex().row());
        if(choosePath->text().indexOf("/") != -1)
            chosenFile = choosePath->text() +"/" + listFiles->at(chooseFile->currentIndex().row());
        else if (choosePath->text().indexOf("\\") != -1)
            chosenFile = choosePath->text() +"\\" + listFiles->at(chooseFile->currentIndex().row());
        else
            chosenFile = listFiles->at(chooseFile->currentIndex().row());
        accept();
    }
    else{
        if (! chooseNewFile->text().isEmpty()){
            if (chooseNewFile->text().endsWith(QString(".sqlite")))
                chosenFile = chooseNewFile->text();
            else
                chosenFile = chooseNewFile->text().append(".sqlite");
            accept();
        }

    }

}

QString SecureFileDialog::getChosenFile(){
    return chosenFile;
}

void SecureFileDialog::refreshFileView(){

    char fileList[MAX_SHOWN_FILE*MAX_PATHNAME], *pFile = NULL;
    uint32_t filelistLength = 0;
    size_t cnt = 0;
    if(browsePath->result()==QDialog::Accepted){
        currentPath.setCurrent(browsePath->selectedFiles().at(0));
        choosePath->setText(QDir::currentPath());
    }else{
        currentPath.setCurrent(choosePath->text());
        choosePath->setText(QDir::currentPath());
    }


    if( secure_ls(choosePath->text().toUtf8().data(), fileList, &filelistLength) ){

        exit(1);
    }

    listFiles = new QStringList ();

    pFile = fileList;
    cnt = (size_t)filelistLength;
    while(cnt > 0){
        listFiles->append(QString::fromUtf8(pFile, (int)strlen(pFile)));
        cnt-=(strlen(pFile)+1);
        pFile+=(strlen(pFile)+1);
    }

    QStringListModel* model = new QStringListModel( *listFiles );
    chooseFile->setModel((QAbstractItemModel *)model);
    updateGeometry();
}

void SecureFileDialog::updateNewFile(){
    QString aux;
    if( chooseFile->currentIndex().row() == -1){
        reject();
        return;
    }
    //        chosenFile = listFiles->at(chooseFile->currentIndex().row());
    if(choosePath->text().indexOf("/") != -1)
        aux = choosePath->text() +"/" + listFiles->at(chooseFile->currentIndex().row());
    else if (choosePath->text().indexOf("\\") != -1)
        aux = choosePath->text() +"\\" + listFiles->at(chooseFile->currentIndex().row());
    else
        aux = listFiles->at(chooseFile->currentIndex().row());
    chooseNewFile->setText(QFileInfo(QFile(aux)).fileName().remove(".sqlite"));
}

void SecureFileDialog::slotAbortFile(){
    reject();
}
