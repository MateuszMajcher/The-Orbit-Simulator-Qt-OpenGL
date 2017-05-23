#pragma once
#ifndef ADDDIALOG_H
#define ADDDIALOG_H
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QStringList>
#include <QDebug>
#include <QGroupBox>
#include <QFormLayout>
#include <QRadioButton>
#include <QDoubleValidator>
#include <QComboBox>
// Include GLM
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class AddDialog : public QDialog {
	Q_OBJECT

private:
	QRadioButton* isPlanet;
	QLineEdit* nameEdit;
	QLineEdit* radiusEdit;
	QLineEdit* massEdit;
	QLineEdit* xPositionEdit;
	QLineEdit* yPositionEdit;
	QLineEdit* zPositionEdit;

	QLineEdit* xVelocityEdit;
	QLineEdit* yVelocityEdit;
	QLineEdit* zVelocityEdit;

	QDialogButtonBox* buttons;
	QComboBox* comboBox;

	//dane
	QList<std::string> nameList;
	QList<double> radiusList;
	QList<double> massList;
	QList<glm::vec3> posList;
	QList<glm::vec3> velList;

	void setupGUI();
	void setDoubleValidator(QLineEdit* edit, double min, double max);

	

public:
	explicit AddDialog(QWidget *parent = 0);

signals:
	void acceptData(QString& name, double radius, double mass, glm::vec3 position, glm::vec3 velocity);
public slots:
	void slotAcceptData();
	void setTemplate();
};

#endif