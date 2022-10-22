#ifndef COMPILER_ASTNODE_H
#define COMPILER_ASTNODE_H

#include <string>
#include<iostream>

namespace Parser {
    class ASTNode {
    public:
        ASTNode(std::string content, int urgency=0):m_content(content),urgency(urgency){
            children=std::vector<ASTNode*>(0);}

        ASTNode(const ASTNode&)=delete;
        ASTNode(const ASTNode&&)=delete;

        inline std::vector<ASTNode*>& getChldren(){
            return children;
        };

        inline std::string getContent(){
            return m_content;
        };

        inline void addChild(ASTNode*newChild){
            children.push_back(newChild);
        };

        inline void setOpFlag(bool flag){
            opFlag=flag;
        }

        inline bool getOpFlag(){
            return opFlag;
        }

        inline int getUrgency(){
            return urgency;
        };

    private:
        std::vector<ASTNode*>children;

        std::string m_content;

        bool opFlag=0;

        int urgency;
    };
}
#endif


