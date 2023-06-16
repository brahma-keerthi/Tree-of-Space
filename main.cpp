#include <bits/stdc++.h>
#include <stdio.h>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string.h>
using namespace std;

// You are given a complete, balanced N-Ary Tree and must support Q queries. There are 3 kinds of queries. Return true or false depending on whether the query was successful.

// Lock(v, id) - Lock vertex v for user - id
// Unlock(v, id) - If vertex v is locked by the same id, unlock it.
// Upgrade(v, id) - If v is unlocked and has at least one locked vertex in it's subtree and every locked vertex in the subtree of v is locked by id, unlock them and lock v instead.

// Further, here are some additional constraints >> 
//    1. A vertex cannot be locked if it has any locked ancestors or descendants, by any ID.
//    2. When a vertex is upgraded, it's locked descendants are automatically unlocked.
//    3. An upgrade operation is not possible if the vertex is already locked or has any locked ancestors
//    4. An unlock operation is only possible if the vertex is already locked and locked by the same id

/*
	 * INPUT
	 * n = total number of nodes
	 * m = number of child per node
	 * q = number of queries
	 *
	 * next 'n' lines = node name in string
	 * next 'q' lines = queries with (opcode, string, uid)
	 *
	 * opcode => 1 = Lock, 2 = Unlock, 3 = Upgrade
	*/

/*
Example Input:
7 -> n ( number of nodes )
2 -> m ( number of links to a node)
4 -> q ( number of queries )
World
Asia
Africa
India
China
SouthAfrica
Egypt
1 China 9
1 India 9
3 Asia 9
2 India 9
*/



struct Node{
    string data;
    vector <Node *> link;
    Node * parent;
    int countLockAnc, countLockDes, uid;
    bool isLocked;

    Node (string s, Node * p){
        data = s;
        parent = p;
        countLockAnc = countLockDes = uid = 0;
        isLocked = false;
    }

    void addLink(vector <string> l, Node * p){
        for(string i : l){
            link.push_back(new Node(i, p));
        }
    }
};

void printTree(Node *r){
    cout << "Parent: " << r -> data << endl;
    cout << "Child " << endl;
    for(auto i : r -> link){
        cout << "Data >>" << i -> data << " Ancestor lock count >> " << i -> countLockAnc << " Descends lock count >> " << i -> countLockDes << " IsLocked >> " << i -> isLocked << " UID >> " << i -> uid << endl;
    }
    cout << endl;

    for(auto i : r -> link ){
        printTree(i);
    }
}

Node * buildTree(Node * root, int m, vector <string> &names){
    queue <Node *> q;
    q.push(root);

    int st = 1;
    while ( !q.empty() ){
        Node * r = q.front();
        q.pop();

        if ( st >= names.size() )
            continue;

        vector <string> temp;
        for(int i = st; i < st + m; ++i)
            temp.push_back(names[i]);
        r ->addLink(temp, r);
        st += m;

        for(auto i : r -> link)
            q.push(i);
    }

    return root;
}

class Tree{
    private:
        Node * root;
        unordered_map <string, Node *> dataToNode;

    public:
        Tree ( Node * r ){
            root = r;
        }

        Node * getRoot(){
            return root;
        }

        void fillDataToNode(Node * r){
            if ( r == NULL )
                return;

            dataToNode[r->data] = r;
            for(auto i : r -> link)
                fillDataToNode(i);
        }

        void informDes(Node * r, int val){
            for(auto i : r -> link){
                i -> countLockAnc += val;
                informDes(i, val);
            }
        }

        bool verifyDes(Node *r, int id, vector <Node *> &v ){
            if ( r -> isLocked ){
                if ( r -> uid != id )
                    return false;
                v.push_back(r);
            }

            if ( r -> countLockDes == 0 )
                return true;

            bool ans = true;
            for(auto i : r -> link){
                ans &= verifyDes(i, id, v);
                if ( ans == false )
                    return false;
            }

            return ans;
        }

        bool lock(string v, int id){
            Node * r = dataToNode[v];

            if ( r -> isLocked )
                return false;

            if ( r -> countLockAnc != 0 || r -> countLockDes != 0 )
                return false;

            Node * cur = r -> parent;
            while ( cur ){
                cur -> countLockDes++;
                cur = cur -> parent;
            }

            informDes(r, 1);
            r -> isLocked = true;
            r -> uid = id;
            return true;
        }

        bool unlock(string v, int id){
            Node * r = dataToNode[v];

            if ( !r -> isLocked )
                return false;

            if ( r -> isLocked && r -> uid != id )
                return false;

            Node * cur = r -> parent;
            while ( cur ){
                cur -> countLockDes--;
                cur = cur -> parent;
            }

            informDes(r, -1);
            r -> isLocked = false;
            r -> uid = 0;
            return true;
        }

        bool upgrade(string v, int id){
            Node * r = dataToNode[v];
            if ( r -> isLocked )
                return false;

            if ( r -> countLockAnc != 0 )
                return false;
            if ( r -> countLockDes == 0 )
                return false;

            vector <Node *> vec;
            if ( verifyDes(r, id, vec) ){
                for(auto i : vec ){
                    unlock(i -> data, id );
                }
            }
            else    
                return false;
            lock(v, id);
            return true;
        }
};

int main(){
    int n, m, q;
    cin >> n >> m >> q;
    vector <string> names(n);

    for(int i = 0; i<n; ++i)
        cin >> names[i];

    Node *r = new Node(names[0], NULL);
    r = buildTree(r, m, names);

    Tree t(r);
    t.fillDataToNode(t.getRoot());

    int op, uid;
    string s;

    for(int i = 0; i<q; ++i){
        cin >> op >> s >> uid;

        switch (op){
        case 1:	if(t.lock(s,uid)){
						cout << "true\n";
						//printTree(r);
					}else
						cout << "false\n";
					break;
			case 2:	if(t.unlock(s,uid))
						cout << "true\n";
					else
						cout << "false\n";
					break;
			case 3:	if(t.upgrade(s,uid)){
						cout << "true\n";
						//printTree(r);
					}else
						cout << "false\n";
					break;
        }
    }

    return 0; 
}
