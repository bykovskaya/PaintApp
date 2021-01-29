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
	createToolbars();
	setWindowTitle(tr("PaintApp"));
	setWindowState(Qt::WindowMaximized);

	openedFile.clear();
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
			openedFile = fileName;
		}
	}
}

void PaintApp::saveAs()
{
	QMessageBox::StandardButton ret;
	if (!saveAsFile())
		ret = QMessageBox::warning(this, tr("Saving failure."), tr("File can't be saved."), QMessageBox::Ok);
}

void PaintApp::about()
{
	QMessageBox::about(this, tr("About PaintApp"),
		tr("<p>Description of this app... will be added soon</p>"));
}

void PaintApp::save()
{
	QMessageBox::StandardButton ret;
	if (!saveFile())
		ret = QMessageBox::warning(this, tr("Saving failure."), tr("File can't be saved."), QMessageBox::Ok);
}

void PaintApp::createActoins()
{
	openAct = new QAction(tr("&Open"), this);
	openAct->setShortcut(QKeySequence::Open);
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	saveAsAct = new QAction(tr("&SaveAs"), this);
	saveAsAct->setShortcut(QKeySequence::SaveAs);
	connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

	saveAct = new QAction(tr("&Save"), this);
	saveAct->setShortcut(QKeySequence::Save);
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcut(QKeySequence::Quit);
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	clearScreenAct = new QAction(tr("&Clear Screan"), this);
	clearScreenAct->setShortcut(tr("Ctrl+L"));
	connect(clearScreenAct, SIGNAL(triggered()), scrArea, SLOT(clearImg()));

	aboutAct = new QAction(tr("&About"), this);
	connect(aboutAct, SIGNAL(triggered()), SLOT(about()));

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
	fileMenu->addAction(saveAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	optionMenu = new QMenu(tr("&Options"), this);
	optionMenu->addAction(clearScreenAct);

	/*drawingMenu = new QMenu(tr("&Drawing"), this);
	drawingMenu->addAction(cursorAct);
	drawingMenu->addAction(drawLineAct);
	drawingMenu->addAction(drawRectAct);
	drawingMenu->addAction(drawEllipseAct);
	drawingMenu->addAction(drawTriangleAct);*/

	helpMenu = new QMenu(tr("&Help"), this);
	helpMenu->addAction(aboutAct);

	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(optionMenu);
	//menuBar()->addMenu(drawingMenu);
	menuBar()->addMenu(helpMenu);
}

void PaintApp::createToolbars()
{
	drawingToolsToolbar = new QToolBar();
	drawingToolsToolbar->setMovable(false);

	lineButton = new QPushButton();
	lineButton->setIcon(QIcon("icons/line.png"));
	lineButton->setIconSize(QSize(20, 20));
	lineButton->setToolTip(tr("Draw Line"));
	lineButton->setMaximumWidth(28);
	lineButton->setCursor(Qt::PointingHandCursor);
	connect(lineButton, SIGNAL(released()), scrArea, SLOT(drawSLine()));
	drawingToolsToolbar->addWidget(lineButton);

	rectButton = new QPushButton();
	rectButton->setIcon(QIcon("icons/rect.png"));
	rectButton->setIconSize(QSize(20, 20));
	rectButton->setToolTip(tr("Draw Rectaingle"));
	rectButton->setMaximumWidth(28);
	rectButton->setCursor(Qt::PointingHandCursor);
	connect(rectButton, SIGNAL(released()), scrArea, SLOT(drawSRect()));
	drawingToolsToolbar->addWidget(rectButton);

	ellipseButton = new QPushButton();
	ellipseButton->setIcon(QIcon("icons/ellipse.png"));
	ellipseButton->setIconSize(QSize(20, 20));
	ellipseButton->setToolTip(tr("Draw Ellipse"));
	ellipseButton->setMaximumWidth(28);
	ellipseButton->setCursor(Qt::PointingHandCursor);
	connect(ellipseButton, SIGNAL(released()), scrArea, SLOT(drawSEllipse()));
	drawingToolsToolbar->addWidget(ellipseButton);

	triangleButton = new QPushButton();
	triangleButton->setIcon(QIcon("icons/triangle.png"));
	triangleButton->setIconSize(QSize(20, 20));
	triangleButton->setToolTip(tr("Draw Triangle"));
	triangleButton->setMaximumWidth(28);
	triangleButton->setCursor(Qt::PointingHandCursor);
	connect(triangleButton, SIGNAL(released()), scrArea, SLOT(drawSTriangle()));
	drawingToolsToolbar->addWidget(triangleButton);

	cursorButton = new QPushButton();
	cursorButton->setIcon(QIcon("icons/arm.png"));
	cursorButton->setIconSize(QSize(20, 20));
	cursorButton->setToolTip(tr("Moove Figures"));
	cursorButton->setMaximumWidth(28);
	cursorButton->setCursor(Qt::PointingHandCursor);
	connect(cursorButton, SIGNAL(released()), scrArea, SLOT(selectCursor()));
	drawingToolsToolbar->addWidget(cursorButton);

	addToolBar(drawingToolsToolbar);
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
			return saveAsFile();
		else if (ret == QMessageBox::Cancel)
			return false;
	}
	return true;
}

bool PaintApp::saveAsFile()
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

bool PaintApp::saveFile()
{
	return scrArea->saveImage(openedFile);
}