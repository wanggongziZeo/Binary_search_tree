/*petclub.c--使用二叉搜索树*/
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include"tree.h"

char menu(void);
void addpet(Tree *pt);
void droppet(Tree *pt);
void showpets(const Tree *pt);
void findpet(const Tree *pt);
void printitem(Item item);
void uppercase(char *str);

int main()
{
    Tree pets;
    char choice;

    InitializeTree(&pets);
    while((choice = menu()) != 'q')
    {
        switch(choice)
        {
            case 'a' :addpet(&pets);
                      break;
            case 'l' :showpets(&pets);
                      break;
            case 'f' :findpet(&pets);
                      break;
            case 'n' :printf("%d pets in club.\n",TreeItemCount(&pets));
                      break;
            case 'd' :droppet(&pets);
                      break;
            default : puts("Switching Error!");
                      break;
        }
    }

    DeleteAll(&pets);
    puts(":::::::::Bye:::::::::");

    return 0;
}

char menu(void)
{
    int ch;
    puts("Nerfville Pet Club Membership Program");
    puts("Enter the letter corresponding to your choice:");
    puts("a)add a pet        l)show list of pets");
    puts("n)number of pets   f)find pets");
    puts("d)delete a pet     q)quit");
    while((ch = getchar()) != EOF)
    {
        while(getchar() != '\n')
            continue;
        //ch = toupper(ch);
        //printf("%c\n",ch);
        if(strchr("alnfdq",ch) == NULL)
            puts("Please enter an a,l,n,f,d,q:");
        else 
            break;
    }

    if(ch == EOF)    //令EOF导致程序退出
        ch = 'q';
    
    return ch;
}

void addpet(Tree *pt)
{
    Item temp;
    if(TreeIsFull(pt))
        puts("No room in the club!");
    else
    {
        puts("Please enter name of pet:");
        gets(temp.petname);
        printf("%s",temp.petname);
        puts("Please enter kind of pet:");
        gets(temp.petkind);
        uppercase(temp.petname);
        uppercase(temp.petkind);
        AddItem(&temp,pt);
    }
}

void showpets(const Tree *pt)
{
    if(TreeIsEmpty(pt))
        puts("No entries!");
    else
        Traverse(pt,printitem);
}

void printitem(Item item)
{
    printf("Pet:%-19s Kind:%-19s\n",item.petname,item.petkind);
}

void findpet(const Tree *pt)
{
    Item temp;

    if(TreeIsEmpty(pt))
    {
        puts("No entries");
        return;     //树为空退出
    }

    puts("Please enter name of pet you wish to find:");
    gets(temp.petname);
    uppercase(temp.petname);
    puts("Please enter pet kind:");
    gets(temp.petkind);
    uppercase(temp.petkind);
    printf("%s",temp.petkind);
    if(InTree(&temp,pt))
        printf("is a member.\n");
    else
        printf("is not a member.\n");
}

void droppet(Tree *pt)
{
    Item temp;

    if(TreeIsEmpty(pt))
    {
        puts("No entries!");
        return;
    }

    puts("Please enter name of pet you wish to delete:");
    gets(temp.petname);
    puts("Please enter pet kind:");
    gets(temp.petkind);
    uppercase(temp.petname);
    uppercase(temp.petkind);
    printf("%s the %s",temp.petname,temp.petkind);
    if(DeleteItem(&temp,pt))
        printf("is dropped from the tree!\n");
    else
        printf("is not a member!\n");
}

void uppercase(char *str)
{
    while(*str)
    {
        *str = toupper(*str);
        str++;
    }
}