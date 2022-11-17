#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{
	
}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    virtual void insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* current);
    virtual void rotateLeft(AVLNode<Key,Value>* current);
    virtual void rotateRight(AVLNode<Key,Value>* current);
		virtual void removeFix(AVLNode<Key, Value>* current, int8_t diff);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO

    if(this->root_ == NULL){
        
        AVLNode<Key, Value>* temp = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);

        this->root_ = temp;			

    } else if(static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(new_item.first)) == NULL){
        // Have to insert a new Node
        
        AVLNode<Key, Value>* temp = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        AVLNode<Key, Value>* parent = static_cast<AVLNode<Key, Value>*>(this->root_);
        
        while(parent != NULL){

            if(parent->getKey() > new_item.first){

                if(parent->getLeft() == NULL){

                    parent->setLeft(temp);
                    temp->setParent(parent);

                    if(parent->getBalance() == 1 || parent->getBalance() == -1){

                        parent->setBalance(0);

                    } else if(parent->getBalance() == 0){

                        parent->updateBalance(-1);
                        insertFix(parent, temp);

                    } 

                    break;

                } else{

                    parent = parent->getLeft();

                }

            } else if(parent->getKey() < new_item.first){

                if(parent->getRight() == NULL){

                    parent->setRight(temp);
                    temp->setParent(parent);

                    if(parent->getBalance() == 1 || parent->getBalance() == -1){

                        parent->setBalance(0);

                    } else if(parent->getBalance() == 0){

                        parent->updateBalance(1);
                        insertFix(parent, temp);
                    }

                    break;

                } else{

                    parent = parent->getRight();

                }
            }
        }
        
    } else{

        AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(new_item.first));

        temp->setValue(new_item.second);

    }

}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* current){

    if(parent == NULL || parent->getParent() == NULL){
    return;
    }

    AVLNode<Key,Value>* grandparent = parent->getParent();

    if(grandparent->getKey() > parent->getKey()){

        grandparent->updateBalance(-1);

        if(grandparent->getBalance() == 0){

            return;

        } else if(grandparent->getBalance() == -1){

            insertFix(grandparent, parent);

        } else{

            if(current->getKey() < parent->getKey()){
                // zig-zig

                rotateRight(grandparent);
                grandparent->setBalance(0);
                parent->setBalance(0);

            } else{
                // zig-zag

                rotateLeft(parent);
                rotateRight(grandparent);

                if(current->getBalance() == -1){

                    parent->setBalance(0);
                    grandparent->setBalance(1);
                    current->setBalance(0);

                } else if(current->getBalance() == 0){

                    parent->setBalance(0);
                    grandparent->setBalance(0);
                    current->setBalance(0);

                } else if(current->getBalance() == 1){

                    parent->setBalance(-1);
                    grandparent->setBalance(0);
                    current->setBalance(0);

                }
            }

        }

    } else{

        grandparent->updateBalance(1);

        if(grandparent->getBalance() == 0){

            return;

        } else if(grandparent->getBalance() == 1){

            insertFix(grandparent, parent);

        } else{

            if(current->getKey() > parent->getKey()){
            // zig-zig

                rotateLeft(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);

            } else{
            // zig-zag

                rotateRight(parent);
                rotateLeft(grandparent);

                if(current->getBalance() == 1){

                parent->setBalance(0);
                grandparent->setBalance(-1);
                current->setBalance(0);

                } else if(current->getBalance() == 0){

                parent->setBalance(0);
                grandparent->setBalance(0);
                current->setBalance(0);

                } else if(current->getBalance() == -1){

                parent->setBalance(1);							
                grandparent->setBalance(0);
                current->setBalance(0);

                }

            }

        }

    }

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* current){
    

    AVLNode<Key, Value>* rightChild = current->getRight();
    AVLNode<Key, Value>* temp = rightChild->getLeft();


    if(current->getParent() == NULL){

        this->root_ = rightChild;
        rightChild->setParent(NULL);
        current->setParent(rightChild);
        rightChild->setLeft(current);
        current->setRight(temp);

        if(temp != NULL){
          temp->setParent(current);
        }

    } else{

        if(current->getParent()->getKey() > current->getKey()){

            rightChild->setParent(current->getParent());
            current->getParent()->setLeft(rightChild);
            current->setParent(rightChild);
            rightChild->setLeft(current);
            current->setRight(temp);
            if(temp != NULL){
                temp->setParent(current);
            }

        } else{

            rightChild->setParent(current->getParent());
            current->getParent()->setRight(rightChild);
            current->setParent(rightChild);
            rightChild->setLeft(current);
            current->setRight(temp);
            if(temp != NULL){
                temp->setParent(current);
            }

        }

    }

}

template<class Key, class Value>
void AVLTree<Key, Value>:: rotateRight(AVLNode<Key, Value>* current){

    AVLNode<Key, Value>* leftChild = current->getLeft();
    AVLNode<Key, Value>* temp = leftChild->getRight();

	if(current->getParent() == NULL){

		this->root_ = leftChild;
		leftChild->setParent(NULL);
		current->setParent(leftChild);
		leftChild->setRight(current);
		current->setLeft(temp);
        if(temp != NULL){
            temp->setParent(current);
        }

	} else{

		if(current->getParent()->getKey() < current->getKey()){

			leftChild->setParent(current->getParent());
			current->getParent()->setRight(leftChild);
			current->setParent(leftChild);
			leftChild->setRight(current);
			current->setLeft(temp);
            if(temp != NULL){
                temp->setParent(current);
            }

		} else{
            
            leftChild->setParent(current->getParent());
            current->getParent()->setLeft(leftChild);
            current->setParent(leftChild);
            leftChild->setRight(current);
            current->setLeft(temp);
            if(temp != NULL){
                temp->setParent(current);
            }

        }

	}


}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO

		int8_t diff = 0;

    if(this->root_ == NULL){
        return;
    }

    if(static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key)) == NULL){
        return;
    }

    if(this->root_->getLeft() == NULL && this->root_->getRight() == NULL){
				
        delete this->root_;
        this->root_ = NULL;
				return;

    }

		AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));

		if(temp->getLeft() != NULL && temp->getRight() != NULL){

			AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(temp));
			nodeSwap(pred, temp);

		}

		AVLNode<Key, Value>* child(NULL);
		AVLNode<Key, Value>* parent = temp->getParent();

		if(temp->getLeft() == NULL && temp->getRight() == NULL){
		// Leaf Node
		
			child = NULL;

		} else if(temp->getLeft() == NULL && temp->getRight() != NULL){
		// Has right Child

			child = temp->getRight();

		} else if(temp->getLeft() != NULL && temp->getRight() == NULL){
		// Has left Child

			child = temp->getLeft();
		}

		if(temp->getParent() != NULL){

			if(temp == temp->getParent()->getRight()){
				diff = -1;
			} else if(temp == temp->getParent()->getLeft()){
				diff = 1;
			}

		}

		if(parent == NULL){

			this->root_ = child;

		} else{

			if(temp == temp->getParent()->getRight()){
				
				parent->setRight(child);

			} else{
				
				parent->setLeft(child);

			}

		}

		if(child != NULL){

			child->setParent(parent);

		}


		delete temp;
	
		if(this->root_->getLeft() == NULL && this->root_->getRight() == NULL){
			return;
    }

		removeFix(parent, diff);

}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* current, int8_t diff){

	int8_t ndiff = 0;

	if(current == NULL){
		return;
	}

	AVLNode<Key, Value>* parent = current->getParent();

	if(parent != NULL){
		
		if(current->getKey() > parent->getKey()){

			ndiff = -1;

		} else if(current->getKey() < parent->getKey()){

			ndiff = 1;

		}
		
	}

	if(diff == -1){

		if(current->getBalance() + diff == -2){

			AVLNode<Key, Value>* c = current->getLeft();

			if(c->getBalance() == -1){

				rotateRight(current);
				current->setBalance(0);
				c->setBalance(0);
				removeFix(parent, ndiff);

			} else if(c->getBalance() == 0){

				rotateRight(current);
				current->setBalance(-1);
				c->setBalance(1);

			} else if(c->getBalance() == 1){

				AVLNode<Key, Value>* g = c->getRight();

				rotateLeft(c);
				rotateRight(current);

				if(g->getBalance() == 1){

					current->setBalance(0);
					c->setBalance(-1);
					g->setBalance(0);

				} else if(g->getBalance() == 0){

					current->setBalance(0);
					c->setBalance(0);
					g->setBalance(0);

				} else if(g->getBalance() == -1){

					current->setBalance(1);
					c->setBalance(0);
					g->setBalance(0);

				}

				removeFix(parent, ndiff);

			}

		} else if(current->getBalance() + diff == -1){

			current->setBalance(-1);

		} else if(current->getBalance() + diff == 0){

			current->setBalance(0);
			removeFix(parent, ndiff);

		}

	} else if(diff == 1){

		if(current->getBalance() + diff == 2){

			AVLNode<Key, Value>* c = current->getRight();

			if(c->getBalance() == 1){

				rotateLeft(current);
				current->setBalance(0);
				c->setBalance(0);
				removeFix(parent, ndiff);

			} else if(c->getBalance() == 0){

				rotateLeft(current);
				current->setBalance(1);
				c->setBalance(-1);

			} else if(c->getBalance() == -1){

				AVLNode<Key, Value>* g = c->getLeft();

				rotateRight(c);
				rotateLeft(current);

				if(g->getBalance() == -1){

					current->setBalance(0);
					c->setBalance(1);
					g->setBalance(0);

				} else if(g->getBalance() == 0){

					current->setBalance(0);
					c->setBalance(0);
					g->setBalance(0);

				} else{

					current->setBalance(-1);
					c->setBalance(0);
					g->setBalance(0);

				}

				removeFix(parent, ndiff);

			}

		} else if(current->getBalance() + diff == 1){

			current->setBalance(1);

		} else if(current->getBalance() + diff == 0){

			current->setBalance(0);
			removeFix(parent, ndiff);

		}
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
