#include <stdio.h>
#include <stdlib.h>


//basic node declerations
struct node {
    int key;
    struct node *left;
    struct node *right;
};

struct node *createnode(int val){
    struct node *newnode=(struct node *)malloc(sizeof(struct node));
    newnode->key = val;
    newnode->left = NULL;
    newnode->right = NULL;
    return newnode;
}

struct QueueNode {
    struct node* tNode;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode *front, *rear;
};

struct QueueNode* newQNode(struct node* tNode) {
    struct QueueNode* temp = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    temp->tNode = tNode;
    temp->next = NULL;
    return temp;
}

struct Queue* createQueue() {
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(struct Queue* queue, struct node* tNode) {
    struct QueueNode* temp = newQNode(tNode);

    if (queue->rear == NULL) {
        queue->front = queue->rear = temp;
        return;
    }

    queue->rear->next = temp;
    queue->rear = temp;
}

struct node* dequeue(struct Queue* queue) {
    if (queue->front == NULL)
        return NULL;

    struct QueueNode* temp = queue->front;
    struct node* treeNode = temp->tNode;
    queue->front = queue->front->next;

    if (queue->front == NULL)
        queue->rear = NULL;

    free(temp);
    return treeNode;
}

int isqueueempty(struct Queue* queue) {
    return queue->front == NULL;
}

struct stack{
    int data;
    struct stack *next;
};

struct stack* stacknode(int data){
    struct stack *node=(struct stack *)malloc(sizeof(struct stack));
    node->data=data;
    node->next=NULL;
    return node;
}

void push(struct stack** root,int data){
    struct stack *node=stacknode(data);
    node->next=*root;
    *root=node;
}

int pop(struct stack **root){
    if(*root==NULL)
        return -1;
    struct stack *temp=*root;
    int val=temp->data;
    *root=(*root)->next;
    free(temp);
    return val;
}

int isstackempty(struct stack *root){
    return !root;
}

void zigzag(struct node *root){
    if(root==NULL)
    return;

    struct Queue *queue=createQueue();
    enqueue(queue,root);
    int flag=0;

    while(!isqueueempty(queue)){
        int levelsize=0;
        struct QueueNode* temp=queue->front;
        while(temp!=queue->rear->next){
            levelsize++;
            temp=temp->next;
        }

        struct stack *stack=NULL;

        for(int i=0;i<levelsize;i++){
            struct node *temp=dequeue(queue);

            if(flag)
            printf("%d ",temp->key);
            else
            push(&stack,temp->key);

            if(temp->left!=NULL)
                enqueue(queue,temp->left);
            if(temp->right!=NULL)
                enqueue(queue,temp->right);
        }

        if(!flag){
            while(!isstackempty(stack)){
                int x=pop(&stack);
                printf("%d ",x);
            }
        }
        flag=!flag;
    }

}


//finding the diameter
int max(int a,int b){
    if(a>b)
        return a;
    else
        return b;
}

int height(struct node *root){
    if(root==NULL)
        return 0;
    int leftheight=height(root->left);
    int rightheight=height(root->right);

    return 1+max(leftheight,rightheight);
}

int diameter(struct node *root){
    if(root==NULL)
        return 0;
    int leftheight=height(root->left);
    int rightheight=height(root->right);

    int leftdiameter=diameter(root->left);
    int rightdiameter=diameter(root->right);
    int rootdiameter= leftheight+rightheight+1;

    int maxdiameter=rootdiameter;
    if(leftdiameter>maxdiameter)
        maxdiameter=leftdiameter;
    if(rightdiameter>maxdiameter)
        maxdiameter=rightdiameter;
    
    return maxdiameter;

}

//finding the level max
void levelMax(struct node* root) {
    if (root == NULL) return;
    struct Queue* queue = createQueue();
    enqueue(queue, root);

    while (!isqueueempty(queue)) {
        int levelSize = 0;
        struct QueueNode* temp = queue->front;

        while (temp != queue->rear->next) {
            levelSize++;
            temp = temp->next;
        }

        struct node* tempNode = dequeue(queue);
        int max = tempNode->key;
        if (tempNode->left != NULL)
            enqueue(queue, tempNode->left);
        if (tempNode->right != NULL)
            enqueue(queue, tempNode->right);

        for (int i = 1; i < levelSize; i++) {
            tempNode = dequeue(queue);
            if (tempNode->key > max)
                max = tempNode->key;

            if (tempNode->left != NULL)
                enqueue(queue, tempNode->left);

            if (tempNode->right != NULL)
                enqueue(queue, tempNode->right);
        }

        printf("%d ", max);
    }
}

//building the tree
int search(int in[], int val, int s, int e){
    for(int i = s; i <= e; i++){
        if(in[i] == val)
            return i;
    }
    return -1;  
}

struct node *buildtree(int in[], int pre[], int s, int e){
    static int index = 0;
    if (s > e)
        return NULL;

    struct node *temp = createnode(pre[index++]);
    
    if(s == e)
        return temp;

    int x = search(in, temp->key, s, e);

    temp->left = buildtree(in, pre, s, x - 1);
    temp->right = buildtree(in, pre, x + 1, e);
    return temp;
}
//postorder
void postorder(struct node *root){
    if(root != NULL){
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->key);
    }
}


//sum of right lead nodes
int isleafnode(struct node * node){
    return(node!=NULL && node->left==NULL && node->right==NULL);
}

int rightleafsum(struct node *root){
if (root==NULL)
    return 0;
int sum=0;
if(isleafnode(root->right))
    sum=sum+root->right->key;
sum=sum+rightleafsum(root->left);
sum=sum+rightleafsum(root->right);
return sum;
}


int main(){
    int n;
    scanf("%d", &n);
    int in[n];
    int pre[n];
    for(int i = 0; i < n; i++){
        scanf("%d", &in[i]);
    }
    for(int j = 0; j < n; j++){
        scanf("%d", &pre[j]);
    }

    struct node *root = buildtree(in, pre, 0, n - 1);
    char c;
    do{
        scanf(" %c", &c);
        switch(c){
            case 'p':
                postorder(root);
                printf("\n");  
                break;

            case 'm':
                levelMax(root);
                printf("\n");
                break;

            case 's':
            int sum=rightleafsum(root);
            printf("%d\n",sum);
            break;

            case 'd':
            int x=diameter(root);
            printf("%d\n",x);
            break;

            case 'z':
            zigzag(root);
            printf("\n");
            break;

            case 'e':
            break;
        }
    } while(c != 'e');

    return 0;
}

       
