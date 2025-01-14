#include <algorithm>
#include <climits>
#include <iostream>
// #include "prettyprint.hpp"
#include <vector>

using namespace std;

vector<pair<int, vector<int> > > list1;
vector<pair<int, vector<int> > > list2;
vector<vector<int> > list2_states;

bool sort_list(pair<int, vector<int> > &a, pair<int, vector<int> > &b) {
  if (a.second == b.second) {
    return a.first < b.first;
  }
  return a.second < b.second;
}

int find_state(vector<int> state) {
  bool found =
      binary_search(list2_states.begin(), list2_states.end(), state);
  if (found) {
    auto it =
        lower_bound(list2_states.begin(), list2_states.end(), state);
    int found_index = it - list2_states.begin();
    return list2[found_index].first;
  } else {
    return -1;
  }
}

vector<int> add_to_state(vector<int> state, int switch_index, int on_off,
    vector<vector<vector<int> > > &switch_effects) {
  for (int room_index = 0; room_index < state.size(); room_index++) {
    state[room_index] += switch_effects[switch_index][room_index][on_off];
  }
  return state;
}

vector<int> remove_from_state(vector<int> state1, vector<int> state2) {
  for (int index = 0; index < state1.size(); index++) {
    state1[index] -= state2[index];
  }
  return state1;
}

void list_all_reachable_states(vector<int> state, int n_switched_switches,
    int next, int bound, vector<vector<vector<int> > > &switch_effects,
    vector<pair<int, vector<int> > > &list) {
  vector<int> state0 = add_to_state(state, next, 0, switch_effects);
  vector<int> state1 = add_to_state(state, next, 1, switch_effects);
  if (next == bound - 1 ) {
    list.push_back(make_pair(n_switched_switches, state0));
    list.push_back(make_pair(n_switched_switches + 1, state1));
  } else {
    list_all_reachable_states(state0, n_switched_switches, next + 1, bound,
        switch_effects, list);
    list_all_reachable_states(state1, n_switched_switches + 1, next + 1, bound,
        switch_effects, list);
  }
}

void test_case() {
  list1 = vector<pair<int, vector<int> > >();
  list2 = vector<pair<int, vector<int> > >();
  list2_states = vector<vector<int> >();
  int n_switches, n_rooms;
  cin >> n_switches >> n_rooms;
  vector<int> required_lights(n_rooms);
  for (int room_index = 0; room_index < n_rooms; room_index++) {
    cin >> required_lights[room_index];
  }
  vector<vector<vector<int> > > switch_effects(n_switches,
      vector<vector<int > >(n_rooms, vector<int>(2)));
  for (int switch_index = 0; switch_index < n_switches; switch_index++) {
    for (int room_index = 0; room_index < n_rooms; room_index++) {
      cin >> switch_effects[switch_index][room_index][0] >>
          switch_effects[switch_index][room_index][1];
    }
  }

  vector<int> state(n_rooms, 0);
  list_all_reachable_states(state, 0, 0, n_switches / 2, switch_effects, list1);
  list_all_reachable_states(state, 0, n_switches / 2, n_switches,
      switch_effects, list2);
  sort(list2.begin(), list2.end(), sort_list);

  for (auto p: list2) {
    list2_states.push_back(p.second);
  }

  int min_switches = INT_MAX;
  for (int list1_index = 0; list1_index < list1.size(); list1_index++) {
    vector<int> state = list1[list1_index].second;
    int n_switches_list2 =
        find_state(remove_from_state(required_lights, state));
    if (n_switches_list2 != -1) {
      min_switches =
          min(min_switches, list1[list1_index].first + n_switches_list2);
    }
  }

  if (min_switches == INT_MAX) {
    std::cout << "impossible" << std::endl;
  } else {
    std::cout << min_switches << std::endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_index = 0; test_index < n_test_cases; test_index++) {
    test_case();
  }
  return 0;
}
