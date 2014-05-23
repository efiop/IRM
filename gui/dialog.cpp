#include "dialog.h"
#include "ui_dialog.h"


using namespace std;
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui (new Ui::Dialog)
{
    ui->setupUi(this);
    //ui->label_4->setVisible(FALSE);
    ui->label_4->setVisible(false);
    NoConnection = false;


}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked(){
    this->close();
}

string Dialog::wget(string i) {
    string s1 = "character=tab&Method=1&ZNum=";
    string s2 = "&OutOpt=PIC&Uploadflag=on&Output=on&WindowXmin=0.001&WindowXmax=0.01&with=on";
    string s3 = i;
    string stro;
	int status;
	if (fork() == 0)
		if (execl("/usr/bin/wget", "wget", "-E",
		"--restrict-file-name=windows","-O", DATA,
        "--post-data", (s1+s3+s2).c_str(),
		"http://physics.nist.gov/cgi-bin/Xcom/data.pl",
		NULL) == -1);
	wait(&status);
	if (WEXITSTATUS(status)) {
           NoConnection = true;
		cout << "Internet error" << endl;
	}
	else {
		ifstream st;
		st.open(DATA);
		if ((st.rdstate() & ifstream::failbit) != 0) {
			cerr << "Error can't open " << DATA << endl;
			exit(0);
		}
        getline(st, stro);
		if ((st.rdstate() & !ifstream::goodbit) != 0) {
			cerr << "Error can't read from " << DATA << endl;
			exit(0);
		}
		getline(st, stro);
		if ((st.rdstate() & !ifstream::goodbit) != 0) {
			cerr << "Error can't read from " << DATA << endl;
			exit(0);
		}
		getline(st, stro);
		if ((st.rdstate() & !ifstream::goodbit) != 0) {
			cerr << "Error can't read from " << DATA << endl;
			exit(0);
		}
		st >> stro;
		if ((st.rdstate() & !ifstream::goodbit) != 0) {
			cerr << "Error can't read from " << DATA << endl;
			exit(0);
		}
		float i;
//		st >> stro;
		st >> stro;
		if ((st.rdstate() & !ifstream::goodbit) != 0) {
			cerr << "Error can't read from " << DATA << endl;
			exit(0);
		}
//		cout << i << endl;
		return stro;
//		cout << stro << endl;
	}
}
	
void Dialog::on_pushButton_2_clicked(){
    string z, z1;
    z = (ui->lineEdit->text()).toStdString();
    z1= wget(z);
    ui->label_3->setText(QString::fromStdString(z1));
    if (NoConnection == true){
       ui->label_4->setVisible(true);
    }
}
