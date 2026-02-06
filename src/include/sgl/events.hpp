#pragma once
#include<SDL3/SDL.h>
#include<iterator>
namespace sgl{
    namespace detail{
        class event_iterator_stop{};
    }
    class event_iterator{
        mutable SDL_Event buf; // input_iterator requires a const iterator to still return a mutable reference; having the buffer be mutable is pretty helpful
        bool exists;
        void read(){
            exists = SDL_PollEvent(&buf);
        }
        public:
            using value_type = SDL_Event;
            using difference_type = std::ptrdiff_t;
            event_iterator(){
                read();
            }
            SDL_Event& operator*() const{
                return buf;
            }
            SDL_Event* operator->() const{
                return &buf;
            }
            SDL_Event* pointer() const{
                return &buf;
            }
            event_iterator& operator++(){
                read();
                return *this;
            }
            bool operator==(detail::event_iterator_stop) const{
                return !exists;
            }
            // why would you ever use this (but we must implement it anyways or we aren't an input_iterator)
            event_iterator operator++(int){
                event_iterator dup{*this};
                read();
                return dup;
            }
    };
    bool operator==(detail::event_iterator_stop lhs,const event_iterator& rhs){
        return rhs == lhs;
    }
    class Events{
        friend Events events();
        Events(){}
        public:
            static event_iterator begin(){
                return event_iterator();
            }
            static detail::event_iterator_stop end(){
                return detail::event_iterator_stop();
            }
    };
    Events events(){
        return {};
    }
}
