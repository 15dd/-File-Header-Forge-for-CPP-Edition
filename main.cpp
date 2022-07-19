
#include "mainwindow.h"


int main(int argc, char* argv[])
{   
    QApplication mainP(argc, argv);

	mainP.setWindowIcon(QIcon("Logo.ico"));

	string dofhfP = ".//data.dofhf"; // 检测data.dofhf和Images是否存在
	if (!MainWindow::isFileExists(dofhfP)) {
		QApplication::beep();
		QMessageBox::critical(nullptr, "文件丢失", "data.dofhf丢失");

	}
	if (_access(".//Images", 0) == -1) {
		QApplication::beep();
		QMessageBox::critical(nullptr, "文件丢失", "Images文件夹丢失");

	}

    MainWindow mainW;

	QApplication::setQuitOnLastWindowClosed(true);

    return mainP.exec();
}