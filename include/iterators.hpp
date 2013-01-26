//=======================================================================
// Copyright Baptiste Wicht 2011-2013.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef ITERATORS_H
#define ITERATORS_H

#include <utility>

namespace eddic {
    
template<typename Container>
struct Iterators {
    typedef typename Container::iterator Iterator;

    Container& container;

    Iterator it;
    Iterator end;

    Iterators(Container& container) : container(container), it(container.begin()), end(container.end()) {}

    typename Container::value_type& operator*(){
        return *it;
    }
    
    typename Container::value_type* operator->(){
        return &*it;
    }

    void operator++(){
        ++it;
    }
    
    void operator--(){
        --it;
    }

    template<typename T>
    void insert(T&& value){
        it = container.insert(it, std::forward<T>(value));
        end = container.end();
    }

    template<typename T>
    void insert_no_move(T&& value){
        it = container.insert(it, std::forward<T>(value));
        end = container.end();
        ++it;
    }
    
    template<typename T>
    void insert_after(T&& value){
        ++it;
        it = container.insert(it, std::forward<T>(value));
        end = container.end();
    }

    void erase(){
        it = container.erase(it);
        end = container.end();
    }

    bool has_next(){
        return it != end;
    }
    
    bool has_previous(){
        return it != container.begin();
    }

    void restart(){
        it = container.begin();
        end = container.end();
    }

    void update(){
        end = container.end();
    }
};

template<typename Container>
Iterators<Container> iterate(Container& container){
    Iterators<Container> iterators(container);

    return iterators;
}

}

#endif
