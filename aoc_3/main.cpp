#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<algorithm>
#include<vector>
#include<cassert>
#include<map>

int read_file_points(std::string input_file);
void read_line(std::vector<int> &w, std::vector<int> &g, std::string myline);
int count_matches(std::vector<int> &w, std::vector<int> &c);
void read_file_scratchcards(std::string input_file, std::map<int,int> &card_counter);

void test_binary();
void test_count_matches();
void test_input();
void test_read_file_scratchcards();

int main(){

    std::map<int, int> card_counter;
    int n(0);

    read_file_scratchcards("input.txt", card_counter);

    for(std::map<int,int>::iterator it=card_counter.begin(); it!=card_counter.end(); it++){
        n += it->second;
    }

    std::cout << "The total number of cards is: " << n << std::endl;

    int points = read_file_points("input.txt");
    std::cout << "The total number of points is: " << points << std::endl;


    test_binary();
    test_count_matches();
    test_input();
    test_read_file_scratchcards();
    return 0;

}

void read_file_scratchcards(std::string input_file, std::map<int,int> &card_counter){

    std::ifstream myfile_or;
    myfile_or.open(input_file);
    std::string myline_or;
    int n_cards = 0;

    // Count the number of cards
    // and define the n_cards
    // dictionary.

    if ( myfile_or.is_open() ) {
        while ( myfile_or ) {

            if( myfile_or.eof() ) break;
            std::getline (myfile_or, myline_or);
            n_cards += 1;
            if(myline_or=="") continue;
            card_counter.insert(std::pair<int,int>(n_cards,1));
        }
    }

    myfile_or.close();

    // Here we restart reading the file,
    // However, we modify cards_counter based
    // on the explained rules.
    std::ifstream myfile;
    myfile.open(input_file);
    std::string myline;
    int line = 0;

    if ( myfile.is_open() ) {
        while ( myfile ) {

            if( myfile.eof() ) break;
            std::getline (myfile, myline);
            if(myline=="") continue;
            line += 1;

            std::vector<int> w;
            std::vector<int> g;
            read_line(w, g, myline);
            std::sort(w.begin(), w.end());

            int matches = count_matches(w,g);

            for(int i=1; i<=matches; i++){
                if ((line + i) > n_cards) break;
                card_counter[line + i] += card_counter.at(line);
            }
        }
    }

    myfile.close();
    return;
}

int read_file_points(std::string input_file){

    std::ifstream myfile;
    myfile.open(input_file);
    std::string myline;
    int total_points = 0;

    if ( myfile.is_open() ) {
        while ( myfile ) {

            if( myfile.eof() ) break;
            std::getline (myfile, myline);
            if (myline=="") continue;

            std::vector<int> w;
            std::vector<int> g;
            read_line(w, g, myline);

            std::sort(w.begin(), w.end());

            int matches = count_matches(w,g);
            int line_points = 0;

            if( matches > 1 ){
                line_points = 1;
                for(int i= 0; i < matches -1; i++){
                    line_points *= 2;
                }
            }
            else if( matches == 1) {
                    line_points = 1;
            }
            else {
                line_points = 0;
            }

            total_points += line_points;
        }
    }

    return total_points;
}

void read_line(std::vector<int> &w, std::vector<int> &g, std::string myline) {

    std::string temp;

    std::istringstream lss(myline);
    lss >> temp;
    lss >> temp;

    int w_size = 10;
    w.resize(w_size);
    for(int i=0; i<w_size; i++){
        lss >> w[i];
    }

    lss >> temp;

    int g_size = 25;
    g.resize(g_size);
    for(int i=0; i<g_size; i++){
        lss >> g[i];
    }
}

bool binarySearch(std::vector<int> &w, int l, int r, int g)
{
    if (r >= l) {
        int mid = l + (r - l) / 2;

        // If the element is present at the middle
        // itself
        if (w[mid] == g)
            return true;

        // If element is smaller than mid, then
        // it can only be present in left subarray
        if (w[mid] > g)
            return binarySearch(w, l, mid - 1, g);

        // Else the element can only be present
        // in right subarray
        return binarySearch(w, mid + 1, r, g);
    }

    // We reach here when element is not
    // present in array
    return false;
}

int count_matches(std::vector<int> &w, std::vector<int> &c){

    int matches=0;
    int n;

    for(int j=0; j<c.size(); j++){
        n =  w.size();
        if( binarySearch(w, 0, n - 1, c[j])){
            matches += 1;
        }
    }

    return matches;
}


void test_binary(){

    std::vector<std::vector<int>> targets{{1,3,8,9,11,24,29,33,38,41},{26,31,42,45,98,76,42,78,81,103},
                                          {16,28,31,32,41,48,56,57,98,97}};
    std::vector<std::vector<int>> candidates{{3,19,42,31,16},{45,102},{42,28,98}};
    std::vector<int> matches{0,0,0};
    int n;

    for(int k_m=0; k_m<3; k_m++){

        for(int j=0; j<candidates[k_m].size(); j++){
            n =  targets[k_m].size();
            if( binarySearch(targets[k_m], 0, n - 1, candidates[k_m][j])){
                matches[k_m] += 1;
            }
        }
    }

    assert (matches == std::vector<int>({1,1,2}));

    std::cout << "test_binary passed" << std::endl;

    return;
}

void test_count_matches(){
    std::vector<int> w{16,28,31,32,98};
    std::vector<int> c{42,28,98};
    int matches{};

    matches = count_matches(w,c);

    assert (matches == 2);

    std::cout << "test_count_matches passed" << std::endl;

    return;
}

void test_input(){

    int points = read_file_points("test.txt");
    assert (points == 1060);
    // 3, 10, 6, 10
    // 2^2 + 2^9  + 2^5 + 2^9
    std::cout << "test_input passed" << std::endl;
    return;
}

void test_read_file_scratchcards(){

    std::map<int, int> card_counter;
    read_file_scratchcards("test.txt", card_counter);
    std::vector<int> test{1,2,4,8};

    for(int i=1; i<=4; i++){
        assert (card_counter[i] == test[i-1]);
    }

    std::cout << "test_read_file_scratchcards passed" << std::endl;
    return;
}