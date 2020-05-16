#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

#define FILENAME "contacts.txt"
#define TABLE_SIZE 100

using namespace std;

class person {
    string id;
    string fname;
    string lname;
    string email;
    string phone;

public:
    person() {}
    ~person() {}

    const string getid(void);
    void read(void);
    void update(void);
    const string tostring(void);
    void unpack(fstream &);
    void display(void);
};

class hashtable {
    int *capacity;
    list< person * > *table;

    const int hash_function(const string key) {
        int value = 0;
        for (int i = 0; i < key.length(); i++) {
            value += key[i] * (i + 1);
        }
        int index = value % *capacity;
        return index;
    }

public:
    hashtable(int V)
        : capacity(new int(V)), table(new list< person * >[*capacity]){};
    ~hashtable() {
        for (int i = 0; i < *capacity; i++) {
            table[i].clear();
        }
        delete[] table;
        delete capacity;
    }

    void set(person *);
    person *get(const string);
    void remove(const string);
    void display(void);
    void tofile(fstream &);
};

class file {
    string filename;

public:
    file(const string V) : filename(V) {}
    ~file() {}

    void read(hashtable *);
    void write(hashtable *);
};

const string person::getid(void) { return id; }

void person::read(void) {
    cin.ignore();
    cout << "Enter id: ";
    getline(cin, id);
    cout << "Enter first name: ";
    getline(cin, fname);
    cout << "Enter last name: ";
    getline(cin, lname);
    cout << "Enter email address: ";
    getline(cin, email);
    cout << "Enter phone number: ";
    getline(cin, phone);
}

void person::update(void) {
    cin.ignore();
    cout << "Enter updated first name: ";
    getline(cin, fname);
    cout << "Enter updated last name: ";
    getline(cin, lname);
    cout << "Enter updated email address: ";
    getline(cin, email);
    cout << "Enter updated phone number: ";
    getline(cin, phone);
}

const string person::tostring(void) {
    string buffer;
    buffer.append(id);
    buffer.append("|");
    buffer.append(fname);
    buffer.append("|");
    buffer.append(lname);
    buffer.append("|");
    buffer.append(email);
    buffer.append("|");
    buffer.append(phone);
    buffer.append("\n");
    return buffer;
}

void person::unpack(fstream &fs) {
    getline(fs, id, '|');
    getline(fs, fname, '|');
    getline(fs, lname, '|');
    getline(fs, email, '|');
    getline(fs, phone, '\n');
}

void person::display(void) {
    cout << "ID: " << id << endl;
    cout << "First name: " << fname << endl;
    cout << "Last name: " << lname << endl;
    cout << "Email address: " << email << endl;
    cout << "Phone number: " << phone << endl;
    cout << endl;
}

void hashtable::set(person *p) {
    string key = (*p).getid();
    if (key.length() == 0) {
        return;
    }

    int index = hash_function(key);
    table[index].push_back(p);
}

person *hashtable::get(string const key) {
    int index = hash_function(key);

    list< person * >::iterator it;
    for (it = table[index].begin(); it != table[index].end(); it++) {
        if (key.compare((**it).getid()) == 0) {
            return *it;
        }
    }

    return NULL;
}

void hashtable::remove(string const key) {
    int index = hash_function(key);

    list< person * >::iterator it;
    for (it = table[index].begin(); it != table[index].end(); it++) {
        if (key.compare((**it).getid()) == 0) {
            break;
        }
    }

    if (it != table[index].end()) {
        table[index].erase(it);
    }
}

void hashtable::display(void) {
    for (int i = 0; i < *capacity; i++) {
        list< person * >::iterator it;
        for (it = table[i].begin(); it != table[i].end(); it++) {
            cout << "INDEX: " << i << endl << endl;
            if (*it != NULL) {
                (**it).display();
            }
        }
    }
}

void hashtable::tofile(fstream &fs) {
    for (int i = 0; i < *capacity; i++) {
        list< person * >::iterator it;
        for (it = table[i].begin(); it != table[i].end(); it++) {
            fs << (**it).tostring();
        }
    }
}

void file::read(hashtable *ht) {
    fstream fs;
    fs.open(filename, ios::in);
    if (!fs.is_open()) {
        return;
    }

    while (!fs.eof()) {
        person *p = new person();
        (*p).unpack(fs);
        (*ht).set(p);
    }
    fs.close();
}

void file::write(hashtable *ht) {
    fstream fs;
    fs.open(filename, ios::out);
    (*ht).tofile(fs);
    fs.close();
}

void press_enter(void) {
    cin.ignore();
    cout << endl << "Press any key to continue";
    getchar();
    system(CLEAR);
}

void new_contact(hashtable *ht, file *f) {
    person *p = new person();
    (*p).read();
    (*ht).set(p);
    (*f).write(ht);
    cout << "Contact created." << endl;
}

void search(hashtable *ht) {
    string key;
    cin.ignore();
    cout << "Enter id to search: ";
    getline(cin, key);
    person *p = (*ht).get(key);
    if (p == NULL) {
        cout << "No such contact found." << endl;
        return;
    }

    cout << "Contact found." << endl << endl;
    (*p).display();
}

void update_contact(hashtable *ht, file *f) {
    string key;
    cin.ignore();
    cout << "Enter id to modify: ";
    getline(cin, key);
    person *p = (*ht).get(key);
    if (p == NULL) {
        cout << "No such contact found." << endl;
        return;
    }

    cout << "Contact found." << endl << endl;
    (*p).display();
    cout << "Enter new values:" << endl;
    (*p).update();
    (*f).write(ht);
    cout << "Contact updated." << endl;
}

void delete_contact(hashtable *ht, file *f) {
    string key;
    cin.ignore();
    cout << "Enter id to delete: ";
    getline(cin, key);
    (*ht).remove(key);
    (*f).write(ht);
    cout << "Contact deleted." << endl;
}

int main(int argc, const char **argv) {
    hashtable *ht = new hashtable(TABLE_SIZE);
    file *f = new file(FILENAME);

    (*f).read(ht);
    system(CLEAR);

loop:
    int ch;
    cout << "1. New contact" << endl
         << "2. Display all" << endl
         << "3. Search" << endl
         << "4. Modify" << endl
         << "5. Delete" << endl
         << "6. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> ch;
    system(CLEAR);

    switch (ch) {
    case 1:
        new_contact(ht, f);
        break;
    case 2:
        (*ht).display();
        break;
    case 3:
        search(ht);
        break;
    case 4:
        update_contact(ht, f);
        break;
    case 5:
        delete_contact(ht, f);
        break;
    case 6:
        goto exit;
        break;
    default:
        cout << "Invalid choice." << endl;
        break;
    }
    press_enter();
    goto loop;

exit:
    delete f;
    delete ht;
    return 0;
}