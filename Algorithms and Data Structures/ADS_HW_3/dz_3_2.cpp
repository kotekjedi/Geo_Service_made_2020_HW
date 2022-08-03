//В одной военной части решили построить в одну шеренгу по росту. Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя,
// а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги. Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту –
// сначала самые высокие, а в конце – самые низкие. За расстановку солдат отвечал прапорщик, который заметил интересную особенность – все солдаты в части разного роста.
//
//Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат, а именно для каждого приходящего солдата указывать,
// перед каким солдатом в строе он должен становится. Требуемая скорость выполнения команды - O(log n) амортизационно.
//
//В реализации используйте сплей деревья.
//
//Формат ввода
//Первая строка содержит число N – количество команд (1 ≤ N ≤ 90 000).
//
//В каждой следующей строке содержится описание команды: число 1 и X если солдат приходит в строй (X – рост солдата, натуральное число до 100 000 включительно)
// и число 2 и Y если солдата, стоящим в строе на месте Y надо удалить из строя. Солдаты в строе нумеруются с нуля.
//
//Формат вывода
//На каждую команду 1 (добавление в строй) вы должны выводить число K – номер позиции, на которую должен встать этот солдат (все стоящие за ним двигаются назад).
#include <iostream>
struct TreeNode {
    TreeNode * left = nullptr , * right = nullptr, * parent = nullptr;
    int key;
    int subtrees = 0;
    explicit TreeNode (int key) : key(key) {}
    ~TreeNode() {
        if (!left)
	        delete left;
        if (!right)
            delete right;
    }
};
class SplayTree {
    private:
        TreeNode * root = nullptr;
        void left_rotate(TreeNode * x);
        void right_rotate(TreeNode *x);
        void Splay(TreeNode * x);
        void Replace(TreeNode *u, TreeNode *v);
        static void Reweight_plus(TreeNode * insertation);
        static void Reweight_minus(TreeNode * insertation);

    public:
        SplayTree()= default;
        ~SplayTree();
        int get_right_subtree();
        void Insert(int key);
        void Remove(TreeNode * z);
        TreeNode * find_order_stat(int k);
};
void SplayTree::left_rotate(TreeNode * x) {
    TreeNode * y = x->right;
    if (y) {
        if (y->right)
            x->subtrees -= y->right->subtrees + 1;
        x->subtrees -= 1;
        x->right = y->left;
        if (y->left)
            y->left->parent = x;
        y->parent = x->parent;
    }
    if (!x->parent)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else x->parent->right = y;
    if (y)
        y->left = x;
    x->parent = y;
    if (y) {
        y->subtrees = 0;
        if (y->right)
            y->subtrees += y->right->subtrees + 1;
        if (y->left)
            y->subtrees += y->left->subtrees + 1;
    }
}
void SplayTree::right_rotate(TreeNode * x) {
    TreeNode *y = x->left;
    if (y) {
        if (y->left)
            x->subtrees -= y->left->subtrees + 1;

        x->subtrees -= 1;

        x->left = y->right;
        if (y->right)
            y->right->parent = x;
        y->parent = x->parent;
    }
    if (!x->parent)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    if (y)
        y->right = x;
    x->parent = y;
    if (y) {
        y->subtrees = 0;
        if (y->right)
            y->subtrees += y->right->subtrees + 1;
        if (y->left)
            y->subtrees += y->left->subtrees + 1;
    }
}
void SplayTree::Splay(TreeNode * x) {
    while (x->parent) {
        if (!x->parent->parent) {
//            zig
            if (x->parent->left == x)
                right_rotate(x->parent);
            else
                left_rotate(x->parent);
        }
        else if (x->parent->left == x && x->parent->parent->left == x->parent) {
//            zigzig
            right_rotate(x->parent->parent);
            right_rotate(x->parent);
        }
        else if (x->parent->right == x && x->parent->parent->right == x->parent) {
//            zigizig
            left_rotate(x->parent->parent);
            left_rotate(x->parent);
        }
        else if (x->parent->left == x && x->parent->parent->right == x->parent) {
//            zigzag
            right_rotate(x->parent);
            left_rotate(x->parent);
        }
        else {
//            zigzag
            left_rotate(x->parent);
            right_rotate(x->parent);
        }
    }
}
void SplayTree::Replace(TreeNode *u, TreeNode *v) {
    if (!u->parent)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v)
        v->parent = u->parent;
}
void SplayTree::Reweight_plus(TreeNode * insertation) {
    while (insertation->parent) {
        insertation = insertation->parent;
        insertation->subtrees += 1;
    }
}
void SplayTree::Reweight_minus(TreeNode * insertation) {
    while (insertation->parent) {
        insertation = insertation->parent;
        insertation->subtrees -= 1;
    }
}
int SplayTree::get_right_subtree() {
    if (root->right)
        return root->right->subtrees;
    else
        return -1;
}
void SplayTree::Insert(int key) {
    TreeNode *z = root;
    TreeNode *p = nullptr;

    while (z) {
        p = z;
        if (z->key < key)
            z = z->right;
        else
            z = z->left;
    }
    z = new TreeNode(key);
    z->parent = p;
    if (p) {
        Reweight_plus(z);
        if (p->key < z->key)
            p->right = z;
        else p->left = z;
    }
    if (!p)
        root = z;

    Splay(z);
}
void SplayTree::Remove(TreeNode * z) {
    if (!z) return;
    Splay(z);
    if (!z->left)
        Replace(z, z->right);
    else if (!z->right)
        Replace(z, z->left);
    else {
        TreeNode * y = z->right;
        while (y->left)
            y = y->left;
        if (y->parent != z) {
            Reweight_minus(y);
            Replace(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        Replace(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->subtrees = 0;
        if (y->right)
            y->subtrees += y->right->subtrees + 1;
        if (y->left)
            y->subtrees += y->left->subtrees + 1;
    }

    delete z;
}
TreeNode * SplayTree:: find_order_stat(int k) {
    TreeNode *  temp = root;
    while (true) {
        if (temp->subtrees == 0)
            return temp;

        if (!temp->right) {
            if (k < 1)
                return temp;
            k--;
            temp = temp->left;
            continue;
        }

        if (k == temp->right->subtrees + 1)
            return temp;

        if (k > temp->right->subtrees) {
            k -= (temp->right->subtrees + 1);
            k--;
            temp = temp->left;
        }
        else
            temp = temp->right;
    }
}

SplayTree::~SplayTree() {
    delete root;
}

int main() {
	SplayTree tree;
	int n, command;
	std::cin >> n;
	for (int i = 0; i < n; i++) {
		std::cin >> command;
		if (command == 1) {
			std::cin >> command;
			tree.Insert(command);
			std::cout << (tree.get_right_subtree() + 1) << std::endl;
		}
		else if (command == 2) {
			std::cin >> command;
			tree.Remove(tree.find_order_stat(command));
		}
	}
	return 0;
}