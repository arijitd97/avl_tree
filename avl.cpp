#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <regex>

using namespace std;

struct node{                            // Type of each node of the tree
    int data;
    int height;
    struct node* left;
    struct node* right;
};

class AVL{
  public:
    struct node *root;
    AVL(){
        this->root=NULL;
    }

    int findHeight(struct node *root){      // Returns the height of a node in the tree
            if(root->left && root->right){
                if (root->left->height < root->right->height)
                    return root->right->height + 1;
                else return  root->left->height + 1;
            }
            else if(root->left && !root->right){
               return root->left->height + 1;
            }
            else if(!root->left && root->right){
               return root->right->height + 1;
            }
            else return 1;
    }

    int balFactor(struct node *root){       // Returns the balance factor of a node in the tree
        if(!root) return 0;
        if(root->left && root->right){
            return root->left->height - root->right->height; 
        }
        else if(root->left && !root->right){
            return root->left->height; 
        }
        else if(!root->left && root->right ){
            return -1 * root->right->height;
        }
        else return 0;
    }

    struct node * llRotation(struct node *root){    // Performs a LL Rotation at the input node
        struct node *p;
        struct node *pl;

        p = root;
        pl = p->left;
        p->left = pl->right;
        pl->right = p;
        p->height=findHeight(p);
        pl->height=findHeight(pl);

        return pl; 
    }


    struct node * rrRotation(struct node *root){    // Performs a RR Rotation at the input node
        struct node *p;
        struct node *pr;

        p = root;
        pr = p->right;

        p->right = pr->left;
        pr->left = p;
        p->height=findHeight(p);
        pr->height=findHeight(pr);

        return pr; 
    }


    struct node * rlRotation(struct node *root){    // Performs a RL Rotation at the input node
        struct node *p;
        struct node *pr;
        struct node *prl;

        p = root;
        pr = p->right;
        prl =p->right->left;

        p->right = prl->left;
        pr->left = prl->right;
        prl->left = p;
        prl->right = pr;
        p->height=findHeight(p);
        pr->height=findHeight(pr);
        prl->height=findHeight(prl);
        
        return prl; 
    }

    struct node * lrRotation(struct node *root){    // Performs a LR Rotation at the input node
        struct node *p;
        struct node *pl;
        struct node *plr;

        p = root;
        pl = p->left;
        plr =p->left->right;

        p->left = plr->right;
        pl->right = plr->left;
        plr->left = pl; 
        plr->right = p;
        p->height=findHeight(p);
        pl->height=findHeight(pl);
        plr->height=findHeight(plr);
        
        return plr; 
    }

    struct node* insertNode(struct node *root,int data){    // Inserts a new node in the tree with key as the input data
        
        if(!root){
            struct node *n = new struct node;
            n->data = data;
            root = n;
            root->left = root->right = NULL;
            root->height = 1; 
            return root;             
        }
        else{
            if(data < root->data)
            root->left = insertNode(root->left,data);
            else
            root->right = insertNode(root->right,data);
        }

        root->height = findHeight(root);

        if(balFactor(root)==2 && balFactor(root->left)==1){
            root = llRotation(root);
        }
        else if(balFactor(root)==-2 && balFactor(root->right)==-1){
            root = rrRotation(root);
        }
        else if(balFactor(root)==-2 && balFactor(root->right)==1){
            root = rlRotation(root);
        }
        else if(balFactor(root)==2 && balFactor(root->left)==-1){
            root = lrRotation(root);
        }        

        return root;

    }

    struct node * deleteNode(struct node *root,int data){      // Deletes a node from the tree having the given key value
        if(!root->left && !root->right){
            if(root==this->root)
                this->root = NULL;
            delete root;
            return NULL;
        }

        struct node *temp;
        if(data > root->data){
            root->right = deleteNode(root->right,data);
        }
        else if(data < root->data){
            root->left = deleteNode(root->left,data);
        }
        else{
            if(root->left){
                temp = inorderPred(root->left);
                root->data = temp->data;
                root->left=deleteNode(root->left,temp->data);
            }
            else{
                temp = inorderSucc(root->right);
                root->data = temp->data;
                root->right = deleteNode(root->right,temp->data);
            }
        }

        root->height=findHeight(root);

        if(balFactor(root)==2 && balFactor(root->left)==1){ root = llRotation(root); }                  
        else if(balFactor(root)==2 && balFactor(root->left)==-1){ root = lrRotation(root); }
        else if(balFactor(root)==2 && balFactor(root->left)==0){ root = llRotation(root); }
        else if(balFactor(root)==-2 && balFactor(root->right)==1){ root = rrRotation(root); }
        else if(balFactor(root)==-2 && balFactor(root->right)==-1){ root = rlRotation(root); }
        else if(balFactor(root)==-2 && balFactor(root->right)==0){ root = rrRotation(root); }
        
        return root;
    }

    struct node* inorderPred(struct node* root){        // Returns the rightmost node of the left subtree of the input node
        while(root->right!=NULL)
            root = root->right;

        return root;    
    }

    struct node* inorderSucc(struct node* root){        // Returns the leftmost node of the right subtree of the input node
        while(root->left!=NULL)
            root = root->left;

        return root;    
    }

    void printTree(){                   // Prints the tree in level order
        if (this->root == NULL){
            cout<<"\n"<<"Empty tree"<<"\n";
            return;            
        }
        queue <struct node *> q;
        struct node *curr;
        q.push(this->root);
        q.push(NULL);      

        while(!q.empty()){
            curr = q.front();
            q.pop();
            if(!curr && q.size()!=0){
                cout<<"\n";
                
                q.push(NULL);
                continue;
            }
            if(curr){
                cout<<" "<<curr->data;

                if (curr->left){
                q.push(curr->left);
                }
                if (curr->right){
                    q.push(curr->right);
                }
            }
        }
    }

    struct node* searchNode(struct node* root, int data){   // Search the node in the tree with the given key value
        if(!root) return NULL;
        if(data>root->data) return searchNode(root->right, data);
        else if(data<root->data) return searchNode(root->left, data);
        else return root;

        return root;    
    }

    void searchNode(struct node* root, int start, int end, vector<struct node*>& nodes){    // Creates a list of nodes with key values within the input range [start,end]
        if(!root || start>end) return;
        if(root->data>start) searchNode(root->left,start,end,nodes);
        if(root->data>=start && root->data<=end){
            nodes.push_back(root);
        }
        if(root->data<end) searchNode(root->right,start,end,nodes);
    }
    

    ~AVL(){

    }
};


void searchTwoKey(int start, int end, AVL *obj, std::ofstream& outputfile){
    printf("Search range %d %d\n", start, end);
    vector<struct node *> nodes;
    obj->searchNode(obj->root,start,end,nodes);

    if (outputfile.is_open()){
        if (nodes.size() == 0) {
            outputfile << "NULL";
        }
        else{
            cout<<"Result - ";
            for(int i = 0; i < nodes.size(); i++){
                cout<<nodes[i]->data<<" ";
                outputfile << nodes[i]->data;
                if (i < nodes.size()-1) {outputfile<<',';}
            }
            cout<<endl;
        }
        outputfile<<endl;
    }
    else cout << "Unable to open output file";
}

void operations(char* comm, int key1, int key2, AVL *obj, std::ofstream& outputfile){
    string command = comm;
    if (command.compare("Insert") == 0){
        cout<<"Insert "<<key1<<endl;
        obj->root = obj->insertNode(obj->root,key1);
    }
    else if (command.compare("Delete") == 0){
        cout<<"Delete "<<key1<<endl;
        obj->root = obj->deleteNode(obj->root,key1);
    }
    else if (command.compare("Search") == 0){
        cout<<"Search "<<key1<<endl;
        struct node* res;
        res = obj->searchNode(obj->root,key1);
        if(outputfile.is_open()){
            if(res){
                cout<<"Found key - "<<res->data<<endl;
                outputfile << res->data;
                outputfile << endl;
            }
            else{
                cout<<"Key not found"<<endl;
                outputfile << "NULL";
                outputfile << endl;
            }
        }
        else cout << "Unable to open output file";

    } 
    else printf("This operation is not defined");
}

int main(int argc, char **argv){

    ifstream inputfile (argv[1]);
    // ifstream inputfile ("input.txt");
    ofstream outputfile ("output_file.txt");
    string line;
    
    // Initialize
    getline(inputfile, line);
    char* buffer = strdup(line.c_str());
    char command[20];
    std::sscanf(buffer, "%[a-zA-Z]()", command);
    AVL obj;
    cout<<command<<" AVL Tree"<<endl;

    if (inputfile.is_open()){
        while (getline(inputfile, line)){
            
            cout<<"\n---------------------------------------\n";
            char* buffer = strdup(line.c_str());
            char command[20];
            int key1, key2;
            std::sscanf(buffer, "%[a-zA-Z] (%d, %d)", command, &key1, &key2);
            
            // Search using two keys
            if(std::regex_match (line, std::regex("Search[ ]*\\([0-9]*,[ ]*[0-9]*\\)") )){
                searchTwoKey(key1, key2, &obj, outputfile);
            }
            // For other operations
            else{
                operations(command, key1, key2, &obj, outputfile);
            }
            obj.printTree();
            free(buffer);
        }
        inputfile.close();
        
    }
    else {
        std::cout << "Unable to open input file"<<endl;
    }
    outputfile.close();
}