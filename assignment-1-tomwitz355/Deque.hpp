#ifndef DEQUE_H
#define DEQUE_H

#include <stdlib.h>
#include <stdio.h>

#define Deque_DEFINE(t)                                                        \
    struct Deque_##t##_Iterator;                                               \
    struct Deque_##t {                                                         \
        t *data;                                                               \
        size_t cont_size;                                                      \
        char type_name[sizeof("Deque_" #t)] = "Deque_" #t;                     \
        size_t num_ele = 0;                                                    \
        int first = -1;                                                        \
        int last = -1;                                                         \
        bool (*comp)(const t &o1, const t &o2);                                \
        size_t (* size)(Deque_##t *);                                          \
        void (*push_back)(Deque_##t *, t obj);                                 \
        void (*push_front)(Deque_##t *, t obj);                                \
        bool (*empty)(Deque_##t *);                                            \
        t & (*front)(Deque_##t *);                                             \
        t & (*back)(Deque_##t *);                                              \
        void (*pop_back)(Deque_##t *);                                         \
        void (*pop_front)(Deque_##t *);                                        \
        void (*clear)(Deque_##t *);                                            \
        Deque_##t##_Iterator (*begin)(Deque_##t *dp);                          \
        Deque_##t##_Iterator (*end)(Deque_##t *dp);                            \
        void (*sort)(Deque_##t *dp, Deque_##t##_Iterator i1, Deque_##t##_Iterator i2); \
        t &(*at)(Deque_##t *, int i);                                          \
        void (*dtor)(Deque_##t *);                                            \
    };                                                                         \
    struct Deque_##t##_Iterator {                                              \
      t *current_obj;                                                          \
      int index;                                                               \
      Deque_##t *dp;                                                           \
      t &(*deref)(Deque_##t##_Iterator * ip);                                  \
      void (*inc)(Deque_##t##_Iterator * ip);                                  \
      void (*dec)(Deque_##t##_Iterator * ip);                                  \
      Deque_##t##_Iterator(){}                                                 \
    };                                                                         \
    bool Deque_##t##_Iterator_equal(Deque_##t##_Iterator ip1, Deque_##t##_Iterator ip2){  \
      if(ip1.index == ip2.index){                                            \
        return true;                                                           \
      }                                                                        \
      return false;                                                            \
    }                                                                          \
    t &dref(Deque_##t##_Iterator * ip){                                        \
      return *(ip->current_obj);                                               \
    }                                                                          \
    void increment(Deque_##t##_Iterator * ip){                                 \
      if(ip->index == ip->dp->last){                                           \
        ip->index = ip->dp->cont_size;                                         \
      }                                                                        \
      else{                                                                    \
        ip->index +=1;                                                         \
        ip->index = ip->index % ip->dp->cont_size;                             \
        ip->current_obj = &ip->dp->data[ip->index];                            \
      }                                                                        \
    }                                                                          \
    void decrement(Deque_##t##_Iterator * ip){                                 \
      if((unsigned long int)ip->index == ip->dp->cont_size){                   \
        ip->index = ip->dp->last;                                              \
        ip->current_obj = &ip->dp->data[ip->index];                            \
      }                                                                        \
      else if(ip->index == 0){                                                 \
        ip->index = ip->dp->cont_size -1;                                      \
        ip->current_obj = &ip->dp->data[ip->index];                            \
      }                                                                        \
      else{                                                                    \
        ip->index -= 1;                                                        \
        ip->current_obj = &ip->dp->data[ip->index];                            \
      }                                                                        \
    }                                                                          \
    void Deque_##t##_Iterator_ctor(Deque_##t##_Iterator * ip, Deque_##t *dp, int index){    \
      if((unsigned long int)index == dp->cont_size){                                              \
        ip->current_obj = nullptr;                                             \
      }                                                                        \
      else{                                                                    \
        ip->current_obj = &dp->data[index];                                    \
      }                                                                        \
      ip->index = index;                                                       \
      ip->dp = dp;                                                             \
      ip->deref = &dref;                                                       \
      ip->inc = &increment;                                                    \
      ip->dec = &decrement;                                                    \
    }                                                                          \
    t &Deque_##t##_at(Deque_##t *dp, int i) {                                  \
        return dp->data[(dp->first + i)%dp->cont_size];                        \
    }                                                                          \
    void Deque_##t##_delete(Deque_##t *dp) {                                   \
        free(dp->data);                                                        \
    }                                                                          \
    size_t sz(Deque_##t *dp){                                                  \
      return dp->num_ele;                                                      \
    }                                                                          \
    bool empt(Deque_##t *dp){                                                  \
      if(dp->num_ele == 0){                                                    \
        return true;                                                           \
      }                                                                        \
      return false;                                                            \
    }                                                                          \
    bool isFull(Deque_##t *dp){                                                \
      if(dp->last + 1 == dp->first){                              \
        return true;                                            \
      }                                                       \
      if((long unsigned int)dp->last == dp->cont_size -1 && dp->first == 0){               \
        return true;                    \
      }                             \
      return false;                 \
    }                                   \
    void resize(Deque_##t *dp){       \
      if(dp->first == 0){                   \
        t *temp = (t*)malloc(dp->cont_size * 2 *sizeof(t));    \
        memcpy(temp, dp->data, dp->cont_size * sizeof(t));      \
        free(dp->data);       \
        dp->data = temp;          \
        dp->cont_size = dp->cont_size * 2;            \
      }               \
      else{             \
        t *temp = (t*)malloc(dp->cont_size * 2 *sizeof(t));        \
        int k = 0;         \
        for(int i = dp->first; (long unsigned int)i < dp->cont_size; i++){       \
          temp[k] = dp->data[i];        \
          k++;        \
        }       \
        for(int i = 0; i < dp->last+1; i++){        \
          temp[k] = dp->data[i];      \
          k++;                        \
        }         \
        free(dp->data);         \
        dp->data = temp;            \
        dp->first = 0;              \
        dp->last = dp->cont_size - 1;         \
        dp->cont_size = dp->cont_size * 2;      \
      }                         \
    }                                   \
    void pb(Deque_##t *dp, t obj){      \
      if(isFull(dp)){                           \
        resize(dp);                           \
      }                                           \
      if(dp->empty(dp)){                          \
        dp->data[0] = obj;                          \
        dp->first = dp->last = 0;                     \
        dp->num_ele += 1;                      \
      }                                   \
      else if((unsigned long int)dp->last < dp->cont_size -1){                  \
        dp->last += 1;                                \
        dp->data[dp->last] = obj;                   \
        dp->num_ele +=1;                                 \
      }                                     \
      else{                                   \
        dp->last = 0;                           \
        dp->data[dp->last] = obj;                     \
        dp->num_ele += 1;                            \
      }                                             \
    }                                                   \
    void pf(Deque_##t *dp, t obj){          \
      if(isFull(dp)){                       \
        resize(dp);                         \
      }                                     \
      if(dp->empty(dp)){                    \
        dp->data[0] = obj;                  \
        dp->first = dp->last = 0;           \
        dp->num_ele += 1;                   \
      }                               \
      else if(dp->first == 0 ){               \
        dp->first = (int)dp->cont_size -1;        \
        dp->data[dp->first] = obj;            \
        dp->num_ele += 1;                     \
      }                               \
      else{                     \
        dp->first -= 1;                   \
        dp->data[dp->first] = obj;                  \
        dp->num_ele +=1;                  \
      }                                       \
    }                                                         \
    t &ft(Deque_##t *dp){                        \
      return dp->data[dp->first];                               \
    }                                                   \
    t &bk(Deque_##t *dp){                               \
      return dp->data[dp->last];                                      \
    }                                                     \
    void ppb(Deque_##t *dp){                                \
      if(dp->empty(dp)){                                  \
        return;                                      \
      }                                                   \
      if(dp->last == 0){                                        \
        dp->last = dp->cont_size - 1;                               \
        dp->num_ele -= 1;                               \
      }                                                   \
      else{                                                     \
        dp->last -= 1;                                              \
        dp->num_ele -=1;                                        \
      }                                                             \
      if(dp->empty(dp)){                                        \
        dp->first = -1;                                           \
        dp->last = -1;                                                \
      }                                                             \
    }                                                               \
      void ppf(Deque_##t *dp){                                      \
        if(dp->empty(dp)){                                        \
          return;                                               \
        }                                                 \
        if((unsigned long int)dp->first == dp->cont_size -1){                                    \
          dp->first = 0;                                      \
          dp->num_ele -=1;                                      \
        }                                                         \
        else{                                                     \
          dp->first += 1;                                           \
          dp->num_ele -= 1;                                     \
        }                                                             \
        if(dp->empty(dp)){                                        \
          dp->first = -1;                                                   \
          dp->last = -1;                                                  \
        }                                                               \
      }                                                                         \
      void clr(Deque_##t *dp){                                                \
        dp->first = dp->last = -1;                                            \
        dp->num_ele = 0;                                                      \
      }                                                                             \
    Deque_##t##_Iterator bgn(Deque_##t *dp){                                  \
      Deque_##t##_Iterator it;                                                  \
      Deque_##t##_Iterator_ctor(&it, dp, dp->first);                             \
      return it;                                                              \
    }                                                                             \
    Deque_##t##_Iterator ed(Deque_##t *dp){                                  \
      Deque_##t##_Iterator it;                                                  \
      Deque_##t##_Iterator_ctor(&it, dp, dp->cont_size);                            \
      return it;                                                              \
    }                                                                           \
    bool Deque_##t##_equal(Deque_##t dp1, Deque_##t dp2){                       \
      if(dp1.num_ele != dp2.num_ele){                                           \
        return false;                                                          \
      }                                                                         \
      Deque_##t##_Iterator it1 = dp1.begin(&dp1);                                              \
      Deque_##t##_Iterator it2 = dp2.begin(&dp2);                                              \
      while(!Deque_##t##_Iterator_equal(it1, dp1.end(&dp1)) && !Deque_##t##_Iterator_equal(it2, dp2.end(&dp2))){ \
        if(dp1.comp(it1.deref(&it1), it2.deref(&it2)) || dp2.comp(it2.deref(&it2), it1.deref(&it1))){   \
          return false;                                                          \
        }                                                                      \
        it1.inc(&it1);                                                         \
        it2.inc(&it2);                                                         \
      }                                                                        \
      return true;                                                             \
    }                                                                          \
    int qsort_r_comp_##t(const void * o1,const void * o2, void * deque){           \
      int ret1 = ((Deque_##t *)(deque))->comp(*(const t*)o1, *(const t*)o2);    \
      int ret2 = ((Deque_##t *)(deque))->comp(*(const t*)o2, *(const t*)o1);    \
      if(ret1 == true){                                                         \
        return -1;                                                              \
      }                                                                        \
      if(ret2 == true){                                                        \
        return 1;                                                             \
      }                                                                        \
      return 0;                                                                \
    }                                                                          \
    void srt(Deque_##t *dp, Deque_##t##_Iterator i1, Deque_##t##_Iterator i2){ \
      int sorted = 1;                                                          \
      while(!Deque_##t##_Iterator_equal(i1, i2)){                              \
        t temp = i1.deref(&i1);                                                \
        i1.inc(&i1);                                                            \
        bool ret1 = dp->comp(temp,i1.deref(&i1));                              \
        bool ret2 = dp->comp(i1.deref(&i1),temp);                              \
        if(!ret1  && ret2){                                                    \
          sorted = 0;                                                          \
          break;                                                               \
        }                                                                      \
      }                                                                         \
      if(sorted == 1){                                                         \
        return;                                                                \
      }                                                                        \
      if(dp->last < dp-> first){                                               \
        int i = i1.dp->first;                                                       \
        int k = 0;                                                               \
        int end = i2.dp->last;                                                      \
        t * temp = (t*)malloc(sizeof(t) * dp->cont_size);                      \
        while(i != end){                                                         \
          temp[k] = dp->data[i % dp->cont_size];                                 \
          k++;                                                                   \
          i++;                                                                   \
          free(dp->data);                                                          \
          dp->data = temp;                                                      \
        }                                                                      \
        temp[k] = dp->data[i % dp->cont_size];                                 \
      }                                                                        \
      qsort_r(&dp->data[dp->first], dp->num_ele, sizeof(t), &qsort_r_comp_##t,(void*)dp);                           \
      dp->first = 0;                                                           \
      dp->last = dp->num_ele -1;                                               \
    }                                                                          \
    Deque_##t *Deque_##t##_ctor(Deque_##t *dp, bool (cp)(const t &o1, const t &o2)) {                             \
        dp->at = &Deque_##t##_at;                                              \
        dp->dtor = &Deque_##t##_delete;                                       \
        dp->comp = cp;                                                         \
        dp->size = &sz;                                                        \
        dp->empty = &empt;                                                     \
        dp->push_back = &pb;                                                   \
        dp->push_front = &pf;                                                  \
        dp->front = &ft;                                                       \
        dp->back = &bk;                                                        \
        dp->pop_back = &ppb;                                                   \
        dp->pop_front = &ppf;                                                  \
        dp->clear = &clr;                                                      \
        dp->begin = &bgn;                                                      \
        dp->end = &ed;                                                         \
        dp->sort = &srt;                                                       \
        dp->data = (t*)malloc(sizeof(t)*8);                                    \
        dp->cont_size = 8;                                                     \
        return dp;                                                             \
    }                                                                          \


#endif
