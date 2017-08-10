#pragma once

#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <fstream>
#include <ctime>
#include <random>
#include <iostream>
#include "ui_Corruptor.h"

int corruptFile(std::string, std::string, int, int, int);

class CorruptorWindow : public QMainWindow
{
	Q_OBJECT

public:
	CorruptorWindow(QWidget *parent = Q_NULLPTR);
private slots:
	void inFileSelectButtonClicked();
	void outFileSelectButtonClicked();
	void corruptButtonClicked();
private:
	Ui::CorruptorClass ui;
};