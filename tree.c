/*tree.c--接口函数定义*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"tree.h"

//SeekItem()要用到的结构体
typedef struct pair{
    Node *parent;
    Node *child;
}Pair;

static bool ToLeft(const Item *p1,const Item *p2);
static bool ToRight(const Item *p1,const Item *p2);
static Pair SeekItem(const Item *pi,const Tree *ptree);
static Node *MakeNode(const Item *pi);
static void AddNode(Node *new_node,Node *root);
static void DeleteNode(Node **ptr);
static void InOrder(const Node *root,void (*pfun)(Item item));
static void DeleteAllNodes(Node *root);

//初始化一棵树
void InitializeTree(Tree *ptree)
{
    ptree->root = NULL;
    //初始化为空意味着设置指针为NULL并设置项目Item为0；
    ptree->size = 0;
}

//检查一棵树是否为空树
bool TreeIsEmpty(const Tree *ptree)
{
    printf("%d",ptree->size);
    return(ptree->size == 0);

    /*if(ptree->root == NULL)
          return true;
      else
          return false;*/
}

//检查一棵树是否已满
bool TreeIsFull(const Tree *ptree)
{
    return(ptree->size == MAXITEMS);
}

//确定树中项目的个数
int TreeItemCount(const Tree *ptree)
{
    return(ptree->size);
}

//向树中添加项目
bool AddItem(const Item *pi,Tree *ptree)
{
    Node *new_node;

    if(TreeIsFull(ptree))
        {
            fprintf(stderr,"The tree is full!\n");
            return false;       //提前返回
        }
    
    if(SeekItem(pi,ptree).child != NULL)    //该项目存在于树中，不能添加
    {
        fprintf(stderr,"Attempted to add duplicate item\n");
        return false;
    }
    //现在可以添加项目到树中了
    //将pi指向的项目添加进节点中
    new_node = MakeNode(pi);    //指向新节点
    if(new_node == NULL)        //分配内存失败
    {
        fprintf(stderr,"Can't creat node!\n");
        return false;
    }
    /*成功创建一节点*/
    ptree->size++;

    if(ptree->root == NULL) //树为空树
        ptree->root = new_node;
    else
        AddNode(new_node,ptree->root);  //把新节点添加到树中
    return true;
}

//在树中查找一个项目
bool InTree(const Item *pi,const Tree *ptree)
{
    //printf("%d",SeekItem(pi,ptree).child);
    //bool a;
    //a = SeekItem(pi,ptree).child == NULL ? false:true;
    //putchar('\n');
    //putchar('\n');
   // putchar('\n');
    //printf("%d\n",a);
    return (SeekItem(pi,ptree).child == NULL ? false:true);
}

//删除项目
bool DeleteItem(const Item *pi,Tree *ptree)
{
    Pair look;
    look = SeekItem(pi,ptree);
    if(look.child == NULL)
        return false;
    
    if(look.parent == NULL)    /*根项目*/
        DeleteNode(&(ptree->root));
    else if (look.parent->left == look.child)
        DeleteNode(&(look.parent->left));
    else
        DeleteNode(&(look.parent->right));
    ptree->size--;

    return true;
}

//遍历树
void Traverse(const Tree *ptree,void(*pfun)(Item item))
{
    if(ptree != NULL)
        InOrder(ptree->root,pfun);
}

//清空树
void DeleteAll(Tree *ptree)
{
    if(ptree != NULL)
        DeleteAllNodes(ptree->root);
    ptree->root = NULL;
    ptree->size = 0;
}

/*局部函数*/
//清空所有节点
static void DeleteAllNodes(Node *root)
{
    Node *pright;

    if(root != NULL)
    {
        pright = root->right;
        DeleteAllNodes(root->left);
        free(root);
        DeleteAllNodes(pright);
    }
}

//SeekItem()函数定义
static Pair SeekItem(const Item *pi,const Tree *ptree)
{
    Pair look;
    look.parent = NULL;
    look.child = ptree->root;

    if(look.child == NULL)   //无项目
        return look;
    while(look.child != NULL)
    {
        if(ToLeft(pi,&(look.child->item)))//搜索项目在左子树
        {
            look.parent = look.child;
            look.child = look.child->left;
        }
        else if (ToRight(pi,&(look.child->item)))//搜索项目在右子树
        {
            look.parent = look.child;
            look.child = look.child->right;
        }
        else //如果前面两种情况都不满足，必定为相等情况
            break;  //look.child是目标项目节点的地址
    }

    return look;        //成功返回
}

//MakeNode()函数定义
static Node *MakeNode(const Item *pi)
{
    Node *new_node;

    new_node = (Node *)malloc(sizeof(Node));
    if(new_node != NULL)
    {
        new_node->item = *pi;
        new_node->left = NULL;
        new_node->right = NULL;
    }
    return new_node;
}

//删除节点
static void DeleteNode(Node **ptr)
/*ptr是指向目标节点的父节点指针成员的地址*/
{
    Node *temp;

    puts((*ptr)->item.petname);
    if((*ptr)->left == NULL)    //待删节点没有左子节点
    {
        temp = *ptr;
        *ptr = (*ptr)->right;
        free(temp);
    }
    else if((*ptr)->right == NULL)   //待删节点没有右子节点
    {
        temp = *ptr;
        *ptr = (*ptr)->left;
        free(temp);
    }
    else        //被删除节点有两个子节点
    {
        //找到右子树的依附位置
        for(temp = (*ptr)->left;temp->right != NULL;temp = temp->right)
            continue;
        temp->right = (*ptr)->right;
        temp = *ptr;
        *ptr = (*ptr)->left;
        free(temp);
    }
}

//添加新节点进树中
static void AddNode(Node *new_node,Node *root)
{
    if(ToLeft(&new_node->item,&root->item)) //new_node项目在根节点左边
    {
        if(root->left == NULL)  //根节点下没有左子树
            root->left = new_node;
        else    
            //递归调用AddNote()函数，直到NULL为止
            AddNode(new_node,root->left);
    }
    if(ToRight(&new_node->item,&root->item)) //new_node项目在根节点右边
    {
        if(root->right == NULL)  //根节点下没有左子树
            root->right = new_node;
        else    
            //递归调用AddNote()函数，直到NULL为止
            AddNode(new_node,root->right);
    }
    else        //该项目在树中
    {
        fprintf(stderr,"Location error in AddNode()!\n");
        exit(1);
    }
}

static void InOrder(const Node *root,void (*pfun)(Item item))
{
    if(root != NULL)
    {
        InOrder(root->left,*pfun);  //先处理左子树
        (*pfun)(root->item);   //处理根节点
        InOrder(root->right,*pfun); //处理右子树
    }
}

//向左搜索
static bool ToLeft(const Item *p1,const Item *p2)
{
    int comp1;
    if((comp1 = strcmp(p1->petname,p2->petname)) < 0)
        return true;
    else if (comp1 == 0 && strcmp(p1->petkind,p2->petkind) < 0)
        return true;
    else
        return false;
}

//向右搜索
static bool ToRight(const Item *p1,const Item *p2)
{
    int comp1;
    if((comp1 = strcmp(p1->petname,p2->petname)) > 0)
        return true;
    else if (comp1 == 0 && strcmp(p1->petkind,p2->petkind) > 0)
        return true;
    else
        return false;
}