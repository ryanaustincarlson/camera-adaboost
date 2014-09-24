#ifndef _TRAINERGUI_H
#define _TRAINERGUI_H

#include <QMainWindow>
#include <QLabel>
#include <QString>
#include <QFile>

#include "clicklabel.h"

namespace Ui {
    class TrainerGui;
}

class TrainerGui : public QMainWindow {
  Q_OBJECT
  public:
    TrainerGui(QWidget *parent = 0);
    ~TrainerGui();

  protected:
    void changeEvent(QEvent *e);

  private slots:
    void on_img1Label_clicked(const QPoint &pos);
    void on_img2Label_clicked(const QPoint &pos);
    void on_img1Button_clicked();
    void on_img2Button_clicked();
    void on_trainingFileButton_clicked();
    void on_positiveRadio_clicked();
    void on_negativeRadio_clicked();

  private:
    Ui::TrainerGui *ui;
    ClickLabel *ui_label1, *ui_label2;

    QString m_img1Fname, m_img2Fname;
    QFile *m_trainingFile;
    int m_defaultImgWidth;
    int m_trainingType;

    // training types -- positive or negative
    static const int m_posTrainingType = 1;
    static const int m_negTrainingType = -1;

    void on_label_clicked(const QPoint& pos); // helper function for img{1,2}Label slots
    void on_loadImageButton_clicked(QString &fname, ClickLabel* label);

  //signals:
    
};

#endif // _TRAINERGUI_H
