#include "PaintApp.h"
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qcolordialog.h>
#include <QImageWriter>

PaintApp::PaintApp()
{
	scrArea = new ScribbleArea;
	setCentralWidget(scrArea);
	createActoins();
	createMenus();
	setWindowTitle(tr("PaintApp"));
	resize(500, 600);
}

void PaintApp::closeEvent(QCloseEvent* e)
{
	if (maybeSave())
		e->accept();
	else
		e->ignore();
}

void PaintApp::open()
{
	if (maybeSave())
	{
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::currentPath(), tr("PaintApp(*.papp)"));
		if (!fileName.isEmpty())
		{
			scrArea->openImage(fileName);
		}
	}
}

void PaintApp::save()
{
	if (saveFile())
	{
		QMessageBox::StandardButton ret;
		ret = QMessageBox::information(this, tr("Success!"),
			tr("File has been saved."),
			 QMessageBox::Ok);
	}
	else
	{
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("Saving failure."),
			tr("File can't be saved."),
			QMessageBox::Ok);
	}

}

void PaintApp::about()
{
	QMessageBox::about(this, tr("About PaintApp"),
		tr("<p>Description of this app... will be added soon</p>"));
}

void PaintApp::createActoins()
{
	openAct = new QAction(tr("&Open"), this);
	openAct->setShortcut(QKeySequence::Open);
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	saveAsAct = new QAction(tr("&SaveAs"), this);
	saveAsAct->setShortcut(QKeySequence::SaveAs);
	connect(saveAsAct, SIGNAL(triggered()), this, SLOT(save()));


	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcut(QKeySequence::Quit);
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	clearScreenAct = new QAction(tr("&Clear Screan"), this);
	clearScreenAct->setShortcut(tr("Ctrl+L"));
	connect(clearScreenAct, SIGNAL(triggered()), scrArea, SLOT(clearImg()));

	aboutAct = new QAction(tr("&About"), this);
	connect(aboutAct, SIGNAL(triggered()), SLOT(about()));

	aboutQtAct = new QAction(tr("&About &Qt"), this);
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(about()));
	////////////////////////////////////////////////////////////////

	cursorAct = new QAction(tr("&Cursor"), this);
	connect(cursorAct, SIGNAL(triggered()), scrArea, SLOT(selectCursor()));

	drawLineAct = new QAction(tr("&Line"), this);
	connect(drawLineAct, SIGNAL(triggered()), scrArea, SLOT(drawSLine()));

	drawRectAct = new QAction(tr("&Rectaingle"), this);
	connect(drawRectAct, SIGNAL(triggered()), scrArea, SLOT(drawSRect()));

	drawEllipseAct = new QAction(tr("&Ellipse"), this);
	connect(drawEllipseAct, SIGNAL(triggered()), scrArea, SLOT(drawSEllipse()));

	drawTriangleAct = new QAction(tr("&Triangle"), this);
	connect(drawTriangleAct, SIGNAL(triggered()), scrArea, SLOT(drawSTriangle()));
}

void PaintApp::createMenus()
{

	fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAsAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	optionMenu = new QMenu(tr("&Options"), this);
	optionMenu->addAction(clearScreenAct);

	drawingMenu = new QMenu(tr("&Drawing"), this);
	drawingMenu->addAction(cursorAct);
	drawingMenu->addAction(drawLineAct);
	drawingMenu->addAction(drawRectAct);
	drawingMenu->addAction(drawEllipseAct);
	drawingMenu->addAction(drawTriangleAct);

	helpMenu = new QMenu(tr("&Help"), this);
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);

	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(optionMenu);
	menuBar()->addMenu(drawingMenu);
	menuBar()->addMenu(helpMenu);
}

bool PaintApp::maybeSave()
{
	if (scrArea->isModified())
	{
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("Scibble"),
			tr("File has been modified.\nWould you like to save your changes?"),
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		if (ret == QMessageBox::Save)
			return saveFile();
		else if (ret == QMessageBox::Cancel)
			return false;
	}
	return true;
}

bool PaintApp::saveFile()
{
	QByteArray fileFormat = "papp";
	QString initPath = QDir::currentPath() + "/untitled." + fileFormat;
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), initPath,
		tr("%1 Files (*.%2);; All Files(*)").arg(QString::fromLatin1(fileFormat.toUpper())).arg(QString::fromLatin1(fileFormat)));
	if (fileName.isEmpty())
		return false;
	else
		return scrArea->saveImage(fileName);
}