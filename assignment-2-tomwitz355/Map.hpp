#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <utility>
#include <exception>
#include <random>

namespace cs540{

template <typename Key_T, typename Mapped_T> class Map{
typedef std::pair<const Key_T, Mapped_T> ValueType;
class SkipList {
  public:
  struct Node {
    std::vector<Node *> forward; // Predecessor
    std::vector<Node *> backward; // Successor
    Node(int level){
      for(int i = 0; i <= level; i++){
        forward.push_back(nullptr);
        backward.push_back(nullptr);
      }
    }
  };
  struct DataNode : public Node {
  public:
    ValueType data;
    DataNode(ValueType value, int level) : Node(level), data(value){}
  };

  int maxLevel;
  int current;
  Node *head;
  Node *tail;
  size_t size;


  SkipList(){
    this->maxLevel = 100;
    this->current = 0;
    this->head = new Node(this->maxLevel);
    this->tail = new Node(this->maxLevel);
    for(int i = 0; i <= this->maxLevel; i++){
      this->head->forward[i] = this->tail;
      this->head->backward[i] = nullptr;
      this->tail->forward[i] = nullptr;
      this->tail->backward[i] = this->head;
    }
    this->size = 0;
  }

  ~SkipList(){
    Node * current = this->head;
    Node * next = nullptr;
    while(current != this->tail){
      next = current->forward[0];
      delete current;
      current = next;
    }
    delete this->tail;
  }

  int randomLevel(){
    float random = (float)rand()/RAND_MAX;
    int level = 0;
    while(random < .5 && level < this->maxLevel){
      level++;
      random = (float)rand()/RAND_MAX;
    }
    return level;
  }

  void insert(ValueType value){


    Node *currentNode = this->head;

    std::vector<Node *> temp = this->head->forward;


    for(int i = this->current; i >= 0; i--){
      while(currentNode->forward[i] != nullptr && currentNode->forward[i] != this->tail && static_cast<DataNode*>(currentNode->forward[i])->data.first < value.first){
          currentNode = currentNode->forward[i];
      }
      temp[i] = currentNode;
    }


    currentNode = currentNode->forward[0];


    if (currentNode == nullptr || currentNode == this->tail || !(static_cast<DataNode*>(currentNode)->data.first == value.first)){

      int rlevel = randomLevel();

      if(rlevel > this->current){
        for(int i = this->current + 1; i < rlevel + 1; i++){
          temp[i] = this->head;
        }

          this->current = rlevel;
      }


      Node * newNode = new DataNode(value, rlevel);


      for(int i = 0; i <= rlevel; i++){
        newNode->forward[i] = temp[i]->forward[i];
        temp[i]->forward[i]->backward[i] = newNode;
        newNode->backward[i] = temp[i];
        temp[i]->forward[i] = newNode;
      }
      this->size++;
      // std::cout << "Successfully Inserted key " << std::endl;
    }
  }

  void deleteNode(const Key_T & key){
    Node *currentNode = this->head;

    std::vector<Node *> temp = this->head->forward;

    for(int i = this->current; i >= 0; i--){
      while(currentNode->forward[i] != NULL  &&  currentNode->forward[i] != this->tail && static_cast<DataNode*>(currentNode->forward[i])->data.first < key){
          currentNode = currentNode->forward[i];
      }
      temp[i] = currentNode;
    }


    currentNode = currentNode->forward[0];

    if(currentNode != NULL && currentNode != this->tail && static_cast<DataNode*>(currentNode)->data.first == key){

      for(int i = 0; i <= this->current; i++){

        if(temp[i]->forward[i] != currentNode){
          break;
        }
        temp[i]->forward[i] = currentNode->forward[i];
        currentNode->forward[i]->backward[i] = temp[i];
      }
      delete currentNode;

      while(this->current > 0 && head->forward[this->current] == this->tail){
        this->current--;
      }
      // std::cout<<"Successfully deleted key "<< std::endl;
      this->size--;
    }
  }

  Node * search(const Key_T & key){
    Node *currentNode = this->head;
    /*
    if(currentNode->forward[0] == this->tail){
      return nullptr;
    }
    */
    for(int i = this->current; i >= 0; i--){
    // std::cout << "made it here" << std::endl;
      while(currentNode->forward[i] && currentNode->forward[i] != this->tail && static_cast<DataNode*>(currentNode->forward[i])->data.first < key){
          currentNode = currentNode->forward[i];
      }
    }

    currentNode = currentNode->forward[0];

    if(currentNode && currentNode != this->tail && static_cast<DataNode*>(currentNode)->data.first == key){
        return currentNode;
    }

    return nullptr;
  }

/*
  void displayList(){
    std::cout<<"\n*****Skip List*****"<< std::endl;
    for(int i=0 ; i<= this->current; i++){
      Node *node = head->forward[i];
      std::cout<<"Level "<<i<<": ";
      while(node != nullptr && node != this->tail){
          std::cout<< static_cast<DataNode*>(node)->data<<" ";
          node = node->forward[i];
      }
      std::cout<<"\n";
    }
  }
*/


    };
SkipList * map;
public:
  class Iterator;
  class ConstIterator;
  class ReverseIterator;
  class Iterator{
  private:
  public:
  typename SkipList::Node * object;
  Iterator() = delete;

  Iterator(const Iterator & obj) = default;

  Iterator(typename SkipList::Node * node){
    this->object = node;
  }

  Iterator(const ConstIterator obj){
    this->object = obj.object;
  }

  Iterator& operator=(const Iterator & obj){
    this->object = obj.object;
    return *this;
  }

  Iterator &operator++(){
    this->object = this->object->forward[0];
    return *this;
  }
  Iterator operator++(int){
    Iterator & ret = *this;
    this->object = this->object->forward[0];
    return ret;
  }

  Iterator &operator--(){
    this->object = this->object->backward[0];
    return *this;
  }

  Iterator operator--(int){
    Iterator & ret = *this;
    this->object = this->object->backward[0];
    return ret;
  }

  ValueType &operator*() const{
    typename SkipList::DataNode * ret = static_cast<typename SkipList::DataNode*>(this->object);
    return ret->data;
  }

  ValueType *operator->() const{
    typename SkipList::DataNode * ret = static_cast<typename SkipList::DataNode*>(this->object);
    return &ret->data;
  }

  friend bool operator==(const Iterator &i1, const Iterator &i2){
    typename SkipList::Node *obj1 = i1.object;
    typename SkipList::Node *obj2 = i2.object;
    if(obj1 == nullptr || obj2 == nullptr){
      return false;
    }
    return obj1 == obj2;
  }

  friend bool operator==(const Iterator &i1, const ConstIterator &i2){
    typename SkipList::Node *obj1 = i1.object;
    typename SkipList::Node *obj2 = i2.object;
    if(obj1 == nullptr || obj2 == nullptr){
      return false;
    }
    return obj1 == obj2;
  }

  friend bool operator!=(const Iterator &i1, const Iterator &i2){
    typename SkipList::Node *obj1 = i1.object;
    typename SkipList::Node *obj2 = i2.object;
    if(obj1 == nullptr || obj2 == nullptr){
      return false;
    }
    return !(obj1 == obj2);
  }

  friend bool operator!= (const Iterator &i1, const ConstIterator &i2){
    typename SkipList::Node *obj1 = i1.object;
    typename SkipList::Node *obj2 = i2.object;
    if(obj1 == nullptr || obj2 == nullptr){
      return false;
    }
    return !(obj1 == obj2);
  }


  };

  class ConstIterator{
  public:
  typename SkipList::Node * object;
  ConstIterator() = delete;

  ConstIterator(const ConstIterator & obj) = default;

  ConstIterator(typename SkipList::Node * node){
    this->object = node;
  }

  ConstIterator(const Iterator obj){
    this->object = obj.object;
  }

  ConstIterator& operator=(const ConstIterator & obj){
    this->object = obj.object;
    return *this;
  }

  ConstIterator &operator++(){
    this->object = this->object->forward[0];
    return *this;
  }
  ConstIterator operator++(int){
    ConstIterator & ret = *this;
    this->object = this->object->forward[0];
    return ret;
  }

  ConstIterator &operator--(){
    this->object = this->object->backward[0];
    return *this;
  }

  ConstIterator operator--(int){
    ConstIterator & ret = *this;
    this->object = this->object->backward[0];
    return ret;
  }

  const ValueType &operator*() const{
    typename SkipList::DataNode * ret = static_cast<typename SkipList::DataNode*>(this->object);
    return ret->data;
  }

  const ValueType *operator->() const{
    typename SkipList::DataNode * ret = static_cast<typename SkipList::DataNode*>(this->object);
    return &ret->data;
  }

  friend bool operator==(const ConstIterator &i1 , const ConstIterator &i2){
    typename SkipList::Node *obj1 = i1.object;
    typename SkipList::Node *obj2 = i2.object;
    if(obj1 == nullptr || obj2 == nullptr){
      return false;
    }
    return obj1 == obj2;
  }

  friend bool operator==(const ConstIterator &i1, const Iterator &i2){
    typename SkipList::Node *obj1 = i1.object;
    typename SkipList::Node *obj2 = i2.object;
    if(obj1 == nullptr || obj2 == nullptr){
      return false;
    }
    return obj1 == obj2;
  }

  friend bool operator!=(const ConstIterator &i1, const ConstIterator &i2){
    typename SkipList::Node *obj1 = i1.object;
    typename SkipList::Node *obj2 = i2.object;
    if(obj1 == nullptr || obj2 == nullptr){
      return false;
    }
    return !(obj1 == obj2);
  }

  friend bool operator!=(const ConstIterator &i1, const Iterator &i2){
    typename SkipList::Node *obj1 = i1.object;
    typename SkipList::Node *obj2 = i2.object;
    if(obj1 == nullptr || obj2 == nullptr){
      return false;
    }
    return !(obj1 == obj2);
  }

  };

  class ReverseIterator{
  private:
  public:
  typename SkipList::Node * object;
  ReverseIterator() = delete;

  ReverseIterator(const ReverseIterator & obj) = default;

  ReverseIterator(typename SkipList::Node * node){
    this->object = node;
  }

  ReverseIterator& operator=(const ReverseIterator & obj){
    this->object = obj.object;
    return *this;
  }

  ReverseIterator &operator++(){
    this->object = this->object->backward[0];
    return *this;
  }
  ReverseIterator operator++(int){
    ReverseIterator & ret = *this;
    this->object = this->object->backward[0];
    return ret;
  }

  ReverseIterator &operator--(){
    this->object = this->object->forward[0];
    return *this;
  }

  ReverseIterator operator--(int){
    ReverseIterator & ret = *this;
    this->object = this->object->forward[0];
    return ret;
  }

  ValueType &operator*() const{
    typename SkipList::DataNode * ret = static_cast<typename SkipList::DataNode*>(this->object);
    return ret->data;
  };

  ValueType *operator->() const{
    typename SkipList::DataNode * ret = static_cast<typename SkipList::DataNode*>(this->object);
    return &ret->data;
  }

  friend bool operator==(const ReverseIterator &i1, const ReverseIterator &i2){
    typename SkipList::Node *obj1 = i1.object;
    typename SkipList::Node *obj2 = i2.object;
    if(obj1 == nullptr || obj2 == nullptr){
      return false;
    }
    return obj1 == obj2;
  }

  friend bool operator!=(const ReverseIterator &i1, const ReverseIterator &i2){
    typename SkipList::Node *obj1 = i1.object;
    typename SkipList::Node *obj2 = i2.object;
    if(obj1 == nullptr || obj2 == nullptr){
      return false;
    }
    return !(obj1 == obj2);
  }
  };

  Map(){
    this->map = new SkipList();
  }
  Iterator begin(){
    Iterator it = Iterator(this->map->head->forward[0]);
    return it;
  }
  Iterator end(){
    Iterator it = Iterator(this->map->tail);
    return it;
  }
  ConstIterator begin() const{
    return ConstIterator(this->map->head->forward[0]);;
  }
  ConstIterator end() const{
    return ConstIterator(this->map->tail);;
  }

  ReverseIterator rbegin(){
    ReverseIterator it = ReverseIterator(this->map->tail->backward[0]);
    return it;
  }
  ReverseIterator rend(){
    ReverseIterator it = ReverseIterator(this->map->head);
    return it;
  }

  Map(const Map & obj){
    this->map = new SkipList();
    for(Iterator it = obj.begin(); it != obj.end(); it++){
      this->map->insert(*it);
    }
  }

  Map &operator=(const Map & obj){
    for(Iterator it = obj.begin(); it != obj.end(); it++){
      this->map->insert(*it);
    }
    return *this;
  }

  Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> input){
    this->map = new SkipList();
    for(auto it = input.begin(); it < input.end(); it++){
      ValueType temp = {it->first, it->second};
      this->map->insert(temp);
    }
  }

  ~Map(){
    delete this->map;
  }

  size_t size() const{
    return this->map->size;
  }

  bool empty() const{
    if(this->map->size == 0){
      return true;
    }
    return false;
  }

  Iterator find(const Key_T & key){
    typename SkipList::Node *ret = this->map->search(key);
    if(ret == nullptr){
      return this->end();
    }
    return Iterator(ret);
  }

  ConstIterator find(const Key_T & key) const{
    typename SkipList::Node *ret = this->map->search(key);
    if(ret == nullptr){
      return this->end();
    }
    return ConstIterator(ret);
  }

  Mapped_T &at(const Key_T & key){
    typename SkipList::Node *ret = this->map->search(key);
    if(ret == nullptr){
      throw std::out_of_range("out of range");
    }
    return static_cast<typename SkipList::DataNode *>(ret)->data.second;
  }

  const Mapped_T &at(const Key_T & key) const{
    typename SkipList::Node *ret = this->map->search(key);
    if(ret == nullptr){
      throw std::out_of_range("out of range");
    }
    return static_cast<typename SkipList::DataNode *>(ret)->data.second;
  }

  Mapped_T &operator[](const Key_T & key){
    typename SkipList::Node *ret = this->map->search(key);
    if(ret != nullptr){
      return static_cast<typename SkipList::DataNode *>(ret)->data.second;
    }
    ValueType newElement = {key, Mapped_T()};
    this->map->insert(newElement);
    typename SkipList::Node *temp = this->map->search(key);
    return static_cast<typename SkipList::DataNode *>(temp)->data.second;
  }

  std::pair<Iterator, bool> insert(const ValueType & value){
    typename SkipList::Node *ret = this->map->search(value.first);
    if(ret == nullptr){
      this->map->insert(value);
      typename SkipList::Node *temp = this->map->search(value.first);
      Iterator it = Iterator(temp);
      return {it, true};
    }
    return {Iterator(ret), false};
  }

  template <typename IT_T>
  void insert(IT_T range_beg, IT_T range_end){
    for(auto i = range_beg; i != range_end; i++){
      ValueType temp(i.first, i.second);
      this->insert(temp);
    }
  }

  void erase(Iterator pos){
    this->map->deleteNode(static_cast<typename SkipList::DataNode *>(pos.object)->data.first);
  }

  void erase(const Key_T & key){
    typename SkipList::Node *ret = this->map->search(key);
    if(ret == nullptr){
      throw std::out_of_range("out of range");
    }
    this->map->deleteNode(key);
  }

  void clear(){
    delete this->map;
    this->map = new SkipList();
  }

  friend bool operator==(const Map &m1, const Map &m2){
    if(m1.size() != m2.size()){
      return false;
    }
    Iterator i1 = m1.begin();
    Iterator i2 = m2.begin();
    for(;i1 != m1.end(); i1++, i2++){
      auto data1 = static_cast<typename SkipList::DataNode *>(i1.object);
      auto data2 = static_cast<typename SkipList::DataNode *>(i2.object);
      if(data1->data != data2->data){
        return false;
      }
    }
    return true;
  }

  friend bool operator!=(const Map &m1, const Map &m2){
    return !(m1 == m2);
  }

  friend bool operator<(const Map &m1, const Map &m2){
    Iterator it1 = m1.begin();
    Iterator it2 = m2.begin();

    if(!it1 && !it2){
      return false;
    }

    if(!it1 && it2){
      return true;
    }
    if(it1 && !it2){
      return false;
    }

    if(m1.size() >= m2.size()){
      for(; it2 != m2.end(); it1++, it2++){
        typename SkipList::DataNode * temp1 = static_cast<typename SkipList::DataNode *>(it1.object);
        typename SkipList::DataNode * temp2 = static_cast<typename SkipList::DataNode *>(it2.object);
        if(!(temp1->data == temp2->data)){
          if(temp2->data < temp1->data){
            return false;
          }
          return true;
        }
      }
      return false;
    }
    else{
      for(; it1 != m1.end(); it1++, it2++){
        typename SkipList::DataNode * temp1 = static_cast<typename SkipList::DataNode *>(it1.object);
        typename SkipList::DataNode * temp2 = static_cast<typename SkipList::DataNode *>(it2.object);
        if(!(temp1->data == temp2->data)){
          if(temp2->data < temp1->data){
            return false;
          }
          return true;
        }
      }
      return true;
    }

  }

};







}

#endif
