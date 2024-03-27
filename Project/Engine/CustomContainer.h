#pragma once
#include "Allocator.h"
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
using namespace std;

template<typename Type>
using xvector = vector<Type, StlAllocator<Type>>;

template<typename Type>
using xlist = list<Type, StlAllocator<Type>>;

template<typename Key, typename Type, typename Pred = less<Key>>
using xmap = map<Key, Type, Pred, StlAllocator<pair<const Key, Type>>>;

template<typename Key, typename Pred = less<Key>>
using xset = set<Key, Pred, StlAllocator<Key>>;

template<typename Type>
using xdeque = deque<Type, StlAllocator<Type>>;

template<typename Type, typename Container = xdeque<Type>>
using xqueue = queue<Type, Container>;

template<typename Type, typename Container = xdeque<Type>>
using xstack = stack<Type, Container>;

template<typename Type, typename Container = xvector<Type>, typename Pred = less<typename Container::value_type>>
using xpriorityQueue = priority_queue<Type, Container, Pred>;

using xstring = basic_string<char, char_traits<char>, StlAllocator<char>>;

using xwstring = basic_string<wchar_t, char_traits<wchar_t>, StlAllocator<wchar_t>>;

template<typename Key, typename Type, typename Hasher = hash<Key>, typename KeyEq = equal_to<Key>>
using xhashmap = unordered_map<Key, Type, Hasher, KeyEq, StlAllocator<pair<const Key, Type>>>;

template<typename Key, typename Hasher = hash<Key>, typename KeyEq = equal_to<Key>>
using xhashset = unordered_set<Key, Hasher, KeyEq, StlAllocator<Key>>;