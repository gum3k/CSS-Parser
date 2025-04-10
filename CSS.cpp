#include <iostream>
#include <string>
#include <string.h>

using namespace std;

int porownaj_strukture_komendy_przecinek(const char* target, char* przecinek)
{
    return przecinek != 0 && strcmp(przecinek, target) == 0;
}

const int MAXCSS = 1000000;
const int MAXLINE = 1000;
struct Attribute {
    char name[MAXLINE];
    char value[MAXLINE];
    Attribute* prev;
    Attribute* next;
};

struct Selector {
    char name[MAXLINE];
    Selector* prev;
    Selector* next;
};

struct Section {
    Selector* selectors[100] = { 0 };
    Attribute* atributes[100] = { 0 };
    unsigned int id;
    Section* prev;
    Section* next;
};

struct Array {
    Section* array[8] = { 0 };
    Array* prev;
    Array* next;
};

Section* add_section(Section* tail, unsigned int id) {
    Section* node = new Section;
    node->id = id;
    node->prev = tail;
    node->next = nullptr;
    if (tail != nullptr) {
        tail->next = node;
    }
    return node;
}


Selector* add_selector(Selector* tail, char* name) {
    Selector* node = new Selector;
    strncpy_s(node->name, name, MAXLINE - 1);
    node->name[MAXLINE - 1] = '\0'; 
    node->prev = tail;
    node->next = nullptr;
    if (tail != nullptr) {
        tail->next = node;
    }
    return node;
}

Attribute* add_attribute(Attribute* tail, char* name, char* value) {
    Attribute* node = new Attribute;
    strncpy_s(node->name, name, MAXLINE - 1);
    strncpy_s(node->value, value, MAXLINE - 1);
    node->prev = tail;
    node->next = nullptr;
    if (tail != nullptr) {
        tail->next = node;
    }
    return node;
}

Array* add_array(Array* tail) {
    Array* node = new Array;
    node->prev = tail;
    node->next = nullptr;
    if (tail != nullptr) {
        tail->next = node;
    }
    return node;
}

void clearArray(char* a, int n) {
    memset(a, 0, n);
}

void printString(char* string, int n) {
    for (int i = 0; i < n; i++) {
        if (string[i] == 0)
            break;
        cout << string[i];
    }
}
void cssRead(char* cssCode) {
    char endMarker[] = "????";
    int index = 0;
    char c;
    while (index < MAXCSS) {
        c = getchar(); // wczytaj pojedynczy znak ze strumienia wejściowego

        if (c == EOF) break; // przerwij jeśli napotkano koniec pliku

        cssCode[index++] = c; // dodaj znak do tablicy i zwiększ indeks

        // sprawdź, czy wczytano znak kończący
        if (index >= strlen(endMarker) &&
            memcmp(cssCode + index - strlen(endMarker), endMarker, strlen(endMarker)) == 0) {
            break;
        }
    }
}

void cssPrass(char* cssCode,
    Section*& sections_head,
    Section*& sections_tail,
    Selector*& selectors_head,
    Selector*& selectors_tail,
    Attribute*& attributes_head,
    Attribute*& attributes_tail,
    Array*& array_head,
    Array*& array_tail,
    unsigned int& sectionId)
{
    char selector[MAXLINE];
    char attribute_name[MAXLINE];
    char attribute_value[MAXLINE];
    clearArray(selector, MAXLINE);
    clearArray(attribute_value, MAXLINE);
    clearArray(attribute_name, MAXLINE);
    int arraySelectorId = 0;
    int arrayAttributeId = 0;
    int num_selectors = 0;
    int arrayId = 0;
    int i = 0, j = 0, k = 0;
    bool inside_selector_name = true;
    bool inside_selector = false;
    bool inside_attribute_name = false;
    bool inside_attribute_value = false;
    bool new_array = false;
    char c;
    sections_tail = add_section(sections_tail, sectionId);
    if (sections_head == nullptr) {
        sections_head = sections_tail;
    }


    if (sections_head == nullptr) {
        sections_head = sections_tail;
    }
    for (int m = 0; m < MAXCSS; m++) {
        c = cssCode[m];
        if (c == '{') {
            c = cssCode[m];
            inside_selector_name = false;
            inside_selector = true;
            selectors_tail = add_selector(selectors_tail, selector);
            i = 0;
            if (selectors_head == nullptr) {
                selectors_head = selectors_tail;
            }
            sections_tail->selectors[arraySelectorId] = selectors_tail;
            arraySelectorId++;
            clearArray(selector, MAXLINE);
            num_selectors++;
        }
        else if (c == ',' && inside_selector_name == true) {
            selectors_tail = add_selector(selectors_tail, selector);
            i = 0;
            if (selectors_head == nullptr) {
                selectors_head = selectors_tail;
            }
            sections_tail->selectors[arraySelectorId] = selectors_tail;
            arraySelectorId++;
            clearArray(selector, MAXLINE);
            num_selectors++;
        }
        else if (c == '}') {
            inside_selector = false;
            inside_selector_name = true;
            if (attribute_name[0] != 0) {
                attributes_tail = add_attribute(attributes_tail, attribute_name, attribute_value);
                j = 0;
                k = 0;
                if (attributes_head == nullptr) {
                    attributes_head = attributes_tail;
                }
                sections_tail->atributes[arrayAttributeId] = attributes_tail;
                for (int i = 0; i < 100; i++) {
                    if (i != arrayAttributeId && sections_tail->atributes[i]->name != nullptr && strcmp(sections_tail->atributes[i]->name, sections_tail->atributes[arrayAttributeId]->name) == 0) {
                        sections_tail->atributes[i] = nullptr;
                        break;
                    }
                }
                arrayAttributeId++;
                inside_attribute_value = false;
                clearArray(attribute_name, MAXLINE);
                clearArray(attribute_value, MAXLINE);
            }
            while (new_array == false) {
                for (int i = 0; i < 8; i++) {
                    if (array_tail->array[i] == nullptr) {
                        array_tail->array[i] = sections_tail;
                        new_array = true;
                        break;
                    }
                }
                if (new_array == true)
                    break;
                else if (array_tail->next != nullptr) {
                    array_tail = array_tail->next;
                }
                else {
                    array_tail = add_array(array_tail);
                    if (array_head == nullptr) {
                        array_head = array_tail;
                    }
                }
            }
            new_array = false;
            sectionId++;
            arrayAttributeId = 0;
            arraySelectorId = 0;
            sections_tail = add_section(sections_tail, sectionId);
            if (sections_head == nullptr) {
                sections_head = sections_tail;
            }
        }
        else if (c == ':') {
            inside_attribute_value = true;
            inside_attribute_name = false;
        }
        else if (c == ';') {

            inside_attribute_value = false;
            attributes_tail = add_attribute(attributes_tail, attribute_name, attribute_value);
            j = 0;
            k = 0;
            if (attributes_head == nullptr) {
                attributes_head = attributes_tail;
            }
            sections_tail->atributes[arrayAttributeId] = attributes_tail;
            for (int i = 0; i < 100; i++) {
                if (i != arrayAttributeId && sections_tail->atributes[i]->name != nullptr && strcmp(sections_tail->atributes[i]->name, sections_tail->atributes[arrayAttributeId]->name) == 0) {
                    sections_tail->atributes[i] = nullptr;
                    break;
                }
            }
            arrayAttributeId++;
            clearArray(attribute_name, MAXLINE);
            clearArray(attribute_value, MAXLINE);
        }
        else if (c == ' ' && inside_attribute_value) {
            attribute_value[k] = c;
            k++;
        }

        else if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
            continue;
        }
        else if (c == '\0')
            break;
        else {
            if (inside_selector_name) {
                selector[i] = c;
                i++;
            }
            else if (inside_attribute_name) {
                attribute_name[j] = c;
                j++;
            }
            else if (inside_attribute_value) {
                attribute_value[k] = c;
                k++;
            }
            else {
                inside_attribute_name = true;
                attribute_name[j] = c;
                j++;
            }
        }
    }

}


void count_selectors_in_section(Array* array_head, unsigned int section_id, int count_del) {
    section_id += count_del;
    if (section_id < 1)
        return;
    int arrayId = (section_id - 1) / 8;
    for (int i = 0; i < arrayId; i++) {
        array_head = array_head->next;
    }
    Section* current_section = array_head->array[0];
    if (arrayId == 0)
        current_section = array_head->array[section_id - 1];
    else
        current_section = array_head->array[(section_id - 1) % 8];
    if (current_section == nullptr) {
        return;
    }
    int num_selectors = 0;
    for (int i = 0; i < 100; i++) {
        if (current_section->selectors[i] != nullptr) {
            num_selectors++;
        }
    }
    cout << section_id << ",S,? == " << num_selectors << endl;
}

void count_attributes_in_section(Array* array_head, unsigned int section_id) {
    if (section_id < 1)
        return;
    int arrayId = (section_id - 1) / 8;
    for (int i = 0; i < arrayId; i++) {
        array_head = array_head->next;
    }
    Section* current_section = array_head->array[0];
    if (arrayId == 0)
        current_section = array_head->array[section_id - 1];
    else
        current_section = array_head->array[(section_id - 1) % 8];
    if (current_section == nullptr) {
        return;
    }
    int num_attributes = 0;
    for (int i = 0; i < 100; i++) {
        if (current_section->atributes[i] != nullptr) {
            num_attributes++;
        }
    }
    cout << section_id << ",A,? == " << num_attributes << endl;
}

void selector_name_in_section(Array* array_head, int i1, int i2) {
    int arrayId = (i1 - 1) / 8;
    for (int i = 0; i < arrayId; i++) {
        array_head = array_head->next;
    }
    Section* current_section = array_head->array[0];
    if (arrayId == 0)
        current_section = array_head->array[i1 - 1];
    else
        current_section = array_head->array[(i1 - 1) % 8];
    if (current_section == nullptr)
        return;
    if (current_section->selectors[i2 - 1] == nullptr)
        return;
    cout << i1 << ",S," << i2 << " == ";
    printString(current_section->selectors[i2 - 1]->name, MAXLINE);
    cout << endl;
}

void attribute_value_in_section(Array* array_head, int i1, char* n) {
    int arrayId = (i1 - 1) / 8;
    for (int i = 0; i < arrayId; i++) {
        array_head = array_head->next;
    }
    Section* current_section = array_head->array[0];
    if (arrayId == 0)
        current_section = array_head->array[i1 - 1];
    else
        current_section = array_head->array[(i1 - 1) % 8];
    if (current_section == nullptr)
        return;
    for (int i = 0; i < 100; i++) {
        if (current_section->atributes[i] != nullptr && strcmp(current_section->atributes[i]->name, n) == 0) {
            cout << i1 << ",A,";
            printString(n, MAXLINE);
            cout << " == ";
            printString(current_section->atributes[i]->value, MAXLINE);
            cout << endl;
            break;
        }
    }
}

int count_attributes_in_blocks(Array* array_head, char* name) {
    int n = 0;
    while (true) {
        if (array_head == nullptr)
            break;
        for (int i = 0; i < 8; i++) {
            Section* current_section = array_head->array[i];
            if (current_section == nullptr)
                break;
            for (int j = 99; j >= 0; j--) {
                if (current_section->atributes[j] != nullptr && strcmp(current_section->atributes[j]->name, name) == 0) {
                    n++;
                    break;
                }

            }
        }
        array_head = array_head->next;
    }
    return n;
}

int count_selectors_in_blocks(Array* array_head, char* name) {
    int n = 0;
    while (true) {
        if (array_head == nullptr)
            break;
        for (int i = 0; i < 8; i++) {
            Section* current_section = array_head->array[i];
            if (current_section == nullptr)
                break;
            for (int j = 0; j < 100; j++) {
                if (current_section->selectors[j] == nullptr)
                    break;
                if (strcmp(current_section->selectors[j]->name, name) == 0)
                    n++;
            }
        }
        array_head = array_head->next;
    }
    return n;
}

void name_attribute_for_selector(Array* array_head, char* nameS, char* nameA) {
    bool search = false;
    while (array_head->next != nullptr) {
        array_head = array_head->next;
    }
    while (true) {
        if (array_head == nullptr || search == true)
            break;
        for (int i = 7; i >= 0; i--) {
            Section* current_section = array_head->array[i];
            if (current_section != nullptr)
                for (int j = 0; j < 100; j++) {
                    if (current_section->selectors[j] != nullptr && strcmp(current_section->selectors[j]->name, nameS) == 0) {
                        for (int k = 0; k < 100; k++) {
                            if (current_section->atributes[k] != nullptr && strcmp(current_section->atributes[k]->name, nameA) == 0) {
                                printString(nameS, MAXLINE);
                                cout << ",E,";
                                printString(nameA, MAXLINE);
                                cout << " == ";
                                printString(current_section->atributes[k]->value, MAXLINE);
                                cout << endl;
                                search = true;
                                break;
                            }
                        }
                        break;
                    }

                }
            if (search == true)
                break;
        }
        array_head = array_head->prev;
    }
}

void delete_block(Array* array_head, int i1, unsigned int& sectionId, int& count_del) {
    int arrayId = (i1 - 1) / 8;
    for (int i = 0; i < arrayId; i++) {
        array_head = array_head->next;
    }
    Section* current_section = array_head->array[0];
    if (arrayId == 0)
        current_section = array_head->array[i1 - 1];
    else
        current_section = array_head->array[(i1 - 1) % 8];
    if (current_section == nullptr)
        return;
    for (int i = 0; i < 100; i++) {
        current_section->atributes[i] = nullptr;
    }
    for (int i = 0; i < 100; i++) {
        current_section->selectors[i] = nullptr;
    }
    current_section = nullptr;
    sectionId--;
    count_del++;
}
void delete_attribute(Array* array_head, int i1, char* n, unsigned int& sectionId, int& count_del) {
    i1 = +count_del;
    bool deleted = true;
    int arrayId = (i1 - 1) / 8;
    for (int i = 0; i < arrayId; i++) {
        array_head = array_head->next;
    }
    Section* current_section = array_head->array[0];
    if (arrayId == 0)
        current_section = array_head->array[i1 - 1];
    else
        current_section = array_head->array[(i1 - 1) % 8];
    if (current_section == nullptr)
        return;
    for (int i = 0; i < 100; i++) {
        if (current_section->atributes[i] != nullptr && strcmp(current_section->atributes[i]->name, n) == 0) {
            current_section->atributes[i] = nullptr;
            break;
        }
    }
    for (int i = 0; i < 100; i++) {
        if (current_section->atributes[i] != nullptr) {
            deleted = false;
            break;
        }
    }
    if (deleted) {
        delete_block(array_head, i1, sectionId, count_del);
    }
}

int main() {

    setvbuf(stdin, NULL, _IONBF, 0); //turn off buffering
    std::cout.setf(std::ios::unitbuf);

    Section* sections_head = nullptr;
    Section* sections_tail = nullptr;
    Selector* selectors_head = nullptr;
    Selector* selectors_tail = nullptr;
    Attribute* attributes_head = nullptr;
    Attribute* attributes_tail = nullptr;
    Array* array_tail = nullptr;
    Array* array_head = nullptr;
    unsigned int sectionId = 1;
    int count_del = 0;

    char cssCode[MAXCSS] = { 0 };
    cssRead(cssCode);
    array_tail = add_array(array_tail);
    if (array_head == nullptr) {
        array_head = array_tail;
    }
    cssPrass(cssCode, sections_head, sections_tail, selectors_head, selectors_tail, attributes_head, attributes_tail, array_head, array_tail, sectionId);
    char c;
    c = 0;
    clearArray(cssCode, MAXCSS);
    char command[MAXLINE];
    clearArray(command, MAXLINE);
    int n = 0;
    int i1, i2;
    char name[MAXLINE] = { 0 };
    char name2[MAXLINE] = { 0 };
    while (true) {
        while (n < MAXLINE) {
            c = getchar();
            if (c == '\n') break;
            if (c == EOF) return 0;
            command[n++] = c;
        }
        char* command_przecinek = strchr(command, ',');

        if (strcmp(command, "?") == 0) {
            cout << "? == " << sectionId - 1 << endl;
        }
        else if (strcmp(command, "****") == 0) {
            cssRead(cssCode);
            cssPrass(cssCode, sections_head, sections_tail, selectors_head, selectors_tail, attributes_head, attributes_tail, array_head, array_tail, sectionId); \
                clearArray(cssCode, MAXCSS);
        }
        else if (porownaj_strukture_komendy_przecinek(",D,*", command_przecinek) && sscanf_s(command, "%d,D,*", &i1) == 1) {
            delete_block(array_head, i1, sectionId, count_del);
            cout << i1 << ",D,* == deleted" << endl;
        }
        else if (porownaj_strukture_komendy_przecinek(",S,?", command_przecinek) && sscanf_s(command, "%d,S,?", &i1) == 1) {
            count_selectors_in_section(array_head, i1, count_del);
        }
        else if (porownaj_strukture_komendy_przecinek(",S,?", command_przecinek) && sscanf_s(command, "%[^,],S,?", name, MAXLINE) == 1) {
            cout << name << ",S,? == " << count_selectors_in_blocks(array_head, name) << endl;
        }
        else if (porownaj_strukture_komendy_przecinek(",A,?", command_przecinek) && sscanf_s(command, "%d,A,?", &i1) == 1) {
            count_attributes_in_section(array_head, i1);
        }
        else if (porownaj_strukture_komendy_przecinek(",A,?", command_przecinek) && sscanf_s(command, "%[^,],A,?", name, MAXLINE) == 1) {
            cout << name << ",A,? == " << count_attributes_in_blocks(array_head, name) << endl;
        }
        else if (sscanf_s(command, "%[^,],E,%s", name, MAXLINE, name2, MAXLINE) == 2) {
            name_attribute_for_selector(array_head, name, name2);
        }
        else if (sscanf_s(command, "%d,S,%d", &i1, &i2) == 2) {

            selector_name_in_section(array_head, i1, i2);
        }
        else if (sscanf_s(command, "%d,D,%s", &i1, name, MAXLINE) == 2) {
            delete_attribute(array_head, i1, name, sectionId, count_del);
            cout << i1 << ",D,";
            printString(name, MAXLINE);
            cout << " == deleted" << endl;
        }
        else if (sscanf_s(command, "%d,A,%s", &i1, name, MAXLINE) == 2) {
            attribute_value_in_section(array_head, i1, name);
        }
        clearArray(command, MAXLINE);
        n = 0;
    }

    return 0;

}