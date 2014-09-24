#include <QDebug>

#include "classifiedviewer.h"
#include "ui_classifiedviewer.h"
#include <qfiledialog.h>

ClassifiedViewer::ClassifiedViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClassifiedViewer),
    m_classified(NULL)
{
    ui->setupUi(this);

    ui_label = qFindChild<QLabel*>(this, "imageLabel");

    ui_hueSlider = qFindChild<QSlider*>(this, "hueSlider");
    ui_thresholdSlider = qFindChild<QSlider*>(this, "thresholdSlider");
    ui_thresholdSpin = qFindChild<QDoubleSpinBox*>(this, "thresholdSpin");
    m_threshold = ui_thresholdSpin->value();

    m_highlightColor.setHsv(ui_hueSlider->value(), 255, 255);

    QRadioButton *imageOnlyRadio = qFindChild<QRadioButton*>(this, "imageOnlyRadio");
    imageOnlyRadio->setChecked(true);
    m_visualizationOption = imageOnly;
}

ClassifiedViewer::~ClassifiedViewer()
{
    delete ui;
}

void ClassifiedViewer::on_classifiedButton_clicked()
{
  QString classifiedFile = QFileDialog::getOpenFileName(this, tr("Select Classified File"), ".");
  if (classifiedFile.isNull()){
    return;
  }

  setClassifiedGrid(classifiedFile);
}

void ClassifiedViewer::setClassifiedGrid(QString classifiedFile)
{
  m_classified = new Grid(classifiedFile);
  scaleImage();
  drawScene();
}

void ClassifiedViewer::on_testImageButton_clicked()
{
  QString imageFile = QFileDialog::getOpenFileName(this, tr("Select Test Image File"), "/home/ryan/local/adaboost-webcam/images/");
  if (imageFile.isNull()){
    return;
  }

  setTestImage(imageFile);
}

void ClassifiedViewer::setTestImage(QString imageFile)
{
  m_image = QImage(imageFile);

  // convert image to grayscale
  m_grayscaleImage = QImage(m_image.width(), m_image.height(), m_image.format());
  for (int h=0; h<m_image.height(); h++){
    for (int w=0; w<m_image.width(); w++){
      m_grayscaleImage.setPixel(w, h, toGray(m_image.pixel(w, h)));
    }
  }
  scaleImage();

  drawScene();
}

void ClassifiedViewer::scaleImage()
{
  if (!m_image.isNull() && m_classified != NULL){
    m_image = m_image.scaled(m_classified->cols(), m_classified->rows());
    m_grayscaleImage = m_grayscaleImage.scaled(m_classified->cols(), m_classified->rows());
  }
}


void ClassifiedViewer::on_colorGrayscaleRadio_clicked()
{
  m_visualizationOption = colorGrayscale;
  drawScene();
}

void ClassifiedViewer::on_highlightingRadio_clicked()
{
  m_visualizationOption = highlightingScores;
  drawScene();
}

// if we disabled highlightingScores, make sure slider can't be moved
void ClassifiedViewer::on_highlightingRadio_toggled(bool checked)
{
  ui_hueSlider->setEnabled(checked);
}

void ClassifiedViewer::on_imageOnlyRadio_clicked()
{
  m_visualizationOption = imageOnly;
  drawScene();
}

// draws the scene based on m_visualizationOption value
void ClassifiedViewer::drawScene()
{
  switch(m_visualizationOption){
    case colorGrayscale:
      drawColorGrayscale();
      break;
    case highlightingScores:
      drawHighlightingScores();
      break;
    case imageOnly:
      ui_label->setPixmap(QPixmap::fromImage(m_image));
      break;
  }
}

void ClassifiedViewer::drawHighlightingScores()
{
  QImage display = m_image.copy();
  for (int r=0; r<m_classified->rows(); r++){
    for (int c=0; c<m_classified->cols(); c++){
      // if classified location is high enough,
      // set to m_highlightColor
      if (m_classified->val(r,c) > m_threshold){
        display.setPixel(c,r,m_highlightColor.rgb());
      }
    }
  }
  ui_label->setPixmap(QPixmap::fromImage(display));
}


void ClassifiedViewer::drawColorGrayscale()
{
  QImage display = m_grayscaleImage.copy();
  for (int r=0; r<m_classified->rows(); r++){
    for (int c=0; c<m_classified->cols(); c++){
      // if classified location is low enough,
      // gray it out
      if (m_classified->val(r,c) > m_threshold){
        display.setPixel(c, r, m_image.pixel(c,r));
      }
    }
  }
  ui_label->setPixmap(QPixmap::fromImage(display));
}

QRgb ClassifiedViewer::toGray(QRgb color){
  int gray = (qRed(color)*11 + qGreen(color)*16 + qBlue(color)*5)/32;
  return QColor(gray, gray, gray).rgb();
}

void ClassifiedViewer::on_hueSlider_valueChanged(int val)
{
  m_highlightColor.setHsv(val, 255, 255);
  drawScene();
}


void ClassifiedViewer::on_thresholdSlider_valueChanged(int val)
{
  m_threshold = val/100.;
  ui_thresholdSpin->setValue(m_threshold);
  drawScene();
}

void ClassifiedViewer::on_thresholdSpin_valueChanged(double val)
{
  m_threshold = val;
  ui_thresholdSlider->setValue(m_threshold*100);
  drawScene();
}

void ClassifiedViewer::changeEvent(QEvent *e)
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