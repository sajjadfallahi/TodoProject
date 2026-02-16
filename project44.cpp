//in the name of allah
//Sajjad Fallahi

//ostad khorashadi
#include <iostream>
#include <vector>//ذخیره لیست کارها
#include <string>
#include <fstream>//خواندن و ذخیره فایل ها

using namespace std;


enum Priority { LOW = 1, MEDIUM, HIGH };

//کلاس پایه Task 
class Task {
protected: // استفاده از پروتکتد برای دسترسی در کلاس فرزند
    string Title;
    string Description;
    bool isDone;
    string DateCreated; // تاریخ ایجاد
    string Deadline;
    int TaskPriority; // Priority: Low, Medium, High

public:
    
    Task(string t, string d, string dl, int p, bool status = false) {
        Title = t;
        Description = d;
        Deadline = dl;
        TaskPriority = p;
        isDone = status;
        DateCreated = "1404/11/15"; 
    }

    // تابع تغییر وضعیت انجام کار /////////////******(toggle status)
    void changeStatus() {
        isDone = !isDone;
    }
//Sajjad Fallahi
    // تابع نمایش اطلاعات کار *****
    virtual void displayInfo() {
        cout << "-----------------------------------" << endl;
        cout << "Title: " << Title << endl;
        cout << "Description: " << Description << endl;
        cout << "Deadline: " << Deadline << endl;
        cout << "Priority: ";
        if (TaskPriority == 1) cout << "Low" << endl;
        else if (TaskPriority == 2) cout << "Medium" << endl;
        else cout << "High" << endl;
        cout << "Status: " << (isDone ? "Done (Anjam Shode)" : "Not Done (Mande)") << endl;
    }

    // Getter ها برای استفاده در مدیریت لیست
    bool getStatus() const { return isDone; }
    string getDeadline() const { return Deadline; }
    string getTitle() const { return Title; }
    int getPriority() const { return TaskPriority; }

    virtual ~Task() {} // Virtual Destructor
};

// --- کلاس Todo مشتق شده از Task 
class Todo : public Task {
public:
    // ارث‌بری از سازنده کلاس پایه
    Todo(string t, string d, string dl, int p, bool s = false) 
        : Task(t, d, dl, p, s) {}
//کلاس تودو  اطلاعات رو به کلاس مادر یعنی تسک می فرستد.    
};

// --- کلاس TodoList برای مدیریت مجموعه‌ای از Taskها ******
class TodoList {
private:
    vector<Todo*> tasks; //این وکتور لیستی از اشاره‌گر به تودو ذخیره می‌کند.
    string fileName = "projects_data.txt";

public:
    TodoList() { loadFromFile(); }//تابع سازنده اطلاعات رو از فایل می گیرد

    // تابع ذخیره در فایل
    void saveToFile() {
        ofstream out(fileName);
        for (auto t : tasks) {
            out << t->getTitle() << "|" << "Desc" << "|" << t->getDeadline() << "|" 
                << t->getPriority() << "|" << t->getStatus() << endl;
        }
        out.close();
    }

    // تابع خواندن از فایل
    void loadFromFile() {
        ifstream in(fileName);
        if (!in.is_open()) return;
        string t, d, dl, p_s, s_s;
        while (getline(in, t, '|') && getline(in, d, '|') && getline(in, dl, '|') 
               && getline(in, p_s, '|') && getline(in, s_s)) {
            tasks.push_back(new Todo(t, d, dl, stoi(p_s), s_s == "1"));
        }
        in.close();
    }

    // 1. افزودن Task جدید
    void addTask() {
        string t, d, dl;
        int p;
        cout << "Title: "<<endl;
         cin.ignore(); 
         getline(cin, t);
        cout << "Description: "<<endl;
         getline(cin, d);
        cout << "Deadline (YYYY/MM/DD): "<<endl; 
        getline(cin, dl);
        cout << "Priority (1:Low, 2:Med, 3:High): "<<endl;
         cin >> p;
        tasks.push_back(new Todo(t, d, dl, p));
        saveToFile();
        cout << "Task added" << endl;
    }
//Sajjad Fallahi

    // 2. نمایش همه Taskها
    void showAll() {
        if (tasks.empty()) { cout << "List  is very!" << endl; return; }//بررسی خالی بودن
        for (int i = 0; i< tasks.size(); i++) {
            cout << i + 1 << ". ";
            tasks[i]->displayInfo();
        }
    }

    // 3. نمایش بر اساس وضعیت (Done / Not Done)//این تابع رو یه مشکلی دارم
    void showByStatus(bool filterDone) {
        for (auto t : tasks) {//range-based for loop//شبیه پایتون //روی همه تسک ها حرکت می کند.
            if (t->getStatus() == filterDone) t->displayInfo();//
        }
    }
    // 4. نمایش Taskهای عقب‌افتاده
    void showOverdue() {
        string emroz = "1404/11/15"; 
        cout << "--- Karhaye Aghab Oftade ---" << endl;
        for (auto t : tasks) {
            if (t->getDeadline() < emroz && !t->getStatus()) t->displayInfo();
        }
    }
//Sajjad Fallahi

    // 5. تغییر وضعیت یک کار
    void changeTaskStatus() {

        if (tasks.empty()) {
            cout << "No tasks available.\n";
            return;
        }

        showAll();

        int idx;
        cout << "Task number: ";

        if (!(cin >> idx)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input!\n";
            return;
        }

        if (idx < 1 || idx > tasks.size()) {
            cout << "Invalid task number!\n";
            return;
        }

        tasks[idx - 1]->changeStatus();
        saveToFile();

        cout << "Status changed successfully!\n";
    }


    // 6. حذف یک کار
    void deleteTask() {
    if (tasks.empty()) {
        cout << "No tasks to delete.\n";
        return;
    }

    showAll();

    int idx;
    cout << "Task number to delete: ";

    if (!(cin >> idx)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input ."<<endl;
        return;
    }

    if (idx < 1 || idx > tasks.size()) {
        cout << "Invalid task number ."<<endl;
        return;
    }

    delete tasks[idx - 1];
    tasks.erase(tasks.begin() + (idx - 1));
    saveToFile();

    cout << "Task deleted successfully . "<<endl;
}


    // مرتب‌سازی بر اساس اولویت (Sorting)//Bubble Sort
    void sortByPriority() {
        for (int i = 0; i< tasks.size(); i++) {
            for (int j = 0; j < (int)tasks.size() - i - 1; j++) {
                if (tasks[j]->getPriority() < tasks[j + 1]->getPriority()) {
                    swap(tasks[j], tasks[j + 1]);
                }
            }
        }
        cout << "List moratab shod (Priority High to Low)." << endl;
    }
};
//Sajjad Fallahi


// --- منوی تعاملی ///******/// */
int main() {
    TodoList myTodoSystem;
    int command;

    while (true) {
        cout << "\n ========= Todo List Menu =========\n" << endl;
        cout << " 1. Add New Task" << endl;
        cout << " 2. Show All Tasks" << endl;
        cout << " 3. Show Done/Not Done Tasks" << endl;
        cout << " 4. Show Overdue Tasks" << endl;
        cout << " 5. Change Task Status" << endl;
        cout << " 6. Delete a Task" << endl;
        cout << " 7. Exit" << endl;
        cout << " Enter Choice: ";
        cin >> command;

        switch (command) {
            case 1: myTodoSystem.addTask(); break;
            case 2: myTodoSystem.showAll(); break;
            case 3: 
                int s; cout << "0 for Not Done, 1 for Done: "; cin >> s;
                myTodoSystem.showByStatus(s == 1);
                break;
            case 4: myTodoSystem.showOverdue(); break;
            case 5: myTodoSystem.changeTaskStatus(); break;
            case 6: myTodoSystem.deleteTask(); break;
            case 7: cout << "goodby" << endl; return 0;
            default: cout << "error, do try again ." << endl;
        }
    }
    return 0;
}
//Sajjad Fallahi
