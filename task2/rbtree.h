#ifndef RBTREE_H
#define RBTREE_H
#include <algorithm>
#include <memory>
template <class Key, class Comp = std::less<Key>>
class Set
{
public:
    Set() : Set(std::less<Key>()) {}
    Set(Comp comp)
        : root_(nullptr), comp(comp)
    {
    }
    enum NodeColor
    {
        Black,
        Red
    };
    struct Node
    {
        Key key;
        Node *lc, *rc;
        Node *prev, *next;
        size_t size{0};
        NodeColor color;

        Node(Key key, NodeColor color, size_t size)
            : key(key), color(color), size(size), lc(nullptr), rc(nullptr), prev(nullptr), next(nullptr) {}

        Node() = default;
    };
    class Iterator
    {
    public:
        Iterator()
            : nd(nullptr) {}
        Iterator(Node *pnd)
            : nd(pnd) {}
        Node *nd;
        Iterator &operator++()
        {
            nd = nd->next;
            return *this;
        }
        Iterator &operator--()
        {
            nd = nd->prev;
            return *this;
        }
        Iterator operator++(int)
        {
            Iterator it = *this;
            operator++();
            return it;
        }

        Iterator operator--(int)
        {
            Iterator it = *this;
            operator--();
            return it;
        }
        bool operator==(const Iterator &l) 
        {
            return nd==l.nd;
        }
        bool operator!=(const Iterator &l)
        {
            return nd != l.nd;
        }
        Key &operator*()
        {
            return nd->key;
        }
    };

    Iterator find(Key k)
    {
        Node *tmp_root = root_;
        while (tmp_root != nullptr)
        {
            if (tmp_root->key == k)
                return Iterator(tmp_root);
            if (comp(k, tmp_root->key))
                tmp_root = tmp_root->lc;
            else
                tmp_root = tmp_root->rc;
        }
        return Iterator();
    }

    Iterator lower_bound(Key key)
    {
        Node *tmp_root = root_, *ans = nullptr;
        while (tmp_root)
        {
            if (!comp(tmp_root->key, key) && (ans == nullptr || comp(tmp_root->key, ans->key)))
                ans = tmp_root;
            if (comp(key, tmp_root->key))
                tmp_root = tmp_root->lc;
            else if (key == tmp_root->key)
                break;
            else
                tmp_root = tmp_root->rc;
        }
        return Iterator(ans);
    }

    Iterator upper_bound(Key key)
    {
        Iterator x = lower_bound(key);
        if (*x == key)
            return ++x;
        else
            return x;
    }

    Iterator begin() const
    {
        Node *tmp_root = root_;
        if (!tmp_root)
            return Iterator();
        while (tmp_root->lc)
            tmp_root = tmp_root->lc;
        return Iterator(tmp_root);
    }

    Iterator end() const
    {
        return Iterator(nullptr);
    }

    void destroyTree(Node *root)
    {
        if (root != nullptr)
        {
            destroyTree(root->lc);
            destroyTree(root->rc);
            root->lc = root->rc = nullptr;
            delete root;
        }
    }

    bool is_red(Node *nd)
    {
        if (!nd || !nd->color)
            return false;
        return true;
    }

    size_t size(Node *nd)
    {
        if (!nd)
            return 0;
        return nd->size;
    }

    Node *rotate_left(Node *node)
    {
        Node *tmp_root = node->rc;
        node->rc = tmp_root->lc;
        tmp_root->lc = node;
        tmp_root->color = node->color;
        node->color = Red;
        tmp_root->size = node->size;
        node->size = size(node->lc) + size(node->rc) + 1;
        return tmp_root;
    }

    Node *rotate_right(Node *node)
    {
        Node *tmp_root = node->lc;
        node->lc = tmp_root->rc;
        tmp_root->rc = node;
        tmp_root->color = node->color;
        node->color = Red;
        tmp_root->size = node->size;
        node->size = size(node->lc) + size(node->rc) + 1;
        return tmp_root;
    }

    NodeColor neg_color(NodeColor n)
    {
        if (n == Black)
            return Red;
        return Black;
    }

    void color_flip(Node *node)
    {
        node->color = neg_color(node->color);
        node->lc->color = neg_color(node->lc->color);
        node->rc->color = neg_color(node->rc->color);
    }

    Node *insert(Node *root, const Key &key)
    {
        if (root == nullptr)
            return new Node(key, Red, 1);
        if (root->key == key);
        else if (comp(key, root->key))
        { 
            auto x = root->lc;
            root->lc = insert(root->lc, key);
            if (x == nullptr)
            {
                auto newone = root->lc, xp = root->prev;
                if (xp)
                    xp->next = newone;
                newone->next = root;
                root->prev = newone;
                newone->prev = xp;
            }
        }
        else
        {
            auto x = root->rc;
            root->rc = insert(root->rc, key);
            if (x == nullptr)
            {
                auto newone = root->rc, xn = root->next;
                root->next = newone;
                newone->next = xn;
                if (xn)
                    xn->prev = newone;
                newone->prev = root;
            }
        }
        return adjust(root);
    }
    Node *delete_any(Node *root, Key key)
    {
        if (comp(key, root->key))
        {
            if (!is_red(root->lc) && !(is_red(root->lc->lc)))
                root = move_red_left(root);
            root->lc = delete_any(root->lc, key);
        }
        else
        {
            if (is_red(root->lc))
                root = rotate_right(root);
            if (key == root->key && root->rc == nullptr)
            {
                delete root;
                return nullptr;
            }
            if (!is_red(root->rc) && !is_red(root->rc->lc))
                root = move_red_right(root);
            if (key == root->key)
            {
                root->key = get_min(root->rc);
                root->rc = delete_min(root->rc);
            }
            else
            {
                root->rc = delete_any(root->rc, key);
            }
        }
        return adjust(root);
    }
    Node *delete_min(Node *root)
    {
        if (root->lc == nullptr)
        {
            delete root;
            return nullptr;
        }
        if (!is_red(root->lc) && !is_red(root->lc->lc))
        {
            root = move_red_left(root);
        }
        root->lc = delete_min(root->lc);
        return adjust(root);
    }
    Node *move_red_right(Node *root)
    {
        color_flip(root);
        if (is_red(root->lc->lc))
        {
            root = rotate_right(root);
            color_flip(root);
        }
        return root;
    }
    Node *move_red_left(Node *root)
    {
        color_flip(root);
        if (is_red(root->rc->lc))
        {
            root->rc = rotate_right(root->rc);
            root = rotate_left(root);
            color_flip(root);
        }
        return root;
    }
    Node *adjust(Node *root)
    {
        if (is_red(root->rc) && !is_red(root->lc))
            root = rotate_left(root);
        if (is_red(root->lc) &&
            is_red(root->lc->lc))
            root = rotate_right(root);
        if (is_red(root->lc) && is_red(root->rc))
            color_flip(root);
        root->size = size(root->lc) + size(root->rc) + 1;
        return root;
    }
    Key get_min(Node *root)
    {
        Node *x = root;
        while (x->lc)
            x = x->lc;
        return x->key;
    }

    size_t size()
    {
        return size(root_);
    }

    size_t count(Key key)
    {
        Node *x = root_;
        while (x != nullptr)
        {
            if (key == x->key)
                return 1;
            if (comp(key, x->key))
                x = x->lc;
            else
                x = x->rc;
        }
        return 0;
    }

    size_t erase(const Key &key)
    {
        if (count(key) > 0)
        {
            if (!is_red(root_->lc) && !(is_red(root_->rc)))
                root_->color = Red;
            root_ = delete_any(root_, key);
            if (root_ != nullptr)
                root_->color = Black;
            return 1;
        }
        else
        {
            return 0;
        }
    }

    void clear()
    {
        destroyTree(root_);
        root_ = nullptr;
    }

    void insert(const Key &key)
    {
        root_ = insert(root_, key);
        root_->color = Black;
    }

    bool empty()
    {
        return size(root_) == 0;
    }

    Comp comp;
    Node *root_;
};

#endif