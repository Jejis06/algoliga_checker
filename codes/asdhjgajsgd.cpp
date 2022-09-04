#include <bits/stdc++.h>

using namespace std;

const int FOREST_SIZE = 30;
const int ALPHABET_SIZE = 27;
const int MAX_DEPTH = 5;
const int LOOPCOST = 3;
const int MIN_REP = 5;

int PHRASE_SIZE;
string EMPTY_STONES;
string PHRASE;

//?? _________________________CORE FUNCTIONS_________________________

//** -------------- char to code --------------

int code(char letter) {
    if(letter >= 'A' && letter <= 'Z') {
        return letter - 'A' + 1;
    }
    return 0;
}

//** -------------- Rolls --------------

int get_num_rolls_only_right(char from, char to) {
    if(code(from) <= code(to)) {
        return code(to) - code(from);
    }
    return ALPHABET_SIZE - code(from) + code(to);
}

int get_num_rolls(char from, char to) {
    return min({
        get_num_rolls_only_right(from, to),
        get_num_rolls_only_right(to, from),
        LOOPCOST + get_num_rolls_only_right(0, to),
        LOOPCOST + get_num_rolls_only_right(to, 0)
    });
}

string print_rolls(char from, char to) {

    vector<pair<int,int>> arr{
        {get_num_rolls_only_right(from, to),1},
        {get_num_rolls_only_right(to, from),2},
        {LOOPCOST + get_num_rolls_only_right(0, to),3},
        {LOOPCOST + get_num_rolls_only_right(to, 0),4}
    };

    pair<int,int> o = *min_element(arr.begin(), arr.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.first < rhs.first;
    });


    int mode = o.second;

    string loop = from < ALPHABET_SIZE/2 ? "[-]" : "[+]";

    if (mode == 1){
        return string(o.first,'+');

    } else if (mode == 2){
        return string(o.first, '-');

    }else if (mode == 3){
        return loop + string(o.first - LOOPCOST, '+');
    }

    return loop + string(o.first - LOOPCOST, '-');

}

//** -------------- Moves --------------

int get_num_moves_only_right(int from, int to) {
    if(from <= to) {
        return to - from;
    }
    return FOREST_SIZE - from + to;
}

int get_num_moves(int from, int to) {
    return min({
        get_num_moves_only_right(from,to),
        get_num_moves_only_right(to,from)
    });
}

string print_moves(int from, int to) {

    int ft =  get_num_moves_only_right(from, to);
    int tf =  get_num_moves_only_right(to, from);

    if (ft < tf)
        return string(ft,'>');
    return string(tf,'<');

}


//?? _________________________CORE STRUCTURES_________________________

//** -------------- Move --------------

struct Mv{
    int pos;
    char charr;
};

//** -------------- Improved state (faster) --------------

struct State{
    State(){
        stones = EMPTY_STONES;
        init_pos = cost = num_spelled  = 0;
        cur_pos = 0;
    }

    string stones;

    optional<pair<int,char>> moves;
    optional<int> cur_pos;

    int init_pos, cost, num_spelled;


    void add_move(Mv mv) {
        cost += move_cost(mv);

        if(!moves.has_value()){
            moves = {
                mv.pos,
                mv.charr
            };
        }

        cur_pos = mv.pos;
        stones[mv.pos] = mv.charr;

        num_spelled += 1;
    }

    void reset_state() {
        init_pos = get_current_pos();
        moves = {};
        cost = 0;
    }

    char get_current_pos() const {
        if(!cur_pos.has_value()){
            return init_pos;
        }
        return cur_pos.value();
    }

    char rune(int pos) const {
        return stones[pos];
    }

    int move_cost(Mv move) const {
        return get_num_moves(get_current_pos(), move.pos) + get_num_rolls(rune(move.pos), move.charr);
    }
};


//?? _________________________LOGIC FUNCTIONS_________________________




State build_next_state(const State& state,Mv move) {
    State new_state = state;
    new_state.add_move({move.pos, move.charr});
    return new_state;
}

void best(const State& state, int n, int last_cost, optional<State>& best_state) {
    if (n == 0 || state.num_spelled == PHRASE_SIZE) {
        if(!best_state.has_value() || state.cost < best_state.value().cost) {
            best_state = state;
        }
        return;
    }
    for(int new_pos = 0; new_pos < FOREST_SIZE; new_pos++) {
        // Simulate one move
        Mv move = {new_pos, PHRASE[state.num_spelled]};
        //calculate cost of that move and check if its affordable for us to go further
        int current_cost = (last_cost + state.move_cost(move));
        if (best_state.has_value() && current_cost >= best_state.value().cost)
            continue;

        // Simulate n-1 moves
        best(build_next_state(state, move), n - 1, current_cost, best_state);
    }
}


pair<string, int> get_shortest_repetition(const string& s) {
    for(int l = 1; l < min(int(s.size()), FOREST_SIZE-1); l++) {
        int found_reps = 1;
        for(int rep = 2;; rep++) {
            int i;

            for(i = 0; i < l; i++) {

                if(l*(rep-1)+i >= s.size() || s[i] != s[l*(rep-1)+i] ) {
                    break;
                }
            }

            if(i == l) {
                found_reps = rep;
            } else {
                break;
            }
        }
        if(found_reps > 1) {
            return {s.substr(0, l), found_reps};
        }
    }
    return {"", 0};
}




pair<pair<int,int>,string> process_pattern(const string& pattern){

    string ptrn;
    int lspc=0,rspc=0;
    bool sw = false;

    //left zeros
    for(int i=0; i < pattern.size(); i++){

        if(!sw && pattern[i] == ' '){
            lspc++;
        }
        else{
            break;
        }
    }
    //right zeros
    for(int i=pattern.size()-1; i > 0; i--){

        if(!sw && pattern[i] == ' '){
            rspc++;
        }
        else{
            break;
        }
    }

    //cerr <<"l :" << lspc << " r : ";

    //regen pattern
    for(int i=lspc; i<pattern.size()-rspc;i++){
        if (pattern[i] == ' ') {
            return {{},""};
            //cerr<<'\n';
        }
        ptrn += pattern[i];
    }

    //cerr << rspc << " |>" << ptrn << '|' << '\n';
    return {{lspc,rspc},ptrn};
}

pair<string,int> Generate_pattern(State& state, const string& pattern, const int rep){

    auto [sides, ptrn] = process_pattern(pattern);

    if (ptrn == "") return {"",0};

    string left;
    string right;

    for(int i=0;i<sides.first;i++) left += '.';
    for(int i=0;i<sides.second;i++) right += '.';


    string out;
    int l=0;
    //space
    out += print_rolls(state.rune(state.get_current_pos() + l),' ') + left + '>' ;
    state.stones[state.get_current_pos() + l] = ' ';
    l++;
    //instructions
    for(char c :  ptrn){
        out += print_rolls(state.rune(state.get_current_pos() + l), c) + ".>";
        state.stones[state.get_current_pos() + l] = c;

        l++;
    }
    //space
    out += print_rolls(state.rune(state.get_current_pos() + l),' ')+right+'>';
    state.stones[state.get_current_pos() + l] = ' ';

    l++;
    //iterator
    out += print_rolls(state.rune(state.get_current_pos() + l), char(65 + rep - 2));
    state.stones[state.get_current_pos() + l] = ' ';
    //repeater
    out += "[<<[<]" + left  +">[.>]" + right +">-]";

    return {out,l};
}


int main()
{

    getline(cin, PHRASE);

    PHRASE_SIZE = PHRASE.size();
    EMPTY_STONES = string(FOREST_SIZE, ' ');



    State current_state;
    while(current_state.num_spelled < PHRASE_SIZE) {

        optional<State> new_state;
        string output;

        //repetition
        string left_runes = PHRASE.substr(current_state.num_spelled);
        auto [pattern, rep] = get_shortest_repetition(left_runes);

        if(rep > MIN_REP) {
            //cerr << prefix << " " << rep <<  '\n';
            rep = rep > ALPHABET_SIZE ? ALPHABET_SIZE : rep;
            auto [out,jump] = Generate_pattern(current_state,pattern,rep);
            if (out != ""){
                cout << out;

                current_state.cur_pos.value() += jump;
                current_state.num_spelled += pattern.size() * rep;

                continue;

            }

        }


        best(current_state, MAX_DEPTH, 0, new_state);

        int wanted_pos = new_state.value().moves.value().first;
        char wanted_char = new_state.value().moves.value().second;



        Mv next_move = {wanted_pos, wanted_char};

        string mvs = print_moves(current_state.get_current_pos(), wanted_pos);
        string rls = print_rolls(current_state.rune(wanted_pos),wanted_char);

        output = mvs + rls + ".";

        current_state.add_move(next_move);
        current_state.reset_state();
        cout << output;
    }
    cout << '\n';




}