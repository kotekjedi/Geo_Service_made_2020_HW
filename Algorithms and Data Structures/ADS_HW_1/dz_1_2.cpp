//Дан массив целых чисел A[0..n), n не превосходит 100 000. Так же задан размер некотрого окна (последовательно расположенных элементов массива) в этом массиве k, k<=n. Требуется для каждого положения окна (от 0 и до n-k) вывести значение максимума в окне. Скорость работы O(n log n), память O(n).
//
//Формат ввода
//Вначале вводится n - количество элементов массива. Затем вводится n строк со значением каждого элемента. Затем вводится k - размер окна.
//
//Формат вывода
//Разделенные пробелом значения максимумов для каждого положения окна.

#include <iostream>
#include <cstring>
#include <cassert>

using namespace std;
class Stack_Element {
    public:
        Stack_Element() = default;
        Stack_Element(int value, int max);
        int getMax();
        int getValue();
    private:
        int value;
        int max;
};

Stack_Element::Stack_Element(int value, int max) {
    this->value = value;
    this->max = max;
}

int Stack_Element::getMax() {
    return max;
}

int Stack_Element::getValue() {
    return value;
}

class Stack {
    public:
        Stack();
        ~Stack();
        Stack_Element pop();
        Stack_Element top();
        void push(int value, int max);
        bool isEmpty() const;
    private:
        int buff_size = 8;
        Stack_Element * begin = nullptr;
        int counter = 0;
};
Stack::Stack() {
    begin = new Stack_Element[buff_size];
}
Stack:: ~Stack() {
    delete[] begin;
}
void Stack::push(int value, int max) {
    if (counter >= buff_size) {
        buff_size <<= 1;
        auto * temp = new Stack_Element[buff_size];
        memcpy(temp, begin, sizeof(Stack_Element) * (buff_size >> 1));
        delete[] begin;
        begin = temp;
    }
    begin[counter] = Stack_Element(value, max);
    counter++;
}

Stack_Element Stack::pop() {
    assert(!isEmpty());
    if (counter <= buff_size>>1) {
        buff_size >>= 1;
        auto * temp = new Stack_Element[buff_size];
        memcpy(temp, begin, sizeof(Stack_Element) * (buff_size));
        delete[] begin;
        begin = temp;
    }
    counter--;
    return begin[counter];
}

inline bool Stack::isEmpty() const {
    return counter == 0;
}

Stack_Element Stack::top() {
    return begin[counter-1];
}

class Queue {
public:
    int pop();
    int getMax();
    void push(int value);
    bool isEmpty() const;
private:
    Stack push_stack;
    Stack pop_stack;
};

int Queue::pop() {
    assert(!isEmpty());
    if (pop_stack.isEmpty()) {
        int value;
        int max;
        while (!push_stack.isEmpty()) {
            value = push_stack.top().getValue();
            push_stack.pop();
//          переносим элементы из стека пополнения в стек забора, и проверяем, не больше ли этот элемент текущего максимума в стеке забора
            max = pop_stack.isEmpty() ? value : ( value >  pop_stack.top().getMax() ? value : pop_stack.top().getMax());
            pop_stack.push (value, max);
        }
    }
    return pop_stack.pop().getValue();
}

void Queue::push(int value) {
    //Если стэк для пополнения пуст, то новый минимальный элемент - текущий
    int max = push_stack.isEmpty() ? value : (value >  push_stack.top().getMax() ? value : push_stack.top().getMax());
    push_stack.push(value, max);
}

bool Queue::isEmpty() const {
    return push_stack.isEmpty() && pop_stack.isEmpty();
}

int Queue::getMax() {
    int max;
    // выбор наибольшего элемента из двух стэков
    if (push_stack.isEmpty() || pop_stack.isEmpty())
        max = push_stack.isEmpty() ? pop_stack.top().getMax() : push_stack.top().getMax();
    else
        max = pop_stack.top().getMax() > push_stack.top().getMax() ? pop_stack.top().getMax() : push_stack.top().getMax();
    return max;
}


int main() {
    int num_amnt;
    cin >> num_amnt;
    int * buffer_array = new int[num_amnt];
    for (int i = 0, value = 0; i < num_amnt; i++) {
        cin >> value;
        buffer_array[i] = value;
    }
    int k_size;
    cin >> k_size;
    Queue queue;
    for (int i = 0; i < k_size-1; i++) {
        queue.push(buffer_array[i]);
    }

    for (int i = k_size-1; i < num_amnt; i++) {
        queue.push(buffer_array[i]);
        cout << queue.getMax() << ' ';
        queue.pop();
    }

    return 0;
}