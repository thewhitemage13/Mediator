#include <iostream>
using namespace std;

class Employee;

__interface IMediator {
	virtual void Notify(Employee* emp, string msg) = 0;
};

class Employee {
protected:
	IMediator* mediator;
public:
	Employee(IMediator* med = nullptr) : mediator(med){}
	void SetMediator(IMediator* med) {
		mediator = med;
	}
	virtual ~Employee(){}
};

class Designer : public Employee {
private:
	bool is_working;
public:
	Designer(IMediator* med = nullptr) : Employee(med){}
	void ExecuterWork() {
		cout << "<-Designer at work\n";
		mediator->Notify(this, "The designer designs...");
	}
	void SetWork(bool state) {
		is_working = state;
		if (state)
			cout << "<-The designer is relieved\n";
		else
			cout << "<-The designer's busy\n";
	}
};

class Director : public Employee {
private:
	string text;
public:
	Director(IMediator* med = nullptr) : Employee(med){}
	void GiveCommand(string txt) {
		text = txt;
		if (text.empty())
			cout << "->The director knows that the designer is already on the job\n";
		else
			cout << "->The director gave the command: " << text << "\n";
		mediator->Notify(this, text);
	}
};

class Controller : public IMediator {
private:
	Designer* designer;
	Director* director;
public:
	Controller(Designer* des, Director* dir) {
		designer = des;
		director = dir;
		designer->SetMediator(this);
		director->SetMediator(this);
	}
	void Notify(Employee* emp, string msg) override {
		if (auto dir = dynamic_cast<Director*> (emp)) {
			if (msg.empty()) {
				designer->SetWork(false);
			}
			else{
				designer->SetWork(true);
			}
		}
		if (auto des = dynamic_cast<Designer*>(emp)) {
			if (msg == "designer designs...") {
				director->GiveCommand("");
			}
		}
	}
};

int main() {
	Designer* designer = new Designer();
	Director* director = new Director();

	Controller* mediator = new Controller(designer, director);
	director->GiveCommand("design");
	cout << "\n";
	designer->ExecuterWork();
	delete designer;
	delete director;
	delete mediator;
}