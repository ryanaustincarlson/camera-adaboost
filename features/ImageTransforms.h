#ifndef _TRAINING_IMAGES_H
#define _TRAINING_IMAGES_H

#include <QImage>
#include <QList>

class ImageTransforms {
  public:
    ImageTransforms(const QImage &img1, const QImage &img2, int trainingType, int xFocus, int yFocus);

    QList<QImage*> images();

    QImage diff() const;
    QImage diffGrayscale() const;
    QImage sobel() const;

    QImage img1() const;
    QImage img2() const;
    int xFocus() const;
    int yFocus() const;
    int trainingType() const; // positive or negative

    void setxFocus(int xFocus);
    void setyFocus(int yFocus);
    
  private:
    QImage m_img1, m_img2;
    QImage m_imgdiff, m_imgdiffGray;
    QImage m_sobel;
    int m_trainingType; // positive or negative
    int m_xFocus, m_yFocus;

    void calculateImgDiff();
    void calculateSobel();
    QRgb toGray(QColor color);
};

#endif // _TRAINING_IMAGES_H