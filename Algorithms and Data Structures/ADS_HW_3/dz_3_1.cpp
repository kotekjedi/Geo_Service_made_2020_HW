//6_2. Выведите элементы в порядке in-order (слева направо).
//Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
//Требуется построить бинарное дерево, заданное наивным порядком вставки.
//Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; 
//иначе в левое поддерево root.
//Рекурсия запрещена.

//
//#include <iostream>
//#include <stack>
//struct TreeNode {
//	explicit TreeNode(int _value) : value(_value) {}
//	int value = 0;
//	TreeNode* left = nullptr;
//	TreeNode* right = nullptr;
//	~TreeNode() {
//	    if (!left)
//	        delete left;
//        if (!right)
//            delete right;
//	}
//};
//
//class Tree {
//    public:
//        ~Tree();
//        const void Print();
//        void Add(int value);
//    private:
//        TreeNode* root = nullptr;
//};
//
//void Tree::Add(int value) {
//	if (!root) {
//		root = new TreeNode(value);
//	}
//	else {
//		TreeNode *temp = root;
//		while (true) {
//			if (temp->value < value) {
//				if (temp->right) {
//					temp = temp->right;
//				}
//				else {
//					temp->right = new TreeNode(value);
//					break;
//				}
//			}
//			else {
//				if (temp->left) {
//					temp = temp->left;
//				}
//				else {
//					temp->left = new TreeNode(value);
//					break;
//				}
//			}
//		}
//	}
//}
//
//const void Tree::Print() {
//    std::stack<TreeNode *>  print_stack;
//	TreeNode * temp = root;
//
//	bool flag = true;
//	while (flag) {
//		if (temp->left) {
//			print_stack.push(temp);
//			temp = temp->left;
//		}
//		else {
//			while (!temp->right) {
//				if (print_stack.empty()) {
//					std::cout << temp->value << " ";
//					break;
//				}
//				std::cout << temp->value << " ";
//				temp = print_stack.top();
//				print_stack.pop();
//			}
//
//			if (temp->right) {
//				std::cout << temp->value << " ";
//				temp = temp->right;
//			}
//			else {
//				flag = false;
//			}
//		}
//	}
//}
//
//Tree::~Tree() {
//    delete root;
//}
//
//int main() {
//	Tree tree;
//	int n , input;
//	std::cin >> n;
//	for (int i=0; i < n; i++)
//	{
//		std::cin >> input;
//		tree.Add(input);
//	}
//	tree.Print();
//	return 0;
//}