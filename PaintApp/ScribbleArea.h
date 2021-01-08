#pragma once

#include<QColor>
#include<QImage>
#include<QPoint>
#include<QWidget>

#include"Shapes.h"

class ScribbleArea : public QWidget
{
	Q_OBJECT
public:
	ScribbleArea(QWidget* parent = nullptr);
	bool openImage(const QString& fileName);
	bool saveImage(const QString& fileName);
	bool isModified() const { return modified; }
	~ScribbleArea();

public slots:
	void clearImg();
	void selectCursor();
	void drawSLine();
	void drawSRect();
	void drawSEllipse();
	void drawSTriangle();

protected:
	void mousePressEvent(QMouseEvent* ev) override;
	void mouseMoveEvent(QMouseEvent* ev) override;
	void mouseReleaseEvent(QMouseEvent* ev) override;
	void paintEvent(QPaintEvent* ev) override;
	void resizeEvent(QResizeEvent* ev) override;

private:
	void drawShapes(const std::vector<BaseShape*>&);
	void drawLines(const std::vector<SLine*>&);
	void resizeImage(QImage* img, const QSize& newSize);

	enum SHAPE_TYPE { CURSOR, LINE, RECT, ELLIPSE, TRIANGLE };
	SHAPE_TYPE shType;

	BaseShape* selectedShape;

	BaseShape* newShape;
	std::vector<BaseShape*> figures;
	SLine* newLine;
	std::vector<SLine*> lines;
	QPoint* startPoint;
	QPoint* endPoint;
	std::vector<QPoint*> points;
	
	bool modified;
	bool scribbling;
	QColor penColor;
	QColor selectedPenColor;
	QImage image;
	QPoint lastPoint;
};

