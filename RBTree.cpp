#include <iostream>
using namespace std;

template <class keyType, class valueType>

class RBTree
{
    private:
    int Size = 0;
    //node
    enum Color {RED, BLACK};
    struct Node
    {
        bool color;
        keyType key;
        valueType value;
        int childrens;
        Node *parent, *right, *left;
        Node(keyType key, valueType value)
        {
            this->key = key;
            this->value = value;
            this->childrens = 1;
            this->color = RED;
            left = right = parent = NULL;
        }
    };
    Node *root;
        

    
    //helper functions

    Node *uncle(Node *n) // returns uncle of node n
    {
        if(n->parent == NULL || n->parent->parent == NULL)
        {
            return NULL;
        }

        if(n->parent == n->parent->parent->left)
        {
            return n->parent->parent->right;
        }
        return n->parent->parent->left;
    }

    Node *Sibling(Node *n) // returns sibling of node n
    {
        if(n->parent == NULL)
        {
            return NULL;
        }
        if(n == n->parent->left)
        {
            return n->parent->right;
        }
        return n->parent->left;
    }

    void swapColors(Node *a, Node *b) // swap colors of nodes 
    {
        bool temp;
        temp = a->color;
        a->color = b->color;
        b->color = temp;
    }

    Node *copyHelper(const Node* copyTo) // returns a copy 
    {
        if(copyTo == NULL) // empty tree
        {
            return NULL;
        }
        Node *copyNode = new Node(copyTo->key, copyTo->value);
        copyNode->parent = copyTo->parent;
        copyNode->left = copyHelper(copyTo->left);
        copyNode->right = copyHelper(copyTo->right);
        return copyNode;
    }

    void destroyTree(Node *n) // helper function for for destructor (works like post-order)
    {
        if(n != NULL)
        {
            destroyTree(n->left);
            destroyTree(n->right);
            delete n;
        }
    }

    Node *findMinimum(Node *node) //  find left most node (smallest key)
    {
        Node *n = node;
        while(n->left != NULL) 
        {
            n = n->left;
        }

        return n;
    }

    Node *findMaximum(Node *node, bool d) // find right most node (biggest key)
    {
        Node *n = node;
        while (n->right != NULL) 
        {
            if(d)
            {
                n->childrens--;
            }
            n = n->right;
        }

        return n;
    }

    void printPreorder(Node *n) // print tree in pre-order
    {
            if(n == NULL)
            {
                return;
            }
            cout << n->key << " ";
            printPreorder(n->left);
            printPreorder(n->right);
    }

    void printInorder(Node *n) // print tree in inorder
    {
        if(n == NULL)
        {
            return;
        }
        printInorder(n->left);
        cout << n->key << " ";
        printInorder(n->right);
    }

    void printPostOrder(Node *n) // print tree in post-order
    {
        if(n == NULL)
        {
            return;
        }
        printPostOrder(n->left);
        printPostOrder(n->right);
        cout<< n->key << " ";
    }

    void rotateLeft(Node *&root, Node *&n) // left rotate on node n
    {
        Node *tempRight = n->right; // holds n.right

        n->right = tempRight->left; // n.right is right.left

        if(n->right != NULL) // if n.right exist
        {
            n->right->parent = n;
        }

        tempRight->parent = n->parent;

        if(n->parent == NULL) // see if n.parent exist
        {
            root = tempRight;
        }
        else if(n == n->parent->left) // if n is parent.left
        {
            n->parent->left = tempRight;
        }
        else // if n is parent.right
        {
            n->parent->right = tempRight;
        }

        // connect tempRight and n
        tempRight->left = n;
        n->parent = tempRight;
        tempRight->childrens = n->childrens;
        if(n->left != NULL && n->right != NULL)
        {
            n->childrens = n->left->childrens + n->right->childrens + 1;
        }
        else if(n->left != NULL && n->right == NULL)
        {
            n->childrens = n->left->childrens + 1;
        }
        else if(n->left == NULL && n->right != NULL)
        {
            n->childrens = n->right->childrens + 1;
        }
        else
        {
            n->childrens = 1;
        }
        
    }

    void rotateRight(Node *&root, Node *&n) // right rotate on node n
    {
        Node *tempLeft = n->left; // stores n.left
        
        n->left = tempLeft->right; // connect n.left with tempLeft.right

        if(n->left != NULL) // if n.left exist
        {
            n->left->parent = n;
        }
        
        tempLeft->parent = n->parent; 

        if(n->parent == NULL) // check to see if n.parent exist
        {
            root = tempLeft;
        }
        else if(n == n->parent->left) // n is parent.left
        {
            n->parent->left = tempLeft;
        }
        else // n is parent.right
        {   
            n->parent->right = tempLeft;
        }

        //connect tempLeft with n
        tempLeft->right = n;
        n->parent = tempLeft;
        tempLeft->childrens = n->childrens;
        if(n->left != NULL && n->right != NULL)
        {
            n->childrens = n->left->childrens + n->right->childrens + 1;
        }
        else if(n->left != NULL && n->right == NULL)
        {
            n->childrens = n->left->childrens + 1;
        }
        else if(n->left == NULL && n->right != NULL)
        {
            n->childrens = n->right->childrens + 1;
        }
        else
        {
            n->childrens = 1;
        }

    }

    void insertCases(Node *x)
    {
       if(x == root) // base case (top of tree)
       {
           x->color = BLACK;
           return;
       }

       Node *Parent = x->parent;
       Node *Grandparent = Parent->parent;
       Node *Uncle = uncle(x);

       if(Parent->color != BLACK)
       {
           if(Uncle != NULL && Uncle->color == RED) // case 1
           {
               Parent->color = BLACK;
               Uncle->color = BLACK;
               Grandparent->color = RED;
               insertCases(Grandparent); 
           }
           else
           {
                if(Parent == Parent->parent->left) // left subtree
                {
                    if(x == x->parent->left) // change root of subtree color 
                    {
                        swapColors(Parent, Grandparent);
                    }
                    else // case 2
                    {
                        rotateLeft(root, Parent);
                        swapColors(x, Grandparent);
                    }
                    rotateRight(root, Grandparent); // case3 3
                }
                else // right subtree (mirror veriosn)
                {
                    if(x == x->parent->left) // mirror case 2
                    {
                        rotateRight(root, Parent);
                        swapColors(x, Grandparent);
                    }
                    else // change root of subtree color
                    {
                        swapColors(Parent, Grandparent);
                    }
                    rotateLeft(root, Grandparent); // mirror case 3
                }
            }
       }
    }

    void fixDoubleBlack(Node *x)
    {
        if(x == root) // base case (top of tree)
        {
            return;
        }

        // find sibling and parent
        Node *parent = x->parent;
        Node *sibling = Sibling(x);


        if(sibling == NULL)
        {
            fixDoubleBlack(parent);
        }
        else
        {
            if(sibling->color == RED) // case 1
            {
                //switch color of sibliing since sibling children are black
                parent->color = RED;
                sibling->color = BLACK;
                if(sibling == sibling->parent->left) // rotation
                {
                    rotateRight(root, parent);
                }
                else // mirror rotation veriosn
                {
                    rotateLeft(root, parent);
                }
                fixDoubleBlack(x);
            }
            else // case 2, 3, 4 because sibling is black
            {
                if((sibling->left !=NULL && sibling->left->color == RED) || (sibling->right != NULL && sibling->right->color == RED)) // case 2 or 3
                {
                    if(sibling->left != NULL && sibling->left->color == RED) // non mirror
                    {
                        if(sibling == sibling->parent->left) // case 4 (line)
                        {
                            sibling->left->color = sibling->color;
                            sibling->color = parent->color;
                            rotateRight(root, parent);
                        }
                        else // case 3 (triangle)
                        {
                            sibling->left->color = parent->color;
                            rotateRight(root, sibling);
                            rotateLeft(root, parent);
                        }
                    }
                    else // mirror 
                    {
                        if(sibling == sibling->parent->left) // case 3 (triangle)
                        {
                            sibling->right->color = parent->color;
                            rotateLeft(root, sibling);
                            rotateRight(root, parent);
                        }
                        else // case 4 (line)
                        {
                            sibling->right->color = sibling->color;
                            sibling->color = parent->color;
                            rotateLeft(root, parent);
                        }
                    }
                    parent->color = BLACK;
                }
                else // case 2 (sibling and both sibling's children are black)
                {
                    sibling->color = RED;
                    if(parent->color == BLACK)
                    {
                        fixDoubleBlack(parent);
                    }
                    else
                    {
                        parent->color = BLACK;
                    }
                }
            }
        }
    }

    Node *BSTInsert(Node *root, Node *newNode) // normal binary search tree insert
    {
        if(root == NULL) // tree is empty
        {
            return newNode;
            
        }
        root->childrens++;
        if(newNode->key < root->key) // recusive left
        {
            root->left = BSTInsert(root->left, newNode);
            root->left->parent = root;
        }
        else if(newNode->key > root->key) // recurse right
        {
            root->right = BSTInsert(root->right, newNode);
            root->right->parent = root;
        }

        return root; // return tree root

    }
    
    Node *BSTreplace(Node *x) 
    {
        if(x->left != NULL && x->right != NULL) // when node has 2 children
        {
            return findMaximum(x->left, true);
        }

        if(x->left == NULL && x->right == NULL) // when node is leave
        {
            return NULL;
        }
        // when node has single child
        if(x->left != NULL)
        {
            return x->left;
        }
        else
        {
            return x->right;
        }
    }

    Node *searchHelper(Node *root, keyType key, bool deletion) // search for key and return node with key
    {
        if(deletion)
        {
            root->childrens--;
        }

        if(root == NULL || root->key == key) // tree is empty or key was not found or key is found
        {
            return root;
        }
        if(root->key < key) // key is possibly in right sub-tree
        {
            return searchHelper(root->right, key, deletion);
        }
        return searchHelper(root->left, key, deletion); // key is possibly in left sub-tree
    }

    Node *OS_Select(Node *x, int i)
    {
        int r;
        if(x->left != NULL)
        {
            r = x->left->childrens + 1;
        }
        else
        {
            r = 1;
        }
        
        if(i == r)
        {
            return x;
        }   
        else if(i < r && x->left != NULL)
        {
            return OS_Select(x->left, i);
        }
        else if(i > r && x->right != NULL)
        {
            return OS_Select(x->right, i - r);
        }
        return x;
    }
    
    int OS_Rank(Node *x)
    {
        int r;
        if(x->left != NULL)
        {
            r = x->left->childrens + 1;
        }
        else
        {
            r = 1;
        }
        
        Node *y = x;
        
        while(y != root)
        {
            if(y->parent->right != NULL && y == y->parent->right)
            {
                if(y->parent->left != NULL)
                {
                    r = r + y->parent->left->childrens + 1;
                }
                else
                {
                    r = r + 1;
                }
                
            }
            y = y->parent;
        }
        return r;
    }
    public:
    // check if tree is too unbalance (longestHeight <= 2 * shortestPath)
    int maxDepth(Node *root)
    {
        if(root == NULL)
        {
            return -1;
        }
        else
        {
            int lDepth = maxDepth(root->left);
            int rDepth = maxDepth(root->right);

            if(lDepth > rDepth)
            {
                return lDepth + 1;
            }
            else
            {
                return rDepth + 1;
            }
        }
    }

    void isUnbalace() // helper function that checks if the tree is unbalance (equation: longest_path <= 2 * second_longest_path)
    {
        int lMAxHeight = maxDepth(root->left) + 1;
        int rMAxHeight = maxDepth(root->right) + 1;
        cout<<"left max height: "<<lMAxHeight<<endl;
        cout<<"right max height: "<<rMAxHeight<<endl;

        if(lMAxHeight > rMAxHeight) // left subtree is longest path
        {
            if(!(lMAxHeight <= (2 * rMAxHeight)))
            {
                cout<<"tree is too unbalance";
            }
            else
            {
                cout<<"tree is balance";
            }
        }
        // right subtree is longest path
        if(!(rMAxHeight <= (2 * lMAxHeight)))
        {
            cout<<"tree is too unbalance";
        }
        else
        {
            cout<<"tree is balance";
        }
        cout<<endl;
    }
    
    void insert(keyType key, valueType value)
    {
        Node *newNode = new Node(key, value);
        root = BSTInsert(root, newNode); // normal binary tree insert
        insertCases(newNode); // fix violations
        Size ++;
    }

    void deleteNode(Node *v)
    {   
        Node *u = BSTreplace(v);
        bool uvBlack = ((u == NULL || u->color == BLACK) && (v->color == BLACK)); 
        Node *parent = v->parent;

        if(u == NULL)
        {
            if(v == root)
            {
                root == NULL;
            }
            else
            {
                if(uvBlack)
                {
                    fixDoubleBlack(v);
                }
                else
                {
                    if(Sibling(v) != NULL)
                    {
                        Sibling(v)->color = RED;
                    }
                }
                if(v == v->parent->left)
                {
                    parent->left = NULL;
                }
                else
                {
                    parent->right = NULL;
                }
            }
            delete v;
            return;
        }
        
        if(v->left == NULL || v->right == NULL)
        {
            if(v == root)
            {
                v->key = u->key;
                v->value = u->value;
                v->left = v->right = NULL;
                delete u;
            }
            else
            {
                if(v == v->parent->left)
                {
                    parent->left = u;
                }
                else
                {
                    parent->right = u;
                }
                delete v;
                u->parent = parent;
                if(uvBlack)
                {
                    fixDoubleBlack(u);
                }
                else
                {
                    u->color = BLACK;
                }
            }
            return;
        }
        //swap data
        keyType keyTemp = u->key;
        valueType valueTemp = u->value;
        u->key = v->key;
        u->value = v->value;
        v->key = keyTemp;
        v->value = valueTemp;
        deleteNode(u);
    }
    
    int remove(keyType key)
    {
        Node *v = searchHelper(root, key, false);
        if(v == NULL)
        {
            return 0;
        }
        v = searchHelper(root, key, true);
        if(Size == 1)
        {
            Size = 0;
            root = NULL;
            return 1;
        }
        deleteNode(v);
        Size--;
        return 1;
    }

    //constructors, deconstructors, operators
    RBTree();
    RBTree(keyType keys[], valueType values[], int size);
    RBTree(const RBTree<keyType, valueType>& copyTo);
    ~RBTree();
    RBTree<keyType, valueType>& operator=(const RBTree<keyType, valueType> &copyTO)
    {
        root = copyHelper(copyTO.root);
        return *this;
    }
    //algorithms 
    valueType *search(keyType key)
    {
        Node *node = searchHelper(root, key, false);
        return &node->value;
    }

    
    //prints tree
    int size()
    {
        return Size;
    }
    void preorder()
    {
        printPreorder(root);
        cout << endl;
    }
    void inorder()
    {
        printInorder(root);
        cout << endl;
    }
    void postorder()
    {
        printPostOrder(root);
        cout<< endl;
    }

    //algorithms
    int rank(keyType key)
    {
        Node *n = searchHelper(root, key, false);
        if(n == NULL) // key not found
        {
            return 0;
        }
        return OS_Rank(n);
    }

    keyType select(int pos)
    {
        Node *s = OS_Select(root, pos);
        return s->key;
    }

    void printk(int k)
    {
        if(k <= 0 || k > size())
        {
            cout<<endl;
            return;
        }
        for(int i = 1; i <= k - 1; i++)
        {
            cout<<select(i)<<" ";
        }
        cout<<select(k);
        cout<<endl;
    }

    keyType *successor(keyType key)
    {
        Node *n = searchHelper(root, key, false);
        if(n == NULL)
        {
            return NULL;
        }

        if(n->right != NULL)
        {
            Node *s = findMinimum(n->right);
            return &s->key;
        }

        Node *p = n->parent;
        while(p != NULL && n == p->right)
        {
            n = p;
            p = p->parent;
        }
        if(p == NULL)
        {
            return NULL;
        }
        return &p->key;
        
    }

    keyType *predecessor(keyType key)
    {
        Node *n = searchHelper(root, key, false);
        if(n == NULL)
        {
            return NULL;
        }

        if(n->right != NULL)
        {
            Node *p = findMaximum(n->left, false);
            return &p->key;
        }

        Node *p = n->parent;
        while(p != NULL && n == p->left)
        {
            n = p;
            p = p->parent;
        }
        if(p == NULL)
        {
            return NULL;
        }
        return &p->key;
        
    }
};

template <class keyType, class valueType>
RBTree<keyType, valueType>::RBTree()
{
        root = NULL;
}

template <class keyType, class valueType>
RBTree<keyType, valueType>::RBTree(keyType keys[], valueType values[], int size)
{
    root = NULL;
    for(int i = 0; i < size; i++)
    {
        insert(keys[i], values[i]);
    }
}

template <class keyType, class valueType>
RBTree<keyType, valueType>::RBTree(const RBTree<keyType, valueType>& copyTo)
{
    root = copyHelper(copyTo.root);
}

template <class keyType, class valueType>
RBTree<keyType, valueType>::~RBTree()
{
    destroyTree(root);
}