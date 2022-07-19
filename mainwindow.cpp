//mainwindow.cpp
#include "mainwindow.h"
// QMainWindow有自带的menuBar,statusBar,有自带的布局
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // 主界面


	getFormatData(dict);

	mainwindow = new QMainWindow;
	mainwindow->setWindowTitle("File Header Forger");
	mainwindow->resize(440, 200);
	mainwindow->setWindowIcon(QIcon("Images//Logo.png"));
	
	Tip = new QLabel("注意>>>重要文件不要使用此程序!可能会导致数据损坏!",mainwindow);
	Tip->setStyleSheet("color:red");
	mainwindow->statusBar()->removeWidget(pathtext);
	mainwindow->statusBar()->addWidget(Tip);

	menubar = new QMenuBar(mainwindow);
	mainwindow->setMenuBar(menubar);
	more = new QMenu("关于");
	menubar->addMenu(more);
	aboutInfo = more->addAction(QIcon("Images//aboutFHF.ico"), "关于本程序");
	aboutQT = more->addAction(QIcon("Images//qt-logo.png"),"关于Qt");


	centerContent = new QWidget; // 设置widget放置的父窗口
	Path = new QLineEdit(centerContent);
	Path->setPlaceholderText("请选择文件地址");
	Path->setClearButtonEnabled(true);
	selectPath = new QPushButton(QIcon("Images//findFile.ico"), " 选择文件",centerContent);
	text1 = new QLabel("文件地址:", centerContent);
	text2 = new QLabel("文件格式:", centerContent);
	Format = new QComboBox(centerContent);

	updateCombobox();

	text4 = new QLabel("选择模式:",centerContent);
	forge = new QRadioButton("伪造模式", centerContent);
	recover = new QRadioButton("还原模式", centerContent);
	execute = new QPushButton(QIcon("Images//execute.ico")," 执行", centerContent);
	execute->setDefault(true);

	fixFormat = new QPushButton(QIcon("Images//fixFormat.ico")," 增减格式", centerContent);
	
	mainwindow->statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}")); // 不显示label的边框

	layout = new QGridLayout;
	layout->addWidget(text1, 0, 0);
	layout->addWidget(Path, 0, 1, 1, 3);
	layout->addWidget(selectPath, 0, 4);
	layout->addWidget(text2, 1, 0);
	layout->addWidget(Format, 1, 1, 1, 3);
	layout->addWidget(fixFormat, 1, 4);
	layout->addWidget(text4, 2, 0);
	layout->addWidget(forge, 2, 1, 1, 2, Qt::AlignHCenter);
	layout->addWidget(recover, 2, 3, 1, 2, Qt::AlignHCenter);
	layout->addWidget(execute, 3, 2, Qt::AlignHCenter);
	centerContent->setLayout(layout); // QWidget应用布局
	mainwindow->setCentralWidget(centerContent); // 设置QMainWindow的中心窗口内容为centerContent(QWidget类)
	mainwindow->show();

	// 弹出关于窗口
	connect(aboutInfo, &QAction::triggered, this, &MainWindow::showAboutWindow);
	// 弹出选择文件窗口
	connect(selectPath, &QPushButton::clicked, this, &MainWindow::selectPathWindow);
	// 执行按钮点击后执行主程序
	connect(execute, &QPushButton::clicked, this, &MainWindow::executeProcess);
	// 弹出增减格式窗口
	connect(fixFormat, &QPushButton::clicked, this, &MainWindow::showFixFormatWindow);
	// 弹出关于qt窗口
	connect(aboutQT, &QAction::triggered, this ,&MainWindow::showAboutQtWindow);
	// 将文本框的内容投射到状态栏上
	connect(Path, &QLineEdit::textChanged, this, &MainWindow::changeStatusText);
	// 设置当还原模式被点击时,禁用下拉框
	connect(recover, &QRadioButton::clicked, this ,&MainWindow::setFormatDisabled);
	// 设置当伪造模式被点击时,启用下拉框
	connect(forge, &QRadioButton::clicked, this, &MainWindow::setFormatEnabled);
}
void MainWindow::setFormatDisabled() {
	Format->setEnabled(false); // 设置当还原模式被点击时,禁用下拉框
}
void MainWindow::setFormatEnabled() {
	Format->setEnabled(true); // 设置当伪造模式被点击时,启用下拉框
}
void MainWindow::showAboutWindow() {
	//关于界面
	QDialog* aboutwindow = new QDialog(centerContent);
	aboutwindow->setWindowTitle("关于File Header Forger");
	aboutwindow->setWindowModality(Qt::ApplicationModal);
	aboutwindow->setFixedSize(280, 180);
	QVBoxLayout* aboutLayout = new QVBoxLayout(centerContent);
	QLabel* text3 = new QLabel(aboutwindow);
	text3->setText("本程序由cyh128制作\n使用C++以及Qt进行构建\n可用于伪造文件类型以绕过上传文件类型的限制\n关于本程序的更新的消息请看Github!\n许可:MIT License\nVersion:1.0");
	text3->setAlignment(Qt::AlignHCenter);
	QPushButton* openGithub = new QPushButton(QIcon("Images//github Logo.png"), "打开Github源项目", aboutwindow);
	QPushButton* bilibiliDetails = new QPushButton(QIcon("Images//bilibili Logo.ico"), "打开作者的BiliBili主页", aboutwindow);
	aboutLayout->addWidget(text3,0);
	aboutLayout->addWidget(openGithub, 1);
	aboutLayout->addWidget(bilibiliDetails, 2);
	aboutwindow->setLayout(aboutLayout);

	aboutwindow->setAttribute(Qt::WA_DeleteOnClose); // 设置该窗口关闭后自动delete
	QObject::connect(openGithub, &QPushButton::clicked, this, &MainWindow::showOpenUrl);
	QObject::connect(bilibiliDetails, &QPushButton::clicked, this, &MainWindow::showBiliBiliDetails);

	aboutwindow->exec();
}


void MainWindow::showFixFormatWindow() {
	//下方有地方内存泄露,不知如何解决
	QDialog fixFormatWindow(mainwindow);
	fixFormatWindow.setWindowModality(Qt::ApplicationModal);
	fixFormatWindow.setWindowTitle("增减格式");
	fixFormatWindow.setFixedSize(390, 300);
	formatList = new QTreeWidget(&fixFormatWindow);
	formatList->setFixedSize(250, 280);
	formatList->setGeometry(10,10,10,10);
	QPushButton formatAdd(" 添加", &fixFormatWindow);
	QPushButton formatDel(" 删除", &fixFormatWindow);
	QPushButton forClose(" 关闭", &fixFormatWindow);
	formatAdd.setGeometry(280,25,90,25);
	formatDel.setGeometry(280,60,90,25);
	forClose.setGeometry(280,250,90,25);
	forClose.setDefault(true);
	formatAdd.setIcon(QIcon("Images//add.png"));
	formatDel.setIcon(QIcon("Images//del.png"));
	forClose.setIcon(QIcon("Images//close.ico"));
	QLabel fixTip("提示:修改内容时\n双击行即可修改\n格式无法修改!" , &fixFormatWindow);
	fixTip.setAlignment(Qt::AlignTop);
	fixTip.setGeometry(280, 90, 90,90);

	m_myItem = NULL;
	m_myCol = -1;

	QStringList strList;
	strList.append("格式");
	strList.append("十六进制数");
	formatList->setHeaderLabels(strList);

	// 遍历dict,将其数据添加到列表上
	map<string, string>::iterator iter;	
	for (iter = dict.begin(); iter != dict.end(); iter++) {
		QString a1 = iter->first.c_str();
		QString a2 = iter->second.c_str();
		QTreeWidgetItem* newItem = new QTreeWidgetItem(formatList);
		newItem->setText(0, a1);
		newItem->setText(1, a2);
		formatList->addTopLevelItem(newItem);
	}
	 
	formatList->setStyleSheet("QTreeWidget::item{height:30px}");// 设置行距大小，可以防止编译修改完成以后，行距会发生变化
	// 双击数据时,启用编辑功能
	connect(formatList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(treeWidgetOpenEditor(QTreeWidgetItem*, int)));
	// 编辑完毕后,关闭编辑功能并保存
	connect(formatList, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(treeWidgetCloseEditor()));
	// 删除按钮点击后,删除选中的项目
	connect(&formatDel, SIGNAL(clicked()), this, SLOT(delItem()));
	// 添加按钮点击后,添加项目
	connect(&formatAdd, SIGNAL(clicked()), this, SLOT(addItemAction()));
	// 关闭按钮点击后,关闭窗口
	connect(&forClose, SIGNAL(clicked()), &fixFormatWindow, SLOT(close()));
	fixFormatWindow.exec();
	// 检测到此窗口关闭后,更新下拉框的数据
	if (fixFormatWindow.close()) {
		updateCombobox();
	}
	delete formatList;
}
void MainWindow::delItem() {
	// 获取点击的项目
	QTreeWidgetItem* pCurrentItem = MainWindow::formatList->currentItem();
	if (pCurrentItem == nullptr) { // 判断是否选中项目
		QApplication::beep();
		QMessageBox::warning(mainwindow, "警告", "当前没有被选中的项目!");
		return;
	}
	QString strText = pCurrentItem->text(0); // keyValue
	dict.erase(strText.toLocal8Bit().data()); // 删除容器中对应的数据
	save(); // 保存所作的更改
	delete pCurrentItem; // 将该项目从列表中删除
}
void MainWindow::addItemAction() {
	add = new QDialog(mainwindow);
	add->setWindowTitle("添加");
	add->setFixedSize(530, 50);
	fName = new QLineEdit(add);
	fName->setGeometry(40, 15, 120, 25);
	fData = new QLineEdit(add);
	fData->setGeometry(250, 15, 120, 25);
	QLabel text_1("格式:", add);
	text_1.setGeometry(10, 15, 30, 25);
	QLabel text_2("十六进制数:", add);
	text_2.setGeometry(185, 15, 65, 25);
	QPushButton confirm(" 添加并关闭", add);
	confirm.setDefault(false);
	confirm.setIcon(QIcon("Images//submit.ico"));
	confirm.setGeometry(400, 15, 120, 25);
	// 点击确认按钮后,将其添加到列表中
	QObject::connect(&confirm, &QPushButton::clicked, this, &MainWindow::addItemActionConfirm);
	add->exec();
	add->setAttribute(Qt::WA_DeleteOnClose); // 设置该窗口关闭后自动delete
	delete fName;
	delete fData;
}
void MainWindow::addItemActionConfirm() {
	string sName = fName->text().toLocal8Bit().data(); // 获取文本框的内容
	string sData = fData->text().toLocal8Bit().data();
	regex OnlyWordNumberExpression("^[a-zA-Z0-9]+$"); // 判断输入的内容是否符合格式要求,只能输入字母或数字
	if (!regex_match(sName, OnlyWordNumberExpression)) {
		QApplication::beep();
		QMessageBox::warning(mainwindow, "输入错误", "添加失败\n格式位置只能输入字母和数字\n或内容为空");
		return;
	}
	if (sName != "" || sData != "") { // 判断输入内容是否为空
		regex HexExpression("[0-9a-fA-F]{8,8}");
		if (regex_match(sData, HexExpression)) { // 判断输入的内容是否符合格式要求,只能输入十六进制数
			dict.insert(make_pair(sName, sData)); // 将数据添加到容器内
			QTreeWidgetItem* newItem = new QTreeWidgetItem(formatList); 
			newItem->setText(0, fName->text());
			newItem->setText(1, fData->text()); 
			formatList->addTopLevelItem(newItem); // 将该数据添加到列表中
			add->close();
			save(); // 保存所作的更改
		}
		else {
			QApplication::beep();
			QMessageBox::warning(mainwindow, "输入错误", "添加失败\n十六进制数位置请输入8位十六进制数(不用带0x)\n或内容为空");
			return;
		}
	}
}

//void MainWindow::addToDict(QTreeWidgetItem* newItemAdd) {
//	formatList->closePersistentEditor(newItemAdd, 0);
//	formatList->closePersistentEditor(newItemAdd, 1);
//	QString key = newItemAdd->text(0);
//	QString value = newItemAdd->text(1);
//	dict.insert(make_pair(key.toLocal8Bit().data(), value.toLocal8Bit().data()));
//	save();
//}
void MainWindow::treeWidgetOpenEditor(QTreeWidgetItem* item, int col)
{	// 双击后,该项目开启编辑模式
	formatList->openPersistentEditor(item, 1);
	m_myItem = item;
	m_myCol = col;
}
void MainWindow::treeWidgetCloseEditor()
{	// 判断是否传入项目
	if (m_myItem != NULL) {
		regex HexExpression("[0-9a-fA-F]{8,8}");// 判断是否符合格式要求(8位十六进制数,不带0x)
		string content = m_myItem->text(1).toLocal8Bit().data(); // 获取修改后的内容
		if (regex_match(content, HexExpression)) {
			formatList->closePersistentEditor(m_myItem, m_myCol); // 关闭该项目的编辑模式
			QString key = m_myItem->text(0);
			QString value = m_myItem->text(1);
			dict.find(key.toLocal8Bit().data())->second = value.toLocal8Bit().data(); // 修改数据到容器中
			save(); // 保存所作的更改
		}
		else {
			QApplication::beep();
			QMessageBox::warning(mainwindow, "输入错误","请输入8位十六进制数(不用带0x)\n或内容为空");
		}
	}
} //https://blog.csdn.net/qq78442761/article/details/80454869

void MainWindow::showAboutQtWindow() {
	QMessageBox::aboutQt(mainwindow); // 显示关于qt界面
}

void MainWindow::showBiliBiliDetails() {
	QUrl url("https://space.bilibili.com/474393760"); // 打开bilibili主页
	QDesktopServices::openUrl(url);
}

void MainWindow::showOpenUrl() {
	QUrl url("https://github.com/15dd/File-Header-Forge-for-CPP-Edition"); // 打开github项目主页
	QDesktopServices::openUrl(url);
}



