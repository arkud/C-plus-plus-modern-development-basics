
template<typename Key, typename Value>
Value &GetRefStrict(std::map<Key, Value> &m, Key key) {
    if (m.count(key) != 0) {
        return m[key];
    } else
        throw std::runtime_error("");
}


