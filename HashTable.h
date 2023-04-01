#include <list>
#include <iostream>

#include "Queue.h"

#ifndef HashTable_h
#define HashTable_h

template <typename K, typename V, int CAP>
class HashTable
{
  struct Node
  {
    K key;
    V value;
  };

  list<Node> data[CAP];
  int(*hashCode)(const K&);
  int siz;

public:
  HashTable(int(*)(const K&) = 0);
  double loadFactor() const {return 1.0 * siz / CAP;}
  V operator[] (const K&) const;
  V& operator[] (const K&);
  bool containsKey(const K&) const;
  void deleteKey(const K&);
  Queue<K> keys() const;
  int size() const;
  void clear();
};

//constructor
template <typename K, typename V, int CAP>
HashTable<K, V, CAP>::HashTable(int(*hashCode)(const K&))
{
  siz = 0;
  this->hashCode = hashCode;
}

//[] getter
template <typename K, typename V, int CAP>
V HashTable<K, V, CAP>::operator[] (const K& key) const
{
  //wrap index
  int index = hashCode(key) % CAP;
  if(index < 0)
  {
    index += CAP;
  }

  //find value at index
  typename list<Node>::const_iterator it;
  for (it = data[index].begin(); it != data[index].end(); it++)
  {
    if (it->key == key)
    {
      return it->value;
    }
  }

  V dummy = V();
  return dummy;
}

//[] setter
template <typename K, typename V, int CAP>
V& HashTable<K, V, CAP>::operator[] (const K& key)
{
  //wrap index
  int index = hashCode(key) % CAP;
  if(index < 0)
  {
    index += CAP;
  }

  //find value at index
  typename list<Node>::iterator it;
  for (it = data[index].begin(); it != data[index].end(); it++)
  {
    if (it->key == key)
    {
      return it->value;
    }
  }

  //add value if it does not already exist
  siz++;
  Node temp;
  temp.key = key;
  temp.value = V();
  data[index].push_back(temp);
  return data[index].back().value;
}

//contains key
template <typename K, typename V, int CAP>
bool HashTable<K, V, CAP>::containsKey(const K& key) const
{
  //wrap index
  int index = hashCode(key) % CAP;
  if (index < 0)
  {
    index += CAP;
  }

  //check if value exist
  typename list<Node>::const_iterator it;
  for (it = data[index].begin(); it != data[index].end(); it++)
  {
    if (it->key == key)
    {
      return true;
    }
  }
  return false;
}

//delete key
template <typename K, typename V, int CAP>
void HashTable<K, V, CAP>::deleteKey(const K& key)
{
  //wrap index
  int index = hashCode(key) % CAP;
  if (index < 0)
  {
    index += CAP;
  }

  typename list<Node>::iterator it;
  for (it = data[index].begin(); it != data[index].end(); it++)
  {
    if (it->key == key)
    {
      data[index].erase(it);
      siz--;
    }
  }
}

//key
template <typename K, typename V, int CAP>
Queue<K> HashTable<K, V, CAP>::keys() const
{
  Queue<K> dataList;
  typename list<Node>::const_iterator it;
  for(int i = 0; i < CAP; i++)
  {
    for (it = data[i].begin(); it != data[i].end(); it++)
    {
      if(!data[i].empty()){
        dataList.push(it->key);
      }
    }
  }
  return dataList;
}

//size
template <typename K, typename V, int CAP>
int HashTable<K, V, CAP>::size() const
{
  return siz;
}

//clear
template <typename K, typename V, int CAP>
void HashTable<K, V, CAP>::clear()
{
  for(int i = 0; i < CAP; i++){
    data[i].clear();
  }
  siz = 0;
}

#endif /* HashTable_h */
