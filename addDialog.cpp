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

	//szablon
	nameList = QList<std::string>() << "sun" << "jupiter" << "saturn" << "uranus" <<"neptune"<<"pluto";
	radiusList = QList<double>() << 1.0 << 0.8 << 0.6<<0.4<<0.2<<0.5;
	massList = QList<double>() << 1.00000597682 << 0.000954786104043<< 0.000285583733151<< 0.0000437273164546<< 0.0000517759138449<< 0.00000000769230;
	posList = QList<glm::vec3>() << glm::vec3(0.0, 0.0, 0.0) << glm::vec3(-3.5023653, -3.8169847, -1.5507963) << glm::vec3(9.0755314, -3.0458353, -1.6483708) << glm::vec3(8.3101420, -16.2901086, -7.2521278) << glm::vec3(11.4707666, -25.7294829, -10.8169456) << glm::vec3(-15.5387357, -25.2225594, -3.1902382);
	velList = QList<glm::vec3>() << glm::vec3(0.0, 0.0, 0.0) << glm::vec3(0.00565429, -0.00412490, -0.00190589) << glm::vec3(0.00168318, 0.00483525, 0.00192462) << glm::vec3(0.00354178, 0.00137102, 0.00055029) << glm::vec3(0.00288930, 0.00114527, 0.00039677) << glm::vec3(0.00276725, -0.00170702, -0.00136504);



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

	comboBox = new QComboBox;
	for (int i = 0; i < nameList.size(); ++i) 
		comboBox->addItem(QString::fromStdString(nameList.at(i)));
	connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setTemplate()));

	mainLayout->addWidget(formGroupBox);
	mainLayout->addWidget(positionGroupBox);
	mainLayout->addWidget(velocityGroupBox);
	mainLayout->addWidget(comboBox);
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
	nameEdit->setText("Pluton");
	radiusEdit->setText("0.2");

	massEdit->setText("0.00000000769230");
	xPositionEdit->setText("-15.5387357");
	yPositionEdit->setText("-25.2225594");
	zPositionEdit->setText("-3.1902382");

	xVelocityEdit->setText("0.00276725");
	yVelocityEdit->setText("-0.00170702");
	zVelocityEdit->setText("-0.00136504");


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

void AddDialog::setTemplate() {
	int index = comboBox->currentIndex();
	QString text = comboBox->currentText();

	nameEdit->setText(text);
	radiusEdit->setText(QString::number(radiusList.at(index)));

	massEdit->setText(QString::number(massList.at(index)));
	xPositionEdit->setText(QString::number(posList.at(index).x));
	yPositionEdit->setText(QString::number(posList.at(index).y));
	zPositionEdit->setText(QString::number(posList.at(index).z));

	xVelocityEdit->setText(QString::number(velList.at(index).x));
	yVelocityEdit->setText(QString::number(velList.at(index).y));
	zVelocityEdit->setText(QString::number(velList.at(index).z));

	qDebug() << index << text << comboBox->itemData(index, Qt::DisplayRole);
}