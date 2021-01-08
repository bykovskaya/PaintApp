#include <QtWidgets>

#include "ScribbleArea.h"

ScribbleArea::ScribbleArea(QWidget* parent) : QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);
	modified = false;
	scribbling = false;
	penColor = Qt::black;
	selectedPenColor = Qt::red;
	shType = CURSOR;
	selectedShape = nullptr;
}

bool ScribbleArea::openImage(const QString& fileName)
{
	clearImg();
	QFile file(fileName);
	QDataStream stream(&file);

	if (file.open(QIODevice::ReadOnly))
	{
		if (file.size() != 0)
		{
			int figuresListSize, type;
			QPoint startP, endP;
			stream >> figuresListSize;
			for (int i = 0; i < figuresListSize; i++)
			{
				stream >> type >> startP >> endP;
				switch (type)
				{
				case 1:
					newShape = new SRect(startP, endP);
					break;
				case 2:
					newShape = new SEllipse(startP, endP);
					break;
				case 3:
					newShape = new STriangle(startP, endP);
					break;
				}
				figures.push_back(newShape);
				newShape->draw(&image, penColor);
			}

			int linesListSize;
			stream >> linesListSize;
			for (int i = 0; i < linesListSize; i++)
			{
				stream >> startP >> endP;
				startPoint = new QPoint(startP);
				endPoint = new QPoint(endP);
				for (int i = 0; i < figures.size(); i++)
				{
					if (figures[i]->isSelected(startP))
						figures[i]->addPointPtr(startPoint);
					if (figures[i]->isSelected(endP))
						figures[i]->addPointPtr(endPoint);
				}
				points.push_back(startPoint);
				points.push_back(endPoint);
				newLine = new SLine(startPoint, endPoint);
				lines.push_back(newLine);
				newLine->draw(&image, penColor);
			}
		}
		modified = false;
		update();

		return true;
	}
	return false;
}

bool ScribbleArea::saveImage(const QString& fileName)
{
	QFile file(fileName);
	QDataStream stream(&file);

	if (file.open(QIODevice::WriteOnly))
	{
		int vectorSize= figures.size();

		stream << vectorSize;
		for (int i = 0; i < vectorSize; i++)	
			stream << figures[i]->id() << *figures[i];
			
		vectorSize = lines.size();

		stream << vectorSize;
		for (int i = 0; i < vectorSize; i++)
			stream << *lines[i];
		
		modified = false;
		return true;
	}

	return false;
}

void ScribbleArea::selectCursor()
{
	shType = CURSOR;
}

void ScribbleArea::drawSLine()
{
	shType = LINE;
}

void ScribbleArea::drawSRect()
{
	shType = RECT;
}

void ScribbleArea::drawSEllipse()
{
	shType = ELLIPSE;
}

void ScribbleArea::drawSTriangle()
{
	shType = TRIANGLE;
}

void ScribbleArea::clearImg()
{
	if (!figures.empty())
	{
		for (int i = 0; i < figures.size(); i++)
			delete figures[i];
	}
	if (!lines.empty())
	{
		for (int i = 0; i < lines.size(); i++)
			delete lines[i];
	}
	if (!points.empty())
	{
		for (int i = 0; i < points.size(); i++)
			delete points[i];
	}
	figures.clear();
	lines.clear();
	points.clear();
	image.fill(qRgb(255, 255, 255));
	modified = true;
	update();
}

void ScribbleArea::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		lastPoint = e->pos();
		scribbling = true;

		switch (shType)
		{
		case CURSOR:
			for (int i = figures.size() - 1; i >= 0; i--)
			{
				if (figures[i]->isSelected(lastPoint))
				{
					selectedShape = figures[i];
					return;
				}
			}
			break;

		case LINE:
			startPoint = new QPoint(lastPoint);
			points.push_back(startPoint);
			endPoint = new QPoint(lastPoint);

			for (int i = figures.size() - 1; i >= 0; i--)
			{
				if (figures[i]->isSelected(lastPoint))
				{
					selectedShape = figures[i];
					break;
				}
			}
			if (selectedShape != nullptr)
			{
				selectedShape->addPointPtr(startPoint);
				selectedShape = nullptr;
			}

			newLine = new SLine(startPoint, endPoint);
			break;

		case RECT:
			newShape = new SRect(lastPoint);
			break;

		case ELLIPSE:
			newShape = new SEllipse(lastPoint);
			break;

		case TRIANGLE:
			newShape = new STriangle(lastPoint);
			break;
		}

	}
}

void ScribbleArea::mouseMoveEvent(QMouseEvent* e)
{
	if ((e->buttons() & Qt::LeftButton) && scribbling)
	{
		image.fill(qRgb(255, 255, 255));

		if (shType == CURSOR && selectedShape != nullptr)
		{
			selectedShape->move(lastPoint, e->pos());
			lastPoint = e->pos();
		}
		else
		{
			if (shType == LINE)
			{
				endPoint->setX(e->pos().x());
				endPoint->setY(e->pos().y());
				newLine->draw(&image, selectedPenColor);
			}
			if (shType == RECT || shType == ELLIPSE || shType == TRIANGLE)
			{
				newShape->setEndPoint(e->pos());
				newShape->draw(&image, selectedPenColor);
			}
		}

		if (!figures.empty())
			drawShapes(figures);
		if (!lines.empty())
			drawLines(lines);

		update();
	}
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton && scribbling)
	{
		if (shType == CURSOR)
		{
			if (selectedShape != nullptr)
				selectedShape = nullptr;
		}
		else if (shType == LINE)
		{
			points.push_back(endPoint);
			lines.push_back(newLine);

			for (int i = figures.size() - 1; i >= 0; i--)
			{
				if (figures[i]->isSelected(QPoint(endPoint->x(), endPoint->y())))
				{
					selectedShape = figures[i];
					break;
				}
			}
			if (selectedShape != nullptr)
			{
				selectedShape->addPointPtr(endPoint);
				selectedShape = nullptr;
			}

			startPoint = nullptr;
			endPoint = nullptr;
		}
		else
		{
			newShape->normalize();
			figures.push_back(newShape);
			newShape = nullptr;
		}

		image.fill(qRgb(255, 255, 255));

		if (!figures.empty())
			drawShapes(figures);
		if (!lines.empty())
			drawLines(lines);

		scribbling = false;
		update();
	}
}

void ScribbleArea::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);
	QRect dirtyRect = e->rect();
	painter.drawImage(dirtyRect, image, dirtyRect);
}

void ScribbleArea::resizeEvent(QResizeEvent* e)
{
	if (width() > image.width() || height() > image.height())
	{
		int newWidth = qMax(width() + 128, image.width());
		int newHeight = qMax(height() + 128, image.height());
		resizeImage(&image, QSize(newWidth, newHeight));
		update();
	}
	QWidget::resizeEvent(e);
}

void ScribbleArea::drawShapes(const std::vector<BaseShape*>& shapes)
{
	for (int i = 0; i < shapes.size(); i++)
	{
		if (shapes[i] == selectedShape)
			shapes[i]->draw(&image, selectedPenColor);
		else
			shapes[i]->draw(&image, penColor);
	}
	modified = true;
}

void ScribbleArea::drawLines(const std::vector<SLine*>& shapes)
{
	for (int i = 0; i < shapes.size(); i++)
		shapes[i]->draw(&image, penColor);
	modified = true;
}


void ScribbleArea::resizeImage(QImage* img, const QSize& newSize)
{
	if (img->size() == newSize)
	{
		return;
	}

	QImage newImage(newSize, QImage::Format_RGB32);
	newImage.fill(qRgb(255, 255, 255));
	QPainter painter(&newImage);
	painter.drawImage(QPoint(0, 0), *img);
	*img = newImage;
}

ScribbleArea::~ScribbleArea()
{
	if (!figures.empty())
	{
		for (int i = 0; i < figures.size(); i++)
			delete figures[i];
	}
	if (!lines.empty())
	{
		for (int i = 0; i < lines.size(); i++)
			delete lines[i];
	}
	if (!points.empty())
	{
		for (int i = 0; i < points.size(); i++)
			delete points[i];
	}
}
