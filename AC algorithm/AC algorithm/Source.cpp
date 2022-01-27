#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <string.h>
#include <queue>
#include <algorithm>
#include <functional> 
#include <fstream>
#include <algorithm>
#include <time.h>
#include <chrono>
using namespace std;

vector<string> p;
string t;

class Timer {
public:
    chrono::system_clock::time_point Begin, End;
    chrono::system_clock::duration Runtime;
    Timer() {//constructor
        Begin = chrono::system_clock::now();
    }
    ~Timer() {
        End = chrono::system_clock::now();
        Runtime = End - Begin;
        cout << "Runtime = " << chrono::duration_cast<chrono::milliseconds>(Runtime).count() << endl;
    }
};

void preparingData() {
    ifstream text("Text.txt");
    getline(text, t);
    text.close();
    string s;
    ifstream keywords("keywords.txt");
    do {
        keywords >> s;
        p.push_back(s);
    } while (!keywords.eof());
    keywords.close();
}
struct node {
    vector<int> output;
    char character;
    node* fail;
    vector <node*> child;
    node(char a) {
        this->character = a;
    }
    node() = default;
};

void insert(node* root, string p) {
    node* newTemp = root;
    for (int i = 0; i < p.length(); i++) {
        int flag = 0;
        for (int j = 0; j < newTemp->child.size(); j++) {
            if (newTemp->child[j]->character == p[i]) {
                flag = 1;
                newTemp = newTemp->child[j];
            }
        }
        if (flag == 0) {
            newTemp->child.push_back(new node(p[i]));
            newTemp = newTemp->child[newTemp->child.size()-1];
        }
    }
    newTemp->output.push_back(p.length());
}


void buildAC(node* root) {
    //Insert words
    for (int i = 0; i < p.size(); i++) {
        insert(root, p[i]);
    }

    queue <node*> que;
    for (int i = 0; i < root->child.size(); i++) {
            que.push(root->child[i]);
            root->child[i]->fail = root;
    }

    while (!que.empty()) {
        node* temp = que.front();
        que.pop();
        for (int i = 0; i < temp->child.size(); i++) {
            int flag = 0;
            node* parentFail = temp->fail;
            while (parentFail && flag == 0) {
                for (int j = 0; j < parentFail->child.size(); j++) {
                    if (temp->child[i]->character == parentFail->child[j]->character) {
                        temp->child[i]->fail = parentFail->child[j];
                        flag = 1;
                    }
                }
                if (flag == 0) {
                    parentFail = parentFail->fail;
                }
            }
            if (parentFail == NULL) {
                temp->child[i]->fail = root;
            }

            for (int j = 0; j < temp->child[i]->fail->output.size(); j++) {
                temp->child[i]->output.push_back(temp->child[i]->fail->output[j]);
            }
            que.push(temp->child[i]);
        }
    }
}

void findText(node* root) {
    int count(0);
    node* newTemp = root;
    for (int i = 0; i < t.length(); i++) {
        int flag = 0;
        for (int j = 0; j < newTemp->child.size(); j++) {
            if (newTemp->child[j]->character == t[i]) {
                flag = 1;
                newTemp = newTemp->child[j];
            }
        }
        while (flag == 0 && newTemp->fail) {
                newTemp = newTemp->fail;
                for (int j = 0; j < newTemp->child.size(); j++) {
                    if (newTemp->child[j]->character == t[i]) {
                        flag = 1;
                        newTemp = newTemp->child[j];
                    }
                }
        }
        for (int j = 0; j < newTemp->output.size(); j++) {
            int loc = newTemp->output[j];
            cout << "Found " << t[i - loc + 1];
            for (int k = 1; k < loc; k++) {
                cout << t[i - loc + 1 + k];
            }
           cout << " from " << i - loc<<" to "<< i -1 <<endl;
           count++;
        }
    }
    cout << "AC Total: " << count << endl;
}

void naiveSearch(vector<string> patt, string txt)
{
    int count(0);
    for (string pat : patt) {

        for (int i = 0; i <= pat.size(); i++) {
            for (int j = 0; j < txt.size(); j++) {
                if (pat[i] == txt[j]) {
                    int k = 0;
                    int flag = 0;
                    do {
                        k++;
                        if (j + k > txt.size() - 1) {
                            flag = 1;
                            break;
                        }
                        if (pat[i + k] != txt[j + k]) {
                            flag = 1;
                        }
                        if (k == pat.size() - 1 && flag == 0) {
                            break;
                        }
                    } while (flag == 0);
                    if (flag == 0) {
                        cout << "BF: Found "<< pat <<" from " << (j + 1) << " to " << (j + pat.size()) << endl;
                        count++;
                    }
                }
            }
        }
    }
    cout << "BF Total: " << count << endl;
}
void main() {
    cout << "104 keywords and 26334 input texts: " << endl;
    preparingData();
    naiveSearch(p, t);
   
    node* root = new node();
    buildAC(root);
    {
        Timer ac;
        findText(root);
        cout << "AC algorithm without preprocessing:"<< endl;
    }
   
}