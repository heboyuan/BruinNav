#ifndef My_M_
#define My_M_

#include<iostream>
#include<string>
#include"provided.h"

using namespace std;
// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap();
	~MyMap();
	void clear();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
	struct Node
	{
		Node(KeyType key, ValueType value)
		{
			left = nullptr;
			right = nullptr;
			m_key = key;
			m_value = value;
		}
		Node* left;
		Node* right;
		KeyType m_key;
		ValueType m_value;
	};
	void Rclear(Node* p);
	Node* root;
	int m_size;
};


template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::Rclear(Node* p)	//recursively clean up the dynamically allocated
{
	if (p != nullptr)
	{
		Rclear(p->left);
		Rclear(p->right);
		delete p;
	}
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear()
{
	Rclear(root);	//call the recursivelly clean functions
	root = nullptr;
}

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap()
{
	root = nullptr;
	m_size = 0;
}

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap()
{
	clear();
}

template<typename KeyType, typename ValueType>
int MyMap<KeyType, ValueType>::size() const
{
	return m_size;
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	if (m_size == 0)	//if is empty
	{
		root = new Node(key,value);
		m_size++;
	}

	Node* p = root;
	while (true)	//keep going
	{
		if (key == p->m_key)	//if find the key
		{
			p->m_value = value;	//change the value
			return;
		}

		if (key < p->m_key)	//if key is smaller
		{
			if (p->left != nullptr)	//if can go left, go
				p = p->left;
			else				//else create new node
			{
				p->left = new Node(key, value);
				m_size++;
				return;
			}
		}
		else if (key > p->m_key)	//if key is bigger
		{
			if (p->right != nullptr)	//if can go right, go
				p = p->right;
			else 				//else create new node
			{
				p->right = new Node(key, value);
				m_size++;
				return;
			}
		}
	}


}

template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const
{
	Node* p = root;
	while (p != nullptr)		//keep going until reach the end
	{
		if (key == p->m_key)	//if find the key, return the value
		{
			return &p->m_value;
		}

		if (key < p->m_key)	//if key is smaller
		{
			p = p->left;		//go left
		}
		else if (key > p->m_key)	//if key is bigger
		{
			p = p->right;		//go right
		}
	}
	return nullptr;
}
#endif