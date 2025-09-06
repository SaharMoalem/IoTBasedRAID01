#include "handleton.hpp"

using namespace ilrd;

std::unordered_map<const std::type_info*, std::shared_ptr<void>,
        Handleton::HashFunction, Handleton::Comparator> Handleton::m_handletons;
std::mutex Handleton::m_mtx;