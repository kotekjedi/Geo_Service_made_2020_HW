
//Реализовать очередь с помощью двух стеков. Использовать стек, реализованный с помощью динамического буфера.
//
//Обрабатывать команды push back и pop front.
//
//Формат ввода
//В первой строке количество команд n. n ≤ 1000000.
//
//Каждая команда задаётся как 2 целых числа: a b.
//
//a = 2 - pop front
//        a = 3 - push back
//
//        Если дана команда pop front, то число b - ожидаемое значение. Если команда pop front вызвана для пустой структуры данных, то ожидается “-1”.
//
//Формат вывода
//Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.ES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.

#include <iostream>
#include <cstring>
#include <assert.h>

using namespace std;

class Stack {
    public:
        Stack();
        ~Stack();
        int pop();
        void push(int value);
        bool isEmpty() const;
    private:
        int buff_size = 8;
        int * begin = nullptr;
        int counter = 0;
};
Stack::Stack() {
    begin = new int[buff_size];
}
Stack:: ~Stack() {
    delete[] begin;
}
void Stack::push(int value) {
    if (counter >= buff_size) {
        buff_size <<= 1;
        auto * temp = new int[buff_size];
        memcpy(temp, begin, sizeof(int) * (buff_size >> 1));
        delete[] begin;
        begin = temp;
    }
    begin[counter] = value;
    counter++;
}

int Stack::pop() {
//    if (isEmpty()) {
//        return -1;
//    }
    assert(!isEmpty());
    if (counter <= buff_size>>1) {
        buff_size >>= 1;
        auto * temp = new int[buff_size];
        memcpy(temp, begin, sizeof(int) * (buff_size));
        delete[] begin;
        begin = temp;
    }
    counter--;
    return begin[counter];
}

inline bool Stack::isEmpty() const {
    return counter == 0;
}

class Queue {
public:
    int pop();
    void push(int value);
    bool isEmpty() const;
private:
    Stack push_stack;
    Stack pop_stack;

};

int Queue::pop() {
    if (pop_stack.isEmpty()) {
        while (!push_stack.isEmpty()) {
            pop_stack.push(push_stack.pop());
        }
    }
    return pop_stack.pop();
}

void Queue::push(int value) {
    push_stack.push(value);
}

bool Queue::isEmpty() const {
    return push_stack.isEmpty() && pop_stack.isEmpty();
}


int main() {
    int num_comm;
    cin >> num_comm;
    Queue queue;

    for (int i = 0, comm_val = 0; i < num_comm; i++) {
        cin >> comm_val;
        if (comm_val == 3) {
            cin >> comm_val;
            queue.push(comm_val);
        }
        else{
            cin >> comm_val;

            if (!queue.isEmpty() && queue.pop() != comm_val) {

                cout << "NO";
                return 0;
            }
        }
    }
    cout << "YES";
    return 0;
}