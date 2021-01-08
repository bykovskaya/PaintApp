#include "Shapes.h"

const int penWidth = 2;

void BaseShape::move(QPoint fromPoint, QPoint toPoint)
{
	QPoint deltaP(toPoint.x() - fromPoint.x(), toPoint.y() - fromPoint.y());

	//add moving for line!!!
	for (int i = 0; i < points.size(); i++)
	{
		if (points[i]->x() > startPoint.x()
			&& points[i]->x() < endPoint.x()
			&& points[i]->y() > startPoint.y()
			&& points[i]->y() < endPoint.y())
		{
			*points[i] += deltaP;
		}
	}

	startPoint += deltaP;
	endPoint += deltaP;
}

void BaseShape::setEndPoint(QPoint ep)
{
	endPoint = ep;
}

void BaseShape::addPointPtr(QPoint* newPoint)
{
	points.push_back(newPoint);
}

BaseShape::~BaseShape() {}

void BaseShape::normalize()
{
	QPoint firstPoint = startPoint;
	QPoint seccondPoint = endPoint;

	if (firstPoint.x() < seccondPoint.x())
	{
		startPoint.setX(firstPoint.x());
		endPoint.setX(seccondPoint.x());
	}
	else
	{
		startPoint.setX(seccondPoint.x());
		endPoint.setX(firstPoint.x());
	}
	if (firstPoint.y() < seccondPoint.y())
	{
		startPoint.setY(firstPoint.y());
		endPoint.setY(seccondPoint.y());
	}
	else
	{
		startPoint.setY(seccondPoint.y());
		endPoint.setY(firstPoint.y());
	}
}

void SLine::draw(QImage* image, QColor penColor) const
{
	QPainter painter(image);
	painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap,
		Qt::RoundJoin));
	painter.drawLine(*startPoint, *endPoint);
}

bool SLine::isSelected(QPoint cursor) const
{
	int startX = (startPoint->x() < endPoint->x()) ? startPoint->x() : endPoint->x();
	int endX = (startPoint->x() > endPoint->x()) ? startPoint->x() : endPoint->x();
	int startY = (startPoint->y() < endPoint->y()) ? startPoint->y() : endPoint->y();
	int endY = (startPoint->y() > endPoint->y()) ? startPoint->y() : endPoint->y();

	if (cursor.x() > startX && cursor.x() < endX
		&& cursor.y() > startY && cursor.y() < endY)
		return true;
	return false;
}

SRect::SRect(QPoint sp, QPoint ep)
{
	startPoint = sp;
	endPoint = ep;
}

SRect::SRect(QPoint sp)
{
	startPoint = sp;
}

void SRect::draw(QImage* image, QColor penColor) const
{
	QPainter painter(image);
	painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap,
		Qt::RoundJoin));
	int width = endPoint.x() - startPoint.x();
	int height = endPoint.y() - startPoint.y();
	painter.drawRect(startPoint.x(), startPoint.y(), width, height);
}

bool SRect::isSelected(QPoint cursor) const
{
	if (cursor.x() > startPoint.x() && cursor.x() < endPoint.x()
		&& cursor.y() > startPoint.y() && cursor.y() < endPoint.y())
		return true;
	return false;
}

int SRect::id()
{
	return 1;
}

SEllipse::SEllipse(QPoint sp, QPoint ep)
{
	startPoint = sp;
	endPoint = ep;
}

SEllipse::SEllipse(QPoint sp)
{
	startPoint = sp;
}

void SEllipse::draw(QImage* image, QColor penColor) const
{
	QPainter painter(image);
	painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap,
		Qt::RoundJoin));
	int width = endPoint.x() - startPoint.x();
	int height = endPoint.y() - startPoint.y();
	painter.drawEllipse(startPoint.x(), startPoint.y(), width, height);
}

bool SEllipse::isSelected(QPoint cursor) const
{
	if (cursor.x() > startPoint.x() && cursor.x() < endPoint.x()
		&& cursor.y() > startPoint.y() && cursor.y() < endPoint.y())
	{
		int horizontal_semiaxis = (endPoint.x() - startPoint.x()) / 2;
		int vertical_semiaxis = (endPoint.y() - startPoint.y()) / 2;
		QPoint center(startPoint.x() + horizontal_semiaxis, startPoint.y() + vertical_semiaxis);
		double x2 = (cursor.x() - center.x()) * (cursor.x() - center.x());
		double y2 = (cursor.y() - center.y()) * (cursor.y() - center.y());
		double a2 = horizontal_semiaxis * horizontal_semiaxis;
		double b2 = vertical_semiaxis * vertical_semiaxis;
		double result = x2 / a2 + y2 / b2;
		if (result <= 1)
			return true;
	}
	return false;
}

int SEllipse::id()
{
	return 2;
}

STriangle::STriangle(QPoint sp, QPoint ep)
{
	startPoint = sp;
	endPoint = ep;
}

STriangle::STriangle(QPoint sp)
{
	startPoint = sp;
}

void STriangle::draw(QImage* image, QColor penColor) const
{
	QPainter painter(image);
	painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap,
		Qt::RoundJoin));

	QPolygon triangle;
	triangle << QPoint(startPoint.x(), endPoint.y()) << QPoint((endPoint.x() - startPoint.x()) / 2 + startPoint.x(), startPoint.y())	<< QPoint(endPoint.x(), endPoint.y());
	painter.drawPolygon(triangle);
}

bool STriangle::isSelected(QPoint cursor) const
{
	if (cursor.x() > startPoint.x() && cursor.x() < endPoint.x()
		&& cursor.y() > startPoint.y() && cursor.y() < endPoint.y())
	{
		int a = (endPoint.x() - startPoint.x()) / 2;
		int b = (endPoint.y() - startPoint.y());
		double k = (double)b / (double)a;
		int y1 = k * cursor.x() - k * endPoint.x() + endPoint.y();
		k = -k;
		int y2 = k * cursor.x() - k * startPoint.x() + endPoint.y();

		if (cursor.y() > y1 && cursor.y() > y2)
			return true;
	}
	return false;
}

int STriangle::id()
{
	return 3;
}

QDataStream& operator<<(QDataStream& out, const SLine& obj)
{
	out << *obj.startPoint << *obj.endPoint;
	return out;
}

QDataStream& operator<<(QDataStream& out, const BaseShape& obj)
{
	out << obj.startPoint << obj.endPoint;
	return out;
}

