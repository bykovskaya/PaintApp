#pragma once

#include<QColor>
#include<QImage>
#include<QPoint>
#include<QWidget>
#include <QtWidgets>

class SLine
{
public:
	SLine(QPoint* sp, QPoint* ep);
	~SLine() { }

	//: startPoint(sp), endPoint(ep) {}
	bool isSelected(QPoint) const;
	void draw(QImage*, QColor) const;
	
	friend QDataStream& operator<<(QDataStream& out, const SLine& obj);
private:
	QPoint* startPoint;
	QPoint* endPoint;
};

class BaseShape
{
public:
	BaseShape();
	virtual ~BaseShape() = 0;

	virtual void draw(QImage*, QColor)const = 0;
	virtual bool isSelected(QPoint) const = 0;
	virtual int id() = 0;

	void move(QPoint, QPoint);
	void setEndPoint(QPoint);
	void normalize();
	void addPointPtr(QPoint*);

	friend QDataStream& operator<<(QDataStream& out, const BaseShape& obj);
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
	~SRect();
	
	void draw(QImage*, QColor) const;
	bool isSelected(QPoint) const;
	int id();
};

class SEllipse : public BaseShape
{
public:
	SEllipse(QPoint, QPoint);
	SEllipse(QPoint);
	~SEllipse();
	
	void draw(QImage*, QColor) const;
	bool isSelected(QPoint) const;
	int id();
};

class STriangle : public BaseShape
{
public:
	STriangle(QPoint, QPoint);
	STriangle(QPoint);
	~STriangle();
	
	void draw(QImage*, QColor) const;
	bool isSelected(QPoint) const;
	int id();
};

