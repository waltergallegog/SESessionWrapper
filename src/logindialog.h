#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QStringList>

#include "se3/L1.h"

#define MAX_PASSWORD_LEN 100

class LoginDialog : public QDialog
{
    Q_OBJECT

private:
    QList<se3_disco_it> device_found;
    se3_session* s;
    se3_device dev;

    QLabel* labelDevice;
    QLabel* labelAlgorithm;
    QLabel* labelKeyId;
    QGridLayout* formGridLayout;

    QComboBox* chooseDevice;
    QComboBox* chooseAlgorithm;
    QComboBox* chooseKeyId;
    /*!
    * A label for the username component.
    */
    //QLabel* labelUsername; //NO USERNAME (SCALIA)

    /*!
    * A label for the password.
    */
    QLabel* labelPassword;

    /*!
    * An editable combo box for allowing the user
    * to enter his username or select it from a list.
    */
    // QComboBox* comboUsername; //NO USERNAME (SCALIA)

    /*!
    * A field to let the user enters his password.
    */
    QLineEdit* editPassword;

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
    explicit LoginDialog(QWidget *parent = 0, se3_session* session=NULL);
    ~LoginDialog();

    /*!
    * Sets the current password to propose to the user for the login.
    * password the password to fill into the dialog form
    */
    void setPassword( QString& password );

    se3_session *getSession();

signals:

    /*!
    * A signal emitted when the login is performed.
    * username the username entered in the dialog
    * password the password entered in the dialog
    * index the number of the username selected in the combobox
    */

    void acceptLogin( QString& password );
    void abortSignal();

public slots:
    /*!
    * A slot to adjust the emitting of the signal.
    */
    void slotAbortLogin();
    void slotAcceptLogin();
    void refreshDevice();
};

#endif // LOGINDIALOG_H
