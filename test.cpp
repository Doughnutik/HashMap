#include <bits/stdc++.h>
#include "hash_map.h"

int main(){
    HashMap<int, int> map;
    map[1] = 2;
    map[2] = 3;
    map[3] = 4;
    auto it = map.find(3);
    std::cout << it->first << " " << it->second << "\n";
    map.erase(3);
    it = map.find(3);
    std::cout << (it == map.end());
}