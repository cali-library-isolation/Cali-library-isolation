#ifndef DEMO_ALLOCTEST_ORDER_STATISTIC_TREE_H
#define DEMO_ALLOCTEST_ORDER_STATISTIC_TREE_H

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

typedef
tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update>
		order_statistic_tree_int_t;

//template<typename T>
//using order_statistic_tree_t = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

template<typename T>
using order_statistic_tree_t = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;


#endif //DEMO_ALLOCTEST_ORDER_STATISTIC_TREE_H
