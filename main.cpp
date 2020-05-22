#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <regex>
#include <string>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

#define FILENAME "contacts.txt"
#define TABLE_SIZE 100

template < typename T >
void print_with_width(T t, const int &width, const char prefix,
                      const char separator = ' ') {
    std::cout << prefix << std::left << std::setw(width)
              << std::setfill(separator) << t;
}

bool is_valid_email(const std::string &email) {
    const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

    return std::regex_match(email, pattern);
}

const std::string trim(const std::string &str,
                       const std::string &whitespace = " \t") {
    const size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) {
        return "";
    }

    const size_t end = str.find_last_not_of(whitespace);
    const size_t range = end - start + 1;

    return str.substr(start, range);
}

class person {
    std::string id;
    std::string fname;
    std::string lname;
    std::string email;
    std::string phone;
    std::string occupation;
    std::string area;

public:
    person() {}
    ~person() {}

    const std::string getid(void);
    void read(const std::string id);
    void update(void);
    const std::string tostring(void);
    void unpack(std::fstream &);
    void display(void);
    void display_table(const int index);
};

class hashtable {
    int *capacity;
    std::list< person * > *table;

    const int hash_function(const std::string key) {
        int value = 0;
        for (int i = 0; i < key.length(); i++) {
            value += key[i] * (i + 1);
        }
        int index = value % *capacity;
        return index;
    }

public:
    hashtable(int V)
        : capacity(new int(V)), table(new std::list< person * >[*capacity]){};
    ~hashtable() {
        for (int i = 0; i < *capacity; i++) {
            table[i].clear();
        }
        delete[] table;
        delete capacity;
    }

    void set(person *);
    person *get(const std::string);
    void remove(const std::string);
    void display(void);
    void tofile(std::fstream &);
};

class file {
    std::string filename;

public:
    file(const std::string V) : filename(V) {}
    ~file() {}

    void read(hashtable *);
    void write(hashtable *);
};

const std::string person::getid(void) { return id; }

void person::read(const std::string id) {
    this->id = id;

    std::cout << "Enter first name: ";
    while (fname.empty()) {
        getline(std::cin, fname);
        fname = trim(fname);
    }

    std::cout << "Enter last name: ";
    while (lname.empty()) {
        getline(std::cin, lname);
        lname = trim(lname);
    }

    std::cout << "Enter email address: ";
    while (email.empty()) {
        getline(std::cin, email);
        email = trim(email);
        if (!is_valid_email(email)) {
            email.clear();
            std::cout << "Enter a valid email address: ";
        }
    }

    std::cout << "Enter phone number: ";
    while (phone.empty()) {
        getline(std::cin, phone);
        phone = trim(phone);
    }

    std::cout << "Enter occupation: ";
    while (occupation.empty()) {
        getline(std::cin, occupation);
        occupation = trim(occupation);
    }

    std::cout << "Enter area: ";
    while (area.empty()) {
        getline(std::cin, area);
        area = trim(area);
    }
}

void person::update(void) {
    std::string buffer;
    std::string temp_fname;
    std::string temp_lname;
    std::string temp_email;
    std::string temp_phone;
    std::string temp_occupation;
    std::string temp_area;

    while (true) {
        std::cout << "Enter first name: (" << fname << ") ";
        getline(std::cin, temp_fname);

        std::cout << "Enter last name: (" << lname << ") ";
        getline(std::cin, temp_lname);

        std::cout << "Enter email address: (" << email << ") ";
        do {
            getline(std::cin, temp_email);
        } while (!trim(temp_email).empty() && !is_valid_email(temp_email));

        std::cout << "Enter phone number: (" << phone << ") ";
        getline(std::cin, temp_phone);

        std::cout << "Enter occupation: (" << occupation << ") ";
        getline(std::cin, temp_occupation);

        std::cout << "Enter area: (" << area << ") ";
        getline(std::cin, temp_area);

        std::cout << std::endl;
        std::cout << "About to write to hash table:" << std::endl << std::endl;
        std::cout << "ID: " << id << std::endl;
        std::cout << "First name: "
                  << (trim(temp_fname).empty() ? fname : trim(temp_fname))
                  << std::endl;
        std::cout << "Last name: "
                  << (trim(temp_lname).empty() ? lname : trim(temp_lname))
                  << std::endl;
        std::cout << "Email address: "
                  << (trim(temp_email).empty() ? email : trim(temp_email))
                  << std::endl;
        std::cout << "Phone number: "
                  << (trim(temp_phone).empty() ? phone : trim(temp_phone))
                  << std::endl;
        std::cout << "Occupation: "
                  << (trim(temp_occupation).empty() ? occupation
                                                    : trim(temp_occupation))
                  << std::endl;
        std::cout << "Area: "
                  << (trim(temp_area).empty() ? area : trim(temp_area))
                  << std::endl;
        std::cout << std::endl;
        std::cout << "Is this OK? (yes) ";
        getline(std::cin, buffer);
        buffer = trim(buffer);
        transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
        if (!buffer.empty() && buffer.compare("yes") != 0) {
            system(CLEAR);
            continue;
        }

        fname = trim(temp_fname).empty() ? fname : trim(temp_fname);
        lname = trim(temp_lname).empty() ? lname : trim(temp_lname);
        email = trim(temp_email).empty() ? email : trim(temp_email);
        phone = trim(temp_phone).empty() ? phone : trim(temp_phone);
        occupation =
            trim(temp_occupation).empty() ? occupation : trim(temp_occupation);
        area = trim(temp_area).empty() ? area : trim(temp_area);
        break;
    }
}

const std::string person::tostring(void) {
    std::string buffer;
    buffer.append(id);
    buffer.append("|");
    buffer.append(fname);
    buffer.append("|");
    buffer.append(lname);
    buffer.append("|");
    buffer.append(email);
    buffer.append("|");
    buffer.append(phone);
    buffer.append("|");
    buffer.append(occupation);
    buffer.append("|");
    buffer.append(area);
    buffer.append("\n");
    return buffer;
}

void person::unpack(std::fstream &fs) {
    getline(fs, id, '|');
    getline(fs, fname, '|');
    getline(fs, lname, '|');
    getline(fs, email, '|');
    getline(fs, phone, '|');
    getline(fs, occupation, '|');
    getline(fs, area, '\n');
}

void person::display(void) {
    std::cout << "ID: " << id << std::endl;
    std::cout << "First name: " << fname << std::endl;
    std::cout << "Last name: " << lname << std::endl;
    std::cout << "Email address: " << email << std::endl;
    std::cout << "Phone number: " << phone << std::endl;
    std::cout << "Occupation: " << occupation << std::endl;
    std::cout << "Area: " << area << std::endl;
    std::cout << std::endl;
}

void person::display_table(const int index) {
    std::cout << std::endl;
    std::cout << "INDEX: " << index << std::endl;

    print_with_width("", 17, '+', '-');
    print_with_width("", 30, '+', '-');
    std::cout << '+' << std::endl;

    print_with_width("Id", 17, '|');
    print_with_width(id, 30, '|');
    std::cout << '|' << std::endl;

    print_with_width("", 17, '+', '-');
    print_with_width("", 30, '+', '-');
    std::cout << '+' << std::endl;

    print_with_width("First name", 17, '|');
    print_with_width(fname, 30, '|');
    std::cout << '|' << std::endl;

    print_with_width("", 17, '+', '-');
    print_with_width("", 30, '+', '-');
    std::cout << '+' << std::endl;

    print_with_width("Last name", 17, '|');
    print_with_width(lname, 30, '|');
    std::cout << '|' << std::endl;

    print_with_width("", 17, '+', '-');
    print_with_width("", 30, '+', '-');
    std::cout << '+' << std::endl;

    print_with_width("Email address", 17, '|');
    print_with_width(email, 30, '|');
    std::cout << '|' << std::endl;

    print_with_width("", 17, '+', '-');
    print_with_width("", 30, '+', '-');
    std::cout << '+' << std::endl;

    print_with_width("Phone number", 17, '|');
    print_with_width(phone, 30, '|');
    std::cout << '|' << std::endl;

    print_with_width("", 17, '+', '-');
    print_with_width("", 30, '+', '-');
    std::cout << '+' << std::endl;

    print_with_width("Occupation", 17, '|');
    print_with_width(occupation, 30, '|');
    std::cout << '|' << std::endl;

    print_with_width("", 17, '+', '-');
    print_with_width("", 30, '+', '-');
    std::cout << '+' << std::endl;

    print_with_width("Area", 17, '|');
    print_with_width(area, 30, '|');
    std::cout << '|' << std::endl;

    print_with_width("", 17, '+', '-');
    print_with_width("", 30, '+', '-');
    std::cout << '+' << std::endl;
}

void hashtable::set(person *p) {
    std::string key = (*p).getid();
    if (key.length() == 0) {
        return;
    }

    int index = hash_function(key);
    table[index].push_back(p);
}

person *hashtable::get(std::string const key) {
    int index = hash_function(key);

    std::list< person * >::iterator it;
    for (it = table[index].begin(); it != table[index].end(); it++) {
        if (key.compare((**it).getid()) == 0) {
            return *it;
        }
    }

    return NULL;
}

void hashtable::remove(std::string const key) {
    int index = hash_function(key);

    std::list< person * >::iterator it;
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
        std::list< person * >::iterator it;
        for (it = table[i].begin(); it != table[i].end(); it++) {
            if (*it != NULL) {
                (**it).display_table(i);
            }
        }
    }
}

void hashtable::tofile(std::fstream &fs) {
    for (int i = 0; i < *capacity; i++) {
        std::list< person * >::iterator it;
        for (it = table[i].begin(); it != table[i].end(); it++) {
            fs << (**it).tostring();
        }
    }
}

void file::read(hashtable *ht) {
    std::fstream fs;
    fs.open(filename, std::ios::in);
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
    std::fstream fs;
    fs.open(filename, std::ios::out);
    (*ht).tofile(fs);
    fs.close();
}

void press_enter(void) {
    std::cout << std::endl << "Press any key to continue";
    getchar();
    system(CLEAR);
}

void new_contact(hashtable *ht, file *f) {
    person *p = new person();
    std::string id;
input:
    std::cout << "Enter id: ";
    while (id.empty()) {
        getline(std::cin, id);
        id = trim(id);
    }
    if ((*ht).get(id) != NULL) {
        std::cout << "ID already used. Enter a new one." << std::endl;
        press_enter();
        system(CLEAR);
        goto input;
    }

    (*p).read(id);
    (*ht).set(p);
    (*f).write(ht);
    std::cout << std::endl;
    std::cout << "Contact created." << std::endl;
}

void search(hashtable *ht) {
    std::string key;
    std::cout << "Enter id to search: ";
    getline(std::cin, key);
    person *p = (*ht).get(key);
    if (p == NULL) {
        std::cout << "No such contact found." << std::endl;
        return;
    }

    std::cout << std::endl;
    (*p).display();
}

void update_contact(hashtable *ht, file *f) {
    std::string key;
    std::cout << "Enter id to modify: ";
    getline(std::cin, key);
    person *p = (*ht).get(key);
    if (p == NULL) {
        std::cout << "No such contact found." << std::endl;
        return;
    }

    (*p).update();
    (*f).write(ht);
    std::cout << "Contact updated." << std::endl;
}

void delete_contact(hashtable *ht, file *f) {
    std::string key;
    std::cout << "Enter id to delete: ";
    getline(std::cin, key);
    (*ht).remove(key);
    (*f).write(ht);
    std::cout << "Contact deleted." << std::endl;
}

void welcome(void) {
    system(CLEAR);
    std::cout << "Welcome to Address Book Management System" << std::endl;
    press_enter();
    system(CLEAR);
}

int main(int argc, const char **argv) {
    hashtable *ht = new hashtable(TABLE_SIZE);
    file *f = new file(FILENAME);

    (*f).read(ht);

    welcome();

    int ch;
loop:
    std::cout << "1. New contact" << std::endl
              << "2. Display all" << std::endl
              << "3. Search" << std::endl
              << "4. Modify" << std::endl
              << "5. Delete" << std::endl
              << "6. Exit" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> ch;
    std::cin.ignore();
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
        std::cout << "Invalid choice." << std::endl;
        break;
    }
    press_enter();
    goto loop;

exit:
    delete f;
    delete ht;
    return 0;
}
