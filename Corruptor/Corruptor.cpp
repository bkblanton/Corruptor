#include "Corruptor.h"

unsigned long long llrand() {
	unsigned long long r = 0;

	for (int i = 0; i < 5; ++i) {
		r = (r << 15) | (rand() & 0x7FFF);
	}

	return r & 0xFFFFFFFFFFFFFFFFULL;
}

int corruptFile(std::string ifname, std::string ofname, int corruptionType, int numCorruptions, int numProtected) {
	std::ifstream inFile(ifname, std::ios::binary | std::ios::ate);
	std::ifstream::pos_type len = inFile.tellg();
	std::vector<char> data(len);
	inFile.seekg(0, std::ios::beg);
	inFile.read(&data[0], len);
	inFile.close();

	std::cout << "test" << std::endl;
	std::srand(time(NULL));
	switch (corruptionType) {
	case 0:
		for (int i = 0; i < numCorruptions; ++i) {
			data[numProtected + llrand() % (data.size() - numProtected)] = rand() % 256;
		}
		break;
	case 1:
		for (int i = 0; i < numCorruptions; ++i) {
			data.insert(data.begin() + numProtected + llrand() % (data.size() - numProtected), rand() % 256);
		}
		break;
	case 2:
		for (int i = 0; i < numCorruptions; ++i) {
			data.erase(data.begin() + numProtected + llrand() % (data.size() - numProtected));
		}
		break;
	case 3:
		for (int i = 0; i < numCorruptions; ++i) {
			int index1 = numProtected + llrand() % (data.size() - numProtected);
			int index2 = numProtected + llrand() % (data.size() - numProtected);
			int temp = data[index1];
			data[index1] = data[index2];
			data[index2] = temp;
		}
		break;
	}

	std::ofstream outFile(ofname, std::ios::binary);
	outFile.write(&data[0], data.size());
	outFile.close();

	return 0;
}

CorruptorWindow::CorruptorWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.corruptButton, SIGNAL(clicked()), this, SLOT(corruptButtonClicked()));
	connect(ui.inFileSelectButton, SIGNAL(clicked()), this, SLOT(inFileSelectButtonClicked()));
	connect(ui.outFileSelectButton, SIGNAL(clicked()), this, SLOT(outFileSelectButtonClicked()));
}

void CorruptorWindow::inFileSelectButtonClicked() {
	QString path = QFileDialog::getOpenFileName(this);
	if (!path.isNull()) {
		ui.inFileLineEdit->setText(path);
	}
}

void CorruptorWindow::outFileSelectButtonClicked() {
	QString path = QFileDialog::getSaveFileName(this, tr("Output"), ui.inFileLineEdit->text());
	if (!path.isNull()) {
		ui.outFileLineEdit->setText(path);
	}
}

void CorruptorWindow::corruptButtonClicked() {
	corruptFile(
		ui.inFileLineEdit->text().toStdString(), 
		ui.outFileLineEdit->text().toStdString(),
		ui.corruptionTypeCombox->currentIndex(),
		ui.numCorruptionsSpinner->value(),
		ui.numProtectedBytesSpinner->value()
	);
}