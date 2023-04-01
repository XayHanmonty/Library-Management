#ifndef Queue_h
#define Queue_h

template <typename V>
class Queue
{
  struct Node
  {
    V value;
    Node* next;
  };
  int siz;
  Node* firstNode; //head pointer
  Node* lastNode; //tail pointer
  V dummy;
  
public:
  Queue(); //main constructor
  Queue(const Queue<V>&); //copy constructor
  ~Queue();
  Queue<V>& operator=(const Queue<V>&);
  void push(const V&);
  const V& front() const;
  const V& back() const;
  void pop();
  int size() const;
  bool empty() const;
  void clear();
};

//main constructor
template <typename V>
Queue<V>::Queue()
{
  firstNode = nullptr;
  lastNode = nullptr;
  siz = 0;
  dummy = V();
}

//copy constructor
template <typename V>
Queue<V>::Queue(const Queue<V>& original)
{
  firstNode = nullptr;
  lastNode = nullptr;
  siz = original.siz;
  
  for(Node* ptr = original.firstNode; ptr != nullptr; ptr = ptr->next)
  {
    Node* temp = new Node;
    temp->value = ptr->value;
    temp->next = nullptr;
    if (lastNode != nullptr)
    {
      lastNode->next = temp;
    }
    else
    {
      firstNode = temp;
    }
    lastNode = temp;
  }
}

//destructor
template <typename V>
Queue<V>::~Queue()
{
  while(firstNode != nullptr)
  {
    Node* ptr = firstNode;
    firstNode = firstNode->next;
    delete ptr;
  }
}

//Assignment Operator
template <typename V>
Queue<V>& Queue<V>::operator=(const Queue<V>& original)
{
  if(this != &original)
  {
    while (firstNode != nullptr)
    {
      Node* ptr = firstNode;
      firstNode = firstNode->next;
      delete ptr;
    }

    lastNode = nullptr;
    for(Node* ptr = original.firstNode; ptr != nullptr; ptr = ptr->next)
    {
      Node* temp = new Node;
      temp->value = ptr->value;
      temp->next = nullptr;
      if(lastNode != nullptr)
      {
        lastNode->next = temp;
      }
      else
      {
        firstNode = temp;
      }
      lastNode = temp;
    }
    siz = original.siz;
  }
  return *this;
}

//push
template <typename V>
void Queue<V>::push(const V& value)
{
  Node* temp = new Node;
  temp->value = value;
  temp->next = nullptr;
  if(lastNode != nullptr)
  {
    lastNode->next = temp;
  }
  else
  {
    firstNode = temp;
  }
  lastNode = temp;
  siz++;
}

//front
template <typename V>
const V& Queue<V>::front() const
{
  if (firstNode == nullptr)
  {
    return dummy;
  }
  return firstNode->value;
}

//back
template <typename V>
const V& Queue<V>::back() const
{
  if (lastNode == nullptr)
  {
    return dummy;
  }
  return lastNode->value;
}

//pop
template <typename V>
void Queue<V>::pop( )
{
  if (firstNode != nullptr)
  {
    Node* ptr = firstNode;
    firstNode = firstNode->next;
    delete ptr;
    siz--;
  }
  
  if(siz == 0)
  {
    lastNode = nullptr;
  }
}

//clear
template <typename V>
void Queue<V>::clear( )
{
  while (firstNode != nullptr)
  {
    Node* ptr = firstNode;
    firstNode = firstNode->next;
    delete ptr;
    siz--;
  }
  
  if(siz == 0)
  {
    lastNode = nullptr;
  }
}

//size
template <typename V>
int Queue<V>::size() const
{
  return siz;
}
  
//empty
template <typename V>
bool Queue<V>::empty() const
{
  return siz == 0;
}

#endif /* Queue_h */

