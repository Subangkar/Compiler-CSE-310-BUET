#ifndef SYMBOLTABLE_H_INCLUDED
#define SYMBOLTABLE_H_INCLUDED


#include<iostream>
#include<string>
#include<fstream>
#include<cstdio>
#include<cstring>
#include<sstream>

#define DEBUG cout<<"ok"<<endl
using namespace std;

const int mod1=431,mod2=47;

class SymbolInfo
{

public:
    string symbol;
    string symboltype;
    string code;
    string TrueLabel;
    string FalseLabel;
    double value;
    
    SymbolInfo(string symbol="",string symboltype="")
    {   
        this->symbol=symbol;
        this->symboltype=symboltype;
        this->code="";
        if(symboltype=="NUM")
        {
        	stringstream(symbol)>>value;
        }
        else
        {
        	value=0;
        }
    }

    int getHashKey()
    {
        int address;
        return address;
    }

    string getSymbol()
    {
        return symbol;
    }
    string getType()
    {
        return symboltype;
    }
};

class Node
{
public:
    Node* prev;
    Node* next;
    SymbolInfo data;
    

    Node()
    {
        prev=next=NULL;
    }

    Node(SymbolInfo sym)
    {
        prev=next=NULL;
        data=sym;
    }
};

class Linkedlist
{
public:

    Node* Head;
    Node* Tail;
    int sz;

    Linkedlist()
    {
        Head=Tail=NULL;
        sz=0;
    }

    void insertNode(Node* node)
    {   sz++; 
        if(Head==NULL) //no node inserted yet
        {
            Head=Tail=node;
            return;
        }

        Tail->next=node;
        node->prev=Tail;
        Tail=node;
        return;
    }

    void printList()
    {
        Node* it;
        it=Head;
        while(it!=NULL)
        { cout<<"  -->  ";
          cout<<"<"<<(it->data).getSymbol()<<" , "<<(it->data).getType()<<" , "<<(it->data).value<<">";
          it=it->next;
        }
        return;
    }

    bool findSymbol(string sym)
    {
        Node* it;
        it=Head;
        while(it!=NULL)
        {
          if( (it->data).getSymbol()==sym)
                return true;
          it=it->next;
        }
        return false;
    }

    SymbolInfo* findWhichSymbol(string sym)
    {
        Node* it;
        it=Head;
        while(it!=NULL)
        {
          if( (it->data).getSymbol()==sym)
                return &(it->data);
          it=it->next;
        }
        return NULL;
    }

    

    void deleteSymbolNode(Node* node)
    {
    if(node->prev==NULL)//HEAD
        {

        Head=node->next;
        if(node->next!=NULL)
        node->next->prev=NULL;
        }
      else if(node->next==NULL)//tail
        {
        Tail=node->prev;
        if(node->prev!=NULL)
        node->prev->next=NULL;
        }
      else
        {

      node->prev->next=node->next;
      node->next->prev=node->prev;
        }

      delete node;
      sz--;

    }
    void deleteSymbol(string sym)
    {
         Node* it;
        it=Head;
        while(it!=NULL)
        {
          if( (it->data).getSymbol()==sym)
                {//cout<<"FOUND"<<endl;
                    deleteSymbolNode(it);
                return;
                }
          it=it->next;
        }

    }
};

class SymbolTable
{
public:
    Linkedlist* table;
    SymbolTable()
    {
        table=new Linkedlist[mod2];
    }

    ~ SymbolTable()
    {
        delete table;
    }
    
    int HashKey(string key)
{   int result=0;
    int fibs[]={1,2,3,5,8,13,21,34,55};
     for(int i=0;i<key.length();i++)
    {
        result+=(key[i]*(fibs[i%9]));
        result%=mod1;
    }
   return result%mod2;
}


    bool lookup(string sym)
    {
        return table[HashKey(sym)].findSymbol(sym);
    }

    SymbolInfo* uplook(string sym)
    {
        return table[HashKey(sym)].findWhichSymbol(sym);
    }

    void insert(SymbolInfo sym)
    {   if(lookup(sym.getSymbol())==false)table[HashKey(sym.getSymbol())].insertNode(new Node(sym));
    }
    
    void insert(string s1,string s2)
    {
	insert(SymbolInfo(s1,s2));	
	}
    void Delete(string sym)
    {
        if(lookup(sym)==true)
             table[HashKey(sym)].deleteSymbol(sym);
        else cout<<"Not found for deletion"<<endl;
    }
    void dump()
    {
        for(int i=0;i<mod2;i++)
        if(table[i].sz!=0)
        {cout<<"["<<i<<"]";
            table[i].printList();
            cout<<endl;
        }

    }
};

#endif
/*
int main()
{

SymbolTable st;
while(1)
    {


    printf("Press 1 for insert ,2 for lookup,3 for delete , 4 for dump,5 for uplook ,6 for exit\n\n");

    int choice;
    string inp1,inp2;
    cin>>choice;

    if(choice==6)break;;
    switch(choice)
    {
    case 1:
            printf("Enter symbol name and type\n");
            cin>>inp1>>inp2;
             st.insert(SymbolInfo(inp1,inp2));
            break;
    case 2:
            printf("Enter symbol name \n");
            cin>>inp1;

            if(st.lookup(inp1)==true)
                printf("found at %dth index\n",HashKey(inp1));
            else cout<<"Not found"<<endl;
            break;
    case 3:
            printf("Enter symbol name \n");
            cin>>inp1;
            st.Delete(inp1);
            break;
        case 4:
            printf("The Symboltable \n \n");
            st.dump();
            break;
         case 5:
            printf("Enter symbol name \n");
            cin>>inp1;

            if(st.lookup(inp1)==true)
                {
				SymbolInfo* stinfo=st.uplook(inp1);
				cout<<"<"<<stinfo->getSymbol()<<" , "<<stinfo->getType()<<" , "<<stinfo->value<<">"<<endl;
          	
			    }
                
            else cout<<"Not found"<<endl;
            break; 
           

    }
    }


    return 0;
}


*/
