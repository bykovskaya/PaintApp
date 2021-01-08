#pragma once

#include<QColor>
#include<QImage>
#include<QPoint>
#include<QWidget>
#include <QtWidgets>

class SLine
{
public:
	SLine(QPoint* sp, QPoint* ep) : startPoint(sp), endPoint(ep) {}
	bool isSelected(QPoint) const;
	void draw(QImage*, QColor) const;
	friend QDataStream& operator<<(QDataStream& out, const SLine& obj);
	~SLine() { }
private:
	QPoint* startPoint;
	QPoint* endPoint;
};

class BaseShape
{
public:
	BaseShape() {}

	virtual void draw(QImage*, QColor)const = 0;
	virtual bool isSelected(QPoint) const = 0;
	virtual int id() = 0;

	void move(QPoint, QPoint);
	void setEndPoint(QPoint);
	void normalize();
	void addPointPtr(QPoint*);

	friend QDataStream& operator<<(QDataStream& out, const BaseShape& obj);
	virtual ~BaseShape() = 0;
protected:
	QPoint startPoint;
	QPoint endPoint;
	std::vector<QPoint*> points;
};

class SRect : public BaseShape
{
public:
	SRect(QPoint, QPoint);
	SRect(QPoint);
	void draw(QImage*, QColor) const;
	bool isSelected(QPoint) const;
	int id();
	~SRect() {}
};

class SEllipse : public BaseShape
{
public:
	SEllipse(QPoint, QPoint);
	SEllipse(QPoint);
	void draw(QImage*, QColor) const;
	bool isSelected(QPoint) const;
	int id();
	~SEllipse() {}
};

class STriangle : public BaseShape
{
public:
	STriangle(QPoint, QPoint);
	STriangle(QPoint);
	void draw(QImage*, QColor) const;
	bool isSelected(QPoint) const;
	int id();
	~STriangle() {}
};

