#include "mainwindow.h"

using namespace std;

bool MainWindow::isFileExists(string& name) { // 检测文件是否存在
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void MainWindow::selectPathWindow() { // 调出选择文件的窗口
	setWindowIcon(QIcon("Images//Logo.png"));
	QString fileName = QFileDialog::getOpenFileName(this,"选择文件","C:/");
	MainWindow::Path->setText(fileName);
}

void MainWindow::getFormatData(map<string, string>& dict) { // 从data.dofhf中导入数据
	string inData;
	ifstream data;
	data.open(".//data.dofhf", ios::in);
	if (!(data.is_open())) {
		QApplication::beep();
		QMessageBox::critical(mainwindow,"文件丢失","文件<data.dofhf>未找到,请确保它在程序目录下,或重新下载程序");
	}
	else {
		while (getline(data, inData)) {
			string request_str(inData);
			//cout << "request_str:" << request_str.c_str() << endl;
			int index = request_str.find(',');
			string key_str = request_str.substr(0, index);
			//cout << "key_str:" << key_str.c_str() << endl;
			string value_str = request_str.substr(index + 1);
			//cout << "value_str:" << value_str.c_str() << endl;
			dict.insert(make_pair(key_str, value_str.c_str())); // 将数据导入到容器中
		}
		data.close();
	}
}
int MainWindow::isFilePathFormatOK() {
	string filePath = getFilePath();
	regex FilePathRegularExpression("^[a-zA-Z]:([\\\\/][^\\s\\\\/:*?<>\"|][^\\\\/:*?<>\"|]*)*([/\\\\])?$"); // 正则表达式检测文件路径是否有效,下面两种表达式都可以检测,推荐用第二个 "(^//.|^/|^[a-zA-Z])?:?/.+(/$)?"       "^[a-zA-Z]:([\\\\/][^\\s\\\\/:*?<>\"|][^\\\\/:*?<>\"|]*)*([/\\\\])?$"
	if (!regex_match(filePath, FilePathRegularExpression)) { // 判断路径格式是否正确
		QApplication::beep();
		QMessageBox::warning(mainwindow, "文件地址格式输入错误", "文件地址输入错误,可能有以下几点导致 \n 1) 未输入内容 \n 2) 该文件未含有扩展名 \n 3) 文件地址格式输入错误,如多次输入错误,请点击<选择文件>按钮手动选择 \n 4) 该文件不存在 \n 5) 文件地址前后有引号,如有请删除", QMessageBox::Ok);
		return -1;
	}
	return 0;
}

string MainWindow::getFilePath() { // 获取QLineEdit里的内容
	QString PathTemp = Path->text();
	string filePath = PathTemp.toLocal8Bit().data(); // toLocal8Bit() 是将QString以自己正确的编码方式读取并转换成编译器编码方式的字节流
	return filePath;
}


void MainWindow::changeStatusText() { // 改变状态栏的文字
	QString path = Path->text();
	pathtext->setText(path);
	mainwindow->statusBar()->removeWidget(Tip);
	mainwindow->statusBar()->addWidget(pathtext);
}

void MainWindow::updateCombobox() { // 更新下拉框数据
	Format->clear();// 删除下拉框所有数据
	map<string, string>::iterator iter;
	for (iter = dict.begin(); iter != dict.end(); iter++) { // 遍历容器添加数据
		Format->addItem(iter->first.c_str());
	}
}

string HexToBin(const string& strHex) // 将十六进制数译成二进制码写入需要被修改的文件中
{
	if (strHex.size() % 2 != 0)
	{
		return "";
	}

	std::string strBin;
	strBin.resize(strHex.size() / 2);
	for (size_t i = 0; i < strBin.size(); i++)
	{
		uint8_t cTemp = 0;
		for (size_t j = 0; j < 2; j++)
		{
			char cCur = strHex[2 * i + j];
			if (cCur >= '0' && cCur <= '9')
			{
				cTemp = (cTemp << 4) + (cCur - '0');
			}
			else if (cCur >= 'a' && cCur <= 'f')
			{
				cTemp = (cTemp << 4) + (cCur - 'a' + 10);
			}
			else if (cCur >= 'A' && cCur <= 'F')
			{
				cTemp = (cTemp << 4) + (cCur - 'A' + 10);
			}
			else
			{
				return "";
			}
		}
		strBin[i] = cTemp;
	}

	return strBin;
} // https://www.cnblogs.com/azbane/p/10179742.html

//void MainWindow::saveChangeItem() {
//	QTreeWidgetItem* pCurrentItem = FixFormatWindow::formatList->currentItem();
//	QString strText = pCurrentItem->text(1); // key
//}
void MainWindow::save() { // 将容器中的数据导出到data.dofhf
	ofstream data;
	cout << "if" << endl;
	data.open(".//data.dofhf", ios::out);
	if (!(data.is_open())) {
		cout << "failed" << endl;
	}
	else {
		map<string, string>::iterator iter;
		for (iter = dict.begin(); iter != dict.end(); iter++) {
			data << iter->first << "," << iter->second << endl;
		}
		data.close();
	}
}

void MainWindow::executeProcess() { // 主程序
	string filePath = getFilePath(); // 获取文件地址
	if (filePath.find('"', 0) != -1 || filePath.rfind('"', filePath.length() - 1) != -1) {
		filePath.erase(remove(filePath.begin(), filePath.end(), '\"'), filePath.end());
		Path->setText(QString::fromUtf8(filePath.c_str()));
	} // 检测手动输入的文件地址前后是否有双引号.如果有,则删去
	  // 不知道为什么把上面这行代码放到isFilePathFormatOK()里会出错,要按两次<执行>才通过 

	filePath = getFilePath(); // 获取文件地址
	if (forge->isChecked()) { // 检测伪造模式是否被选中
		if (isFilePathFormatOK() != -1) { // 检测文件
			ofstream outData(filePath, ios_base::out |ios_base::in | ios_base::binary);
			if (!(outData.is_open())) { // 判断文件是否打开
				QApplication::beep();
				QMessageBox::warning(mainwindow, "文件打开失败", "1) 文件未含有扩展名 \n2) 文件不存在,请重新选择 \n3) 文件地址格式错误\n 4) 路径或文件名包含中文,将其改为英文", QMessageBox::Ok);
				return;
			}
			getFormatData(dict); // 容器数据更新
			string currentFormat = Format->currentText().toLocal8Bit().data(); // 获取选择的格式
			if (dict.find(currentFormat) == dict.end()) { // 判断是否有该格式
				QApplication::beep();
				QMessageBox::warning(mainwindow, "格式选择", "未选择格式式或该格式不存在");
				return;
			}
			string data = dict.at(currentFormat); // 获取该格式对应的十六进制数
			data = HexToBin(data); // 将十六进制数译成二进制码
			outData.seekp(ios::beg); // 覆盖写入
			outData << data;
			outData.close();

			QApplication::beep();
			QMessageBox::information(mainwindow, "完成", "伪造文件完成", QMessageBox::Ok);
		}	
	}
	else if (recover->isChecked()) { // 检测还原模式是否被选中
		if (isFilePathFormatOK() != -1) { // 检测文件
			ofstream outData(filePath, ios_base::out | ios_base::in | ios_base::binary);
			if (!(outData.is_open())) { // 判断文件是否打开
				QApplication::beep();
				QMessageBox::warning(mainwindow, "文件打开失败", "1) 文件未含有扩展名 \n2) 文件不存在,请重新选择 \n3) 文件地址格式错误\n 4) 路径或文件名包含中文,将其改为英文", QMessageBox::Ok);
				return;
			}
			getFormatData(dict); // 容器数据更新
			string currentFormat = filePath.substr(filePath.find_last_of('.') + 1); // 获取文件后缀名
			int flag = -1;
			map<string, string>::iterator iter;
			for (iter = dict.begin(); iter != dict.end(); iter++) { // 循环遍历容器判断该文件后缀数据在本程序中是否存在
				if (currentFormat == iter->first) {
					flag = 0; 
				}
			}
			if (flag != 0) { // 判断是否寻找到该格式的数据
				QApplication::beep();
				QMessageBox::warning(mainwindow, "查找失败", "该文件后缀名的数据未在本程序中添加,请自行添加!");
				return;
			}
			string data = dict.at(currentFormat); // 获取后缀对应的数据
			data = HexToBin(data); // 转二进制码
			outData.seekp(ios::beg);
			outData << data;
			outData.close();

			QApplication::beep();
			QMessageBox::information(mainwindow, "完成", "还原文件完成", QMessageBox::Ok);
		}
	}
	else { 
		QApplication::beep();
		QMessageBox::warning(mainwindow, "警告", "请选择模式", QMessageBox::Ok);
		return;
	}
}

