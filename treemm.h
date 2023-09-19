#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <vector>
#include <cassert>
#include <string>

template <typename KeyType, typename ValueType>
class TreeMultimap
{
  public:
    class Iterator
    {
      public:
        Iterator()
        {
            valueNumber = 0;
            values = nullptr;
        }

        Iterator(std::vector<ValueType>* v){
            valueNumber = 0;
            values = v;
        }
        
        ValueType& get_value() const //
        {
            return (*values)[valueNumber];
        }

        bool is_valid() const
        {
            if (values == nullptr || valueNumber >= values->size())
                return false;  //if iterator doesn't point anywhere OR has passed end of the vector
            else
                return true;
        }

        void advance()
        {
            valueNumber++;
        }

      private:
        int valueNumber;
        std::vector<ValueType>* values;
    };

    TreeMultimap()
    {
        // Replace this line with correct code.
        m_root = nullptr;
        m_invalid_iterator = new Iterator;
    }

    ~TreeMultimap()
    {
        for (int i = 0; i < iterators.size(); i++){
            delete iterators[i]; // delete all the iterators that treeMultiMap allocated
            iterators[i] = nullptr;
        }
        delete m_invalid_iterator;
        m_invalid_iterator = nullptr;
        //delete the nodes of the multimap
        freeTree(m_root);
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        // Replace this line with correct code.
        if (m_root == nullptr){
            m_root = createNode(m_root, key, value);
            return;
        }
        Node* cur = m_root;
        for (;;){
            if (key == cur->key){
                cur->values.push_back(value);
                return;
            }
            else if (key < cur->key){
                if (cur->left != nullptr){
                    cur = cur->left;
                }
                else{
                    cur->left = createNode(cur->left, key, value);
                    return;
                }
            }
            else if (key > cur->key){
                if (cur->right != nullptr){
                    cur = cur->right;
                }
                else{
                    cur->right = createNode(cur->right, key, value);
                    return;
                }
            }
        }
    }

    Iterator find(const KeyType& key) const
    {
        return *(findHelper(key, m_root));
    }

  private:
    std::vector<Iterator*> iterators;
    Iterator* m_invalid_iterator; // When an item isn't found, return this invalid iterator we created
    struct Node {
        KeyType key;
        std::vector<ValueType> values;
        Node* left;
        Node* right;
        Iterator* m_iterator_at_beginning;
    };
    Node* m_root;
    
    Node* createNode(Node* parent, const KeyType& key, const ValueType& value){
        parent = new Node;
        parent->key = key;
        parent->values.push_back(value);
        //Create a new iterator at the beginning of the parent object, and add the iterator to the array of iterators
        Iterator* temp = new Iterator(&(parent->values));//Create a new iterator object at the beginning of the values vector
        parent->m_iterator_at_beginning = temp;
        iterators.push_back(temp); // keep track of this newly created iterator
        
        parent->left = nullptr;
        parent->right = nullptr;
        return parent;
    }
    
    Iterator* findHelper(const KeyType& key, Node* root) const{
        if (root == nullptr){
            return m_invalid_iterator;// Give an iterator that will always return invalid if a node with the key is never found
        }
        else if (root->key == key ){
            return root->m_iterator_at_beginning; // return the iterator at the beginning
        }
        else if (root->key > key){
            return findHelper(key, root->left);//look in left brach if key is < root's key
        }
        else if (root->key < key){
            return findHelper(key, root->right);// look in right branch if key > root's key
        }
        return nullptr;
    }
    void freeTree(Node* root){
        if (root == nullptr){
            return;
        }
        freeTree(root->left);//free left branch
        freeTree(root->right);//free right branch
        delete root;//free the root node
        root = nullptr;
    }
};

#endif // TREEMULTIMAP_INCLUDED
