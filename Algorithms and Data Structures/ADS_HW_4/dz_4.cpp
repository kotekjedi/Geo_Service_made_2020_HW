//Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией. 
//Хранимые строки непустые и состоят из строчных латинских букв. Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера. 
//Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4. 
//Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству. 1_1. 
//Для разрешения коллизий используйте квадратичное пробирование. i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки. 1_2. Для разрешения коллизий используйте двойное хеширование.

//Формат ввода
//Каждая строка входных данных задает одну операцию над множеством. Запись операции состоит из типа операции и следующей за ним через пробел строки, над которой проводится операция. 
//Тип операции – один из трех символов: + означает добавление данной строки в множество; - означает удаление строки из множества; ? означает проверку принадлежности данной строки множеству. При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в этом множестве. 
//При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.

//Формат вывода
//Программа должна вывести для каждой операции одну из двух строк OK или FAIL.

//Для операции '?': OK, если элемент присутствует во множестве. FAIL иначе.

//Для операции '+': FAIL, если добавляемый элемент уже присутствует во множестве и потому не может быть добавлен. OK иначе.

//Для операции '-': OK, если элемент присутствовал во множестве и успешно удален. FAIL иначе.




#include <iostream>
#include <assert.h>
#include <string>


class HashTable {
public:
    explicit HashTable(int initial_size_) : initial_size(initial_size_) {
        mas = new std::string[initial_size];
        mas_h1 = new unsigned int [initial_size];
        mas_h2 = new unsigned int [initial_size];
    };

    HashTable() = default;
    ~HashTable();
    bool Add(const std::string& key);
    bool Has(const std::string& key);
    bool Has(const std::string & key, unsigned int hash1_, unsigned int hash2_);
    bool Remove(const std::string& key);

private:
    int initial_size = 0;
    int num_elem = 0;
    std::string * mas = nullptr;
    unsigned int * mas_h1 = nullptr;
    unsigned int * mas_h2 = nullptr;
   
    static void hash(const std::string & str, unsigned int & hash_1, unsigned int & hash_2);

    bool Add_Rehash(const std::string& key, unsigned int hash1_, unsigned int hash2_);
    void Rehash();


};
void HashTable::hash(const std::string & str, unsigned int &  hash_1, unsigned int & hash_2) {
    int a_1 = 3;
    int a_2 = 43;
    int len = str.length();
    for (int i = 0; str[i]; i++) {
        hash_1 = (hash_1 * a_1 + str[i]);
        hash_2 = (hash_2 * a_2 + str[len - i]);
    }
    hash_2 = (2 * hash_2) + 1;
}

bool HashTable::Add(const std::string& key) {
    assert(!key.empty());

    if (num_elem * 4 >= 3 * initial_size) {
        Rehash();
    }
    unsigned int key_hash1, key_hash2;
    key_hash1 = key_hash2 = 1;
    hash( key, key_hash1, key_hash2);

    if (Has(key, key_hash1, key_hash2))
        return false;

    num_elem++;

    int i = 0;
    unsigned int key_hash = key_hash1 % initial_size;


    while (!mas[key_hash].empty()) {
        i++;
        key_hash = (key_hash1 + i * key_hash2) % initial_size;
    }
    mas_h1[key_hash] = key_hash1;
    mas_h2[key_hash] = key_hash2;

    mas[key_hash] = key;

    return true;

}

bool HashTable::Add_Rehash(const std::string& key, unsigned int hash1_, unsigned int hash2_)  {
    assert(!key.empty());

    unsigned int key_hash1 = hash1_;
    unsigned int key_hash2 = hash2_;

    num_elem++;

    int i = 0;
    unsigned int key_hash = key_hash1 % initial_size;

    while (!mas[key_hash].empty()) {
        i++;
        key_hash = (key_hash1 + i*key_hash2) % initial_size;
    }

    mas_h1[key_hash] = key_hash1;
    mas_h2[key_hash] = key_hash2;

    mas[key_hash] = key;

    return true;
}

bool HashTable::Has(const std::string & key)
{
    unsigned int key_hash1, key_hash2;
    key_hash1 = key_hash2 = 1;
    hash(key, key_hash1, key_hash2);

    int i = 0;
    unsigned int key_hash = key_hash1 % initial_size;

    while (!mas[key_hash].empty() || (mas_h1[key_hash] == 2038 && mas_h2[key_hash] == 2038) ) {
        if (!mas[key_hash].empty() && mas[key_hash] == key) {
            return true;
        }

        i++;
        key_hash = (key_hash1 + i*key_hash2) % initial_size;
    }

    return false;
}

bool HashTable::Has(const std::string & key, unsigned int hash1_, unsigned int hash2_)
{

    unsigned int key_hash1 = hash1_;
    unsigned int key_hash2 = hash2_;
    int i = 0;
    unsigned int key_hash = key_hash1 % initial_size;

    while (!mas[key_hash].empty() || (mas_h1[key_hash] == 2038 && mas_h2[key_hash] == 2038)) {
        if (!mas[key_hash].empty() && mas[key_hash] == key) { 
            return true;
        }
        i++;
        key_hash = (key_hash1 + i*key_hash2) % initial_size;
    }

    return false;
}


bool HashTable::Remove(const std::string & key) {
    unsigned int key_hash1, key_hash2;
    key_hash1 = key_hash2 = 1;
    hash(key, key_hash1, key_hash2);

    int i = 0;
    unsigned int key_hash = key_hash1 % initial_size;

    while (!mas[key_hash].empty() || (mas_h1[key_hash] == 2038 && mas_h2[key_hash] == 2038)) {

        if (!mas[key_hash].empty() && mas[key_hash] == key) { 
            mas[key_hash].clear();
            mas_h1[key_hash] = mas_h2[key_hash] = 2038;
            return true;
        }
        i++;
        key_hash = (key_hash1 + i*key_hash2) % initial_size;
    }

    return false;
}



void HashTable::Rehash() {

    initial_size  <<= 1;
    num_elem = 0;

    std::string * tmp = mas;
    unsigned int * tmp_h1 = mas_h1;
    unsigned int * tmp_h2 = mas_h2;


    mas = new std::string[initial_size];
    mas_h1 = new unsigned int [initial_size];
    mas_h2 = new unsigned int [initial_size];


    for (int i = 0; i < (initial_size >> 1); i++) {
        if (!tmp[i].empty()) {
            Add_Rehash(tmp[i], tmp_h1[i], tmp_h2[i]);
        }
    }

    delete[] tmp_h1;
    delete[] tmp_h2;
    delete[] tmp;
}

HashTable::~HashTable() {
    delete[] mas_h1;
    delete[] mas_h2;
    delete[] mas;
}


int main() {
    HashTable table(8);

    char command = ' ';
    std::string value;


    while (std::cin >> command >> value) {
        switch (command) {
            case '+':
                std::cout << ((table.Add(value)) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                std::cout << ((table.Has(value)) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << ((table.Remove(value)) ? "OK" : "FAIL") << std::endl;
                break;
        }
    }

    return 0;
}