#include<string.h>
#include<string>
#ifndef SYMBOLINFO_H_INCLUDED
#define SYMBOLINFO_H_INCLUDED

using namespace std;

class SymbolInfo{
        string type;
        string symbol;
    public:
        string code;
        SymbolInfo *next;


        SymbolInfo(){
            symbol="";
            type="";
            code="";
        }
        SymbolInfo(string symbol, string type){
            this->symbol=symbol;
            this->type=type;
            code="";
        }
        SymbolInfo(char *symbol, char *type){
            this->symbol=string(symbol);
            this->type= string(type);
            code="";
        }

        SymbolInfo(const SymbolInfo *sym){
         	symbol=sym->symbol;
         	type=sym->type;
         	code=sym->code;
        }

        string getSymbol(){return symbol;}
        string getType(){return type;}

        void setSymbol(char *symbol){
            this->symbol=string(symbol);
        }

        void setType(char *type){
            this->type= string(type);
        }

        void setSymbol(string symbol){
            this->symbol=symbol;
        }
        void setType(string type){
            this->type=type;
        }

};

#endif
