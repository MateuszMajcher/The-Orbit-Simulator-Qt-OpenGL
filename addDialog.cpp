#include "addDialog.h"

AddDialog::AddDialog(QWidget *parent) :QDialog(parent) {
	int width = 300;
	int height = 400;
	if (parent != NULL) {
		setGeometry(parent->x() + parent->width() / 2 - width / 2,
			parent->y() + parent->height() / 2 - height / 2,
			width, height);
	}
	else {
		resize(width, height);
	}
	

	setupGUI();
	setWindowTitle(tr("Dodaj planete/slonce"));
	setModal(true);
}

void AddDialog::setupGUI() {
	// set up the layout
	QVBoxLayout *mainLayout = new QVBoxLayout;

	isPlanet = new QRadioButton(tr("Slonce"));

	nameEdit = new QLineEdit;
	radiusEdit = new QLineEdit;
	
	massEdit = new QLineEdit;
	xPositionEdit = new QLineEdit;
	yPositionEdit = new QLineEdit;
	zPositionEdit = new QLineEdit;

	xVelocityEdit = new QLineEdit;
	yVelocityEdit = new QLineEdit;
	zVelocityEdit = new QLineEdit;


	//init buttons
	buttons = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttons, SIGNAL(accepted()), this, SLOT(slotAcceptData()));
	connect(buttons, SIGNAL(rejected()), this, SLOT(close()));

	

	QGroupBox* formGroupBox = new QGroupBox(tr(""));
	formGroupBox->setStyleSheet("QGroupBox::title{ color: gray }");
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(new QLabel(tr("Name")), 1, 0, 1, 1);
	layout->addWidget(nameEdit, 1, 1, 1, 1);
	layout->addWidget(new QLabel(tr("Radius")), 2, 0, 1, 1);
	layout->addWidget(radiusEdit, 2, 1, 1, 1);
	layout->addWidget(new QLabel(tr("Mass")), 3, 0, 1, 1);
	layout->addWidget(massEdit, 3, 1, 1, 1);
	formGroupBox->setLayout(layout);

	/*********************************/
	/**************pozycja************/
	/*********************************/
	QGroupBox* positionGroupBox = new QGroupBox(tr("Position"));
	QGridLayout *positionLayout = new QGridLayout;
	positionLayout->addWidget(new QLabel(tr("X")), 1, 0, 1, 1);
	positionLayout->addWidget(new QLabel(tr("Y")), 1, 1, 1, 1);
	positionLayout->addWidget(new QLabel(tr("Z")), 1, 2, 1, 1);

	positionLayout->addWidget(xPositionEdit, 2, 0, 1, 1);
	positionLayout->addWidget(yPositionEdit, 2, 1, 1, 1);
	positionLayout->addWidget(zPositionEdit, 2, 2, 1, 1);
	positionGroupBox->setLayout(positionLayout);


	/*********************************/
	/**************ped****************/
	/*********************************/
	QGroupBox* velocityGroupBox = new QGroupBox(tr("Position"));
	QGridLayout *velocityLayout = new QGridLayout;
	velocityLayout->addWidget(new QLabel(tr("X")), 1, 0, 1, 1);
	velocityLayout->addWidget(new QLabel(tr("Y")), 1, 1, 1, 1);
	velocityLayout->addWidget(new QLabel(tr("Z")), 1, 2, 1, 1);

	velocityLayout->addWidget(xVelocityEdit, 2, 0, 1, 1);
	velocityLayout->addWidget(yVelocityEdit, 2, 1, 1, 1);
	velocityLayout->addWidget(zVelocityEdit, 2, 2, 1, 1);
	velocityGroupBox->setLayout(velocityLayout);


	mainLayout->addWidget(formGroupBox);
	mainLayout->addWidget(positionGroupBox);
	mainLayout->addWidget(velocityGroupBox);
	mainLayout->addWidget(buttons);



	setDoubleValidator(radiusEdit, 0.1, 10.0);
	setDoubleValidator(massEdit, 0.1, 10.0);
	setDoubleValidator(xPositionEdit, 0.1, 10.0);
	setDoubleValidator(yPositionEdit, 0.1, 10.0);
	setDoubleValidator(zPositionEdit, 0.1, 10.0);
	setDoubleValidator(xVelocityEdit, 0.1, 10.0);
	setDoubleValidator(yVelocityEdit, 0.1, 10.0);
	setDoubleValidator(zVelocityEdit, 0.1, 10.0);


	/***********Debug***********/
	nameEdit->setText("Mars");
	radiusEdit->setText("0.8");

	massEdit->setText("5");
	xPositionEdit->setText("0");
	yPositionEdit->setText("1.5");
	zPositionEdit->setText("2");

	xVelocityEdit->setText("0.8");
	yVelocityEdit->setText("0.125");
	zVelocityEdit->setText("1.0");


	setLayout(mainLayout);
}

void AddDialog::setDoubleValidator(QLineEdit* edit, double min, double max) {
	QDoubleValidator *massDoubleVal = new QDoubleValidator(0.1, 10.0, 4, edit);
	massDoubleVal->setNotation(QDoubleValidator::StandardNotation);
	massDoubleVal->setLocale(QLocale::C);
	edit->setValidator(massDoubleVal);
}

void AddDialog::slotAcceptData() {

	QString username = nameEdit->text();
	double radius = radiusEdit->text().toDouble();
	double mass = massEdit->text().toDouble();

	double xPosition = xPositionEdit->text().toDouble();
	double yPosition = yPositionEdit->text().toDouble();
	double zPosition = zPositionEdit->text().toDouble();

	double xVelocity = xVelocityEdit->text().toDouble();
	double yVelocity = yVelocityEdit->text().toDouble();
	double zVelocity = zVelocityEdit->text().toDouble();

	emit acceptData(username, radius, mass, glm::vec3(xPosition, yPosition, zPosition), glm::vec3(xVelocity, yVelocity, zVelocity));
	close();
}