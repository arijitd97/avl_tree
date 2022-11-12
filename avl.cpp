#include <iostream>
#include <queue>
#include <unordered_map>

using namespace std;

struct node{
    int data;
    int height;
    struct node* left;
    struct node* right;
};

class AVL{
//   private:
    // struct node *root;
  public:
    struct node *root;
    AVL(){
        this->root=NULL;
    }

    int findHeight(struct node *root){      //gives the height of a node in the tree
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

    int balFactor(struct node *root){       //gives the balance factor of a node in the tree
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

    struct node * llRotation(struct node *root){
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


    struct node * rrRotation(struct node *root){
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


    struct node * rlRotation(struct node *root){
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

    struct node * lrRotation(struct node *root){
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

    struct node* insert(struct node *root,int data){
        
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
            root->left = insert(root->left,data);
            else
            root->right = insert(root->right,data);
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

    void levelorder_newline(){
        if (this->root == NULL){
            cout<<"\n"<<"Empty tree"<<"\n";
            return;            
        }
        levelorder_newline(this->root);
    }

    void levelorder_newline(struct node *v){
        queue <struct node *> q;
        struct node *cur;
        q.push(v);
        q.push(NULL);      

        while(!q.empty()){
            cur = q.front();
            q.pop();
            if(cur == NULL && q.size()!=0){
                cout<<"\n";
                
                q.push(NULL);
                continue;
            }
            if(cur!=NULL){
                cout<<" "<<cur->data;

                if (cur->left!=NULL){
                q.push(cur->left);
                }
                if (cur->right!=NULL){
                    q.push(cur->right);
                }
            }
            
            
        }
    }
 
    struct node * deleteNode(struct node *root,int data){
        cout<<root->data<<" "<<data<<endl;
        if(!root->left && !root->right){
            cout<<"Inside first if"<<endl;
            if(root==this->root)
                this->root = NULL;
            delete root;
            return NULL;
        }

        struct node *t;
        struct node *q;
        if(data > root->data){
            root->right = deleteNode(root->right,data);
        }
        else if(data < root->data){
            root->left = deleteNode(root->left,data);
        }
        else{
            if(root->left){
                q = inorderPred(root->left);
                root->data = q->data;
                root->left=deleteNode(root->left,q->data);
            }
            else{
                q = inorderSucc(root->right);
                root->data = q->data;
                root->right = deleteNode(root->right,q->data);
            }
        }

        root->height=findHeight(root);
        cout<<"After height"<<endl;

        if(balFactor(root)==2 && balFactor(root->left)==1){ root = llRotation(root); }                  
        else if(balFactor(root)==2 && balFactor(root->left)==-1){ root = lrRotation(root); }
        else if(balFactor(root)==2 && balFactor(root->left)==0){ root = llRotation(root); }
        else if(balFactor(root)==-2 && balFactor(root->right)==1){ root = rrRotation(root); }
        else if(balFactor(root)==-2 && balFactor(root->right)==-1){ root = rlRotation(root); }
        else if(balFactor(root)==-2 && balFactor(root->right)==0){ root = rrRotation(root); }
        cout<<"After rotation"<<endl;
        
        return root;
    }

    struct node* inorderPred(struct node* root){
        cout<<"insdie pred"<<endl;
        while(root->right!=NULL)
            root = root->right;

        return root;    
    }

    struct node* inorderSucc(struct node* root){
        while(root->left!=NULL)
            root = root->left;

        return root;    
    }

    struct node* searchNode(struct node* root, int data){
        if(!root) return NULL;
        if(data>root->data) return searchNode(root->right, data);
        else if(data<root->data) return searchNode(root->left, data);
        else return root;

        return root;    
    }

    void searchNode(struct node* root, int start, int end, vector<struct node*>& nodes){
        if(!root || start>end) return;
        // if(root->data>=start && root->data<=end){
        //     nodes.push_back(root);
        // }
        if(root->data>start) searchNode(root->left,start,end,nodes);
        if(root->data>=start && root->data<=end){
            nodes.push_back(root);
        }
        if(root->data<end) searchNode(root->right,start,end,nodes);
    }
    

    ~AVL(){

    }
};

int main(){
    AVL obj;
    int c,x;

    // while(true){
    //     cout<<endl<<endl;
    //     cout<<"Enter"<<endl;
    //     cin>>x;
    //     obj.root = obj.insert(obj.root,x);
    //     obj.levelorder_newline();

    // }
    do{
        cout<<"\n1.Display levelorder on newline";
        cout<<"\n2.Insert";
        cout<<"\n3.Delete\n";
        cout<<"\n4.Search\n";
        cout<<"\n0.Exit\n";
        cout<<"\nChoice: ";

        cin>>c;


        switch (c)
        {
        // case 1:
        //     obj.levelorder_newline();
        //     // to print the tree in level order
        //     break;
        case 0:
            break;  

        case 1:
            cout<<"\nEnter no. ";
            cin>>x;
            obj.root = obj.insert(obj.root,x);
            obj.levelorder_newline();
            break;
        
        case 2:
            cout<<"\nWhat to delete? ";
            cin>>x;
            obj.root = obj.deleteNode(obj.root,x);
            obj.levelorder_newline();
            break;
        
        case 3:
            cout<<"\nWhat to search? ";
            cin>>x;
            struct node* temp;
            temp = obj.searchNode(obj.root,x);
            if(temp) cout<<"Search key present - "<<temp->data<<endl;
            else cout<<"Search key not found"<<endl;
            break;

        case 4:
            cout<<"Enter range - ";
            int start,end;
            cin>>start;
            cin>>end;
            vector<struct node *> nodes;
            obj.searchNode(obj.root,start,end,nodes);
            if(nodes.size()!=0){
                cout<<"Nodes within the search range are - ";
                for(int i=0;i<nodes.size();i++){
                    cout<<nodes[i]->data<<" ";
                }
                cout<<endl;
            }
            else{
                cout<<"There are no nodes within the search range"<<endl;
            }
            break;
            
        // case 0:
        //     break;
        }

     } while(c!=0);
}