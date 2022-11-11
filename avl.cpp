#include <iostream>
#include <queue>
#include <unordered_map>

using namespace std;

struct node{
    int data;
    int height;
    struct node* left=NULL;
    struct node* right=NULL;
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
            else return 0;
    }

    int balFactor(struct node *root){       //gives the balance factor of a node in the tree
            if(root->left && root->right){
                return root->left->height - root->right->height; 
            }
            else if(root->left && !root->right){
                return root->left->height; 
            }
            else if(!root->left && root->right ){
                return -1 * root->right->height;
            }
    }

    struct node * llRotation(struct node *root){
        struct node *p;
        struct node *pl;

        p = root;
        pl = p->left;
        p->left = pl->right;
        pl->right = p;

        return pl; 
    }


    struct node * rrRotation(struct node *root){
        struct node *p;
        struct node *pr;

        p = root;
        pr = p->right;

        p->right = pr->left;
        pr->left = p;

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
 
    struct node * deleteNode(struct node *p,int data){

        if(p->left == NULL && p->right == NULL){
                if(p==this->root)
                    this->root = NULL;
            delete p;
            return NULL;
        }

        struct node *t;
        struct node *q;
        if(p->data < data){
            p->right = deleteNode(p->right,data);
        }
        else if(p->data > data){
            p->left = deleteNode(p->left,data);
        }
        else{
            if(p->left != NULL){
                q = inpre(p->left);
                p->data = q->data;
                p->left=deleteNode(p->left,q->data);
            }
            else{
                q = insuc(p->right);
                p->data = q->data;
                p->right = deleteNode(p->right,q->data);
            }
        }

        if(balFactor(p)==2 && balFactor(p->left)==1){ p = llRotation(p); }                  
        else if(balFactor(p)==2 && balFactor(p->left)==-1){ p = lrRotation(p); }
        else if(balFactor(p)==2 && balFactor(p->left)==0){ p = llRotation(p); }
        else if(balFactor(p)==-2 && balFactor(p->right)==-1){ p = rrRotation(p); }
        else if(balFactor(p)==-2 && balFactor(p->right)==1){ p = rlRotation(p); }
        else if(balFactor(p)==-2 && balFactor(p->right)==0){ p = llRotation(p); }

        
        return p;
    }

     struct node* inpre(struct node* p){
        while(p->right!=NULL)
            p = p->right;
        return p;    
    }

    struct node* insuc(struct node* p){
        while(p->left!=NULL)
            p = p->left;

        return p;    
    }
    

    ~AVL(){

    }
};

int main(){
    AVL obj;
    int c,x;

    do{
        cout<<"\n1.Display levelorder on newline";
        cout<<"\n2.Insert";
        cout<<"\n3.Delete\n";
        cout<<"\n0.Exit\n";
        cout<<"\nChoice: ";

        cin>>c;

        switch (c)
        {
        case 1:
            obj.levelorder_newline();
            // to print the tree in level order
            break;
                  
        case 2:
            cout<<"\nEnter no. ";
            cin>>x;
            obj.root = obj.insert(obj.root,x);
            break;
        
        case 3:
            cout<<"\nWhat to delete? ";
            cin>>x;
            obj.root = obj.deleteNode(obj.root,x);
            break;
            
        case 0:
            break;
        }

     } while(c!=0);
}