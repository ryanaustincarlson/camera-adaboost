#include <cstdio>

#include <QFileDialog>
#include <QMessageBox>

#include "trainergui.h"
#include "ui_trainergui.h"

TrainerGui::TrainerGui(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::TrainerGui)
{
  ui->setupUi(this);
  ui_label1 = qFindChild<ClickLabel*>(this, "img1Label");
  ui_label2 = qFindChild<ClickLabel*>(this, "img2Label");

  // defaults
  QRadioButton *posRadio = qFindChild<QRadioButton*>(this, "positiveRadio");
  posRadio->setChecked(true);
  m_trainingType = m_posTrainingType;

  m_trainingFile = NULL;
  m_defaultImgWidth = 640;
}

TrainerGui::~TrainerGui()
{
  m_trainingFile->flush();
  m_trainingFile->close();
  delete ui;
}

void TrainerGui::on_img1Label_clicked(const QPoint& pos)
{
  on_label_clicked(pos);
}

void TrainerGui::on_img2Label_clicked(const QPoint& pos)
{
  on_label_clicked(pos);
}

void TrainerGui::on_label_clicked(const QPoint& pos)
{
  // write to trainingFile
  /* verbose...
  printf("TrainingType: %d, File1: %s, File2: %s, x: %d, y: %d, Res: %dx%d\n",
                                                     m_trainingType,
                                                     m_img1Fname.toStdString().c_str(),
                                                     m_img2Fname.toStdString().c_str(),
                                                     pos.x(),
                                                     pos.y(),
                                                     ui_label1->pixmap()->width(), // assume label1 has same resolution as label2
                                                     ui_label1->pixmap()->height()); // */

  if (ui_label1 == NULL || ui_label2 == NULL){
    return;
  }

  if (m_img1Fname.isNull() || m_img2Fname.isNull() ||
    m_trainingFile == NULL || !m_trainingFile->isOpen()){
      return;
  }
/*
  printf("%d %s %s %d %d %dx%d\n",
         m_trainingType,
         m_img1Fname.toStdString().c_str(),
         m_img2Fname.toStdString().c_str(),
         pos.x(),
         pos.y(),
         ui_label1->pixmap()->width(), // assume label1 has same resolution as label2
         ui_label1->pixmap()->height());*/

  QString output = QString("%1 %2 %3 %4 %5 %6 %7\n")
      .arg(m_trainingType)
      .arg(m_img1Fname)
      .arg(m_img2Fname)
      .arg(pos.x())
      .arg(pos.y())
      .arg(ui_label1->pixmap()->width())
      .arg(ui_label1->pixmap()->height());

  printf("%s",output.toStdString().c_str());

  int writeErrCode = -1;
  if (m_trainingFile->write(output.toStdString().c_str()) == writeErrCode){
    QMessageBox::information(this, "Write Failed", "Write to Training File failed. Uh oh...");
    return;
  }

  if (!m_trainingFile->flush()){
    QMessageBox::information(this, "Flush Failed", "Training Examples File flush failed. You should look into that...");
    return;
  }
}

void TrainerGui::on_img1Button_clicked()
{
  on_loadImageButton_clicked(m_img1Fname, ui_label1);
}

void TrainerGui::on_img2Button_clicked()
{
  on_loadImageButton_clicked(m_img2Fname, ui_label2);
}

// utility function for on_img{1,2}Button_clicked()
void TrainerGui::on_loadImageButton_clicked(QString &fname, ClickLabel* label)
{
  fname = QFileDialog::getOpenFileName(this, "Select Image");
  if (fname.isNull()){
    return;
  }

  QPixmap img(fname);
  if (img.isNull()){
    QMessageBox::information(this, "Invalid File", "File is not a valid Image File");
    return;
  }
  img = img.scaledToWidth(m_defaultImgWidth);

  label->setPixmap(img);
}

void TrainerGui::on_trainingFileButton_clicked()
{
  QString fname = QFileDialog::getOpenFileName(this, "Select File to Store Training Examples");

  if (fname.isNull()){
    return;
  }

  m_trainingFile = new QFile(fname);

  if (!m_trainingFile->open(QIODevice::Append | QIODevice::WriteOnly)){
    return;
  }

  printf("Loaded Training File: %s\n",m_trainingFile->fileName().toStdString().c_str());
}

void TrainerGui::on_positiveRadio_clicked()
{
  m_trainingType = m_posTrainingType;
}

void TrainerGui::on_negativeRadio_clicked()
{
  m_trainingType = m_negTrainingType;
}

void TrainerGui::changeEvent(QEvent *e)
{
  QMainWindow::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
      break;
    default:
      break;
  }
}
